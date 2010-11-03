/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CTestProviderAppServer
*
*/


#include <mnappserviceuids.hrh>
#include "debug.h"

#include <e32property.h>
#include <lbsposition.h>
#include <epos_cposlandmark.h>

#include "testprovidergeocodingservice.h"
#include "testprovidernavigationservice.h"
#include "testprovidermapviewservice.h"

#ifdef RD_MAPNAV_BITMAP_ACCESS
#include "testprovidermapimageservice.h"
#endif

#include "testproviderappui.h"
#include "testproviderappserver.h"

#include "testproviderdump.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderAppServer::~CTestProviderAppServer()
    {
    if ( iListener )
        {
        iListener->Cancel();
        delete iListener;
        }

    LOG("server destroyed");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppServer::ConstructL( const TDesC& aFixedServerName )
    {
    CAknAppServer::ConstructL( aFixedServerName );
    
    iListener = new (ELeave) CTestCommandListener( TCallBack( TestCommandCallback, this ) );
    iListener->Start();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CTestProviderAppServer::TestCommandCallback( TAny* aPtr )
    {
    CTestProviderAppServer* self =    
        static_cast<CTestProviderAppServer*> ( aPtr );
        
    TRAP_IGNORE( self->HandleTestCommandL() );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderAppServer::HandleTestCommandL()
    {
    iListener->Property().Get( iTestCommand );
    LOG1("test command changed %d ", iTestCommand );
    
    if ( iTestCommand == EServerActionExit )
        {
        AppUi()->ProcessCommandL( EAknCmdExit );
        }
    else
    if ( iMapViewService )
        {
        iMapViewService->HandleTestCommandL( iTestCommand );
        }
    else 
    if ( iNavigationService )
        {
        iNavigationService->HandleTestCommandL( iTestCommand );
        }
    else 
    if ( iGeocodingService )
        {
        iGeocodingService->HandleTestCommandL( iTestCommand );
        }
    else
#ifdef RD_MAPNAV_BITMAP_ACCESS
    if ( iMapImageService )
        {
        iMapImageService->HandleTestCommandL( iTestCommand );
        }
    else
#endif
        {
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CApaAppServiceBase* CTestProviderAppServer::CreateServiceL(TUid aServiceType) const
    {
    RProcess process;
    RProperty property;
    
    TProcessId id = process.Id();
    TPckg<TProcessId> packId( id );

    property.Define( TUid::Uid( KPropCategoryUid ), KPropKeyProcessId, RProperty::EByteArray );
    property.Set( TUid::Uid( KPropCategoryUid ), KPropKeyProcessId, packId );

    LOG1("CreateServiceL, svcUid = %X", aServiceType.iUid);
    switch ( aServiceType.iUid )
        {
        case KMnAppGeocodingService:
            return CreateGeocodingServiceL();
            
        case KMnAppNavigationService:
            return CreateNavigationServiceL();
            
        case KMnAppMapViewService:
            return CreateMapViewServiceL();
            
#ifdef RD_MAPNAV_BITMAP_ACCESS
        case KMnAppMapImageService:
            return CreateMapImageServiceL();
#endif

        default:
            return CAknAppServer::CreateServiceL( aServiceType );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CTestProviderAppServer::HandleAllClientsClosed()
    {
    LOG( "HandleAllClientsClosed" );

    // create fresh dump to signal successful exit
    // TRAP_IGNORE( DumpEmptyL() );
    iBusy = EFalse;
    CAknAppServer::HandleAllClientsClosed();
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderGeocodingService* CTestProviderAppServer::CreateGeocodingServiceL() const
    {
    LOG( "CreateGeocodingServiceL" );
    __ASSERT_DEBUG( !iGeocodingService, Panic( KErrAlreadyExists ) );
    iGeocodingService = CTestProviderGeocodingService::NewL();
    TRAP_IGNORE( DumpEmptyL() );
    iBusy = ETrue;
    return iGeocodingService;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderNavigationService* CTestProviderAppServer::CreateNavigationServiceL() const
    {
    LOG( "CreateNavigationServiceL" );
    __ASSERT_DEBUG( !iNavigationService, Panic( KErrAlreadyExists ) );
    iNavigationService = CTestProviderNavigationService::NewL();
    TRAP_IGNORE( DumpEmptyL() );
    iBusy = ETrue;
    return iNavigationService;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapViewService* CTestProviderAppServer::CreateMapViewServiceL() const
    {
    LOG( "CreateMapViewServiceL" );
    __ASSERT_DEBUG( !iMapViewService, Panic( KErrAlreadyExists ) );
    iMapViewService = CTestProviderMapViewService::NewL();
    TRAP_IGNORE( DumpEmptyL() );
    iBusy = ETrue;
    return iMapViewService;
    }

#ifdef RD_MAPNAV_BITMAP_ACCESS
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapImageService* CTestProviderAppServer::CreateMapImageServiceL() const
    {
    LOG( "CreateMapImageServiceL" );
    __ASSERT_DEBUG( !iMapImageService, Panic( KErrAlreadyExists ) );
    iMapImageService = CTestProviderMapImageService::NewL();
    TRAP_IGNORE( DumpEmptyL() );
    iBusy = ETrue;
    return iMapImageService;
    }
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderAppUi* CTestProviderAppServer::AppUi() const
    {
    CTestProviderAppUi* ui = (CTestProviderAppUi*) CEikonEnv::Static()->AppUi();
    if ( !ui )
        {
        LOG("UI not found!");
        }
    return ui;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppServer::DumpEmptyL() const
    {
    CTestProviderDump* dump = CTestProviderDump::NewLC();
    dump->AddRunModeL( AppUi()->IsChainedMode() );
    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CTestProviderAppServer::IsFree()
    {
    return !iBusy;
    }
