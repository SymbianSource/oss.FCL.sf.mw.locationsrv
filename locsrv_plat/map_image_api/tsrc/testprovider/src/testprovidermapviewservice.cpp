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
* Description:  ÑTestProviderMapViewServiceBase class implementation
*
*/


#include <e32base.h>
#include <eikenv.h>
#include <w32std.h>
#include <e32property.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>

#include "debug.h"
#include <mnmapviewservicebase.h>

#include "testproviderappui.h"
#include "testprovidermapview.h"
#include "testproviderappserver.h"
#include "testproviderdump.h"

#include "testprovidermapviewservice.h"

const TTimeIntervalMicroSeconds32 KProcessingDelay = 200 * 1000;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapViewService* CTestProviderMapViewService::NewL()
    {
    CTestProviderMapViewService* self = new (ELeave) CTestProviderMapViewService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapViewService::CTestProviderMapViewService()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapViewService::~CTestProviderMapViewService()
    {
    delete iLandmark;
    if ( iProcessor )
        {
        iProcessor->Cancel();
        delete iProcessor;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::ConstructL()
    {
    BaseConstructL();
    iProcessor = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::LeaveIfBusyL()
    {
    if ( iProcessor->IsActive() )
        {
        LOG("CTestProviderMapViewService::LeaveIfBusyL BUSY");
        User::Leave( KErrInUse );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::LeaveIfInstructedL()
    {
    RProperty property;
    TInt code = KErrNone;
    TInt err = property.Get( TUid::Uid( KPropCategoryUid ), KPropKeyErrorCode, code );
    if ( err == KErrNone )
        {
        User::LeaveIfError( code );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::StartProcessingL()
    {
    iProcessor->Start( KProcessingDelay, KProcessingDelay,
        TCallBack( CTestProviderMapViewService::SelectionIdleCallBack, this ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderMapViewService::SelectionIdleCallBack( TAny* aPtr )
    {
    CTestProviderMapViewService* self =
        static_cast<CTestProviderMapViewService*> ( aPtr );

    TRAP_IGNORE( self->HandleSelectionL() );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::HandleSelectionL()
    {
    if ( GetTestCommand() != EServerActionDelayCompletion )
        {
        iProcessor->Cancel();
        TInt err = KErrNone;
        TRAP( err, DoSelectionL() );
        if ( err )
            {
            CompleteRequest( err );
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderMapViewService::GetTestCommand()
    {
    CTestProviderAppServer* server = (CTestProviderAppServer*) Server();
    TInt command = server->GetTestCommand();
    return command;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::HandleTestCommandL( TInt aCommand )
    {
    LOG1("CTestProviderMapViewService::HandleTestCommandL in, %d", aCommand );
    switch ( aCommand )
        {
        case EServerActionBaseComplete:
            CompleteRequest( KErrCompletion );
            break;
            
        case EServerActionCompleteSelectionFree:
            {
            CPosLandmark* lm = CPosLandmark::NewLC();
            CompleteSelectionRequestL( *lm );
            CleanupStack::PopAndDestroy( lm );
            }
            break;
            
        case EServerActionCompleteSelectionIndex:
            CompleteSelectionRequest( 0 );
            break;

        case EServerActionCompleteSelectionNegativeIndex:
            CompleteSelectionRequest( -1 );
            break;
            
        case EServerActionCompleteSelectionLinked:
            CompleteSelectionRequest( 0, 1 );
            break;
            
        case EServerActionCompleteSelectionWrongLinked:
            CompleteSelectionRequest( 0, 1000 );
            break;
            
        case EServerActionCompleteSelectionNegativeDbIndex:
            CompleteSelectionRequest( 0, -1 );
            break;
            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::DoSelectionL()
    {
    LOG("DoSelectionL in");
    
    HandleTestCommandL( GetTestCommand() );
    
    if ( Options() & CMnMapView::EOptionRestrictSelection )
        {
        LOG("MnTestProvider::HandleSelectFromMapL restricted");
        if ( LandmarksToShowDatabases().Count() &&
             LinkedLandmarksToShow( 0 ).Count() ) 
            {
            LOG("HandleSelectFromMapL taking first linked");
            
            // take first linked landmark from first database
            TInt dbIndex = 0;
            TPosLmItemId lmId = LinkedLandmarksToShow( dbIndex )[0];
            CompleteSelectionRequest( lmId, dbIndex );
            }
        else if ( LandmarksToShow().Count() )
            {
            LOG("HandleSelectFromMapL taking first not linked");
            // take first not linked landmark
            CompleteSelectionRequest( 0 );
            }
        else
            {
            LOG("HandleSelectFromMapL restricted, but no landmarks");
            // this shall never happen, because base implementation
            // will check that landmarks are given if selection is restricted
            User::Leave( KErrArgument );
            }
        }
    else
        {
        // return random landmark
        LOG("HandleSelectFromMapL returning random");
        CPosLandmark* lm = CPosLandmark::NewLC();
        
        _LIT( KSelected, "RandomLandmark" );
        lm->SetLandmarkNameL( KSelected );
        
        // north pole
        lm->SetPositionL( TLocality( TCoordinate( -90, 0 ), 0 ) );
        
        CompleteSelectionRequestL( *lm );
        CleanupStack::PopAndDestroy( lm );
        }
    LOG("DoSelectionL out");
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::DumpDataL()
    {
    LOG("DumpDataL in");
    if ( GetTestCommand() == EServerActionDisableDump )
        {
        LOG("DumpDataL disabled, out");
        return;
        }

    CTestProviderDump* dump = CTestProviderDump::NewLC();
    
    CTestProviderAppServer* server = ( CTestProviderAppServer* ) Server();
    CTestProviderAppUi* ui = server->AppUi();
    dump->AddRunModeL( ui->IsChainedMode() );

    TCoordinate center;
    GetAreaCentralPoint( center );
    dump->AddMapViewOptionsL( Options(), CurrentLocationOption(), AreaRadius(), center );
    
    dump->AddRequestTextL( SelectionRequestText() );
    
    // free landmarks
    TArray<const CPosLandmark*> landmarks = LandmarksToShow();
    for ( TInt i = 0; i < landmarks.Count(); i++ )
        {
        dump->AddLandmarkL( *(landmarks[i]) );
        }
    
    // linked landmarks
    TArray<const HBufC*> lmdbs = LandmarksToShowDatabases();
    for ( TInt i = 0; i < lmdbs.Count(); i++ )
        {
        dump->AddLandmarkListL( *( lmdbs[i] ), LinkedLandmarksToShow( i ) );
        }

    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    LOG("DumpDataL out");
    }

// ---------------------------------------------------------------------------
// From class CMnMapViewServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::HandleShowMapL()
    {
    LOG("HandleShowMapL in");
    LeaveIfInstructedL();
    DumpDataL();
    
    CTestProviderAppServer* server = ( CTestProviderAppServer* ) Server();
    CTestProviderAppUi* ui = server->AppUi();
    ui->ShowMapViewL();

    LOG("HandleShowMapL out");
    }
        
// ---------------------------------------------------------------------------
// From class CMnMapViewServiceBase
//
//  Implementation differs from HandleShowMapL in a way that central point
//  setting is ignored and current location is used instead as central point.
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::HandleShowCurrentLocationL()
    {
    LOG("HandleShowCurrentLocationL in");
    LeaveIfInstructedL();
    DumpDataL();
    LOG("HandleShowCurrentLocationL out");
    }

// ---------------------------------------------------------------------------
// From class CMnMapViewServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::HandleSelectFromMapL()
    {
    LOG("HandleSelectFromMapL in");
    LeaveIfInstructedL();
    LeaveIfBusyL();

    DumpDataL();

    StartProcessingL();
    LOG("HandleSelectFromMapL out");
    }

// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderMapViewService::DoCancel()
    {
    iProcessor->Cancel();
    }
