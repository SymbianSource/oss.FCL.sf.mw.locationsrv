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
* Description:  The implementation of Shutdown Timer class of Location 
*                Settings UI Server
*
*/


#include <eikenv.h>

#include "locsettingsuisrvshutdowntimer.h"

//Local constants
// Shutdown Time 10 seconds expressed in micro seconds
const TInt KShutdownTime = 10000000;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::NewL()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvShutdown* CLocSettingsUISrvShutdown::NewL()
    {
    CLocSettingsUISrvShutdown* self = 
            new(ELeave) CLocSettingsUISrvShutdown();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

   
// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::~CLocSettingsUISrvShutdown()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvShutdown::~CLocSettingsUISrvShutdown()
    {
    iTimer.Cancel();
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::Start()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvShutdown::Start()
    {
    // Just to be safe, Cancel the Timer if it is running currently.
    iTimer.Cancel();
    iStatus = KRequestPending;
    iTimer.After(iStatus, KShutdownTime);
    SetActive();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::RunL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvShutdown::RunL()
    {
    // The server object will be deleted by the Application class which 
    // creates and owns the object
    // Shutting down the server by stopping the Active Scheduler loop.
    // Since currently there are no sessions, we don't need to explicitly
    // close sesisons.
    // Also since no requests are currently being executed, we don't need to
    // worry about another Active Scheduler loop running.
    CActiveScheduler::Stop();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::DoCancel()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvShutdown::DoCancel()
    {
    iTimer.Cancel();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::RunError(TInt aError)
// ---------------------------------------------------------------------------
//
TInt CLocSettingsUISrvShutdown::RunError(TInt /*aError*/)
    {
    // See if any error handling needed here.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::CLocSettingsUISrvShutdown()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvShutdown::CLocSettingsUISrvShutdown()
    :CActive(EPriorityStandard)
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvShutdown::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvShutdown::ConstructL()
    {
    CActiveScheduler::Add(this);
    TInt errorCode = iTimer.CreateLocal();
    User::LeaveIfError(errorCode);
    }


// End of file
