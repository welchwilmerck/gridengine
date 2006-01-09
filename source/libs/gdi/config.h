#ifndef _CONFIG_H_
#define _CONFIG_H_
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

#ifndef __CULL_H
#   include "cull.h"
#endif

#include "cull_parse_util.h"

enum {
   MODE_RELATIVE,
   MODE_SET,
   MODE_ADD,
   MODE_SUB
};

int add_nm_to_set(int fields[], int name_nm);

enum {
   RCL_NO_VALUE = 0x0001
};

int read_config_list(FILE *fp, lList **clpp, lList **alpp, lDescr *dp, int nm1, int nm2, int nm3, char *delimitor, int flag, char *buf, int size);

char *get_conf_value(lList **alpp, lList *lp, int name_nm, int value_nm, const char *key);

lList *get_conf_sublist(lList **alpp, lList *lp, int name_nm, int value_nm, const char *key);

int set_conf_string(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm);

int set_conf_bool(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm);

int set_conf_ulong(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm);

int set_conf_double(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm, int operation_nm);

int set_conf_deflist(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm, lDescr *descr, intprt_type *interpretation_rule);

int set_conf_timestr(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm);

int set_conf_memstr(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm);

int set_conf_enum(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm, char **enum_strings);

int set_conf_list(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm, lDescr *descr, int sub_name_nm);

int set_conf_subordlist(lList **alpp, lList **clpp, int fields[], const char *key, lListElem *ep, int name_nm, lDescr *descr, int subname_nm, int subval_nm);

#endif /* _CONFIG_H_ */