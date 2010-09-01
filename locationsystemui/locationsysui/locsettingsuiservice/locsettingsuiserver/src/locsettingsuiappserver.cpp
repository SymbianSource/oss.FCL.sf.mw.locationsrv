/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation for AppServer class of Location Settings 
*                UI Server
*
*/


#include <apaserverapp.h>
#include <eikappui.h>
#include <aknViewAppUi.h>
#include <eikenv.h>
#include <aknappui.h>
#include <apgwgnam.h>

#include "locsettingsuiappserver.h"
#include "locsettingsuiserverinterface.h"
#include "locsettingsuilaunchmgr.h"
#include "locsettingsuiservice.h"
#include "locsettingsuisrvshutdowntimer.h"
#include "locsettingsuisrvappui.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::CLocSettingsUIAppServer()
// ---------------------------------------------------------------------------
//
CLocSettingsUIAppServer::CLocSettingsUIAppServer()
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIAppServer::ConstructL(const TDesC& aFixedServerName)
    {
    iShutdownTimer = CLocSettingsUISrvShutdown::NewL();
    // Base Call and pass the Server Name to be used.
    // Will be called by Framework. Not needed here.
    CAknAppServer::ConstructL(aFixedServerName);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::CLocSettingsUIAppServer()
// ---------------------------------------------------------------------------
//
CLocSettingsUIAppServer::~CLocSettingsUIAppServer()
    {
    delete iShutdownTimer;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::CancelShutdownTimer()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIAppServer::CancelShutdownTimer()
    {
    iShutdownTimer->Cancel();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::CreateServiceL
// ---------------------------------------------------------------------------
//
CApaAppServiceBase* CLocSettingsUIAppServer::CreateServiceL(
    TUid aServiceType) const
    {
    if (KLocSettingsUiServiceId == aServiceType.iUid)
        {
        return new(ELeave) CLocSettingsUIService;
        }
        
    return CAknAppServer::CreateServiceL(aServiceType);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIAppServer::HandleAllClientsClosed()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIAppServer::HandleAllClientsClosed()
    {
    // The default behaviour is to close server immediately.
    // This is what we want when the server is running as Embedded Application.
    // But when the server is processing Framework request, we need to run
    // a shutdown timer. When the Timer expires, the server is closed.
    // In case a new request comes in before the timer expires, the timer is
    // cancelled.
    if ( CEikonEnv::Static()->StartedAsServerApp() )
        {
        // If running as embedded application server then stop immediately.
        CActiveScheduler::Stop();
        }
    else
        {
        iShutdownTimer->Start();
        }
    }


// End of file
