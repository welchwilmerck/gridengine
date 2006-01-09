#ifndef _SGE_HOSTGROUP_QMASTER_H_
#define _SGE_HOSTGROUP_QMASTER_H_
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



#include "sge_c_gdi.h"


#ifndef __SGE_NO_USERMAPPING__

int hostgrp_success(lListElem *ep, lListElem *old_ep, gdi_object_t *object);

int hostgrp_mod(lList **alpp, lListElem *modp, lListElem *ep, int add, char *ruser, char *rhost, gdi_object_t *object, int sub_command);

int hostgrp_spool(lList **alpp, lListElem *upe, gdi_object_t *object);

int sge_del_hostgrp(lListElem *cep, lList **alpp, char *ruser, char *rhost);

#endif /* __SGE_NO_USERMAPPING__ */


#endif /* _SGE_HOSTGROUP_QMASTER_H_ */
