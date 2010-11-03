/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Debug File.
*
*/


#ifndef _LBTMGMTDEBUG_H_
#define _LBTMGMTDEBUG_H_

// INCLUDES
#include <e32std.h>
#include "lbtmgmtdebugconfig.hrh"

// MACROS
#define DEBUG(TEXT) 				 { \
									  _LIT( KText, #TEXT ); \
									   Debug(KText); \
									 }
									 
#define LBTMGMTUI(TEXT, ARG1) 			 { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText, (ARG1) ); \
									 }
									 
#define LBTMGMTUI2(TEXT, ARG1, ARG2) 		 { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText,(ARG1), (ARG2) ); \
									 }
									 
#define LBTMGMTUI3(TEXT, ARG1, ARG2, ARG3) { \
									   _LIT( KText, #TEXT ); \
									   Debug( KText, (ARG1), (ARG2), (ARG3) ); \
									 }

void Debug( TRefByValue<const TDesC> aText, ...);

/**
* By using component specific debug macros unnecessary 
* debug prints can be avoided. 
* Also component specific debugging is possible by compiling 
* only certain components when LBTMGMTUI_ENABLE_DEBUG_PRINT is defined.
*/

#ifdef LBTMGMTUI_ENABLE_DEBUG_PRINT
							 
#define LBTMGMTUIDEBUG(TEXT)                     DEBUG(TEXT);
#define LBTMGMTUIDEBUG1(TEXT, ARG1)              LBTMGMTUI(TEXT, ARG1);
#define LBTMGMTUIDEBUG2(TEXT, ARG1, ARG2)        LBTMGMTUI2(TEXT, ARG1, ARG2);
#define LBTMGMTUIDEBUG3(TEXT, ARG1, ARG2, ARG3)  LBTMGMTUI3(TEXT, ARG1, ARG2, ARG3);

#else

#define LBTMGMTUIDEBUG(TEXT)                     ;
#define LBTMGMTUIDEBUG1(TEXT, ARG1)              ;
#define LBTMGMTUIDEBUG2(TEXT, ARG1, ARG2)        ;
#define LBTMGMTUIDEBUG3(TEXT, ARG1, ARG2, ARG3)  ;

#endif

#endif // _LBTMGMTDEBUG_H_
