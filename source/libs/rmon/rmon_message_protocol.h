#ifndef __RMON_MESSAGE_PROTOCOL_H
#define __RMON_MESSAGE_PROTOCOL_H
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



#include "rmon_message_list.h"

#define MESSAGE 0
#define EOL     1
#define ERROR   2

typedef struct eol_type {
   u_long n;
   u_long port;
   u_long inet_addr;
} eol_type;

extern eol_type eol;

int rmon_receive_message_list(int sfd, message_list_type **mlp);
int rmon_send_message_list(int sfd, message_list_type *ml, int *count, u_long addr, u_long port);

message_list_type *rmon_receive_message(int sfd, u_long *status);
int rmon_send_eol(int sfd);
int rmon_send_message(int sfd, message_list_type *ml);

#endif /* __RMON_MESSAGE_PROTOCOL_H */


