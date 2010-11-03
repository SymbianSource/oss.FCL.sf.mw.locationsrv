/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation of Launch Active Object class of Location 
*                Settings UI Server
*
*/


#include <aknViewAppUi.h>

#include "locsettingsuilaunchao.h"
#include "locsysuiengine.h"
#include "locsettingsuisrvappui.h"
#include "locsettingsuisrvdocument.h"
#include "locationui.h"
#include "locsettingsuiserverinterface.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::NewL()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvLaunchAO* CLocSettingsUISrvLaunchAO::NewL()
    {
    CLocSettingsUISrvLaunchAO* self = NewLC();
    CleanupStack::Pop(self);
    return self;    
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::NewLC()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvLaunchAO* CLocSettingsUISrvLaunchAO::NewLC()
    {
    CLocSettingsUISrvLaunchAO* self = new(ELeave) CLocSettingsUISrvLaunchAO;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;    
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::~CLocSettingsUISrvLaunchAO()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvLaunchAO::~CLocSettingsUISrvLaunchAO()
    {
    // Deque request queue and send KErrServerTerminated Message for every
    // request.
    TSglQueIter<CLocSettingsUIInfo> iter(iRequestQue);
    CLocSettingsUIInfo* tmp = iter++;
    while( tmp )
        {
        // Complete each message with KErrServerTerminated
        // deque and delete them.
        CompleteRequest(tmp, KErrServerTerminated);
        tmp = iter++;
        }
    // The Engine reference will be deleted in the document class destructor
    // and should not be deleted here.
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::EnqueueRequest( CLocSettingsUIInfo& aReqInfo )
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::EnqueueRequest( CLocSettingsUIInfo* aReqInfo )
    {
    // Enque the request and set the active object to active state if
    // it is already not active.
    TBool listWasEmpty = iRequestQue.IsEmpty();
    iRequestQue.AddLast(*aReqInfo);
    // If this is the first request in the queue then trigger the Active Object
    // to Run. This is the only scenario where we want to trigger the 
    // Active Object from this method. In all other cases we will trigger the
    // Run from the Cancel method or the SettingsClosed method.
    if ( listWasEmpty && !IsActive() )
        {
        ScheduleAORun();
        // Bring the Application UI to foreground.
        BringAppToForeground();
        }
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::CancelRequest( const CSession2* aSession )
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::CancelRequest( const CSession2* aSession )
    {
    // Sanity Check the Request Queue
    if (iRequestQue.IsEmpty())
        {
        // Nothing to do if the request queue is empty.
        return;
        }
    // There are 2 cases here
    // 1. The Settings UI is currently running. In this case this Active 
    //    Object's cancel needs to be called and then the request needs
    //    to be dequeued in RunL when KErrCancel is obtained.
    // 2. The Settings UI is enqueued in the Request Queue. Here the 
    //    request needs to be dequeued and just responded with KErrCancel.
    CLocSettingsUIInfo* req = iRequestQue.First();
    if (req->Session() == aSession)
        {
        // Found the Settings UI and it is currently running.
        // Call Cancel. The request will be dequeued once the Cancel
        // Synchronous call is completed.
        // The RunL is usaually completed by the time the cancel comes. So the
        // Active Object is not necessarily Active. hence the call to Cancel
        // will fail. Instead we can directly call the DoCancel here.
        DoCancel();
        // In case there are more requests in the queue set this Active Object
        // to active state so that the next request will be processed.
        if (!iRequestQue.IsEmpty())
            {
            ScheduleAORun();
            }
        else
            {
            // Since there are no more requests to process let us go into
            // background. We won't wait till the client sessions are closed
            // because sometimes the client might have a UI response mechanism
            // or might even keep the session object till the end.
            SendAppToBackground();
            }
        }
    else
        {
        // Search the whole Queue.
        TSglQueIter<CLocSettingsUIInfo> iter(iRequestQue);
        CLocSettingsUIInfo* tmp = iter++;
        while( tmp )
            {
            if (tmp->Session() == aSession)
                {
                // Found the Settings UI. Complete the message with
                // KErrCancel and deque this from the Queue.
                CompleteRequest(tmp, KErrCancel);
                break;
                }
            tmp = iter++;
            }
        }
    return;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::IsSrvRunningAsEmbeddedApp()
// ---------------------------------------------------------------------------
//
TBool CLocSettingsUISrvLaunchAO::IsSrvRunningAsEmbeddedApp() const
    {
    return iIsSrvRunningAsEmbeddedApp;
    }


// ---------------------------------------------------------------------------
// From Base class MLocationUIObserver
// CLocSettingsUISrvLaunchAO::LocationUIDismissed(TInt aErrorCode)
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::LocationUIDismissed( TInt aErrorCode )
    {
    CLocSettingsUIInfo* req = iRequestQue.First();
    CompleteRequest(req, aErrorCode);
    
    // The request has been completed. Hence, assign it to NULL
    iLocationUI = NULL;
    
    //Check if more requests are queued.
    if ( !iRequestQue.IsEmpty() )
        {
        // Set this Active Object to Active State, so that the next request
        // will be processed in the RunL.
        ScheduleAORun();
        }
    else
        {
        // Since there are no more requests to process let us go into
        // background. We won't wait till the client sessions are closed
        // because sometimes the client might have a UI response mechanism
        // or might even keep the session object till the end.
        SendAppToBackground();
        }
    }


// ---------------------------------------------------------------------------
// From Base class CActive
// CLocSettingsUISrvLaunchAO::RunL()
// 
// Once the first request gets queued we get to this RunL execution.
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::RunL()
    {
    if ( !iEngine )
        {
        // The Engine is created here and then ownership is passed to the
        // Document class. There are 2 reasons for this,
        // 1. The Engine needs to be created late because it takes a AppUi
        //    parameter. Hence it has to be delayed till the AppUi is
        //    created. Also this class needs to add itself as the Observer
        //    for the Engine. hence this place is logically correct to
        //    create the Engine class.
        //
        // 2. The Engine class destruction needs to be delayed till the AppUi
        //    is destructed since the CAknViewAppUi calls view deactivate at
        //    that stage. When the CLocSysUiEngine is deleted then it frees all
        //    the ECOM implementation as well. So for view based plugins the
        //    AppUi deactivate after Engine destruction will cause a 
        //    KERN-EXEC 3. Hence we delay the destruction to the Document class
        //    destructor.
        // But still we maintain a local reference in this Launch class so that
        // we don't need to access the document class everytime which result
        // in a call to CEikonEnv::Static().
        CLocSettingsUISrvAppUi* appUi = 
                        static_cast<CLocSettingsUISrvAppUi*>
                        (iEikEnv->EikAppUi());
        
        CLocSettingsUISrvDoc* doc =                         
                    static_cast<CLocSettingsUISrvDoc*>
                    (const_cast<CEikDocument*>(appUi->Document()));
        
        iEngine = CLocSysUiEngine::NewL(*appUi);
        // HandOver the Engine Ownership to the Document class.
        doc->SetEngineInstance(iEngine);
        }

    // Sanity check that there is atleast one request.
    if ( !iRequestQue.IsEmpty() )
        {
        CLocSettingsUIInfo* req = iRequestQue.First();
        
        // Create the Location UI corresponding to the Location Request
        switch( req->IPCMessage().Function())
            {
            case ELaunchSettingsWithString:
                {
                TRAPD(error, iLocationUI = iEngine->CreateLocationSubSettingsUIL(req->SettingsUID()););
                // TRAP and check whether the error is KErrAlreadyExists.
                // In this case we don't need to leave. For all other
                // error cases we leave here which will reuslt in the RunError
                // getting executed with the error code.
                if ( error != KErrNone && error != KErrAlreadyExists )
                    {
                    User::Leave(error);
                    }
                    
                // Set Full Screen App
                CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
                appUi->SetFullScreenState( ETrue );
                
                // Launch the Location UI
                iLocationUI->LaunchLocationUIL( req->SettingsUIStringParamsL(),
                                                this );             
                break;
                }
            case ELaunchSettings:
                {
                TRAPD(error, iLocationUI = iEngine->CreateLocationSubSettingsUIL(req->SettingsUID()););
                // TRAP and check whether the error is KErrAlreadyExists.
                // In this case we don't need to leave. For all other
                // error cases we leave here which will reuslt in the RunError
                // getting executed with the error code.
                if ( error != KErrNone && error != KErrAlreadyExists )
                    {
                    User::Leave(error);
                    }
                    
                // Set Full Screen App
                CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
                appUi->SetFullScreenState( ETrue );
                
                // Launch the Location UI
                iLocationUI->LaunchLocationUIL( req->SettingsUIIntParams(),
                                                this );                
                break;
                }
            case ELaunchPosSettings:
                {
                  TRAPD(error, iLocationUI = iEngine->CreatePositioningSettingsUIL(););
                // TRAP and check whether the error is KErrAlreadyExists.
                // In this case we don't need to leave. For all other
                // error cases we leave here which will reuslt in the RunError
                // getting executed with the error code.
                if ( error != KErrNone )
                    {
                    User::Leave(error);
                    }
                    
                // Set Full Screen App
                CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
                appUi->SetFullScreenState( ETrue );
                                    
                // Launch the Location UI
                iLocationUI->LaunchLocationUIL( 0, this );
                break;               
                }
            default:
                {
                User::Leave( KErrNotSupported );
                break;
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// From Base class CActive
// CLocSettingsUISrvLaunchAO::DoCancel()
// 
// Once the first request gets queued we get to this RunL execution.
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::DoCancel()
    {
    // Set Full Screen App
    CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
    appUi->SetFullScreenState( EFalse );
                    
        
    // Call the LocSysUiEngine Interface to Cancel the launched Settings UI
    CLocSettingsUIInfo* req = iRequestQue.First();
    // The Location UI Close is a synchronous call.
    iLocationUI->Close();
    
    // The UI request has been cancelled. Hence, setting the value to NULL
    iLocationUI = NULL;
        
    // Deque the request and respond with the Error Code KErrCancel.
    CompleteRequest(req, KErrCancel);
    }


// ---------------------------------------------------------------------------
// From Base class CActive
// CLocSettingsUISrvLaunchAO::RunError(TInt aError)
// ---------------------------------------------------------------------------
//
TInt CLocSettingsUISrvLaunchAO::RunError(TInt aError)
    {
    // Set Full Screen App
    CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
    appUi->SetFullScreenState( EFalse );                   
                        
    // No specific error scenario seen as of now. So doing the logical thing.
    // Just complete the current reuqest with the returned error code.
    CLocSettingsUIInfo* req = iRequestQue.First();
    CompleteRequest(req, aError);
    //Check if more requests are queued.
    if (!iRequestQue.IsEmpty())
        {
        // Set this Active Object to Active State, so that the next request
        // will be processed in the RunL.
        ScheduleAORun();
        }
    else
        {
        // Since there are no more requests to process let us go into
        // background. We won't wait till the client sessions are closed
        // because sometimes the client might have a UI response mechanism
        // or might even keep the session object till the end.
        SendAppToBackground();
        }
    return KErrNone;
    }    
    
// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::CLocSettingsUISrvLaunchAO()
// ---------------------------------------------------------------------------
//
CLocSettingsUISrvLaunchAO::CLocSettingsUISrvLaunchAO()
    :CActive(EPriorityUserInput),
     iRequestQue(CLocSettingsUIInfo::QueLinkOffset())
    {
    iEikEnv = CEikonEnv::Static();
    iIsSrvRunningAsEmbeddedApp = iEikEnv->StartedAsServerApp();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::ConstructL()
    {
    CActiveScheduler::Add(this);
    }


// ======== HELPER MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::CompleteRequest( CLocSettingsUIInfo* aReq,
//                                             TInt aErrorCode )
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::CompleteRequest( 
    CLocSettingsUIInfo* aReq,
    TInt aErrorCode )
    {
    // Set Full Screen App
    CLocSettingsUISrvAppUi* appUi = static_cast<CLocSettingsUISrvAppUi *>( iEikEnv->AppUi());
    appUi->SetFullScreenState( EFalse );
        
    // The UI request is completed. Hence, setting the value to NULL
    iLocationUI = NULL;
    
    // Sanity check the request queue.
    if ( !iRequestQue.IsEmpty() )
        {
        // If queue is not empty then the aReq is valid 
        // otherwise it is invalid
        aReq->Complete(aErrorCode);
        iRequestQue.Remove(*aReq);
        delete aReq;
        }
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::SendAppToBackground()
// This is specifically needed when Location Server is running as standalone
// and not embedded
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::SendAppToBackground()
    {
    if ( !IsSrvRunningAsEmbeddedApp() )
        {
        // Do this only if the server is running as standalone server.
        // In case of embedded server application, it will be closed 
        // immediately. Hence there is no need to take it to background.
        CAknAppUi* appUi = static_cast<CAknAppUi*>(iEikEnv->EikAppUi());
        // The last view cannot be destroyed till the server shutsdown.
        // Hence do a Hide from FSW here and move into background.
        // Once the shutdown timer is completed, the view will be destroyed.
        appUi->HideApplicationFromFSW(ETrue);
        if ( appUi->IsForeground() )
            {
            TApaTask task(iEikEnv->WsSession());
            task.SetWgId(iEikEnv->RootWin().Identifier());
            task.SendToBackground();
            }
        }
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::BringAppToForeground()
// This is specifically needed when Location Server is running as standalone
// and not embedded
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::BringAppToForeground()
    {
    if ( !IsSrvRunningAsEmbeddedApp() )
        {
        // Do this only if the server is running as standalone server.
        // In case of embedded server application, it will be closed 
        // immediately. Hence there is no need to bring it to foreground.
        CAknAppUi* appUi = static_cast<CAknAppUi*>(iEikEnv->EikAppUi());
        appUi->HideApplicationFromFSW(EFalse);
        TApaTask task(iEikEnv->WsSession());
        task.SetWgId(iEikEnv->RootWin().Identifier());
        task.BringToForeground();
        }
    }


// ---------------------------------------------------------------------------
// CLocSettingsUISrvLaunchAO::ScheduleAORun()
// ---------------------------------------------------------------------------
//
void CLocSettingsUISrvLaunchAO::ScheduleAORun()
    {
    SetActive();
    TRequestStatus* status = &iStatus;
    // This is a dummy AO to drive the UI launch requests. hence it is okay to
    // complete the request with KErrNone since all we want is to get the RunL
    // called by the Active Scheduler.
    User::RequestComplete(status, KErrNone);
    }
    
    
// End of file
