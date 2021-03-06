// User.h: interface for the User class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__5B34943E_8A3E_430F_B830_3A5E241F6373__INCLUDED_)
#define AFX_USER_H__5B34943E_8A3E_430F_B830_3A5E241F6373__INCLUDED_
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include "CBasicTypes.h"
#include "CKernelObject.h"

namespace GridEngine {

class CUser : public CKernelObject {
 private:
   // Password fur user m_Username
   CString m_Password;

   // Username  
   CString m_Username;

 private:
   static BOOL m_bTestOk;
   static BOOL TestOk();

 private:
   virtual void LogonCurrentUser();
   virtual void LogonNewUser();

 public:
   CUser();
	CUser(const CString& Username, const CString& Password);
   CUser(const CUser& User);
	virtual ~CUser();

   virtual CString GetUsername();

   virtual void Impersonate();
   virtual void ReturnToSelf();
};

} // namespace

#endif // !defined(AFX_USER_H__5B34943E_8A3E_430F_B830_3A5E241F6373__INCLUDED_)
