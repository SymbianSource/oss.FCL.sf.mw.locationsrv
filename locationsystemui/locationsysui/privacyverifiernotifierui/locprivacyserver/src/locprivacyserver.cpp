/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  server class implementation.
*
*/

// INCLUDE FILES

#include "locprivacyserver.h"
#include "locprivacycommon.h"
#include "locprivacyserversession.h"
#include "locprivacyserverdebugpanic.h"


// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CLocPrivacyServer::CLocPrivacyServer(TInt aPriority)
    : CServer2(aPriority),
    iNumSessions(0)
    {
    // This does not do anything.
    __DECLARE_NAME(_S("CLocPrivacyServer"));
    }

// EPOC default constructor can leave.
void CLocPrivacyServer::ConstructL()
    {
    User::LeaveIfError(Start(KLocPrivacyServerName));
    }

// Two-phased constructor
CLocPrivacyServer* CLocPrivacyServer::NewL()
    {
    CLocPrivacyServer* self = new (ELeave) CLocPrivacyServer(EPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CLocPrivacyServer::~CLocPrivacyServer()
    {
    }

// ---------------------------------------------------------
// CLocPrivacyServer::IncrementSessions
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLocPrivacyServer::IncrementSessions()
    {
    iNumSessions++;
    }

// ---------------------------------------------------------
// CLocPrivacyServer::DecrementSessions
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLocPrivacyServer::DecrementSessions(
    CLocPrivacyServerSession* /*aSession*/)
    {
    iNumSessions--;
    if (iNumSessions == 0)
        {
         // Shutdown the server by shutting down the active scheduler.
        CActiveScheduler::Stop();
        }
    }

// ---------------------------------------------------------
// CLocPrivacyServer::NewSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSession2* CLocPrivacyServer::NewSessionL(
    const TVersion& /*aVersion*/,
    const RMessage2& /*aMessage*/) const
    {
    
    // Make new session
    CLocPrivacyServerSession* newSession = CLocPrivacyServerSession::NewL(
            const_cast<CLocPrivacyServer&>(*this));

    return newSession;
    }

// ---------------------------------------------------------
// CLocPrivacyServer::RunError
// This method is called by the active scheduler whenever an
// untrapped leave occurs in the server active object.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLocPrivacyServer::RunError(TInt aError)
    {
    Message().Complete(aError);
    ReStart();
    return KErrNone;
    }


//  End of File
