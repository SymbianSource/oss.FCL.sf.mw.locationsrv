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
* Description:  ÑTestProviderNavigationServiceBase class implementation
*
*/


#include <e32base.h>
#include <eikenv.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>

#include "debug.h"
#include <mnnavigationservicebase.h>

#include "testproviderappui.h"
#include "testprovidernaviview.h"
#include "testproviderappserver.h"
#include "testproviderdump.h"

#include "testprovidernavigationservice.h"

extern CTestProviderAppUi* AppUi();

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderNavigationService* CTestProviderNavigationService::NewL()
    {
    CTestProviderNavigationService* self = new (ELeave) CTestProviderNavigationService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderNavigationService::CTestProviderNavigationService()
    {
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CTestProviderNavigationService::~CTestProviderNavigationService()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderNavigationService::ConstructL()
    {
    BaseConstructL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTestProviderNavigationService::HandleTestCommandL( TInt aCommand )
    {
    LOG1("CTestProviderNavigationService::HandleTestCommandL in, %d", aCommand );
    }

// ---------------------------------------------------------------------------
// From class CMnNavigationServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderNavigationService::HandleNavigateToL(
    const CPosLandmark& aLandmark)
    {
    LOG("CTestProviderNavigationService::HandleNavigateToL in ");

    CTestProviderAppServer* server = ( CTestProviderAppServer* ) Server();
    CTestProviderAppUi* ui = server->AppUi();

    CTestProviderDump* dump = CTestProviderDump::NewLC();
    dump->AddRunModeL( ui->IsChainedMode() );
    dump->AddLandmarkL( aLandmark );
    dump->CommitL();
    CleanupStack::PopAndDestroy( dump );
    
    ui->ShowNaviViewL();

    CEikAppUi* parentUi = ui->ContainerAppUi();
    LOG1("container UI: %d", parentUi );

    LOG("CTestProviderNavigationService::HandleNavigateToL out");
    }
        
// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CTestProviderNavigationService::DoCancel()
    {
    }
