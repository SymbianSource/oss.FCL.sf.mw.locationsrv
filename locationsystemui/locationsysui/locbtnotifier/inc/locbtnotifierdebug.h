/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Defines debug macros for locbtnotifier
*
*/


#ifndef LOCBTNOTIFIERDEBUG_H
#define LOCBTNOTIFIERDEBUG_H

// INCLUDES
#include <e32std.h>
#include "locbtnotifierdebugconfig.hrh"



// Macros

#define DEBUGLOCBTNOTIFIER(TEXT) 			 { \
									  		 _LIT( KText, #TEXT ); \
									   		 DebugLocBtNotifier(KText); \
									 		 }
									 
#define LOGLOCBTNOTIFIER(TEXT, ARG1) 		 { \
									         _LIT( KText, #TEXT ); \
									         DebugLocBtNotifier( KText, (ARG1) ); \
									         }


/**
* Logs messages into the c:\logs\locbtnotifier\locbtnotifier.txt file
* provided the folder c:\logs\locbtnotifier\ exists.
*/
void DebugLocBtNotifier( TRefByValue<const TDesC> aText, ... );


/**
* Logging is enabled only when LOG_ENABLED_LOCBTNOTIFIER is defined
*/
#ifdef LOG_ENABLED_LOCBTNOTIFIER

#define LOCBTNOTIFIERDEBUG(TEXT)                     DEBUGLOCBTNOTIFIER(TEXT)
#define LOCBTNOTIFIERDEBUG1(TEXT, ARG1)              LOGLOCBTNOTIFIER(TEXT, ARG1)

#else

#define LOCBTNOTIFIERDEBUG(TEXT) 
#define LOCBTNOTIFIERDEBUG1(TEXT, ARG1) 

#endif


#endif      // LOCBTNOTIFIERDEBUG_H



// End of file