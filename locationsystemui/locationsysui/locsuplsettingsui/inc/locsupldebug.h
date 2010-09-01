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
* Description:  Utility class for Debug traces
*
*/


#ifndef LOCSUPL_DEBUG_H
#define LOCSUPL_DEBUG_H

// INCLUDES
#include <e32std.h>
#include "locsupldebugconfig.h"

// MACROS
#ifdef _LOCSUPLDEBUG

#define DEBUG( TEXT) Debug( _L( #TEXT ));
#define DEBUG1( TEXT, ARG1 ) Debug( _L( #TEXT ), ( ARG1 ));
#define DEBUG2( TEXT, ARG1, ARG2 ) Debug( _L( #TEXT ), ( ARG1 ), ( ARG2 ));
#define DEBUG3( TEXT, ARG1, ARG2, ARG3 ) Debug( _L( #TEXT ), ( ARG1 ), ( ARG2 ), ( ARG3 ));

void Debug( TRefByValue<const TDesC> aText, ...);

#else

#define DEBUG( TEXT ) ;
#define DEBUG1( TEXT, ARG1 ) ;
#define DEBUG2( TEXT, ARG1, ARG2 ) ;
#define DEBUG3( TEXT, ARG1, ARG2, ARG3 ) ;

#endif // _LOCSUPLDEBUG

#endif // LOCSUPL_DEBUG_H
