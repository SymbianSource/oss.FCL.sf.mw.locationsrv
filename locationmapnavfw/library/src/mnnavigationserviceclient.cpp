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
* Description:  RMnNavigationServiceClient class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mninternal.h"
#include "mnerrors.h"

#include "mnappserviceuids.hrh"
#include "mnnavigationserviceclient.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnNavigationServiceClient::RMnNavigationServiceClient()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnNavigationServiceClient::~RMnNavigationServiceClient()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnNavigationServiceClient::NavigateToL( const CPosLandmark& aDestinationLanmdark )
    {
    LOG("RMnNavigationServiceClient::NavigateToL");
    TIpcArgs args;
    
    // landmark
    HBufC8* lmBuf = PosLandmarkSerialization::PackL( aDestinationLanmdark ); 
    CleanupStack::PushL( lmBuf );

    args.Set( EMnIpcNaviLandmarkParamIndex, lmBuf );
    
    User::LeaveIfError( SendReceive( EMnIpcNavigateTo, args ) );
    CleanupStack::PopAndDestroy( lmBuf );
    }

// ---------------------------------------------------------------------------
// From class RApaAppServiceBase.
// ---------------------------------------------------------------------------
//
TUid RMnNavigationServiceClient::ServiceUid() const
    { 
    return TUid::Uid(KMnAppNavigationService); 
    }
