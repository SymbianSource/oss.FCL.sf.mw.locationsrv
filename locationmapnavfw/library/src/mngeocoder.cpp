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
* Description:  CMnGeocoder class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <AknServerApp.h>
#include <apaserverapp.h>

#include <lbsfields.h>
#include <lbsfieldids.h>
#include <lbsposition.h>

#include <EPos_CPosLandmark.h>

#include "mnprovider.h"
#include "mngeocodingserviceclient.h"
#include "mngeocoder.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
//  Copies address information (address and building position fields) from 
//  source landmark to target.
// ---------------------------------------------------------------------------
//
void CopyAddressL( CPosLandmark& aSrc, CPosLandmark& aTrg )
    {
    // clean address fields (CMnGeocoder says that "all <data> will be updated")
    for ( TPositionFieldId fieldId = EPositionFieldAddressCapabilitiesBegin + 1;
          fieldId < EPositionFieldNMEACapabilitiesBegin;
          fieldId++ )
        {
        aTrg.RemovePositionField( fieldId );
        }
    
    // copy fields
    TPositionFieldId fieldId = aSrc.FirstPositionFieldId();
    while ( fieldId != EPositionFieldNone )
        {
        if ( fieldId > EPositionFieldAddressCapabilitiesBegin &&
             fieldId < EPositionFieldNMEACapabilitiesBegin)
            {
            TPtrC field;
            if ( KErrNone == aSrc.GetPositionField( fieldId, field ) )
                {
                aTrg.SetPositionFieldL( fieldId, field );
                }
            }
        fieldId = aSrc.NextPositionFieldId( fieldId );            
        }
    }

// ---------------------------------------------------------------------------
//  Copies coordinate information (locality and coverage) from 
//  source landmark to target. If some fields are empty in source,
//  they will also be emptied in result.
// ---------------------------------------------------------------------------
//
void CopyCoordinatesL( CPosLandmark& aSrc, CPosLandmark& aTrg )
    {
    TLocality loc;
    TInt posErr = aSrc.GetPosition( loc );
    if( !posErr )
    	{
    	aTrg.SetPositionL( loc );	
    	}
    
    TRealX nan;
    nan.SetNaN();
    TReal32 coverage( nan );
    
    TInt radiusErr = aSrc.GetCoverageRadius( coverage );
    if( !radiusErr )
    	{
    	aTrg.SetCoverageRadius( coverage );
    	}
    
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnGeocoder::CMnGeocoder()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocoder::ConstructCommonL( CMnProvider& aProvider )
    {
    if ( !( aProvider.SupportedServices() & CMnProvider::EServiceGeocoding ) )
        {
        User::Leave( KErrNotSupported );
        }

    iSession = new (ELeave) RMnGeocodingServiceClient;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocoder::ConstructNewL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectL( aProvider.Uid() );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocoder::ConstructChainedL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectChainedL( aProvider.Uid() );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocoder* CMnGeocoder::NewL( CMnProvider& aProvider )
    {
    CMnGeocoder* self = new (ELeave) CMnGeocoder;
    CleanupStack::PushL( self );
    self->ConstructNewL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocoder* CMnGeocoder::NewChainedL( CMnProvider& aProvider )
    {
    CMnGeocoder* self = new (ELeave) CMnGeocoder;
    CleanupStack::PushL( self );
    self->ConstructChainedL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocoder::~CMnGeocoder()
    {
	delete iExitMonitor;

    if ( iSession )
        {
        iSession->Close();
        delete iSession;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocoder::TOptions CMnGeocoder::Options() const
    {
    return iOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::SetOptions( TOptions aOptions )
    {
    iOptions = aOptions;
    }
        
// ---------------------------------------------------------------------------
//  1. Get result from session in form of landmark
//  2. If last request was for address, copy all position fields from 
//     result to target landmark
//  3. If last request was for coordinate, copy position data.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::RetrieveGeocodingResultL( CPosLandmark& aTargetLandmark )
    {
    CPosLandmark* lm = NULL;
    TMnGeocodingResultType resultType; 
    iSession->GetLastResultL( lm, resultType );
    CleanupStack::PushL( lm );
    
    switch ( resultType )
        {
        case EMnGeoResultAddress:
            CopyAddressL( *lm, aTargetLandmark );
            break;
        case EMnGeoResultCoordinates:
            CopyCoordinatesL( *lm, aTargetLandmark );
            break;
        }
    CleanupStack::PopAndDestroy( lm );
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::FindCoordinateByAddressL(
    const CPosLandmark& aLandmark, 
    TRequestStatus& aRequest)
    {
    iSession->CoordinateByAddressL( aLandmark, iOptions, aRequest );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::FindCoordinateByAddressL(
    const TDesC& aAddress, 
    TRequestStatus& aRequest)
    {
    iSession->CoordinateByAddressL( aAddress, iOptions, aRequest );
    }
            
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::FindAddressByCoordinateL(
    const CPosLandmark& aLandmark, 
    TRequestStatus& aRequest)
    {
    iSession->AddressByCoordinateL( aLandmark, iOptions, aRequest );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::Cancel()
    {
    iSession->Cancel();
    }
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::SetExitObserverL( MAknServerAppExitObserver& aObserver )
	{
	if ( iExitMonitor )
		{
		User::Leave( KErrAlreadyExists );
		}
	else
		{
		iExitMonitor = CApaServerAppExitMonitor::NewL( 
			*iSession, aObserver, CActive::EPriorityStandard );
		}
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocoder::RemoveExitObserver()
	{
	delete iExitMonitor;
	iExitMonitor = NULL;
	}
