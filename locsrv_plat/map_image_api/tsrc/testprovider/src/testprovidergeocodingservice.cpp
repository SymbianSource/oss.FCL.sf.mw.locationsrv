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
* Description:  ÑTestProviderGeocodingServiceBase class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <e32property.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mngeocodingservicebase.h>
#include "debug.h"
#include "testproviderdump.h"
#include "testproviderappui.h"
#include "testproviderappserver.h"
#include "testprovidergeocodingservice.h"

const TInt KMaxCoordStrSize = KMaxPrecision + 5;
const TTimeIntervalMicroSeconds32 KProcessingDelay = 200 * 1000;

// ======== LOCAL FUNCTIONS ========

void CoordinateToString(
    TReal aCoord,
    TDes& aStr )
    {
    _LIT( KNan, "<NaN>" );
    _LIT( KOutOfBounds, "<oob>" );
    _LIT( KCoordFormat, "%.1f" );

    if ( Math::IsNaN( aCoord ) )
        {
        aStr.Copy( KNan );
        }
    else if ( Abs( aCoord ) > 360 )
        {
        aStr.Copy( KOutOfBounds );
        }
    else
        {
        aStr.Format( KCoordFormat, aCoord );
        }
    }
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderGeocodingService* CTestProviderGeocodingService::NewL()
    {
    CTestProviderGeocodingService* self = new (ELeave) CTestProviderGeocodingService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderGeocodingService::CTestProviderGeocodingService()
    {
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderGeocodingService::~CTestProviderGeocodingService()
    {
    iProcessor->Cancel();
    delete iProcessor;

    delete iLandmark;
    delete iPlainAddress;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::ConstructL()
    {
    BaseConstructL();
    
    iProcessor = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::LeaveIfBusyL()
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
void CTestProviderGeocodingService::LeaveIfInstructedL()
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
void CTestProviderGeocodingService::StartProcessingL()
    {
    iProcessor->Start( KProcessingDelay, KProcessingDelay,
        TCallBack( CTestProviderGeocodingService::GeocodingProcessingCallBack, this ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderGeocodingService::GeocodingProcessingCallBack( TAny* aPtr )
    {
    CTestProviderGeocodingService* self =    
        static_cast<CTestProviderGeocodingService*> ( aPtr );
        
    TRAP_IGNORE( self->HandleGeocodingCompletedL() );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::HandleGeocodingCompletedL()
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
TInt CTestProviderGeocodingService::GetTestCommand()
    {
    CTestProviderAppServer* server = (CTestProviderAppServer*) Server();
    TInt command = server->GetTestCommand();
    return command;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::FinishProcessingL()
    {
    LOG1("FinishProcessingL in, cursvc %d", iCurrentService);
    TCoordinate coord;
    switch ( iCurrentService )
        {
        case ECoordToAddr:
            {
            TBuf<KMaxCoordStrSize> coordStr;
            
            TLocality loc;
            iLandmark->GetPosition( loc );
            
            CoordinateToString( loc.Latitude(), coordStr );
            iLandmark->SetPositionFieldL( EPositionFieldCountry, coordStr );

            CoordinateToString( loc.Longitude(), coordStr );
            iLandmark->SetPositionFieldL( EPositionFieldCity, coordStr );
            }
            break;

        case EAddrToCoord:
            coord.SetCoordinate( 90, 0 ); // North
            iLandmark->SetPositionL( TLocality( coord, 0 ) );
            break;

        case EPlainToCoord:
            coord.SetCoordinate( -90, 0 ); // South
            iLandmark->SetPositionL( TLocality( coord, 0 ) );
            break;
            
        default:            
            User::Leave( KErrGeneral );
        }

    iCurrentService = ENone;        
    CompleteGeocodingRequestL( *iLandmark );
    LOG("FinishProcessingL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::HandleTestCommandL( TInt aCommand )
    {
    LOG1("CTestProviderGeocodingService::HandleTestCommandL in, %d", aCommand );
    switch ( aCommand )
        {
        case EServerActionCompleteGeocoding:
            {
            CPosLandmark* lm = CPosLandmark::NewLC();
            CompleteGeocodingRequestL( *lm );
            CleanupStack::PopAndDestroy( lm );
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
void CTestProviderGeocodingService::DumpDataL(
    const CPosLandmark& aLandmark )
    {
    CTestProviderDump* dump = CTestProviderDump::NewLC();

    CTestProviderAppServer* server = ( CTestProviderAppServer* ) Server();
    CTestProviderAppUi* ui = server->AppUi();
    dump->AddRunModeL( ui->IsChainedMode() );

    dump->AddGeocodingOptionsL( Options() );
    dump->AddLandmarkL( aLandmark );
    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::DumpDataL(
    const TDesC& aAddress )
    {
    CTestProviderDump* dump = CTestProviderDump::NewLC();

    CTestProviderAppServer* server = ( CTestProviderAppServer* ) Server();
    CTestProviderAppUi* ui = server->AppUi();
    dump->AddRunModeL( ui->IsChainedMode() );

    dump->AddGeocodingOptionsL( Options() );
    dump->AddAddressL( aAddress );
    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    }

// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::HandleFindAddressByCoordinateL(
    const CPosLandmark& aLandmark)
    {
    LeaveIfInstructedL();
    LeaveIfBusyL();

    DumpDataL( aLandmark );

    TLocality loc;
    if ( aLandmark.GetPosition( loc ) != KErrNone )
        {
        User::Leave( KErrArgument );
        }

    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL(aLandmark);
    
    iCurrentService = ECoordToAddr;
    StartProcessingL();
    }
        
// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::HandleFindCoordinateByAddressL(
    const CPosLandmark& aLandmark)
    {
    LeaveIfInstructedL();
    LeaveIfBusyL();

    DumpDataL( aLandmark );

    // verify that landmark has address info
    TBool valid = EFalse;
    TPositionFieldId fieldId = aLandmark.FirstPositionFieldId();
    while ( fieldId != EPositionFieldNone )
        {
        if ( fieldId > EPositionFieldAddressCapabilitiesBegin &&
             fieldId < EPositionFieldNMEACapabilitiesBegin)
            {
            TPtrC field;
            aLandmark.GetPositionField( fieldId, field );
            if ( field.Length() )
                {
                valid = ETrue;
                break;
                }
            }
        fieldId = aLandmark.NextPositionFieldId( fieldId );
        }
        
    if ( !valid )
        {
        User::Leave( KErrArgument );
        }
  
    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL( aLandmark );
    
    iCurrentService = EAddrToCoord;
    StartProcessingL();
    }

// ---------------------------------------------------------------------------
// From class CMnGeocodingServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::HandleFindCoordinateByAddressL(
    const TDesC& aAddress)
    {
    LeaveIfInstructedL();
    LeaveIfBusyL();

    DumpDataL( aAddress );

    if ( aAddress.Length() == 0 )
        {
        User::Leave( KErrArgument );
        }

    delete iPlainAddress;
    iPlainAddress = NULL;
    iPlainAddress = aAddress.AllocL();
    
    delete iLandmark;
    iLandmark = NULL;
    iLandmark = CPosLandmark::NewL();
    
    iCurrentService = EPlainToCoord;
    StartProcessingL();
    }

// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderGeocodingService::DoCancel()
    {
    iProcessor->Cancel();
    iCurrentService = ENone;
    }
