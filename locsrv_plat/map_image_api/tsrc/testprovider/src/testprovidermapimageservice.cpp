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
* Description:  ÑTestProviderMapImageServiceBase class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <e32property.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mnmapimage.h>
#include <mnmapimageparams.h>
#include <mnmapimageservicebase.h>

#include "debug.h"
#include "testproviderdump.h"
#include "testproviderappui.h"
#include "testproviderappserver.h"
#include "testprovidermapimageservice.h"

const TInt KMaxCoordStrSize = KMaxPrecision + 5;
const TTimeIntervalMicroSeconds32 KProcessingDelay = 200 * 1000;

// ======== LOCAL FUNCTIONS ========

extern void CoordinateToString( TReal aCoord, TDes& aStr );
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapImageService* CTestProviderMapImageService::NewL()
    {
    CTestProviderMapImageService* self = new (ELeave) CTestProviderMapImageService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapImageService::CTestProviderMapImageService()
    {
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderMapImageService::~CTestProviderMapImageService()
    {
    iProcessor->Cancel();
    delete iProcessor;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::ConstructL()
    {
    BaseConstructL();
    
    iProcessor = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::LeaveIfBusyL()
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
void CTestProviderMapImageService::LeaveIfInstructedL()
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
void CTestProviderMapImageService::StartProcessingL()
    {
    iProcessor->Start( KProcessingDelay, KProcessingDelay,
        TCallBack( CTestProviderMapImageService::MapImageProcessingCallBack, this ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderMapImageService::MapImageProcessingCallBack( TAny* aPtr )
    {
    CTestProviderMapImageService* self =    
        static_cast<CTestProviderMapImageService*> ( aPtr );
        
    TRAP_IGNORE( self->HandleMapImageCompletedL() );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::HandleMapImageCompletedL()
    {
    if ( GetTestCommand() != EServerActionDelayCompletion )
        {
        iProcessor->Cancel();
        TInt err = KErrNone;
        TRAP( err, FinishProcessingL() );
        if ( err )
            {
            CompleteRequest( err );
            }
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderMapImageService::GetTestCommand()
    {
    CTestProviderAppServer* server = (CTestProviderAppServer*) Server();
    return server->GetTestCommand();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::RenderL()
    {
    TMnMapImageParams& params = MapImageParams();

    if ( params.Direction() != 0 )
        {
        User::Leave( KErrNotSupported );
        }

    CFbsBitmap& bitmap = TargetBitmap();

    CFbsBitmapDevice* bitmapDevice = 
      CFbsBitmapDevice::NewL( &( bitmap ) );
    CleanupStack::PushL( bitmapDevice );

    CFbsBitGc* gc = NULL;
    User::LeaveIfError( bitmapDevice->CreateContext( gc ) );
    CleanupStack::PushL( gc );

    switch ( params.ImageType() ) 
        {
        case TMnMapImageParams::ETypeSatelliteImage:
            gc->SetBrushColor( KRgbGreen );
            break;  
        default:
            params.SetImageType( TMnMapImageParams::ETypeVectorMap );
        case TMnMapImageParams::ETypeVectorMap:
            gc->SetBrushColor( KRgbYellow );
            break;
        }
        
    // Very simple map example
    gc->Clear( TargetRect() ); // clear the area

    // cleanup
    CleanupStack::PopAndDestroy( gc );
    CleanupStack::PopAndDestroy( bitmapDevice );

    // complete request
    params.SetProjectionId(0);
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::FinishProcessingL()
    {
    LOG1("FinishProcessingL in, cursvc %d", iCurrentService);
    TCoordinate coord;
    switch ( iCurrentService )
        {
        case ERender:
            {
            RenderL();
            //TargetBitmap().Clear();
            }
            break;

        default:            
            User::Leave( KErrGeneral );
        }

    iCurrentService = ENone;        
    CompleteRendering();
    LOG("FinishProcessingL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::HandleTestCommandL( TInt aCommand )
    {
    LOG1("CTestProviderMapImageService::HandleTestCommandL in, %d", aCommand );
    switch ( aCommand )
        {
        case EServerActionCompleteRendering:
            {
            MapImageParams().SetImageType( TMnMapImageParams::ETypeVectorMap );
            CompleteRendering();
            }
            break;
            
        case EServerActionBaseComplete:
            {
            CompleteRequest( KErrCompletion );
            }
            break;

        default:
            break;    
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::DumpDataL()
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

    TMapImageOptions options;
    options.iParams = MapImageParams();
    options.iBitmapHandle = TargetBitmap().Handle();
    options.iBitmapSize = TargetBitmap().SizeInPixels();
    options.iTargetOrigin = TargetRect().iTl;
    options.iShowOptions = ShowOptions();

    dump->AddMapImageOptionsL( options );

    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    LOG("DumpDataL out");
    }
    
// ---------------------------------------------------------------------------
// From class CMnMapImageServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::HandleRenderingL()
    {
    LeaveIfInstructedL();
    LeaveIfBusyL();

    DumpDataL();

    iCurrentService = ERender;
    StartProcessingL();
    }
        
// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderMapImageService::DoCancel()
    {
    iProcessor->Cancel();
    iCurrentService = ENone;
    }

