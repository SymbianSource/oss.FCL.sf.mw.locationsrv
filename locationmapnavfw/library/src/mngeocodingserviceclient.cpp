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
* Description:  RMnGeocodingServiceClient class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mninternal.h"
#include "mnappserviceuids.hrh"

#include "mnprovider.h"
#include "mngeocoder.h"
#include "mngeocodingserviceclient.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnGeocodingServiceClient::RMnGeocodingServiceClient()
  : iResultSizePtr( iResultSize ) 
    {
    iResultSize = 0;
    iLandmarkBuffer = NULL;
    iPlainAddress = NULL;
    iLastRequest = EMnIpcNone;
    }

RMnGeocodingServiceClient::~RMnGeocodingServiceClient()
    {
    delete iLandmarkBuffer;
    delete iPlainAddress;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnGeocodingServiceClient::AddressByCoordinateL(
    const CPosLandmark& aCoordinateLanmdark,
    CMnGeocoder::TOptions aOptions,
    TRequestStatus& aStatus)
    {
    LOG("RMnGeocodingServiceClient::AddressByCoordinateL");
    TIpcArgs args;
    
    // options
    iOptions = aOptions;
    TPckg<CMnGeocoder::TOptions> optsPack( iOptions );
    iOptionsPtr.Set( optsPack );
    args.Set( EMnIpcGeoOptionsParamIndex, &iOptionsPtr );

    // landmark
    delete iLandmarkBuffer;
    iLandmarkBuffer = NULL;
    iLandmarkBuffer = PosLandmarkSerialization::PackL( aCoordinateLanmdark );
    args.Set( EMnIpcGeoLandmarkParamIndex, iLandmarkBuffer );
    
    // size of result
    args.Set( EMnIpcGeoResultSizeParamIndex, &iResultSizePtr );
    
    SendReceive( EMnIpcCoordToAddress, args, aStatus );
    iLastRequest = EMnIpcCoordToAddress;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnGeocodingServiceClient::CoordinateByAddressL(
    const CPosLandmark& aAddressLanmdark,
    CMnGeocoder::TOptions aOptions,
    TRequestStatus& aStatus )
    {
    LOG("RMnGeocodingServiceClient::CoordinateByAddressL");
    TIpcArgs args;
    
    // options
    iOptions = aOptions;
    TPckg<CMnGeocoder::TOptions> optsPack( iOptions );
    iOptionsPtr.Set( optsPack );
    args.Set( EMnIpcGeoOptionsParamIndex, &iOptionsPtr );

    // landmark
    delete iLandmarkBuffer;
    iLandmarkBuffer = NULL;
    iLandmarkBuffer = PosLandmarkSerialization::PackL( aAddressLanmdark );
    args.Set( EMnIpcGeoLandmarkParamIndex, iLandmarkBuffer );
    
    // size of result
    args.Set( EMnIpcGeoResultSizeParamIndex, &iResultSizePtr );
    
    SendReceive( EMnIpcAddressToCoord, args, aStatus );
    iLastRequest = EMnIpcAddressToCoord;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnGeocodingServiceClient::CoordinateByAddressL(
    const TDesC& aAddress,
    CMnGeocoder::TOptions aOptions,
    TRequestStatus& aStatus )
    {
    LOG("RMnGeocodingServiceClient::CoordinateByAddressL");
    TIpcArgs args;
    
    // options
    iOptions = aOptions;
    TPckg<CMnGeocoder::TOptions> optsPack( iOptions );
    iOptionsPtr.Set( optsPack );
    args.Set( EMnIpcGeoOptionsParamIndex, &iOptionsPtr );

    // landmark
    delete iPlainAddress;
    iPlainAddress = NULL;
    iPlainAddress = aAddress.AllocL();
    args.Set( EMnIpcGeoPlainAddressParamIndex, iPlainAddress );
    
    // size of result
    args.Set( EMnIpcGeoResultSizeParamIndex, &iResultSizePtr );
    
    SendReceive( EMnIpcPlainAddressToCoord, args, aStatus );
    iLastRequest = EMnIpcPlainAddressToCoord;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnGeocodingServiceClient::GetLastResultL( 
    CPosLandmark*& aResult,
    TMnGeocodingResultType& aResultType )
    {
    LOG("RMnGeocodingServiceClient::GetLastResultL in");
    TIpcArgs args;
    
    if ( iResultSize <= 0 )
        {
        User::Leave( KErrNotFound );
        }
    
    // receiving buffer
    HBufC8* lmBuf = HBufC8::NewLC( iResultSize );
    TPtr8 lmBufPtr( lmBuf->Des() );
    args.Set( EMnIpcGeoResultParamIndex, &lmBufPtr );
    
    TPckg<TMnGeocodingResultType> resultTypePack( aResultType );
    args.Set( EMnIpcGeoResultTypeParamIndex, &resultTypePack);
    
    TInt err = SendReceive( EMnIpcGetConversionResult, args );
    User::LeaveIfError( err );
    
    CPosLandmark* lm = PosLandmarkSerialization::UnpackL( *lmBuf );
    aResult = lm;
    CleanupStack::PopAndDestroy( lmBuf );
    LOG("RMnGeocodingServiceClient::GetLastResultL out");
    }

// ---------------------------------------------------------------------------
// From class RApaAppServiceBase.
// ---------------------------------------------------------------------------
//
EXPORT_C TUid RMnGeocodingServiceClient::ServiceUid() const
    { 
    LOG("RMnGeocodingServiceClient::ServiceUid");
    return TUid::Uid( KMnAppGeocodingService ); 
    }
