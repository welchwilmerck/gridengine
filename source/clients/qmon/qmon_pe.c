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
#include <ctype.h>

#include <Xm/Xm.h>
#include <Xm/List.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include <Xmt/Xmt.h>
#include <Xmt/Create.h>
#include <Xmt/Layout.h>
#include <Xmt/Chooser.h>
#include <Xmt/Dialog.h>
#include <Xmt/Dialogs.h>
#include <Xmt/MsgLine.h>
#include <Xmt/InputField.h>

#include "Spinbox.h"
#include "sge_all_listsL.h"
#include "sge_gdi.h"
#include "sge_answer.h"
#include "commlib.h"
#include "qmon_proto.h"
#include "qmon_rmon.h"
#include "qmon_cull.h"
#include "qmon_util.h"
#include "qmon_appres.h"
#include "qmon_timer.h"
#include "qmon_comm.h"
#include "qmon_widgets.h"
#include "qmon_message.h"
#include "qmon_pe.h"
#include "qmon_globals.h"
#include "AskForItems.h"
#include "sge_dstring.h"
#include "sge_pe.h"


static Widget qmon_pe = 0;
static Widget pe_names = 0;
static Widget pe_conf_list = 0;
static Widget pe_ask_layout = 0;
static Widget pe_name_w = 0;
static Widget pe_slots_w = 0;
static Widget pe_acl_w = 0;
static Widget pe_xacl_w = 0;
static Widget pe_start_w = 0;
static Widget pe_stop_w = 0;
static Widget pe_alloc_w = 0;
static Widget pe_control_slaves_w = 0;
static Widget pe_job_is_first_task_w = 0;
static int add_mode = 0;
static lList *ql_saved = NULL;

/*-------------------------------------------------------------------------*/
static void qmonPopdownPEConfig(Widget w, XtPointer cld, XtPointer cad);
static Widget qmonCreatePEConfig(Widget parent);
static void qmonPEFillConf(Widget w, lListElem *ep);
static void qmonSelectPE(Widget w, XtPointer cld, XtPointer cad);
static void qmonPEAdd(Widget w, XtPointer cld, XtPointer cad);
static void qmonPEModify(Widget w, XtPointer cld, XtPointer cad);
static void qmonPEDelete(Widget w, XtPointer cld, XtPointer cad);
static void qmonPEOk(Widget w, XtPointer cld, XtPointer cad);
static void qmonPECancel(Widget w, XtPointer cld, XtPointer cad);
static void qmonPEResetAsk(void);
static void qmonPESetAsk(lListElem *pep);
static Widget qmonCreatePEAsk(Widget parent);
static Boolean qmonPEGetAsk(lListElem *pep);
static void qmonPEAskForUsers(Widget w, XtPointer cld, XtPointer cad);

/*-------------------------------------------------------------------------*/
void qmonPopupPEConfig(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   Widget shell;
   lList *alp = NULL;

   DENTER(GUI_LAYER, "qmonPopupPEConfig");

   /* set busy cursor */
   XmtDisplayBusyCursor(w);

   if (!qmon_pe) {
      shell = XmtGetTopLevelShell(w);
      qmon_pe = qmonCreatePEConfig(shell);
      XmtAddDeleteCallback(shell, XmDO_NOTHING, 
                              qmonPopdownPEConfig, NULL);
      /*
      ** create ask layout
      */
      pe_ask_layout = qmonCreatePEAsk(qmon_pe);

   } 
   XSync(XtDisplay(qmon_pe), 0);
   XmUpdateDisplay(qmon_pe);

   qmonMirrorMultiAnswer(PE_T | QUEUE_T | USERSET_T, &alp);
   if (alp) {
      qmonMessageBox(w, alp, 0);
      alp = lFreeList(alp);
      /* set default cursor */
      XmtDisplayDefaultCursor(w);
      DEXIT;
      return;
   }
   
   qmonTimerAddUpdateProc(PE_T, "updatePeList", updatePeList);
   qmonStartTimer(PE_T | QUEUE_T | USERSET_T);
   updatePeList();
   XmListSelectPos(pe_names, 1, True);

   XtManageChild(qmon_pe);
   XRaiseWindow(XtDisplay(XtParent(qmon_pe)), XtWindow(XtParent(qmon_pe)));

   /* set default cursor */
   XmtDisplayDefaultCursor(w);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
void updatePeList(void)
{
   lList *cl;
   
   DENTER(GUI_LAYER, "updatePeList");

   cl = qmonMirrorList(SGE_PE_LIST);
   lPSortList(cl, "%I+", PE_name);
   UpdateXmListFromCull(pe_names, XmFONTLIST_DEFAULT_TAG, cl, PE_name);

   DEXIT;
}


/*-------------------------------------------------------------------------*/
static void qmonPopdownPEConfig(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   DENTER(GUI_LAYER, "qmonPopdownPEConfig");

   XtUnmanageChild(qmon_pe);
   qmonStopTimer(PE_T | QUEUE_T | USERSET_T);
   qmonTimerRmUpdateProc(PE_T, "updatePeList");

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEFillConf(
Widget w,
lListElem *ep 
) {
   lList *ql, *ul;
   lListElem *qep, *uep;
   XmString *items;
   Cardinal itemCount; 
   char buf[BUFSIZ];
   StringConst str;
   int i;
   dstring sb = DSTRING_INIT;

   DENTER(GUI_LAYER, "qmonPEFillConf");
   
   if (!ep) {
      /*
      ** clear the pe_conf_list
      */
      XtVaSetValues( pe_conf_list, 
                  XmNitems, NULL,
                  XmNitemCount, 0,
                  NULL);
      DEXIT;
      return;
   }
   
   itemCount = 9;
   items = (XmString*) XtMalloc(sizeof(XmString)*itemCount); 

   i = 0;

   /* pe name */
   sprintf(buf, "%-20.20s %s", "PE Name", lGetString(ep, PE_name));
   items[i++] = XmStringCreateLocalized(buf);

   /* slots */
   sprintf(buf, "%-20.20s %d", "Slots", (int)lGetUlong(ep, PE_slots));
   items[i++] = XmStringCreateLocalized(buf);

#ifdef HOP
   /* queue list */
   ql = lGetList(ep, PE_queue_list);
   sge_dstring_sprintf(&sb, "%-20.20s", "Queues");
   for_each(qep, ql) {
      (void) sge_dstring_append(&sb, " "); 
      (void) sge_dstring_append(&sb, lGetString(qep, QR_name));
   }
   if (!lGetNumberOfElem(ql))
      (void) sge_dstring_append(&sb, " NONE"); 
/*    items[i++] = XmStringCreateLtoR(sb.s, "LIST"); */
   items[i++] = XmStringCreateLocalized((char*)sge_dstring_get_string(&sb));
#endif   
   
   /* users list */
   ul = lGetList(ep, PE_user_list);
   sge_dstring_sprintf(&sb, "%-20.20s", "Users");
   for_each(uep, ul) {
      (void) sge_dstring_append(&sb, " "); 
      (void) sge_dstring_append(&sb, lGetString(uep, US_name));
   }
   if (!lGetNumberOfElem(ul))
      (void) sge_dstring_append(&sb, " NONE"); 
   items[i++] = XmStringCreateLocalized(sb.s);
   
   /* xusers list */
   ul = lGetList(ep, PE_xuser_list);
   sge_dstring_sprintf(&sb, "%-20.20s", "Xusers");
   for_each(uep, ul) {
      (void) sge_dstring_append(&sb, " "); 
      (void) sge_dstring_append(&sb, lGetString(uep, US_name));
   }
   if (!lGetNumberOfElem(ul))
      (void) sge_dstring_append(&sb, " NONE"); 
   items[i++] = XmStringCreateLocalized(sb.s);
   sge_dstring_free(&sb);
   
   /* start_proc_args */
   str = lGetString(ep, PE_start_proc_args);
   sprintf(buf, "%-20.20s %s", "Start Proc Args", str ? str : "NONE" );
   items[i++] = XmStringCreateLocalized(buf);

   /* stop_proc_args */
   str = lGetString(ep, PE_stop_proc_args);
   sprintf(buf, "%-20.20s %s", "Stop Proc Args", str ? str : "NONE" );
   items[i++] = XmStringCreateLocalized(buf);

   /* allocation_rule */
   str = lGetString(ep, PE_allocation_rule);
   sprintf(buf, "%-20.20s %s", "Allocation Rule", str ? str : "NONE" );
   items[i++] = XmStringCreateLocalized(buf);

   /* control slaves */
   sprintf(buf, "%-20.20s %s", "Control Slaves", 
            (int)lGetBool(ep, PE_control_slaves) ? "true" : "false");
   items[i++] = XmStringCreateLocalized(buf);

   /* job_is_first_task */
   sprintf(buf, "%-20.20s %s", "Job is first task", 
            (int)lGetBool(ep, PE_job_is_first_task) ? "true" : "false");
   items[i++] = XmStringCreateLocalized(buf);

   
   XtVaSetValues( pe_conf_list, 
                  XmNitems, items,
                  XmNitemCount, itemCount,
                  NULL);
   XmStringTableFree(items, itemCount);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonSelectPE(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   XmListCallbackStruct *cbs = (XmListCallbackStruct*) cad;
   char *pename;
   lListElem *ep;
   
   DENTER(GUI_LAYER, "qmonSelectPE");

   if (! XmStringGetLtoR(cbs->item, XmFONTLIST_DEFAULT_TAG, &pename)) {
      fprintf(stderr, "XmStringGetLtoR failed\n");
      DEXIT;
      return;
   }

   ep = pe_list_locate(qmonMirrorList(SGE_PE_LIST), pename);

   XtFree((char*) pename);

   qmonPEFillConf(pe_conf_list, ep);
   
   DEXIT;
}


/*-------------------------------------------------------------------------*/
static Widget qmonCreatePEConfig(
Widget parent 
) {
   Widget pe_layout, pe_add, pe_delete, pe_modify, pe_done, pe_main_link;

   DENTER(GUI_LAYER, "qmonCreatePEConfig");
   
   pe_layout = XmtBuildQueryDialog( parent, "qmon_pe",
                           NULL, 0,
                           "pe_names", &pe_names,
                           "pe_conf_list", &pe_conf_list,
                           "pe_add", &pe_add,
                           "pe_delete", &pe_delete,
                           "pe_done", &pe_done,
                           "pe_modify", &pe_modify,
                           "pe_main_link", &pe_main_link,
                           NULL);

   XtAddCallback(pe_main_link, XmNactivateCallback, 
                     qmonMainControlRaise, NULL);
   XtAddCallback(pe_names, XmNbrowseSelectionCallback, 
                     qmonSelectPE, NULL);
   XtAddCallback(pe_done, XmNactivateCallback, 
                     qmonPopdownPEConfig, NULL);
   XtAddCallback(pe_add, XmNactivateCallback, 
                     qmonPEAdd, NULL); 
   XtAddCallback(pe_modify, XmNactivateCallback, 
                     qmonPEModify, NULL); 
   XtAddCallback(pe_delete, XmNactivateCallback, 
                     qmonPEDelete, NULL); 

   XtAddEventHandler(XtParent(pe_layout), StructureNotifyMask, False, 
                        SetMinShellSize, NULL);

   DEXIT;
   return pe_layout;
}



/*-------------------------------------------------------------------------*/
static Widget qmonCreatePEAsk(
Widget parent 
) {
   Widget pe_ok, pe_cancel, pe_usersPB, pe_xusersPB;

   DENTER(GUI_LAYER, "qmonCreatePEAsk");
   
   pe_ask_layout = XmtBuildQueryDialog( parent, "pe_ask_shell",
                           NULL, 0,
                           "pe_ok", &pe_ok,
                           "pe_cancel", &pe_cancel,
                           "pe_usersPB", &pe_usersPB,
                           "pe_xusersPB", &pe_xusersPB,
                           "pe_name", &pe_name_w,
                           "pe_slots", &pe_slots_w,
                           "pe_users", &pe_acl_w, 
                           "pe_xusers", &pe_xacl_w,
                           "pe_start_proc_args", &pe_start_w,
                           "pe_stop_proc_args", &pe_stop_w,
                           "pe_allocation_rule", &pe_alloc_w,
                           "pe_control_slaves", &pe_control_slaves_w,
                           "pe_job_is_first_task", &pe_job_is_first_task_w,
                           NULL);

   XtAddCallback(pe_ok, XmNactivateCallback, 
                     qmonPEOk, NULL);
   XtAddCallback(pe_cancel, XmNactivateCallback, 
                     qmonPECancel, NULL);
   XtAddCallback(pe_usersPB, XmNactivateCallback, 
                     qmonPEAskForUsers, (XtPointer)pe_acl_w);
   XtAddCallback(pe_xusersPB, XmNactivateCallback, 
                     qmonPEAskForUsers, (XtPointer)pe_xacl_w);

   XtAddEventHandler(XtParent(pe_ask_layout), StructureNotifyMask, False, 
                        SetMinShellSize, NULL);

   DEXIT;
   return pe_ask_layout;
}

#ifdef HOP
/*-------------------------------------------------------------------------*/
static void qmonPEAllQueues(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   lList *ql_out = NULL;
   XmToggleButtonCallbackStruct *cbs = (XmToggleButtonCallbackStruct*)cad;

   DENTER(GUI_LAYER, "qmonPEAllQueues");
   
   if (cbs->set) {
      ql_saved = XmStringToCull(pe_queues_w, QR_Type, QR_name, ALL_ITEMS);
      lAddElemStr(&ql_out, QR_name, "all", QR_Type);
      XtSetSensitive(pe_queuesPB, False);
   }   
   else {
      ql_out = ql_saved;
      ql_saved = NULL;
      XtSetSensitive(pe_queuesPB, True);
   }   

   UpdateXmListFromCull(pe_queues_w, XmFONTLIST_DEFAULT_TAG, ql_out,
                              QR_name);
   ql_out = lFreeList(ql_out);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEAskForQueues(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   lList *ql_out = NULL;
   lList *ql_in = NULL;
   static lCondition *where = NULL;
   static lEnumeration *what = NULL;
   int status;

   DENTER(GUI_LAYER, "qmonPEAskForQueues");
   
   /*
   ** the template q is removed here
   */
   if (!where) {
      where = lWhere("%T(%I!=%s)", QU_Type, QU_qname, "template");
      what = lWhat("%T(ALL)", QU_Type);
   }

   ql_in = lSelect("QL", qmonMirrorList(SGE_QUEUE_LIST), where, what);
   ql_out = XmStringToCull(pe_queues_w, QR_Type, QR_name, ALL_ITEMS);

   status = XmtAskForItems(w, NULL, NULL, "@{Select Queues}", ql_in, QU_qname, 
                  "@{@fBAvailable Queues}", &ql_out, QR_Type, QR_name, 
                  "@{@fBChosen Queues}", NULL);

   if (status) {
      UpdateXmListFromCull(pe_queues_w, XmFONTLIST_DEFAULT_TAG, ql_out,
                              QR_name);
   }
   ql_out = lFreeList(ql_out);
   ql_in = lFreeList(ql_in);

   DEXIT;
}
#endif

/*-------------------------------------------------------------------------*/
static void qmonPEAskForUsers(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   lList *ql_out = NULL;
   lList *ql_in = NULL;
   int status;
   Widget list = (Widget) cld;

   DENTER(GUI_LAYER, "qmonPEAskForUsers");
   
   ql_in = qmonMirrorList(SGE_USERSET_LIST);
   ql_out = XmStringToCull(list, US_Type, US_name, ALL_ITEMS);

   status = XmtAskForItems(w, NULL, NULL, "@{Select Access Lists}", ql_in, US_name,
                  "@{@fBAvailable Access Lists}", &ql_out, US_Type, US_name, 
                  "@{@fBChosen Access Lists}", NULL);

   if (status) {
      UpdateXmListFromCull(list, XmFONTLIST_DEFAULT_TAG, ql_out,
                              US_name);
   }
   ql_out = lFreeList(ql_out);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEAdd(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   DENTER(GUI_LAYER, "qmonPEAdd");

   qmonPEResetAsk();
   XtVaSetValues( pe_name_w,
                  XmNeditable, True,
                  NULL);
   add_mode = 1;
   XtManageChild(pe_ask_layout);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEModify(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   XmString *penames;
   Cardinal penum;
   String pestr;
   lListElem *pep = NULL;

   DENTER(GUI_LAYER, "qmonPEModify");

   /*
   ** on opening the dialog fill in the old values
   */
   XtVaGetValues( pe_names,
                  XmNselectedItems, &penames,
                  XmNselectedItemCount, &penum,
                  NULL);
   
   if (penum == 1 && 
         XmStringGetLtoR(penames[0], XmFONTLIST_DEFAULT_TAG, &pestr)) {
      XmtInputFieldSetString(pe_name_w, pestr);
      XtVaSetValues( pe_name_w,
                     XmNeditable, False,
                     NULL);
      pep = pe_list_locate(qmonMirrorList(SGE_PE_LIST), pestr);
      XtFree((char*)pestr);
      if (pep) {
         add_mode = 0;
         qmonPESetAsk(pep);
         XtManageChild(pe_ask_layout);
      }
      XtManageChild(pe_ask_layout);
   }


   DEXIT;
}



/*-------------------------------------------------------------------------*/
static void qmonPEOk(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   lList *pel = NULL;
   lList *alp;
   lEnumeration *what;
   Boolean status = False;
   XmString xpename = NULL;
   StringConst pename = NULL;

   DENTER(GUI_LAYER, "qmonPEOk");
   /*
   ** get the contents of the dialog fields here,
   ** build the cull list and send gdi request
   ** depending on success of gdi request close the dialog or stay open
   */
   pel = lCreateElemList("PE_ADD", PE_Type, 1);
   
   if (pel) {
      if (qmonPEGetAsk(lFirst(pel))) {

         pename = lGetString(lFirst(pel), PE_name);
         /*
         ** gdi call 
         */
         what = lWhat("%T(ALL)", PE_Type);
         
         if (add_mode) {
            alp = qmonAddList(SGE_PE_LIST, qmonMirrorListRef(SGE_PE_LIST),
                              PE_name, &pel, NULL, what);
         }
         else {
            alp = qmonModList(SGE_PE_LIST, qmonMirrorListRef(SGE_PE_LIST),
                              PE_name, &pel, NULL, what);
         }

         if (lFirst(alp) && lGetUlong(lFirst(alp), AN_status) == STATUS_OK)
            status = True;

         qmonMessageBox(w, alp, 0);

         if (status) {
            XtUnmanageChild(pe_ask_layout);
            updatePeList();
            /*
            ** select the modified or added PE
            */
            xpename = XmtCreateXmString(pename);
            XmListSelectItem(pe_names, xpename, True);
            XmStringFree(xpename);
         }
         lFreeWhat(what);
         alp = lFreeList(alp);
      }
      pel = lFreeList(pel);
   }


   DEXIT;
}



/*-------------------------------------------------------------------------*/
static void qmonPECancel(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   DENTER(GUI_LAYER, "qmonPECancel");

   XtUnmanageChild(pe_ask_layout);
   ql_saved = lFreeList(ql_saved);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEDelete(w, cld, cad)
Widget w;
XtPointer cld, cad;
{
   lList *lp = NULL;
   lList *alp = NULL;
   lEnumeration *what = NULL;
   Cardinal itemCount = 0;
   Boolean status, answer;

   DENTER(GUI_LAYER, "qmonPEDelete");
    
   lp = XmStringToCull(pe_names, PE_Type, PE_name, SELECTED_ITEMS); 

   if (lp) {
      status = XmtAskForBoolean(w, "xmtBooleanDialog", 
                     "@{pe.askdel.Do you really want to delete the\nselected Parallel Environment\nConfiguration ?}", 
                     "@{Delete}", "@{Cancel}", NULL, XmtNoButton, XmDIALOG_WARNING, 
                     False, &answer, NULL);
      if (answer) { 
         what = lWhat("%T(ALL)", PE_Type);
         alp = qmonDelList(SGE_PE_LIST, qmonMirrorListRef(SGE_PE_LIST),
                                 PE_name, &lp, NULL, what);

         qmonMessageBox(w, alp, 0);

         lFreeWhat(what);
         alp = lFreeList(alp);

         updatePeList();
         XtVaGetValues( pe_names,
                        XmNitemCount, &itemCount,
                        NULL);
         if (itemCount)
            XmListSelectPos(pe_names, 1, True);
         else
            qmonPEFillConf(pe_names, NULL);

      }
      lp = lFreeList(lp);
   }
   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPESetAsk(
lListElem *pep 
) {
   StringConst pe_name = NULL;
   u_long32 pe_slots = 0;
   lList *ql = NULL;
   lList *acl = NULL;
   lList *xacl = NULL;
   StringConst start_args = NULL;
   StringConst stop_args = NULL;
   StringConst alloc_rule = NULL;

   DENTER(GUI_LAYER, "qmonPESetAsk");

   if (!pep) {
      DEXIT;
      return;
   }

   pe_name = lGetString(pep, PE_name);
   if (pe_name)
      XmtInputFieldSetString(pe_name_w, pe_name);

   pe_slots = lGetUlong(pep, PE_slots);
   XmpSpinboxSetValue(pe_slots_w, pe_slots, True);

#ifdef HOP
   /*
   ** the lists have to be converted to XmString
   */
   ql = lGetList(pep, PE_queue_list);
   if (lGetNumberOfElem(ql) == 1 && 
       !strcasecmp(lGetString(lFirst(ql), QR_name), "ALL")) {
      XmToggleButtonSetState(pe_all_queues_w, True, True);
   } 
   else {
      XmToggleButtonSetState(pe_all_queues_w, False, False);
      UpdateXmListFromCull(pe_queues_w, XmFONTLIST_DEFAULT_TAG, ql, QR_name);
   } 
#endif   

   acl = lGetList(pep, PE_user_list);
   UpdateXmListFromCull(pe_acl_w, XmFONTLIST_DEFAULT_TAG, acl, US_name);
    
   xacl = lGetList(pep, PE_xuser_list);
   UpdateXmListFromCull(pe_xacl_w, XmFONTLIST_DEFAULT_TAG, xacl, US_name);
      
   start_args = lGetString(pep, PE_start_proc_args);
   if (start_args)
      XmtInputFieldSetString(pe_start_w, start_args);

   stop_args = lGetString(pep, PE_stop_proc_args);
   if (stop_args)
      XmtInputFieldSetString(pe_stop_w, stop_args);

   alloc_rule = lGetString(pep, PE_allocation_rule);
   if (alloc_rule)
      XmtInputFieldSetString(pe_alloc_w, alloc_rule);

   XmToggleButtonSetState(pe_control_slaves_w, 
               lGetBool(pep, PE_control_slaves), False);

   XmToggleButtonSetState(pe_job_is_first_task_w, 
               lGetBool(pep, PE_job_is_first_task), False);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static void qmonPEResetAsk(void)
{

   DENTER(GUI_LAYER, "qmonPEResetAsk");

   XmtInputFieldSetString(pe_name_w, "");

   XmpSpinboxSetValue(pe_slots_w, 0, True);

   /*
   ** the lists have to be converted to XmString
   */
#ifdef HOP   
   UpdateXmListFromCull(pe_queues_w, XmFONTLIST_DEFAULT_TAG, NULL, QR_name);
#endif   

   UpdateXmListFromCull(pe_acl_w, XmFONTLIST_DEFAULT_TAG, NULL, US_name);
    
   UpdateXmListFromCull(pe_xacl_w, XmFONTLIST_DEFAULT_TAG, NULL, US_name);
      
   XmtInputFieldSetString(pe_start_w, "/bin/true");

   XmtInputFieldSetString(pe_stop_w, "/bin/true");

   XmtInputFieldSetString(pe_alloc_w, "$pe_slots");

   XmToggleButtonSetState(pe_control_slaves_w, 0, False);

   XmToggleButtonSetState(pe_job_is_first_task_w, 0, False);

   DEXIT;
}

/*-------------------------------------------------------------------------*/
static Boolean qmonPEGetAsk(
lListElem *pep 
) {
   String pe_name = NULL;
   u_long32 pe_slots = 0;
   u_long32 pe_control_slaves = 0;
   u_long32 pe_job_is_first_task = 0;
   lList *ql = NULL;
   lList *acl = NULL;
   lList *xacl = NULL;
   String start_args = NULL;
   String stop_args = NULL;
   String alloc_rule = NULL;

   DENTER(GUI_LAYER, "qmonPEGetAsk");

   if (!pep) {
      DEXIT;
      return False;
   }

   pe_name = XmtInputFieldGetString(pe_name_w);
   if (!pe_name || pe_name[0] == '\0') {
      qmonMessageShow(pe_ask_layout, True, "@{PE name required !}");
      DEXIT;
      return False;
   }
   lSetString(pep, PE_name, qmon_trim(pe_name));
  

   pe_slots = XmpSpinboxGetValue(pe_slots_w);
   lSetUlong(pep, PE_slots, pe_slots);

   /*
   ** XmString entries --> Cull
   */
#ifdef HOP   
   ql = XmStringToCull(pe_queues_w, QR_Type, QR_name, ALL_ITEMS);
   lSetList(pep, PE_queue_list, ql);
   ql_saved = lFreeList(ql_saved);
#endif

   acl = XmStringToCull(pe_acl_w, US_Type, US_name, ALL_ITEMS);
   lSetList(pep, PE_user_list, acl);

   xacl = XmStringToCull(pe_xacl_w, US_Type, US_name, ALL_ITEMS);
   lSetList(pep, PE_xuser_list, xacl);
   
   start_args = XmtInputFieldGetString(pe_start_w);
   if (!start_args || !strcasecmp(start_args, "none"))
      lSetString(pep, PE_start_proc_args, NULL);
   else   
      lSetString(pep, PE_start_proc_args, start_args);

   stop_args = XmtInputFieldGetString(pe_stop_w);
   if (!stop_args || !strcasecmp(stop_args, "none"))
      lSetString(pep, PE_stop_proc_args, NULL);
   else   
      lSetString(pep, PE_stop_proc_args, stop_args);

   alloc_rule = XmtInputFieldGetString(pe_alloc_w);
   lSetString(pep, PE_allocation_rule, alloc_rule);

   pe_control_slaves = XmToggleButtonGetState(pe_control_slaves_w); 
   lSetBool(pep, PE_control_slaves, pe_control_slaves);

   pe_job_is_first_task = XmToggleButtonGetState(pe_job_is_first_task_w); 
   lSetBool(pep, PE_job_is_first_task, pe_job_is_first_task);

   DEXIT;
   return True;
}
