/*
* Copyright (c) 2002,2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  LocAppDebug contains debug macros.
*
*/


#ifndef LOCPSYAPPDEBUG_H 
#define LOCPSYAPPDEBUG_H

// SYSTEM INCLUDES
#include <e32std.h>

// USER INCLUDES
#include "locpsyengdebug.h"

// MACROS

/**
* By using component specific debug macros unnecessary 
* calls to LocUtils.dll can be avoided.
*/

#ifdef LOC_ENABLE_DEBUG_PRINT
#define LOCAPPDEBUG(TEXT) DEBUG(TEXT)
#define LOCAPPDEBUG1(TEXT, ARG1) LOC(TEXT, ARG1)
#define LOCAPPDEBUG2(TEXT, ARG1, ARG2) LOC2(TEXT, ARG1, ARG2)
#define LOCAPPDEBUG3(TEXT, ARG1, ARG2, ARG3) LOC3(TEXT, ARG1, ARG2, ARG3)
#else
#define LOCAPPDEBUG(TEXT)
#define LOCAPPDEBUG1(TEXT, ARG1)
#define LOCAPPDEBUG2(TEXT, ARG1, ARG2)
#define LOCAPPDEBUG3(TEXT, ARG1, ARG2, ARG3)
#endif

#endif // LOCPSYAPPDEBUG_H
