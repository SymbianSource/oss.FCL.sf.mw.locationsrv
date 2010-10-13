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
* Description:  Notifier class implementation for SuplSettings
*
*/



// INCLUDE FILES
#include    <centralrepository.h>

#include "epos_csuplsettings.h"
#include "epos_csuplsettingsnotifier.h"
#include "epos_msuplsettingsobserver.h"
#include "epos_csuplsettingsinternalcrkeys.h"

const TInt KMaxStrlen = 255;
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSettingsNotifier::CSuplSettingsNotifier(
    MSuplSettingsObserver& aObserver,TBool aDBListener)
    :CActive(CActive::EPriorityStandard),
    iObserver(aObserver),
    iStopObserve(EFalse),iDBListener(aDBListener),iImsi(NULL)
    {
    }
    

// EPOC default constructor can leave.
void CSuplSettingsNotifier::ConstructL()
    {
    CActiveScheduler::Add(this);
    iRepository = CRepository::NewL(KCRUidSuplSettings);
    iImsi = HBufC::NewL(KMaxStrlen);
    // Start listening
    StartListening();
    }

// Two-phased constructor.
CSuplSettingsNotifier* CSuplSettingsNotifier::NewL(
    MSuplSettingsObserver& aObserver,TBool aDBListener)
    {
    CSuplSettingsNotifier* self = 
        new (ELeave) CSuplSettingsNotifier(aObserver,aDBListener);

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }
    

// Destructor
CSuplSettingsNotifier::~CSuplSettingsNotifier()
    {
    Cancel();
    
    delete iRepository;
    iRepository = NULL;
    
    delete iImsi;
    }

// ---------------------------------------------------------
// CSuplSettingsNotifier::RunL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSettingsNotifier::RunL()
    {
    if(iDBListener) //if this flag is set this instance is listening for changes on the database
        {
        TInt event;
        iRepository->Get(KSuplSettingsDBEvent, event);
        TReal slpID;
        iRepository->Get(KSuplSettingsDBChangedSLPId, slpID);
        

        MSuplSettingsObserver::TSuplSettingsEventType eventType = (MSuplSettingsObserver::TSuplSettingsEventType)event;
        iObserver.HandleSuplSettingsChangeL(eventType,slpID);
        }
    else
        {
        TInt usage;

        iRepository->Get(KSuplSettingsUsage, usage);

        if (iUsage != usage) //check if there was a change in the Supl usage value in the cen rep
            iObserver.HandleSuplSettingsChangeL(MSuplSettingsObserver::ESuplSettingsEventSuplUsageChange);
        else //else check if any of the other config parameters changed
            {
            TInt fallBackValue;
            TInt fallBackTimerValue;
            TBuf<KMaxStrlen> imsi;
            iRepository->Get(KSuplSettingsFallBack,fallBackValue);
            iRepository->Get(KSuplSettingsFallBackTimer,fallBackTimerValue);
            iRepository->Get(KSuplSettingsIMSI,imsi);
            if(fallBackValue != iFallBackValue || fallBackTimerValue != iFallBackTimerValue || imsi.Compare(*iImsi))
                iObserver.HandleSuplSettingsChangeL(MSuplSettingsObserver::ESuplSettingsEventCommParameterChange);
            }
        }
    StartListening();
    }

// ---------------------------------------------------------
// CSuplSettingsNotifier::DoCancel
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSettingsNotifier::DoCancel()
    {
    iStopObserve = ETrue;
    if(iDBListener)
        {
        iRepository->NotifyCancel(KSuplSettingsDBEvent);
        }
    else
        iRepository->NotifyCancel(0x00000000, 0x00000000);
    }

// ---------------------------------------------------------
// CSuplSettingsNotifier::RunError
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSettingsNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CSuplSettingsNotifier::StartListening
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSettingsNotifier::StartListening()
    {
    iStatus = KRequestPending;

    if(iDBListener)
        {
        // Request for notification for DB field change
        iRepository->NotifyRequest(KSuplSettingsDBEvent, iStatus);
        }
    else
        {
        TInt usage;
        TBuf<KMaxStrlen> imsi;
        iRepository->Get(KSuplSettingsUsage, usage);
        iRepository->Get(KSuplSettingsFallBack,iFallBackValue);
        iRepository->Get(KSuplSettingsFallBackTimer,iFallBackTimerValue);
        iRepository->Get(KSuplSettingsIMSI,imsi);
        if(iImsi)
            iImsi->Des() = imsi;
        iUsage = (CSuplSettings::TSuplSettingsUsage) usage;

        // Request for notification for any field change
        iRepository->NotifyRequest(0x00000000, 0x00000000, iStatus);
        }
    
    SetActive();
    }
//  End of File
