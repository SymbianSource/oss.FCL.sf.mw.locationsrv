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
* Description:  CMnGeocodingServiceBase class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mnerrors.h"
#include "mninternal.h"
#include "mninternal.inl"

#include "mngeocodingservicebase.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocodingServiceBase::CMnGeocodingServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocodingServiceBase::~CMnGeocodingServiceBase()
    {
    delete iResult;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocodingServiceBase::BaseConstructL()
    {
    CMnServiceBase::BaseConstructL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnGeocoder::TOptions CMnGeocodingServiceBase::Options() const 
    {
    LOG("CMnGeocodingServiceBase::Options");
    return iOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocodingServiceBase::CompleteGeocodingRequestL( 
    const CPosLandmark& aLandmark )
    {
    LOG("CMnGeocodingServiceBase::CompleteGeocodingRequestL in");
    switch ( CurrentAsyncRequest() )
        {
        case EMnIpcCoordToAddress:
            iResultType = EMnGeoResultAddress;
            break;
            
        case EMnIpcAddressToCoord:
        case EMnIpcPlainAddressToCoord:
            iResultType = EMnGeoResultCoordinates;
            break;
        
        default:
            PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }

    delete iResult;
    iResult = NULL;
    iResult = PosLandmarkSerialization::PackL( aLandmark );
    
    TInt resultSize = iResult->Size();
    TPckg<TInt> resultSizePack( resultSize );
    TInt err = iMessage.Write( EMnIpcGeoResultSizeParamIndex, resultSizePack );
    CompleteRequest( err );

    LOG("CMnGeocodingServiceBase::CompleteGeocodingRequestL out");
    }
    
// ---------------------------------------------------------------------------
//  From class CAknAppServiceBase
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnGeocodingServiceBase::ServiceL( const RMessage2& aMessage )
    {
    LOG1("CMnGeocodingServiceBase::ServiceL in, function %d", aMessage.Function());
    
    switch ( aMessage.Function() )
        {
        case EMnIpcCoordToAddress:
        case EMnIpcAddressToCoord:
        case EMnIpcPlainAddressToCoord:
            PrepareAsyncRequestL( aMessage );
            StartAsyncRequestL( aMessage );
            break;

        case EMnIpcGetConversionResult:
            HandleGetConversionResultL( aMessage );
            break;
            
        case EMnIpcCancel:
            HandleCancelRequestL( aMessage );
            break;
            
        default:
            CMnServiceBase::ServiceL( aMessage );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocodingServiceBase::StartAsyncRequestL( const RMessage2& aMessage )
    {
    LOG("CMnGeocodingServiceBase::StartAsyncRequestL in");

    HBufC* addr = NULL;
    CPosLandmark* lm = NULL;

    UnpackOptionsL( aMessage );

    switch ( CurrentAsyncRequest() )
        {
        case EMnIpcCoordToAddress:
            lm = UnpackLandmarkLC( aMessage, EMnIpcGeoLandmarkParamIndex );
            HandleFindAddressByCoordinateL( *lm );
            CleanupStack::PopAndDestroy( lm );
            break;
            
        case EMnIpcAddressToCoord:
            lm = UnpackLandmarkLC( aMessage, EMnIpcGeoLandmarkParamIndex );
            HandleFindCoordinateByAddressL( *lm );
            CleanupStack::PopAndDestroy( lm );
            break;

        case EMnIpcPlainAddressToCoord:
            addr = UnpackPlainAddressLC( aMessage );
            HandleFindCoordinateByAddressL( addr->Des() );
            CleanupStack::PopAndDestroy( addr );
            break;

        default:
            User::Leave( KErrNotSupported );
        }

    LOG("CMnGeocodingServiceBase::StartAsyncRequestL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocodingServiceBase::UnpackOptionsL( const RMessage2& aMessage )
    {
    LOG("CMnGeocodingServiceBase::UnpackOptionsL");
    
    TPckg<CMnGeocoder::TOptions> optsPack( iOptions );
    aMessage.ReadL( EMnIpcGeoOptionsParamIndex, optsPack );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
HBufC* CMnGeocodingServiceBase::UnpackPlainAddressLC( const RMessage2& aMessage )
    {
    LOG("CMnGeocodingServiceBase::UnpackPlainAddressLC");
    return CopyClientBufferLC( aMessage, EMnIpcGeoPlainAddressParamIndex );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnGeocodingServiceBase::HandleGetConversionResultL( const RMessage2& aMessage )
    {
    LOG("CMnGeocodingServiceBase::HandleGetConversionResultL in");
    
    if ( !iResult )
        {
        User::Leave( KErrNotFound );
        }
    
    aMessage.WriteL( EMnIpcGeoResultParamIndex, *iResult );
    TPckg<TInt> typePack( iResultType );
    aMessage.WriteL( EMnIpcGeoResultTypeParamIndex, typePack );
    
    Complete( aMessage, KErrNone );

    LOG("CMnGeocodingServiceBase::HandleGetConversionResultL out");
    }
