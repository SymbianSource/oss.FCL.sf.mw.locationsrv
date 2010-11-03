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
* Description:  CMnMapViewServiceBase class implementation
*
*/


#include <e32base.h>
#include <s32mem.h>
#include <e32math.h>
#include <lbsposition.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mnerrors.h"
#include "mninternal.h"
#include "mninternal.inl"
#include "mnmapselectionresultinfo.inl"

#include "mnmapviewservicebase.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapViewServiceBase::CMnMapViewServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapViewServiceBase::~CMnMapViewServiceBase()
    {
    LOG("~CMnMapViewServiceBase in");
    ResetLandmarks();
    iLandmarks.Close();
    iLinkedLandmarksLists.Close();
    iTempDatabaseList.Close();

    CleanSelectionResult();
    delete iSelectionRequestText;
    LOG("~CMnMapViewServiceBase out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::BaseConstructL()
    {
    LOG("CMnMapViewServiceBase::BaseConstructL");
    CMnServiceBase::BaseConstructL();
    iSelectionRequestText = HBufC::NewL( 0 );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::TOptions CMnMapViewServiceBase::Options()
    {
    LOG("CMnMapViewServiceBase::Options");
    return iOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::TCurrentLocationOption CMnMapViewServiceBase::CurrentLocationOption()
    {
    LOG("CMnMapViewServiceBase::CurrentLocationOption");
    return iCurrentLocationOption;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TArray<const CPosLandmark*> CMnMapViewServiceBase::LandmarksToShow()
    {
    LOG("CMnMapViewServiceBase::LandmarksToShow");
    return iLandmarks.Array();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TArray<TPosLmItemId> CMnMapViewServiceBase::LinkedLandmarksToShow( TInt aDbIndex )
    {
    LOG1("CMnMapViewServiceBase::LinkedLandmarksToShow, db index %d", aDbIndex);
    if ( aDbIndex < 0 || aDbIndex >= iLinkedLandmarksLists.Count() )
        {
        PanicServer( KMnPanicArrayIndexOutOfBounds );
        }
    return iLinkedLandmarksLists[aDbIndex]->iLandmarkIds.Array();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TArray<const HBufC*> CMnMapViewServiceBase::LandmarksToShowDatabases()
    {
    LOG("CMnMapViewServiceBase::LandmarksToShowDatabases");
    if ( iTempDatabaseList.Count() != iLinkedLandmarksLists.Count() )
        {
        // update database list
        iTempDatabaseList.Reset();    
        for ( TInt i = 0; i < iLinkedLandmarksLists.Count(); i++ )
            {
            iTempDatabaseList.Append( iLinkedLandmarksLists[i]->iDatabaseUri );
            }
        }
    return iTempDatabaseList.Array();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CMnMapViewServiceBase::SelectionRequestText()
    {
    LOG("CMnMapViewServiceBase::SelectionRequestText");
    return iSelectionRequestText->Des();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CMnMapViewServiceBase::IsAreaCentralPointSet()
    {
    LOG("CMnMapViewServiceBase::IsAreaCentralPointSet");
    return !( Math::IsNaN( iCentralPoint.Latitude() ) &&
              Math::IsNaN( iCentralPoint.Longitude() ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::GetAreaCentralPoint( TCoordinate& aCentralPoint )
    {
    LOG("CMnMapViewServiceBase::GetAreaCentralPoint");
    aCentralPoint = iCentralPoint;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TReal CMnMapViewServiceBase::AreaRadius()
    {
    LOG("CMnMapViewServiceBase::AreaRadius");
    return iRadius;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::CompleteSelectionRequestL( const CPosLandmark& aLandmark )
    {
    LOG("CMnMapViewServiceBase::CompleteSelectionRequestL(free) in");

    if ( !IsAsyncRequestActive() )
        {
        PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }
        
    if ( iOptions & CMnMapView::EOptionRestrictSelection )
        {
        User::Leave( KErrArgument );
        }

    CleanSelectionResult();
    
    iResultLandmark = PosLandmarkSerialization::PackL( aLandmark );
    
    TMnMapSelectionResultInfo info( *iResultLandmark );
    TPckgC<TMnMapSelectionResultInfo> resultInfoPack( info );

    TInt err = iMessage.Write( EMnIpcMapSelectionResultInfoParamIndex, resultInfoPack );
    
    CompleteRequest( err );

    LOG("CMnMapViewServiceBase::CompleteSelectionRequestL(free) out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::CompleteSelectionRequest( TInt aLandmarkIndex )
    {
    LOG1("CMnMapViewServiceBase::CompleteSelectionRequest(index) in, index=%d", aLandmarkIndex);

    if ( !IsAsyncRequestActive() )
        {
        PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }
    
    if ( aLandmarkIndex < 0 || aLandmarkIndex >= iLandmarks.Count() )
        {
        PanicServer( KMnPanicArrayIndexOutOfBounds );
        }

    CleanSelectionResult();
    
    TMnMapSelectionResultInfo info( aLandmarkIndex );
    TPckgC<TMnMapSelectionResultInfo> resultInfoPack( info );

    TInt err = iMessage.Write( EMnIpcMapSelectionResultInfoParamIndex, resultInfoPack );

    CompleteRequest( err );

    LOG("CMnMapViewServiceBase::CompleteSelectionRequest(index) out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::CompleteSelectionRequest(
    TPosLmItemId aLandmarkId, 
    TInt aDbIndex)
    {
    LOG2("CMnMapViewServiceBase::CompleteSelectionRequest(id,db) in, dbIndex %d, lmId %d",
        aDbIndex, aLandmarkId);

    if ( !IsAsyncRequestActive() )
        {
        PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }

    if ( aDbIndex < 0 || aDbIndex >= iLinkedLandmarksLists.Count() )
        {
        LOG1("CMnMapViewServiceBase::CompleteSelectionRequest(id,db) dbIndex invalid, max %d",
            iLinkedLandmarksLists.Count());
        PanicServer( KMnPanicArrayIndexOutOfBounds );
        }
        
    if ( KErrNotFound == iLinkedLandmarksLists[aDbIndex]->iLandmarkIds.Find( aLandmarkId ) )
        {
        LOG("CMnMapViewServiceBase::CompleteSelectionRequest(id,db) lmId not found");
        PanicServer( KMnPanicArrayIndexOutOfBounds );
        }

    CleanSelectionResult();
    
    TMnMapSelectionResultInfo info( 
        aLandmarkId, 
        *(iLinkedLandmarksLists[aDbIndex]->iDatabaseUri) );

    TPckgC<TMnMapSelectionResultInfo> resultInfoPack( info );
    TInt err = iMessage.Write( EMnIpcMapSelectionResultInfoParamIndex, resultInfoPack );

    CompleteRequest( err );

    LOG("CMnMapViewServiceBase::CompleteSelectionRequest(id,db) out");
    }
       
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapViewServiceBase::ServiceL( const RMessage2& aMessage )
    {
    LOG1("CMnMapViewServiceBase::ServiceL in, function %d", aMessage.Function());
    
    switch ( aMessage.Function() )
        {
        case EMnIpcAddLmIdsToShow:
            HandleAddLandmarkIdsToShowL( aMessage );
            break;
            
        case EMnIpcAddLandmarksToShow:
            HandleAddLandmarksToShowL( aMessage );
            break;
            
        case EMnIpcResetLandmarksToShow: // blind request
            ResetLandmarks();
            break;
            
        case EMnIpcCancel:
            HandleCancelRequestL( aMessage );
            break;
            
        case EMnIpcShowMap:
            UnpackOptionsL( aMessage );
		    LOG("CMnMapViewServiceBase::ServiceL, calling HandleShowMapL");
            HandleShowMapL();
            Complete( aMessage, KErrNone );
            break;
            
        case EMnIpcShowCurrentLocation:
            UnpackOptionsL( aMessage );
		    LOG("CMnMapViewServiceBase::ServiceL, calling HandleShowCurrentLocationL");
            HandleShowCurrentLocationL();
            Complete( aMessage, KErrNone );
            break;

        case EMnIpcInitRequestText:
            UnpackSelectionRequestTextL( aMessage );
            Complete( aMessage, KErrNone );
            break;
    
        case EMnIpcSelectFromMap:
            PrepareAsyncRequestL( aMessage );
            UnpackOptionsL( aMessage );
            UnpackSelectionRequestTextL( aMessage );
            VerifySelectionRequestL();
		    LOG("CMnMapViewServiceBase::ServiceL, calling HandleSelectFromMapL");
            HandleSelectFromMapL();
            break;
            
        case EMnIpcGetSelectionResultLandmark:
            HandleGetSelectionResultLandmarkL( aMessage );
            break;

        case EMnIpcGetSelectionResultDbUri:
            HandleGetSelectionResultDbUriL( aMessage );
            break;

        default:
            CMnServiceBase::ServiceL( aMessage );
        }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::CleanSelectionResult()
    {
    delete iResultLandmark;
    iResultLandmark = NULL;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::HandleAddLandmarkIdsToShowL( const RMessage2& aMessage )
    {
    LOG("CMnMapViewServiceBase::HandleAddLandmarkIdsToShowL in");

    if ( IsAsyncRequestActive() )
        {
        User::Leave( KErrLocked );
        }
        
    // Read client's data
    HBufC* uri = CopyClientBufferLC( aMessage, EMnIpcMapDbUriParamIndex );
    HBufC8* buf = CopyClientBuffer8LC( aMessage, EMnIpcMapLmIdsParamIndex );

    if ( buf->Size() % ( sizeof( TPosLmItemId ) ) != 0 )
        {
        User::Leave( KErrArgument );
        }

    TInt lmCount = buf->Size() / ( sizeof( TPosLmItemId ) );
    const TPosLmItemId* ids = reinterpret_cast<const TPosLmItemId*>( buf->Ptr() );

    // Save data to internal structs
    iTempDatabaseList.Reset();        
    CLinkedLandmarksList* linked = NULL;

    // check if this database is already listed
    TInt dbIndex = FindDatabase( *uri );
    if ( dbIndex == KErrNotFound )
        {
        linked = new (ELeave) CLinkedLandmarksList;
        CleanupStack::PushL( linked );
        linked->iDatabaseUri = uri;
        iLinkedLandmarksLists.AppendL( linked );
        CleanupStack::Pop( linked );
        }
    else
        {
        linked = iLinkedLandmarksLists[dbIndex];
        }

    for ( TInt i = 0; i < lmCount; i++ )
        {
        linked->iLandmarkIds.AppendL( ids[i] );
        }

    // Finalize
    Complete( aMessage, KErrNone );

    CleanupStack::PopAndDestroy( buf );
    CleanupStack::Pop( uri );

    LOG("CMnMapViewServiceBase::HandleAddLandmarkIdsToShowL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::HandleAddLandmarksToShowL( const RMessage2& aMessage )
    {
    LOG("CMnMapViewServiceBase::HandleAddLandmarksToShowL in");

    if ( IsAsyncRequestActive() )
        {
        User::Leave( KErrLocked );
        }

    HBufC8* buf = CopyClientBuffer8LC( aMessage, EMnIpcMapLandmarksParamIndex );
    RDesReadStream readStream( *buf );

    LOG1("CMnMapViewServiceBase::HandleAddLandmarksToShowL buffersize %d", buf->Size());
    
    TInt lmCount = readStream.ReadInt32L();
    
    LOG1("CMnMapViewServiceBase::HandleAddLandmarksToShowL lmCount %d", lmCount);

    for ( TInt i = 0; i < lmCount; i++ )
        {
        TInt lmSize = readStream.ReadInt32L();
        
        HBufC8* lmBuf = HBufC8::NewLC( lmSize );
        
        TPtr8 lmData( lmBuf->Des() );
        readStream.ReadL( lmData, lmSize );
        
        CPosLandmark* lm = PosLandmarkSerialization::UnpackL( *lmBuf );
        CleanupStack::PushL( lm );

        iLandmarks.AppendL( lm );

        CleanupStack::Pop( lm );
        CleanupStack::PopAndDestroy( lmBuf );
        }
    
    readStream.Close();
    CleanupStack::PopAndDestroy( buf );

    Complete( aMessage, KErrNone );
    LOG("CMnMapViewServiceBase::HandleAddLandmarksToShowL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::HandleGetSelectionResultLandmarkL( const RMessage2& aMessage )
    {
    LOG("CMnMapViewServiceBase::HandleGetSelectionResultLandmarkL in");
    
    if ( iResultLandmark )
        {
        aMessage.WriteL( EMnIpcMapSelResultBufferParamIndex, *iResultLandmark );
        Complete( aMessage, KErrNone );
        }
    else
        {
        User::Leave( KErrNotFound );
        }

    LOG("CMnMapViewServiceBase::HandleGetSelectionResultLandmarkL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::HandleGetSelectionResultDbUriL( const RMessage2& aMessage )
    {
    LOG("CMnMapViewServiceBase::HandleGetSelectionResultDbUriL in");
    
    __ASSERT_DEBUG( 
        iResultDbIndex >= 0 && iResultDbIndex < iLinkedLandmarksLists.Count() ,
        Panic( KErrGeneral ) );
    
    if ( iResultDbIndex != KErrNotFound )
        {
        __ASSERT_DEBUG( 
            iLinkedLandmarksLists[iResultDbIndex]->iDatabaseUri ,
            Panic( KErrGeneral ) );

        aMessage.WriteL( 
            EMnIpcMapSelResultBufferParamIndex, 
            *(iLinkedLandmarksLists[iResultDbIndex]->iDatabaseUri) );
            
        Complete( aMessage, KErrNone );
        }
    else
        {
        User::Leave( KErrNotFound );
        }

    LOG("CMnMapViewServiceBase::HandleGetSelectionResultDbUriL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::ResetLandmarks()
    {
    if ( !IsAsyncRequestActive() )
        {
	    iLandmarks.ResetAndDestroy();
	    iLinkedLandmarksLists.ResetAndDestroy();
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnMapViewServiceBase::FindDatabase( const TDesC& aUri )
    {
    for ( TInt i = 0; i < iLinkedLandmarksLists.Count(); i++ )
        {
        CLinkedLandmarksList* list = iLinkedLandmarksLists[i];
        if ( list->iDatabaseUri->Compare( aUri ) == 0 )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::UnpackOptionsL( const RMessage2& aMsg )
    {
    HBufC8* buf = CopyClientBuffer8LC( aMsg, EMnIpcMapOptionsParamIndex );
    const TMnMapOptions& options = *( 
        reinterpret_cast<const TMnMapOptions*>( buf->Ptr() ) );

    iOptions = options.iOptions;
    iCurrentLocationOption = options.iCurrentLocationOption;
    iCentralPoint = options.iCentralPoint;
    iRadius = options.iRadius;

    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::UnpackSelectionRequestTextL( const RMessage2& aMsg )
    {
    HBufC* buf = CopyClientBufferLC( aMsg, EMnIpcMapSelectionTextParamIndex );
    
    delete iSelectionRequestText;
    iSelectionRequestText = NULL;
    iSelectionRequestText = buf;

    CleanupStack::Pop( buf );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapViewServiceBase::VerifySelectionRequestL()
    {
    if ( iOptions & CMnMapView::EOptionRestrictSelection )
        {
        // find out how many landmarks are given
        TInt num = iLandmarks.Count();
        for ( TInt i = 0; i < iLinkedLandmarksLists.Count(); i++ )
            {
            CLinkedLandmarksList* linked = iLinkedLandmarksLists[i];
            num += linked->iLandmarkIds.Count();
            }
        LOG1("CMnMapViewServiceBase::VerifySelectionRequestL, restricted with %d landmarks", num);
        if ( num < 1 )
            {
            User::Leave( KErrArgument );
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnMapViewServiceBase::CLinkedLandmarksList::~CLinkedLandmarksList()
    {
    delete iDatabaseUri;
    iLandmarkIds.Close();
    }


