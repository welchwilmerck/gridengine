#ifndef __SGE_PAL_H
#define __SGE_PAL_H

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

#include "sge_boundaries.h"
#include "cull.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* *INDENT-OFF* */

/* 
 * this data structures describes a path alias element
 */
enum {
   PA_origin = PA_LOWERBOUND,
   PA_submit_host,
   PA_exec_host,
   PA_translation
};

SLISTDEF(PA_Type, PathAlias)
   /* configuration fields */
   SGE_STRING(PA_origin)
   SGE_HOST(PA_submit_host)       /* CR - hostname change */
   SGE_HOST(PA_exec_host)         /* CR - hostname change */
   SGE_STRING(PA_translation)
LISTEND 

NAMEDEF(PAN)
   NAME("PA_origin")
   NAME("PA_submit_host")
   NAME("PA_exec_host")
   NAME("PA_translation")
NAMEEND

/* *INDENT-ON* */  

#define PAS sizeof(PAN)/sizeof(char*)
#ifdef  __cplusplus
}
#endif
#endif                          /* __SGE_PAL_H */