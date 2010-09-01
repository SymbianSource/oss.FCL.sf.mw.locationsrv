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
* Description:  CMnFetchCoordinatesHandler class implementation
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
#include "mnfetchcoordinateshandler.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnFetchCoordinatesHandler::CMnFetchCoordinatesHandler( CMnProvider& aProvider )
:   CMnGeocodingCmdHandlerBase( aProvider )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnFetchCoordinatesHandler* CMnFetchCoordinatesHandler::NewL( 
    CMnProvider& aProvider,
    TRunMode aRunMode )
    {
    CMnFetchCoordinatesHandler* self = 
        new (ELeave) CMnFetchCoordinatesHandler( aProvider );
    CleanupStack::PushL( self );        
    self->ConstructL( aRunMode );
    CleanupStack::Pop( self );        
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::ConstructL( TRunMode aRunMode )
    {
    LOG("CMnFetchCoordinatesHandler::ConstructL in");

    CMnGeocodingCmdHandlerBase::ConstructL( aRunMode );

    LOG("CMnFetchCoordinatesHandler::ConstructL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::DoStartL()
    {
    LOG("CMnFetchCoordinatesHandler::DoStartL in");
    
    if ( iLandmarkData.Size() )
        {
        CPosLandmark* lm = PosLandmarkSerialization::UnpackL( iLandmarkData );
        CleanupStack::PushL( lm );

        Geocoder().FindCoordinateByAddressL( *lm, iStatus );
        
        CleanupStack::PopAndDestroy( lm );
        }
    else if ( iPlainAddress.Length() )
        {
        Geocoder().FindCoordinateByAddressL( iPlainAddress, iStatus );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
    LOG("CMnFetchCoordinatesHandler::DoStartL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::RunL()
    {
    LOG("CMnFetchCoordinatesHandler::RunL in");
    
    TInt result = iStatus.Int();
    if ( result == KErrNone )
        {
        CPosLandmark* lm = CPosLandmark::NewLC();
        Geocoder().RetrieveGeocodingResultL( *lm );
        WriteLandmarkL( *lm );
        CleanupStack::PopAndDestroy( lm );
        }
    Done( result );
    LOG("CMnFetchCoordinatesHandler::RunL out");
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::DoCancel()
    {
    Geocoder().Cancel();
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::HandleAiwParamL( const TAiwGenericParam& aParam )
    {
    if ( aParam.SemanticId() == EGenericParamUnspecified &&
         aParam.Value().TypeId() == EVariantTypeDesC )
        {
        LOG("CMnFetchCoordinatesHandler::HandleAiwParam, PlainString");
        if ( !iPlainAddress.Length() && !iLandmarkData.Size() )
            {
            iPlainAddress.Set( aParam.Value().AsDes() );
            }
        }
    else
        {
        CMnGeocodingCmdHandlerBase::HandleAiwParamL( aParam );
        }
    }

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList )
    {
    // ignore landmarks if plain address is already found
    if ( !iPlainAddress.Length() )
        {
        CMnGeocodingCmdHandlerBase::HandleAiwParamLandmarksL( aLandmarkDataList );
        }
    }    

// -----------------------------------------------------------------------------
//  From class CMnAiwCommandHandlerBase
// -----------------------------------------------------------------------------
//
void CMnFetchCoordinatesHandler::ResetParametersL()
    {
    iPlainAddress.Set( NULL, 0 );
    CMnGeocodingCmdHandlerBase::ResetParametersL();
    }
