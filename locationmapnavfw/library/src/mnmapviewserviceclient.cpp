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
* Description:  RMnMapViewServiceClient class implementation
*
*/


#include <e32base.h>
#include <s32mem.h>

#include <lbsposition.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mnerrors.h"
#include "mndebug.h"
#include "mninternal.h"
#include "mninternal.inl"
#include "mnmapselectionresultinfo.inl"

#include "mnappserviceuids.hrh"
#include "mnmapviewserviceclient.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnMapViewServiceClient::RMnMapViewServiceClient()
  : iSelectionRequestText( NULL ),
    iSelectionRequestTextOwned( EFalse ),
    iResultInfoPtr( iResultInfo )
    {
    Mem::FillZ( &iResultInfo, sizeof( TMnMapSelectionResultInfo ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnMapViewServiceClient::~RMnMapViewServiceClient()
    {
    ClearSelectionRequestText();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::AddLandmarksToShowL(
    const TDesC& aDatabaseUri,
    RArray<TPosLmItemId>& aLandmarkIds)
    {
    TIpcArgs args;
    
    // db uri
    args.Set( EMnIpcMapDbUriParamIndex, &aDatabaseUri );
    
    // landmark ids
    TInt lmCount = aLandmarkIds.Count();
    TPosLmItemId* ids = new (ELeave) TPosLmItemId[ lmCount ];
    
    for ( TUint32 i = 0; i < lmCount; i++ )
        {
        *(ids + i) = aLandmarkIds[i];
        }
        
    TPtrC8 idsDes( reinterpret_cast<const TUint8*>( ids ), sizeof( TPosLmItemId ) * lmCount );
    args.Set( EMnIpcMapLmIdsParamIndex, &idsDes );

    TInt err = SendReceive( EMnIpcAddLmIdsToShow, args );
    delete[] ids;
    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::AddLandmarksToShowL( RPointerArray<CPosLandmark>& aLandmarks )
    {
    LOG("RMnMapViewServiceClient::AddLandmarksToShowL in");

    TIpcArgs args;

    const TInt KGranularity = 1024;      // expand in chunks of 1K
    CBufFlat* lmBuffer = CBufFlat::NewL( KGranularity );
    CleanupStack::PushL( lmBuffer );

    RBufWriteStream writeStream;
    writeStream.Open( *lmBuffer );
    CleanupClosePushL( writeStream );

    writeStream.WriteInt32L( aLandmarks.Count() );

    LOG1("RMnMapViewServiceClient::AddLandmarksToShowL count %d", aLandmarks.Count());

    for ( TInt i = 0; i < aLandmarks.Count(); i++ )
        {
        HBufC8* lmBuf = PosLandmarkSerialization::PackL( *aLandmarks[i] );
        CleanupStack::PushL( lmBuf );
        
        writeStream.WriteInt32L( lmBuf->Size() );
        writeStream.WriteL( *lmBuf );

        CleanupStack::PopAndDestroy( lmBuf );
        }
    writeStream.CommitL();
    CleanupStack::PopAndDestroy( &writeStream );

    LOG1("RMnMapViewServiceClient::AddLandmarksToShowL totalsize %d", lmBuffer->Size());

    //lmBuffer->Compress();
    TPtr8 lmBufferPtr( lmBuffer->Ptr( 0 ) );
    User::LeaveIfError( 
        SendReceive( EMnIpcAddLandmarksToShow, TIpcArgs( &lmBufferPtr ) ) );

    CleanupStack::PopAndDestroy( lmBuffer );
    LOG("RMnMapViewServiceClient::AddLandmarksToShowL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::AddLandmarksToShowL( RArray<TPtrC8>& aPackedLandmarks )
    {
    LOG("RMnMapViewServiceClient::AddLandmarksToShowL (packed) in");
    TIpcArgs args;

    const TInt KGranularity = 1024;      // expand in chunks of 1K
    CBufFlat* lmBuffer = CBufFlat::NewL( KGranularity );
    CleanupStack::PushL( lmBuffer );

    RBufWriteStream writeStream;
    writeStream.Open( *lmBuffer );
    CleanupClosePushL( writeStream );

    writeStream.WriteInt32L( aPackedLandmarks.Count() );

    LOG1("RMnMapViewServiceClient::AddLandmarksToShowL (packed) count %d", aPackedLandmarks.Count());

    for ( TInt i = 0; i < aPackedLandmarks.Count(); i++ )
        {
        TPtrC8 data( aPackedLandmarks[i] );
        
        writeStream.WriteInt32L( data.Size() );
        writeStream.WriteL( data );
        }

    writeStream.CommitL();
    CleanupStack::PopAndDestroy( &writeStream );

    LOG1("RMnMapViewServiceClient::AddLandmarksToShowL (packed) totalsize %d", lmBuffer->Size());

    lmBuffer->Compress();
    TPtr8 lmBufferPtr( lmBuffer->Ptr( 0 ) );
    User::LeaveIfError( 
        SendReceive( EMnIpcAddLandmarksToShow, TIpcArgs( &lmBufferPtr ) ) );

    CleanupStack::PopAndDestroy( lmBuffer );
    LOG("RMnMapViewServiceClient::AddLandmarksToShowL (packed) out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::ResetLandmarksToShow()
    {
    Send( EMnIpcResetLandmarksToShow );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::SetSelectionRequestTextL(
    const TDesC& aSelectionRequestText)
    {
    ClearSelectionRequestText();    
    iSelectionRequestText = aSelectionRequestText.AllocL();
    iSelectionRequestTextOwned = ETrue;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::SetSelectionRequestText( HBufC*& aSelectionRequestText )
    {
    ClearSelectionRequestText();
    iSelectionRequestText = aSelectionRequestText;
    iSelectionRequestTextOwned = EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::InitSelectionRequestTextL( HBufC*& aSelectionRequestText )
    {
    SetSelectionRequestText( aSelectionRequestText );

    TIpcArgs args;
    args.Set( EMnIpcMapSelectionTextParamIndex, iSelectionRequestText );
    
    TInt err = SendReceive( EMnIpcInitRequestText, args );
    LOG1("RMnMapViewServiceClient::InitSelectionRequestTextL, sendreceive = %d", err);
    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::ClearSelectionRequestText()
    {
    if ( iSelectionRequestTextOwned )
        {
        delete iSelectionRequestText;
        }
    iSelectionRequestText = NULL;
    iSelectionRequestTextOwned = ETrue;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::ShowMapL( TMnMapOptions aOptions )
    {
    TIpcArgs args;
    
    TPckgC<TMnMapOptions> optsPack( aOptions );
    args.Set( EMnIpcMapOptionsParamIndex, &optsPack );
    
    User::LeaveIfError( SendReceive( EMnIpcShowMap, args ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::ShowCurrentLocationL( TMnMapOptions aOptions )
    {
    TIpcArgs args;
    
    TPckgC<TMnMapOptions> optsPack( aOptions );
    args.Set( EMnIpcMapOptionsParamIndex, &optsPack );
    
    User::LeaveIfError( SendReceive( EMnIpcShowCurrentLocation, args ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::SelectFromMap(
    TMnMapOptions aOptions,
    TRequestStatus& aStatus )
    {
    __ASSERT_DEBUG( iSelectionRequestText, PanicSelf( KErrGeneral ) );
    
    TIpcArgs args;
    
    // options
    iOptions = aOptions;
    TPckgC<TMnMapOptions> optsPack( iOptions );
    iOptionsPtr.Set( optsPack );
    args.Set( EMnIpcMapOptionsParamIndex, &iOptionsPtr );
    
    // selection text
    args.Set( EMnIpcMapSelectionTextParamIndex, iSelectionRequestText );

    // selection result info
    args.Set( EMnIpcMapSelectionResultInfoParamIndex, &iResultInfoPtr );
    
    SendReceive( EMnIpcSelectFromMap, args, aStatus );
    iLastRequest = EMnIpcSelectFromMap;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnMapView::TSelectionResultType RMnMapViewServiceClient::SelectionResultType()
    {
    return iResultInfo.iType;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CPosLandmark* RMnMapViewServiceClient::GetSelectedLandmarkL()
    {
    if ( iResultInfo.iType != CMnMapView::ESelectionFreeLandmark )
        {
        User::Leave( KErrNotFound );
        }
    
    TIpcArgs args;

    // landmark receiving buffer
    HBufC8* lmBuf = HBufC8::NewLC( iResultInfo.iLandmarkSize );
    TPtr8 lmBufPtr( lmBuf->Des() );

    args.Set( EMnIpcMapSelResultBufferParamIndex, &lmBufPtr );
    User::LeaveIfError( SendReceive( EMnIpcGetSelectionResultLandmark, args ) );

    CPosLandmark* l = PosLandmarkSerialization::UnpackL( *lmBuf );
    CleanupStack::PopAndDestroy( lmBuf );
    return l;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::GetSelectedLandmarkL( TInt& aLandmarkIndex )
    {
    if ( iResultInfo.iType == CMnMapView::ESelectionLandmarkIndex )
        {
        aLandmarkIndex = iResultInfo.iLandmarkIndex;
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnMapViewServiceClient::GetSelectedLandmarkL( TPosLmItemId& aLandmarkId, HBufC*& aDbUri )
    {
    if ( iResultInfo.iType != CMnMapView::ESelectionLinkedLandmark )
        {
        User::Leave( KErrNotFound );
        }

    aLandmarkId = iResultInfo.iLandmarkId;

    TIpcArgs args;
    
    // database URI receiving buffer
    HBufC* uriBuf = HBufC::NewLC( iResultInfo.iDbUriSize );
    TPtr uriBufPtr( uriBuf->Des() );
    args.Set( EMnIpcMapSelResultBufferParamIndex, &uriBufPtr );

    User::LeaveIfError( SendReceive( EMnIpcGetSelectionResultDbUri, args ) );
    
    CleanupStack::Pop( uriBuf );
    aDbUri = uriBuf;
    }

// ---------------------------------------------------------------------------
// From class RApaAppServiceBase.
// ---------------------------------------------------------------------------
//
TUid RMnMapViewServiceClient::ServiceUid() const
    { 
    return TUid::Uid(KMnAppMapViewService); 
    }
