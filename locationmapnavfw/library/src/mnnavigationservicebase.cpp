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
* Description:  CMnNavigationServiceBase class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>

#include "mndebug.h"
#include "mnerrors.h"
#include "mninternal.h"
#include "mninternal.inl"
#include "mnnavigationservicebase.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnNavigationServiceBase::CMnNavigationServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnNavigationServiceBase::~CMnNavigationServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnNavigationServiceBase::BaseConstructL()
    {
    CMnServiceBase::BaseConstructL();
    }

// ---------------------------------------------------------------------------
//  From class CAknAppServiceBase
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnNavigationServiceBase::ServiceL(const RMessage2& aMessage)
    {
    LOG1("CMnNavigationServiceBase::ServiceL in, function %d", aMessage.Function());
    
    switch ( aMessage.Function() )
        {
        case EMnIpcNavigateTo:
            {
            CPosLandmark* lm = UnpackLandmarkLC( aMessage, EMnIpcNaviLandmarkParamIndex );
            HandleNavigateToL( *lm );
            CleanupStack::PopAndDestroy( lm );
            Complete( aMessage, KErrNone );
            }
            break;
            
        default:
            CMnServiceBase::ServiceL( aMessage );
        }
    }
