#ifndef __PARSE_H
#define __PARSE_H
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

/* Group Option Constants */
#define GROUP_TASK_GROUPS              0x00000000
#define GROUP_NO_TASK_GROUPS           0x00000001

char **parse_noopt(char **sp, const char *shortopt, const char *longopt, lList **ppcmdline, lList **alpp);

char **parse_until_next_opt(char **sp, const char *shortopt, const char *longopt, lList **ppcmdline, lList **alpp);

char **parse_until_next_opt2(char **sp, const char *shortopt, const char *longopt, lList **ppcmdline, lList **alpp);

char **parse_param(char **sp, const char *opt, lList **ppcmdline, lList **alpp);

lListElem *sge_add_arg(lList **popt_list, u_long32 opt_number, 
                       u_long32 opt_type, const char *opt_switch, 
                       const char *opt_switch_arg);

lListElem *sge_add_noarg(lList **popt_list, u_long32 opt_number, const char *opt_switch, const char *opt_switch_arg);

int parse_multi_stringlist(lList **ppcmdline, const char *opt, lList **ppal, lList **ppdestlist, lDescr *type, int field);

int parse_flag(lList **ppcmdline, const char *opt, lList **ppal, u_long32 *pflag);

int parse_string(lList **ppcmdline, const char *opt, lList **ppal, char **str);

int parse_multi_jobtaskslist(lList **ppcmdline, const char *opt, lList **ppal, lList **ppdestlist);

void sge_parse_string_list(lList **lp, const char *str, int field, 
                           lDescr *descr);

int sge_unparse_ma_list(lList *head, char *mail_str, unsigned int mail_str_len); 

int sge_parse_jobtasks(lList **lp, lListElem **idp, const char *str, lList **alpp);

u_long32 parse_group_options(lList *string_list);

#endif /* __PARSE_H */