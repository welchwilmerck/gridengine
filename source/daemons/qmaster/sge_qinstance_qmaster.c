/*___INFO__MARK_BEGIN__*/
/*************************************************************************
 * 
 *  The Contents of this file are made available subject to the terms of
 *  the Sun Industry Standards Source License Version 1.2
 * 
 *  Sun Microsystems Inc., March, 2001
 * 
 * 
 *  Sun Industry Standards Source License Version 1.2
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.2 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://gridengine.sunsource.net/Gridengine_SISSL_license.html
 * 
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 * 
 *   The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 * 
 *   Copyright: 2001 by Sun Microsystems, Inc.
 * 
 *   All Rights Reserved.
 * 
 ************************************************************************/
/*___INFO__MARK_END__*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include "sge.h"
#include "sgermon.h"
#include "sge_time.h"
#include "sge_signal.h"
#include "sge_event_master.h"
#include "sge_event.h"
#include "sge_log.h"
#include "sge_persistence_qmaster.h"
#include "sge_queue_event_master.h"
#include "sge_qinstance.h"
#include "sge_qinstance_state.h"
#include "sge_qinstance_qmaster.h"
#include "sge_subordinate_qmaster.h"
#include "sge_qmod_qmaster.h"
#include "sge_job.h"
#include "sge_ja_task.h"

#include "sge_attr.h"
#include "sge_calendar.h"
#include "sge_centry.h"
#include "sge_answer.h"
#include "sge_cqueue.h"
#include "sge_object.h"
#include "sge_subordinate.h"
#include "sge_parse_num_par.h"
#include "sge_reporting_qmaster.h"
#include "sge_calendar_qmaster.h"

#include "msg_qmaster.h"

typedef struct {
   u_long32 transition;
   long state_mask;
   bool (*has_state)(const lListElem *this_elem);
   bool is;
   void (*set_state)(lListElem *this_elem, bool set); 
   bool set;
   const char *success_msg;
} change_state_t;

static bool
qinstance_change_state_on_calender_(lListElem *qi_elem, u_long32 cal_order, 
                                    lList **state_change_list, monitoring_t *monitor);

bool
qinstance_modify_attribute(lListElem *this_elem, lList **answer_list,
                           const lListElem *cqueue, 
                           int attribute_name, 
                           int cqueue_attibute_name,
                           int sub_host_name, int sub_value_name,
                           int subsub_key, 
                           const char **matching_host_or_group,
                           const char **matching_group,
                           bool *is_ambiguous, 
                           bool *has_changed_conf_attr, 
                           bool *has_changed_state_attr,
                           const bool initial_modify, 
                           monitoring_t *monitor)
{
#if 0 /* EB: DEBUG: enable debugging for qinstance_modify_attribute() */
#define QINSTANCE_MODIFY_DEBUG
#endif
   bool ret = true;
  
#ifdef QINSTANCE_MODIFY_DEBUG
   DENTER(TOP_LAYER, "qinstance_modify_attribute");
#else 
   DENTER(BASIS_LAYER, "qinstance_modify_attribute");
#endif

   if (this_elem != NULL && cqueue != NULL && 
      attribute_name != NoName && cqueue_attibute_name != NoName) {
      const char *hostname = lGetHost(this_elem, QU_qhostname);
      const lList *attr_list = lGetList(cqueue, cqueue_attibute_name);
      const lDescr *descr = lGetElemDescr(this_elem);
      int pos = lGetPosInDescr(descr, attribute_name);
      int type = lGetPosType(descr, pos);
      bool value_found = true;

      switch (cqueue_attibute_name) {
         case CQ_calendar:
            {
               const char *old_value = lGetString(this_elem, attribute_name);
               const char *new_value;

               str_attr_list_find_value(attr_list, answer_list,
                                        hostname, &new_value, 
                                        matching_host_or_group,
                                        matching_group, is_ambiguous);
               if (old_value == NULL || new_value == NULL ||
                   strcmp(old_value, new_value)) {
                  lList *master_calendar_list = 
                             *(object_type_get_master_list(SGE_TYPE_CALENDAR)); 
                  lListElem *calendar = 
                         calendar_list_locate(master_calendar_list, new_value);
  
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                           lNm2Str(attribute_name),
                           old_value ? old_value : "<null>",
                           new_value ? new_value : "<null>"));
#endif
                  if (calendar != NULL) { 
                     qinstance_change_state_on_calendar(this_elem, calendar, monitor);
                  } else {
                     qinstance_state_set_cal_disabled(this_elem, false);
                     qinstance_state_set_cal_suspended(this_elem, false);
                     lSetList(this_elem, QU_state_changes, NULL);
                  }
                  lSetString(this_elem, attribute_name, new_value);
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_qtype:
            {
               u_long32 old_value = lGetUlong(this_elem, attribute_name);
               u_long32 new_value;

               qtlist_attr_list_find_value(attr_list, answer_list, 
                                           hostname, &new_value,
                                           matching_host_or_group,
                                           matching_group,
                                           is_ambiguous);
               if (old_value != new_value) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ" from "sge_u32" to "sge_u32"\n",
                           lNm2Str(attribute_name), old_value, new_value));
#endif
                  lSetUlong(this_elem, attribute_name, new_value);
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_s_fsize:
         case CQ_h_fsize:
         case CQ_s_data:
         case CQ_h_data:
         case CQ_s_stack:
         case CQ_h_stack:
         case CQ_s_core:
         case CQ_h_core:
         case CQ_s_rss:
         case CQ_h_rss:
         case CQ_s_vmem:
         case CQ_h_vmem:
            {
               const char *old_value = lGetString(this_elem, attribute_name);
               const char *new_value = NULL;

               mem_attr_list_find_value(attr_list, answer_list, 
                                        hostname, &new_value, 
                                        matching_host_or_group,
                                        matching_group, is_ambiguous);
               if (old_value == NULL || new_value == NULL ||
                   strcmp(old_value, new_value)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                           lNm2Str(attribute_name),
                           old_value ? old_value : "<null>",
                           new_value ? new_value : "<null>")); 
#endif
                  lSetString(this_elem, attribute_name, new_value);
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_s_rt:
         case CQ_h_rt:
         case CQ_s_cpu:
         case CQ_h_cpu:
            {
               const char *old_value = lGetString(this_elem, attribute_name);
               const char *new_value = NULL;

               time_attr_list_find_value(attr_list, answer_list, 
                                         hostname, &new_value, 
                                         matching_host_or_group,
                                         matching_group,
                                         is_ambiguous);

               if (old_value == NULL || new_value == NULL ||
                   strcmp(old_value, new_value)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                           lNm2Str(attribute_name),
                           old_value ? old_value : "<null>",
                           new_value ? new_value : "<null>")); 
#endif
                  lSetString(this_elem, attribute_name, new_value);
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_suspend_interval:
         case CQ_min_cpu_interval:
         case CQ_notify:
            {
               const char *old_value = lGetString(this_elem, attribute_name);
               const char *new_value = NULL;

               inter_attr_list_find_value(attr_list, answer_list, 
                                          hostname, &new_value, 
                                          matching_host_or_group,
                                          matching_group, is_ambiguous);
               if (old_value == NULL || new_value == NULL ||
                   strcmp(old_value, new_value)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                           lNm2Str(attribute_name),
                           old_value ? old_value : "<null>",
                           new_value ? new_value : "<null>"));
#endif
                  if (attribute_name == QU_suspend_interval &&
                      new_value != NULL) {
                     u_long32 interval;

                     parse_ulong_val(NULL, &interval, TYPE_TIM,
                                     new_value, NULL, 0);
                     if (interval == 0) {
                        /*
                         * Suspend Threshold state will be reset later 
                         */
                        lSetUlong(this_elem, QU_gdi_do_later, GDI_DO_LATER); 
                     }
                  }
                       
                  lSetString(this_elem, attribute_name, new_value);
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_ckpt_list:
         case CQ_pe_list:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;

               strlist_attr_list_find_value(attr_list, answer_list,
                                            hostname, &new_value, 
                                            matching_host_or_group,
                                            matching_group, is_ambiguous);
               if (object_list_has_differences(old_value, answer_list, 
                                               new_value, false)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif
                  lSetList(this_elem, attribute_name, lCopyList("", new_value));
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_owner_list:
         case CQ_acl:
         case CQ_xacl:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;
   
               usrlist_attr_list_find_value(attr_list, answer_list,
                                            hostname, &new_value, 
                                            matching_host_or_group,
                                            matching_group, is_ambiguous);
               if (object_list_has_differences(old_value, answer_list,
                                               new_value, false)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif
                  lSetList(this_elem, attribute_name, lCopyList("", new_value));
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_projects:
         case CQ_xprojects:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;

               prjlist_attr_list_find_value(attr_list, answer_list,
                                            hostname, &new_value, 
                                            matching_host_or_group,
                                            matching_group, is_ambiguous);
               if (object_list_has_differences(old_value, answer_list,
                                               new_value, false)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif
                  lSetList(this_elem, attribute_name, lCopyList("", new_value));
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_consumable_config_list:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;
               lListElem *tmp_elem = lCopyElem(this_elem);
               lList *master_centry_list = *object_type_get_master_list(SGE_TYPE_CENTRY);

               celist_attr_list_find_value(attr_list, answer_list,
                                           hostname, &new_value, 
                                           matching_host_or_group,
                                           matching_group, is_ambiguous);

               if( centry_list_fill_request(new_value, answer_list, master_centry_list, 
                                        true, true, false) == 0 ) {

                   /* We make a copy of new_value here because it will ultimately
                    * end up in the qinstance.  If we didn't copy it, we would end
                    * up with the list linked into both the cluster queue and
                    * queue instance. */
                   if (new_value != NULL) {
                      new_value = lCopyList ("", new_value);
                   }
                   
                   lSetList(tmp_elem, attribute_name, new_value);
                   new_value = NULL;
                   
                   qinstance_reinit_consumable_actual_list(tmp_elem, answer_list);
                   
                   lXchgList(tmp_elem, attribute_name, &new_value);
                   lFreeElem(&tmp_elem);
                   
                   if (object_list_has_differences(old_value, answer_list,
                                                   new_value, false)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                      DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif
                      /* the following lSetList will free old_value */
                      lSetList(this_elem, attribute_name, new_value);
                      *has_changed_conf_attr = true;
                      if (attribute_name == QU_consumable_config_list) {
                         qinstance_reinit_consumable_actual_list(this_elem, 
                                                                 answer_list);
                      }
                   } else {
                      /* Either new_value is a copy we made, or it was created by
                       * qinstance_reinit_consumable_actual_list().  Either way, it
                       * has to be freed.
                       */
                      lFreeList(&new_value);
                   }
               } else {
                   ret &= false;
               }
            }
            break;
         case CQ_load_thresholds:
         case CQ_suspend_thresholds:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;

               celist_attr_list_find_value(attr_list, answer_list,
                                           hostname, &new_value, 
                                           matching_host_or_group,
                                           matching_group, is_ambiguous);
               if (object_list_has_differences(old_value, answer_list,
                                               new_value, false)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif
                  /*
                   * Suspend Threshold state will be reset later 
                   */
                  if (attribute_name == QU_suspend_thresholds) {
                     lSetUlong(this_elem, QU_gdi_do_later, GDI_DO_LATER);
                  }
   
                  lSetList(this_elem, attribute_name, lCopyList("", new_value));
                  *has_changed_conf_attr = true;
               }
            }
            break;
         case CQ_subordinate_list:
            {
               lList *old_value = lGetList(this_elem, attribute_name);
               lList *new_value = NULL;

               solist_attr_list_find_value(attr_list, answer_list,
                                           hostname, &new_value, 
                                           matching_host_or_group,
                                           matching_group, is_ambiguous);
               if (object_list_has_differences(old_value, answer_list,
                                               new_value, false)) {
                  lList *master_list = 
                               *(object_type_get_master_list(SGE_TYPE_CQUEUE));
                  lList *unsuspended_so = NULL;
                  lList *suspended_so = NULL;
      
#ifdef QINSTANCE_MODIFY_DEBUG
                  DPRINTF(("Changed "SFQ"\n", lNm2Str(attribute_name)));
#endif

                  /*
                   * If this queue instance itself is a subordinated queue
                   * than it might we necessary to set the sos state.
                   * We have to check the subordinated list of all
                   * other queue instances to intitialize the state.
                   *
                   * This queue can't be subordinated if the queue was freshly added.
                   */
                  if (initial_modify == false) {
                     qinstance_initialize_sos_attr(this_elem, monitor);
                  }
   

                  /*
                   * Find list of subordinates that are suspended currently 
                   */
                  ret &= qinstance_find_suspended_subordinates(this_elem,
                                                               answer_list,
                                                               &unsuspended_so);

                  /*
                   * Modify sublist
                   */
                  lSetList(this_elem, attribute_name, lCopyList("", new_value));
                  *has_changed_conf_attr = true;

                  /*
                   * Find list of subordinates that have to be suspended after
                   * the modification of CQ_subordinate_list-sublist 
                   */
                  ret &= qinstance_find_suspended_subordinates(this_elem,
                                                               answer_list,
                                                               &suspended_so);

                  /* 
                   * Remove equal entries in both lists 
                   */
                  lDiffListStr(SO_name, &suspended_so, &unsuspended_so);

                  /*
                   * (Un)suspend subordinated queue instances
                   */
                  cqueue_list_x_on_subordinate_so(master_list, answer_list, 
                                                  false, unsuspended_so, false, monitor);
                  cqueue_list_x_on_subordinate_so(master_list, answer_list, 
                                                  true, suspended_so, false, monitor);

                  /*
                   * Cleanup
                   */
                  lFreeList(&suspended_so);
                  lFreeList(&unsuspended_so);
               }
            }
            break;
         default:
            value_found = false;
            break;
      }

      if (!value_found) {
         switch (type) {
            case lStringT:
               {
                  const char *old_value = lGetString(this_elem, attribute_name);
                  const char *new_value = NULL;

                  str_attr_list_find_value(attr_list, answer_list,
                                           hostname, &new_value, 
                                           matching_host_or_group,
                                           matching_group, is_ambiguous);
                  if (old_value == NULL || new_value == NULL ||
                      strcmp(old_value, new_value)) {
#ifdef QINSTANCE_MODIFY_DEBUG
                     DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                              lNm2Str(attribute_name),
                              old_value ? old_value : "<null>",
                              new_value ? new_value : "<null>"));
#endif
                     lSetString(this_elem, attribute_name, new_value);
                     *has_changed_conf_attr = true;
                  }
               }
               break;
            case lUlongT:
               {
                  u_long32 old_value = lGetUlong(this_elem, attribute_name);
                  u_long32 new_value;

                  ulng_attr_list_find_value(attr_list, answer_list, hostname, 
                                            &new_value, 
                                            matching_host_or_group,
                                            matching_group, is_ambiguous);
                  if (old_value != new_value) {
#ifdef QINSTANCE_MODIFY_DEBUG
                     DPRINTF(("Changed "SFQ" from "sge_u32" to "sge_u32"\n",
                              lNm2Str(attribute_name),
                              old_value, new_value));
#endif
                     lSetUlong(this_elem, attribute_name, new_value);
                     *has_changed_conf_attr = true;
                     if (attribute_name == QU_job_slots) {
                        qinstance_reinit_consumable_actual_list(this_elem,
                                                                answer_list);
                     } else if (attribute_name == QU_nsuspend &&
                                new_value == 0) {
                        /*
                         * Suspend Threshold state will be reset later 
                         */

                        lSetUlong(this_elem, QU_gdi_do_later, GDI_DO_LATER);
                     }
                  }
               }
               break;
            case lBoolT:
               {
                  bool old_value = lGetBool(this_elem, attribute_name) ? true : false;
                  bool new_value;

                  bool_attr_list_find_value(attr_list, answer_list,
                                            hostname, &new_value, 
                                            matching_host_or_group,
                                            matching_group, is_ambiguous);
                  if (old_value != new_value) {
#ifdef QINSTANCE_MODIFY_DEBUG
                     DPRINTF(("Changed "SFQ" from "SFQ" to "SFQ"\n",
                              lNm2Str(attribute_name),
                              (old_value ? "true" : "false"),
                              (new_value ? "true" : "false")));
#endif
                     lSetBool(this_elem, attribute_name, new_value);
                     *has_changed_conf_attr = true;
                  }
               }
               break;
            default:
               DPRINTF(("unhandled attribute\n"));
               break;
         }
      }
   }
   DEXIT;
   return ret;
}

bool
qinstance_change_state_on_command(lListElem *this_elem, lList**answer_list,
                                  u_long32 transition, bool force_transition,
                                  const char *user, const char *host,
                                  bool is_operator, bool is_owner, monitoring_t *monitor)
{
   bool ret = true;
   dstring buffer = DSTRING_INIT;
   const char *qinstance_name = qinstance_get_name(this_elem, &buffer);
   change_state_t transitions[] = {
      { QI_DO_CLEARERROR,     ~QI_ERROR,     qinstance_state_is_error,            true,  qinstance_state_set_error,            false},
      { QI_DO_ENABLE,         ~QI_DISABLED,  qinstance_state_is_manual_disabled,  true,  qinstance_state_set_manual_disabled,  false},
      { QI_DO_DISABLE,        QI_DISABLED,   qinstance_state_is_manual_disabled,  false, qinstance_state_set_manual_disabled,  true },
      { QI_DO_SUSPEND,        QI_SUSPENDED,  qinstance_state_is_manual_suspended, false, qinstance_state_set_manual_suspended, true },
      { QI_DO_UNSUSPEND,      ~QI_SUSPENDED, qinstance_state_is_manual_suspended, true,  qinstance_state_set_manual_suspended, false},
#ifdef __SGE_QINSTANCE_STATE_DEBUG__
      { QI_DO_SETERROR,       QI_ERROR,      qinstance_state_is_error,            false, qinstance_state_set_error,            true},
      { QI_DO_SETORPHANED,    QI_ORPHANED,   qinstance_state_is_orphaned,         false, qinstance_state_set_orphaned,         true},
      { QI_DO_CLEARORPHANED,  ~QI_ORPHANED,  qinstance_state_is_orphaned,         true,  qinstance_state_set_orphaned,         false},
      { QI_DO_SETUNKNOWN,     QI_UNKNOWN,    qinstance_state_is_unknown,          false, qinstance_state_set_unknown,          true},
      { QI_DO_CLEARUNKNOWN,   ~QI_UNKNOWN,   qinstance_state_is_unknown,          true,  qinstance_state_set_unknown,          false},
      { QI_DO_SETAMBIGUOUS,   QI_AMBIGUOUS,  qinstance_state_is_ambiguous,        false, qinstance_state_set_ambiguous,        true},
      { QI_DO_CLEARAMBIGUOUS, ~QI_AMBIGUOUS, qinstance_state_is_ambiguous,        true,  qinstance_state_set_ambiguous,        false},
#endif
      { QI_DO_NOTHING,        0,             NULL,                                true,  NULL,                                 true }
   };

   DENTER(TOP_LAYER, "qinstance_change_state_on_command");
   if (is_owner || is_operator) {
      int i = 0;

      while (transitions[i].transition != QI_DO_NOTHING) {
         if (transitions[i].transition == transition) {
            break;
         }
         i++;
      }

      /*
       * Verify current state
       */
      if (transitions[i].has_state(this_elem) == transitions[i].is ||
          force_transition) {
         bool did_something = false;

         DTRACE;

         /*
          * Some transitions need extra work
          */
          switch(transition){
            case QI_DO_SUSPEND : 
                  if ((!qinstance_state_is_susp_on_sub(this_elem) &&
                       !qinstance_state_is_cal_suspended(this_elem)) ||
                      force_transition) {
                     sge_signal_queue(SGE_SIGSTOP, this_elem, NULL, NULL, monitor);
                     did_something = true;
                  }   
               break;
            case QI_DO_UNSUSPEND :    
                  if (!qinstance_state_is_susp_on_sub(this_elem) &&
                      !qinstance_state_is_cal_suspended(this_elem)) {
                     sge_signal_queue(SGE_SIGCONT, this_elem, NULL, NULL, monitor);
                     did_something = true;
                  }   
               break;
            case QI_DO_CLEARERROR :
                  qinstance_message_trash_all_of_type_X(this_elem, QI_ERROR);
                  did_something = true;
               break;   
#ifdef __SGE_QINSTANCE_STATE_DEBUG__
            case QI_DO_SETERROR :
                 qinstance_message_add(this_elem, QI_ERROR, "this is a debug message\n");
                 did_something = true;
               break;  
#endif                  
            default:   
                  did_something = true;
         }
      
         /*
          * Change state
          */
         if (did_something) {
            transitions[i].set_state(this_elem, transitions[i].set);
         }

         /*
          * Make changes persistent
          */
         if (did_something) {
            qinstance_increase_qversion(this_elem);
            reporting_create_queue_record(NULL, this_elem, sge_get_gmt());
            ret &= sge_event_spool(answer_list, 0, sgeE_QINSTANCE_MOD,
                                   0, 0, lGetString(this_elem, QU_qname),
                                   lGetHost(this_elem, QU_qhostname), NULL,
                                   this_elem, NULL, NULL, false, true);

            if (ret) {
               if (force_transition) {
                  INFO((SGE_EVENT, MSG_QINSTANCE_FORCEDSTATE_SSSS,
                        user, host, qinstance_name, 
                        qinstance_state_as_string(transitions[i].state_mask)));
               } else {
                  INFO((SGE_EVENT, MSG_QINSTANCE_CHANGEDST_SSSS,
                        user, host, qinstance_name, 
                        qinstance_state_as_string(transitions[i].state_mask)));
               }
               answer_list_add(answer_list, SGE_EVENT,
                               STATUS_OK, ANSWER_QUALITY_ERROR);
            } else {
               ERROR((SGE_EVENT, MSG_QINSTANCE_STATENOTMOD_S, qinstance_name));
               answer_list_add(answer_list, SGE_EVENT,
                               STATUS_ESEMANTIC, ANSWER_QUALITY_ERROR);

               /*
                * Rollback
                */
               if (!force_transition) {
                  transitions[i].set_state(this_elem, transitions[i].set ? false : true);
               }
            }
         }
      } else {
         INFO((SGE_EVENT, MSG_QINSTANCE_HASSTATE_SS, qinstance_name, 
               qinstance_state_as_string(transitions[i].state_mask)));
         answer_list_add(answer_list, SGE_EVENT,
                         STATUS_ESEMANTIC, ANSWER_QUALITY_WARNING);
      }
   } else {
      WARNING((SGE_EVENT, MSG_QINSTANCE_STATENOTMODPERM_S, qinstance_name));
      answer_list_add(answer_list, SGE_EVENT,
                      STATUS_ESEMANTIC, ANSWER_QUALITY_WARNING);
   }
   sge_dstring_free(&buffer);
   DEXIT;
   return ret;
}


/****** sge_qinstance_qmaster/qinstance_change_state_on_calendar() *************
*  NAME
*     qinstance_change_state_on_calendar() --- changes the state of a given qi (wraper)
*
*  SYNOPSIS
*     bool qinstance_change_state_on_calendar(lListElem *this_elem, const 
*     lListElem *calendar) 
*
*  FUNCTION
*     Changes the state of a given qi based on its calendar.
*
*  INPUTS
*     lListElem *this_elem      - quinstance
*     const lListElem *calendar - calendar
*
*  RESULT
*     bool - state got changed or not
*
*  NOTES
*     MT-NOTE: qinstance_change_state_on_calendar() is MT safe 
*
*******************************************************************************/
bool qinstance_change_state_on_calendar(lListElem *this_elem, 
                                   const lListElem *calendar, monitoring_t *monitor)
{
   bool ret = true;

   DENTER(TOP_LAYER, "qinstance_signal_on_calendar");

   if (this_elem != NULL && calendar != NULL) {
      lList *state_changes_list = NULL;
      u_long32 state;
      time_t when = 0; 

      state = calender_state_changes(calendar, &state_changes_list, &when, NULL);

      ret = qinstance_change_state_on_calender_(this_elem, state, &state_changes_list, monitor);

   }
   DEXIT;
   return ret;
}

/****** sge_qinstance_qmaster/qinstance_change_state_on_calendar_all() *********
*  NAME
*     qinstance_change_state_on_calendar_all() -- changes the state of all qis (wraper)
*
*  SYNOPSIS
*     bool qinstance_change_state_on_calendar_all(const char* cal_name, 
*     u_long32 cal_order, const lList *state_change_list) 
*
*  FUNCTION
*     ??? 
*
*  INPUTS
*     const char* cal_name     - calendar name
*     u_long32 cal_order       - calendar state (todo)
*     const lList *state_change_list - state list for the qis
*
*  RESULT
*     bool - true, if it worked
*
*  NOTES
*     MT-NOTE: qinstance_change_state_on_calendar_all() is not MT safe 
*     Directly access the cluster queue list
*
*******************************************************************************/
bool qinstance_change_state_on_calendar_all(const char* cal_name, u_long32 cal_order, 
                                            const lList *state_change_list, monitoring_t *monitor)
{
   bool ret = true;
   lListElem *cqueue;

   DENTER(TOP_LAYER, "qinstance_signal_on_calendar_all");

   for_each (cqueue, *(object_type_get_master_list(SGE_TYPE_CQUEUE)))
   {
      lList *qinstance_list = lGetList(cqueue, CQ_qinstances);
      lListElem *qinstance = NULL;

      for_each(qinstance, qinstance_list)
      {
         const char *queue_calendar = lGetString(qinstance, QU_calendar);

         if (queue_calendar != NULL && !strcmp(queue_calendar, cal_name)) {
            lList *copy_state_change_list = lCopyList("state list", state_change_list);
            ret = qinstance_change_state_on_calender_(qinstance, cal_order, &copy_state_change_list, monitor);
         }
      }
   }

   DEXIT;
   return ret;
}

/****** sge_qinstance_qmaster/qinstance_change_state_on_calender_() ************
*  NAME
*     qinstance_change_state_on_calender_() -- changes qi state based on calendar
*
*  SYNOPSIS
*     static bool qinstance_change_state_on_calender_(lListElem *this_elem, 
*     u_long32 cal_order, lList **state_change_list) 
*
*  FUNCTION
*     ??? 
*
*  INPUTS
*     lListElem *this_elem     - qi
*     u_long32 cal_order       - next state (order)
*     lList **state_change_list - qi state list
*
*  RESULT
*     static bool - true, if it worked
*
*  NOTES
*     MT-NOTE: qinstance_change_state_on_calender_() is MT safe 
*
*******************************************************************************/
static bool qinstance_change_state_on_calender_(lListElem *this_elem, u_long32 cal_order, 
                                                lList **state_change_list, monitoring_t *monitor) 
{
   bool ret = true;
   bool old_cal_disabled = qinstance_state_is_cal_disabled(this_elem);
   bool old_cal_suspended = qinstance_state_is_cal_suspended(this_elem);
   bool new_cal_disabled = (cal_order == QI_DO_CAL_DISABLE) ? true : false;
   bool new_cal_suspended = (cal_order == QI_DO_CAL_SUSPEND) ? true : false;
   bool state_changed = false;

   DENTER(TOP_LAYER, "qinstance_signal_on_calendar_");

   lSetList(this_elem, QU_state_changes, *state_change_list);
   *state_change_list = NULL;

   if (old_cal_disabled != new_cal_disabled) {
      qinstance_state_set_cal_disabled(this_elem, new_cal_disabled);
      state_changed = true;
   }
   
   if (old_cal_suspended != new_cal_suspended) {
      const char *name = lGetString(this_elem, QU_full_name);

      qinstance_state_set_cal_suspended(this_elem, new_cal_suspended);
      if (new_cal_suspended) {
         if (qinstance_state_is_susp_on_sub(this_elem)) {
            INFO((SGE_EVENT, MSG_QINSTANCE_NOUSSOS_S, name));
         } else if (qinstance_state_is_manual_suspended(this_elem)) {
            INFO((SGE_EVENT, MSG_QINSTANCE_NOUSADM_S, name));
         } else {
            sge_signal_queue(SGE_SIGSTOP, this_elem, NULL, NULL, monitor);
         }
      } else {
         if (qinstance_state_is_susp_on_sub(this_elem)) {
            INFO((SGE_EVENT, MSG_QINSTANCE_NOSSOS_S, name));
         } else if (qinstance_state_is_manual_suspended(this_elem)) {
            INFO((SGE_EVENT, MSG_QINSTANCE_NOSADM_S, name));
         } else {
            sge_signal_queue(SGE_SIGCONT, this_elem, NULL, NULL, monitor);
         }
      }
      state_changed = true;
   }

   if (state_changed) {
      reporting_create_queue_record(NULL, this_elem, sge_get_gmt());
   }
   qinstance_add_event(this_elem, sgeE_QINSTANCE_MOD);
   

   DEXIT;
   return ret;

}
