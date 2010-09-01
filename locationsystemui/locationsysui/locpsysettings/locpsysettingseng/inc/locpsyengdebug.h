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
* Description:  Contains debug macros.
*
*/


#ifndef LOCPSYENGDEBUG_H 
#define LOCPSYENGDEBUG_H

// INCLUDES
#include <e32std.h>
#include "locpsyengdebugconfig.hrh"

// MACROS

/**
* Contains debug macros. By using component specific debug macros unnecessary 
* calls to LocUtils.dll can be avoided.
*/

#ifdef LOC_ENABLE_DEBUG_PRINT
#define LOCENGDEBUG(TEXT) DEBUG(TEXT);
#define LOCENGDEBUG1(TEXT, ARG1) LOC(TEXT, ARG1);
#define LOCENGDEBUG2(TEXT, ARG1, ARG2) LOC2(TEXT, ARG1, ARG2);
#define LOCENGDEBUG3(TEXT, ARG1, ARG2, ARG3) LOC3(TEXT, ARG1, ARG2, ARG3);

// MACROS
#define DEBUG(TEXT) 				 { \
									  _LIT( KText, #TEXT ); \
									  Debug(KText); \
									 }
									 
#define LOC(TEXT, ARG1) 			 { \
									  _LIT( KText, #TEXT ); \
									  Debug( KText, (ARG1) ); \
									 }
									 
#define LOC2(TEXT, ARG1, ARG2) 		 { \
									  _LIT( KText, #TEXT );\
									  Debug( KText, (ARG1), (ARG2) );\
									 }
									 
#define LOC3(TEXT, ARG1, ARG2, ARG3) { \
									  _LIT( KText, #TEXT );  \
									  Debug( KText, (ARG1), (ARG2), (ARG3) ); \
									 }


#else
#define LOCENGDEBUG(TEXT);
#define LOCENGDEBUG1(TEXT, ARG1);
#define LOCENGDEBUG2(TEXT, ARG1, ARG2);
#define LOCENGDEBUG3(TEXT, ARG1, ARG2, ARG3);
#endif

IMPORT_C void Debug( TRefByValue<const TDesC> aText, ...);

#endif // LOCPSYENGDEBUG_H
