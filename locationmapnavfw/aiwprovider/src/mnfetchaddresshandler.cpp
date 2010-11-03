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
* Description:  CMnFetchAddressHandler class implementation
*
*/


#include <e32base.h>

#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mngeocoder.h>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"
#include "mnfetchaddresshandler.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnFetchAddressHandler::CMnFetchAddressHandler( CMnProvider& aProvider )
:   CMnGeocodingCmdHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnFetchAddressHandler* CMnFetchAddressHandler::NewL( 
    CMnProvider& aProvider,
    TRunMode aRunMode )
    {
    CMnFetchAddressHandler* self = 
        new (ELeave) CMnFetchAddressHandler( aProvider );
    CleanupStack::PushL( self );        
    self->ConstructL( aRunMode );
    CleanupStack::Pop( self );        
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnFetchAddressHandler::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnFetchAddressHandler::ConstructL in");

    CMnGeocodingCmdHandlerBase::ConstructL( aRunMode );

    LOG("CMnFetchAddressHandler::ConstructL out");
    }

// ================= INTERNAL FUNCTIONS =======================

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnFetchAddressHandler::DoStartL()
    {
    LOG("CMnFetchAddressHandler::DoStartL in");
    
    if ( iLandmarkData.Size() )
        {
        CPosLandmark* lm = PosLandmarkSerialization::UnpackL( iLandmarkData );
        CleanupStack::PushL( lm );

        Geocoder().FindAddressByCoordinateL( *lm, iStatus );
        
        CleanupStack::PopAndDestroy( lm );
        }
    else    
        {
        User::Leave( KErrArgument );
        }

    LOG("CMnFetchAddressHandler::DoStartL out");
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnFetchAddressHandler::RunL()
    {
    LOG("CMnFetchAddressHandler::RunL in");
    
    TInt result = iStatus.Int();
    if ( result == KErrNone )
        {
        CPosLandmark* lm = CPosLandmark::NewLC();
        Geocoder().RetrieveGeocodingResultL( *lm );
        WriteLandmarkL( *lm );
        CleanupStack::PopAndDestroy( lm );
        }
    Done( result );
    LOG("CMnFetchAddressHandler::RunL out");
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnFetchAddressHandler::DoCancel()
    {
    Geocoder().Cancel();
    }
