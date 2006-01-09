#ifndef __SGE_BOUNDARIES_H
#define __SGE_BOUNDARIES_H
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

/*----------------------------------------------------
 *
 * 1. sge_boundaries.h
 * 
 * 	is included by every file of group 2 
 * 	defines boundaries of all lists
 * 
 * 2. sge_jobL.h sge_queueL.h
 * 
 * 	is used to define a specific list like:
 * 	- job list
 * 	- queue list
 * 
 *--------------------------------------------------*/

#ifdef  __cplusplus
extern "C" {
#endif

#define BASIC_UNIT 50         /* Don't touch */
enum NameSpaceBoundaries {

   /* cull version of sge_list_type */
   /* is finally removed, first 50 are free now */
      
   /* job list */
   JB_LOWERBOUND = 1*BASIC_UNIT,
   JB_UPPERBOUND = JB_LOWERBOUND + 4*BASIC_UNIT - 1,

   /* queue list */
   QU_LOWERBOUND = JB_UPPERBOUND + 1,
   QU_UPPERBOUND = QU_LOWERBOUND + 4*BASIC_UNIT - 1,

   /* exec host list */
   EH_LOWERBOUND = QU_UPPERBOUND + 1,
   EH_UPPERBOUND = EH_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* admin host list */
   AH_LOWERBOUND = EH_UPPERBOUND + 1,
   AH_UPPERBOUND = AH_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* submit host list */
   SH_LOWERBOUND = AH_UPPERBOUND + 1,
   SH_UPPERBOUND = SH_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* answer list for gdi requests */
   AN_LOWERBOUND = SH_UPPERBOUND + 1,
   AN_UPPERBOUND = AN_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* host aliases list */
   HA_LOWERBOUND = AN_UPPERBOUND + 1,
   HA_UPPERBOUND = HA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* host load list */
   HL_LOWERBOUND = HA_UPPERBOUND + 1,
   HL_UPPERBOUND = HL_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* host scaling list */
   HS_LOWERBOUND = HL_UPPERBOUND + 1,
   HS_UPPERBOUND = HS_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* host scaling list */
   HT_LOWERBOUND = HS_UPPERBOUND + 1,
   HT_UPPERBOUND = HT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* clients getting events sent */
   EV_LOWERBOUND = HT_UPPERBOUND + 1,
   EV_UPPERBOUND = EV_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* a certain entry of the complex-list */
   CE_LOWERBOUND = EV_UPPERBOUND + 1,
   CE_UPPERBOUND = CE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* list of complexes */ 
   CX_LOWERBOUND = CE_UPPERBOUND + 1,
   CX_UPPERBOUND = CX_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* order list */ 
   OR_LOWERBOUND = CX_UPPERBOUND + 1,
   OR_UPPERBOUND = OR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* entries for OR_queuelist-field */
   OQ_LOWERBOUND = OR_UPPERBOUND + 1,
   OQ_UPPERBOUND = OQ_LOWERBOUND + 1*BASIC_UNIT - 1,

  /* the request-list */ 
   RQ_LOWERBOUND = OQ_UPPERBOUND + 1,
   RQ_UPPERBOUND = RQ_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* an entry */
   RE_LOWERBOUND = RQ_UPPERBOUND + 1,
   RE_UPPERBOUND = RE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* a list for the to handle the range */
   RR_LOWERBOUND = RE_UPPERBOUND + 1,
   RR_UPPERBOUND = RR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* structure of a load report */
   LR_LOWERBOUND = RR_UPPERBOUND + 1,
   LR_UPPERBOUND = LR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* (x)access-lists for the queues */
   US_LOWERBOUND = LR_UPPERBOUND + 1,
   US_UPPERBOUND = US_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* (x)access-lists entries for the queues */
   UE_LOWERBOUND = US_UPPERBOUND + 1,
   UE_UPPERBOUND = UE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* range list */
   RN_LOWERBOUND = UE_UPPERBOUND + 1,
   RN_UPPERBOUND = RN_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* path name list */
   PN_LOWERBOUND = RN_UPPERBOUND + 1,
   PN_UPPERBOUND = PN_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* account list */
   AT_LOWERBOUND = PN_UPPERBOUND + 1,
   AT_UPPERBOUND = AT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* variable list */
   VA_LOWERBOUND = AT_UPPERBOUND + 1,
   VA_UPPERBOUND = VA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* mail list */
   MR_LOWERBOUND = VA_UPPERBOUND + 1,
   MR_UPPERBOUND = MR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* manager and operator list */
   MO_LOWERBOUND = MR_UPPERBOUND + 1,
   MO_UPPERBOUND = MO_LOWERBOUND + 1*BASIC_UNIT - 1,
   
   /* the event itself */
   ET_LOWERBOUND = MO_UPPERBOUND + 1,
   ET_UPPERBOUND = ET_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* config list */
   CONF_LOWERBOUND = ET_UPPERBOUND + 1,
   CONF_UPPERBOUND = CONF_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler configuration list */
   CFSCHED_LOWERBOUND = CONF_UPPERBOUND + 1,
   CFSCHED_UPPERBOUND = CFSCHED_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* the configuration itself */
   CF_LOWERBOUND = CFSCHED_UPPERBOUND + 1,
   CF_UPPERBOUND = CF_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* the parallel environmet */
   PE_LOWERBOUND = CF_UPPERBOUND + 1,
   PE_UPPERBOUND = PE_LOWERBOUND + 1*BASIC_UNIT - 1,
   
   /* queue references - a sublist containing qnames (used in the pe object) */
   QR_LOWERBOUND = PE_UPPERBOUND + 1,
   QR_UPPERBOUND = QR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* job counter used in schedd to keep number of jobs per user/group */
   JC_LOWERBOUND = QR_UPPERBOUND + 1,
   JC_UPPERBOUND = JC_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* combined job/queue list as needed in execd to hold jobs/queues */
   JQ_LOWERBOUND = JC_UPPERBOUND + 1,
   JQ_UPPERBOUND = JQ_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* string list */
   ST_LOWERBOUND = JQ_UPPERBOUND + 1,
   ST_UPPERBOUND = ST_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* jobs granted destination identifier list */
   JG_LOWERBOUND = ST_UPPERBOUND + 1,
   JG_UPPERBOUND = JG_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* history directory list */
   HD_LOWERBOUND = JG_UPPERBOUND + 1,
   HD_UPPERBOUND = HD_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* history file list */
   HF_LOWERBOUND = HD_UPPERBOUND + 1,
   HF_UPPERBOUND = HF_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* list for sorted output of job sums with QACCT */
   QAJ_LOWERBOUND = HF_UPPERBOUND + 1,
   QAJ_UPPERBOUND = QAJ_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* argument list for limited parser */
   SPA_LOWERBOUND = QAJ_UPPERBOUND + 1,
   SPA_UPPERBOUND = SPA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* report list */
   REP_LOWERBOUND = SPA_UPPERBOUND + 1,
   REP_UPPERBOUND = REP_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* list of subordinated queues */
   SO_LOWERBOUND = REP_UPPERBOUND + 1,
   SO_UPPERBOUND = SO_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* usage list */
   UA_LOWERBOUND = SO_UPPERBOUND + 1,
   UA_UPPERBOUND = UA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* list of users/projects */
   UP_LOWERBOUND = UA_UPPERBOUND + 1,
   UP_UPPERBOUND = UP_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* list of share tree nodes */
   STN_LOWERBOUND = UP_UPPERBOUND + 1,
   STN_UPPERBOUND = STN_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler config */
   SC_LOWERBOUND = STN_UPPERBOUND + 1,
   SC_UPPERBOUND = SC_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* no longer used, just to fix the order of nms */
   YYYCONF_LOWERBOUND = SC_UPPERBOUND + 1,
   YYYCONF_UPPERBOUND = YYYCONF_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* job report */
   JR_LOWERBOUND = YYYCONF_UPPERBOUND + 1,
   JR_UPPERBOUND = JR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* license report */
   LIC_LOWERBOUND = JR_UPPERBOUND + 1,
   LIC_UPPERBOUND = LIC_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* deadline structure */
   DL_LOWERBOUND = LIC_UPPERBOUND + 1,
   DL_UPPERBOUND = DL_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* ptf job list */
   JL_LOWERBOUND = DL_UPPERBOUND + 1,
   JL_UPPERBOUND = JL_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* ptf pid list */
   JP_LOWERBOUND = JL_UPPERBOUND + 1,
   JP_UPPERBOUND = JP_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* user/project sublist job usage */
   UPU_LOWERBOUND = JP_UPPERBOUND + 1,
   UPU_UPPERBOUND = UPU_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* checkpointing object */
   CK_LOWERBOUND = UPU_UPPERBOUND + 1,
   CK_UPPERBOUND = CK_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* kerberos connection list  */
   KRB_LOWERBOUND = CK_UPPERBOUND + 1,
   KRB_UPPERBOUND = KRB_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* path alias list */
   PA_LOWERBOUND = KRB_UPPERBOUND + 1,
   PA_UPPERBOUND = PA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* obsolete - reuse me */
   CS_LOWERBOUND = PA_UPPERBOUND + 1,
   CS_UPPERBOUND = CS_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* obsolete - reuse me */
   CO_LOWERBOUND = CS_UPPERBOUND + 1,
   CO_UPPERBOUND = CO_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* job reference */
   JRE_LOWERBOUND = CO_UPPERBOUND + 1,
   JRE_UPPERBOUND = JRE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* id struct used for qmod request */
   ID_LOWERBOUND = JRE_UPPERBOUND + 1,
   ID_UPPERBOUND = ID_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* multi gdi struct used for combined gdi requests */
   MA_LOWERBOUND = ID_UPPERBOUND + 1,
   MA_UPPERBOUND = MA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* multi gdi struct used for combined gdi requests */
   TE_LOWERBOUND = MA_UPPERBOUND + 1,
   TE_UPPERBOUND = TE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* CAL, CA, TMR and TM are for calendar management */
   /* SGE calendar object */
   CAL_LOWERBOUND = TE_UPPERBOUND + 1,
   CAL_UPPERBOUND = CAL_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* parsed calendar week or year */
   CA_LOWERBOUND = CAL_UPPERBOUND + 1,
   CA_UPPERBOUND = CA_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* time range */
   TMR_LOWERBOUND = CA_UPPERBOUND + 1,
   TMR_UPPERBOUND = TMR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* cullified struct tm */
   TM_LOWERBOUND = TMR_UPPERBOUND + 1,
   TM_UPPERBOUND = TM_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* remote tasks */
   RT_LOWERBOUND = TM_UPPERBOUND + 1,
   RT_UPPERBOUND = RT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* user/project sublist project usage */
   UPP_LOWERBOUND = RT_UPPERBOUND + 1,
   UPP_UPPERBOUND = UPP_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* ptf osjobid sublist */
   JO_LOWERBOUND = UPP_UPPERBOUND + 1,
   JO_UPPERBOUND = JO_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* Kerberos TGT sublist */
   KTGT_LOWERBOUND = JO_UPPERBOUND + 1,
   KTGT_UPPERBOUND = KTGT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler information messages */
   SME_LOWERBOUND = KTGT_UPPERBOUND + 1,
   SME_UPPERBOUND = SME_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler job information element */
   MES_LOWERBOUND = SME_UPPERBOUND + 1,
   MES_UPPERBOUND = MES_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* JobArray task structure */
   JAT_LOWERBOUND = MES_UPPERBOUND +1,
   JAT_UPPERBOUND = JAT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler job category element */
   CT_LOWERBOUND = JAT_UPPERBOUND + 1,
   CT_UPPERBOUND = CT_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler access tree: priority group */
   PGR_LOWERBOUND = CT_UPPERBOUND + 1,
   PGR_UPPERBOUND = PGR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler access tree: user */
   USR_LOWERBOUND = PGR_UPPERBOUND + 1,
   USR_UPPERBOUND = USR_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler access tree: job reference list */
   JRL_LOWERBOUND = USR_UPPERBOUND + 1,
   JRL_UPPERBOUND = JRL_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* scheduler sge job sort element */
   SGEJ_LOWERBOUND = CT_UPPERBOUND + 1,
   SGEJ_UPPERBOUND = SGEJ_LOWERBOUND + 1*BASIC_UNIT - 1,

   ULNG_LOWERBOUND = SGEJ_UPPERBOUND + 1,
   ULNG_UPPERBOUND = ULNG_LOWERBOUND + 1*BASIC_UNIT - 1,

   PREF_LOWERBOUND = ULNG_UPPERBOUND + 1,
   PREF_UPPERBOUND = PREF_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* group list */
   GRP_LOWERBOUND = PREF_UPPERBOUND + 1,
   GRP_UPPERBOUND = GRP_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* permissions list */
   PERM_LOWERBOUND = GRP_UPPERBOUND + 1,
   PERM_UPPERBOUND = PERM_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* user mapping entry list */
   UME_LOWERBOUND = PERM_UPPERBOUND + 1,
   UME_UPPERBOUND = UME_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* load sensor list */
   LS_LOWERBOUND = UME_UPPERBOUND + 1,
   LS_UPPERBOUND = LS_LOWERBOUND + 1*BASIC_UNIT - 1,  

   /* user mapping list */
   UM_LOWERBOUND = LS_UPPERBOUND + 1,
   UM_UPPERBOUND = UM_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* user unknown list */
   RU_LOWERBOUND = UM_UPPERBOUND + 1,
   RU_UPPERBOUND = RU_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* feature sets */
   FES_LOWERBOUND = RU_UPPERBOUND + 1,
   FES_UPPERBOUND = FES_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* feature list */
   FE_LOWERBOUND = FES_UPPERBOUND + 1,
   FE_UPPERBOUND = FE_LOWERBOUND + 1*BASIC_UNIT - 1,

   /* submit user */
   SU_LOWERBOUND = FE_UPPERBOUND + 1,
   SU_UPPERBOUND = SU_LOWERBOUND + 1*BASIC_UNIT - 1

   /* CHANGE LAST_UPPERBOUND below !!!!!!!!!!!!! */
      
   /* security connection list  */
  ,SEC_LOWERBOUND = FE_UPPERBOUND + 1,
   SEC_UPPERBOUND = SEC_LOWERBOUND + 1*BASIC_UNIT - 1

#ifdef ENABLE_438_FIX 
   /* finished pe task info */
  ,FJR_LOWERBOUND = SEC_UPPERBOUND + 1,
   FJR_UPPERBOUND = FJR_LOWERBOUND + 1 * BASIC_UNIT -1,

   FTR_LOWERBOUND = FJR_UPPERBOUND + 1,
   FTR_UPPERBOUND = FTR_LOWERBOUND + 1 * BASIC_UNIT -1,

   FPR_LOWERBOUND = FTR_UPPERBOUND + 1,
   FPR_UPPERBOUND = FPR_LOWERBOUND + 1 * BASIC_UNIT -1
   
#  define LAST_UPPERBOUND FPR_UPPERBOUND
#else    
#  define LAST_UPPERBOUND SEC_UPPERBOUND
#endif /* ENABLE_438_FIX */

   /* security connection list  */
  ,FCAT_LOWERBOUND = LAST_UPPERBOUND + 1,
   FCAT_UPPERBOUND = FCAT_LOWERBOUND + 1*BASIC_UNIT - 1


#ifdef QIDL
/* this is just a fake boundary. there is no QIDL cull */
/* i only need this to reserve some space for QIDL-only */
/* fields in other culls (those beginning with SGE_I* */
/* i suggest leaving these entries ALWAYS at the bottom */
/* of this enum. don't append anything behind this */
  ,QIDL_LOWERBOUND = FCAT_UPPERBOUND + 1,
   QIDL_UPPERBOUND = QIDL_LOWERBOUND + 2*BASIC_UNIT - 1
#endif
};

#ifdef  __cplusplus
}
#endif

#endif /* __SGE_BOUNDARIES_H */