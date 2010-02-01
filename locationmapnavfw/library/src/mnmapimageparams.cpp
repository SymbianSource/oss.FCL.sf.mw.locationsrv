/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  TMnMapImageParams class implementation
*
*/


#include <e32base.h>
#include <fbs.h>

#include "mnmapimageparams.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TMnMapImageParams::TMnMapImageParams() :
    iAreaWidth( 0 ), 
    iAreaHeight( 0 ), 
    iSize( 0,0 ), 
    iDirection( 0 ),
    iImageType( ETypeUnspecified ),
    iProjectionId( 0 )
    {
    memclr( &reserved, sizeof(reserved) );
    }
