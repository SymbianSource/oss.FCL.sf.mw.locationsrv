/*
 * Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  Verifier plug-in which derives from CPosPrivacyNotifier.
 *
 */

// INCLUDE FILES
#include <eikenv.h>
#include <eiknotapi.h>
#include <AknIconArray.h>
#include <apgcli.h>
#include <lbs/epos_rposrequestorstack.h>
#include <epos_csuplsettingsconstants.h>
#include <StringLoader.h>
#include <locverifierdlg.rsg>
#include "lpdverifierplugin.h"
#include "lpdrequestao.h"
#include "lpdrequestorprocessor.h"
#include "lpdverifierquerylauncher.h"
#include "lpdnotifierquerylauncher.h"
#include "lpdbasemodel.h"
#include "locconsts.h"
#include "locverifierdlgdebug.h"
#include "locfileutils.h"
#include "locrequestorutilsresolver.h"
#include "lpdperiodicprocessor.h"

// INCLUDE FILES
#include <s32mem.h>
#include "locphonenumberformat.h"
#include "locverifiercoverui.h"
// CONSTANTS

// The Increment size for the package buffer used for packing the descriptors
const TInt KReqBufferIncrSize = 256;
const TInt KLpdItemArrayGranularity = 50;

const TInt KNonPeriodicRequest = 0;
const TInt KPeriodicRequest = 1;

const TInt KNotifyMessageLength = 300;

// CONSTANTS

// Unnamed namespace for local definitions

const MEikSrvNotifierBase2::TNotifierPriority KNotifierPriority =
        MEikSrvNotifierBase2::ENotifierPriorityHigh;
const TUid KNotifierChannel =
    {
    0x100065ac
    };
_LIT_SECURE_ID(KUikonSrvSecureId,0x10003a4a);
#ifdef _DEBUG
_LIT( KPanicText, "CLpdVerifierPlugin" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CLpdVerifierPlugin
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdVerifierPlugin::CLpdVerifierPlugin() :
    iCurrentRequest(KPosNullQNRequestId), iPeriodicNotQue(
            KLpdItemArrayGranularity)

    {
    iEnv = CEikonEnv::Static();
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::ConstructL()
    {
    BaseConstructL(KNotifierChannel, KNotifierPriority);

    TFileName* resourceFile = new (ELeave) TFileName;
    CleanupStack::PushL(resourceFile);
    // these appends are always safe:
    resourceFile->Append(KLocNotifierRscPath);
    resourceFile->Append(KLocVerifierRscFileName);
    TFileName* dllDrive = new (ELeave) TFileName;
    CleanupStack::PushL(dllDrive);
    Dll::FileName(*dllDrive);
    LocFileUtils::GetNearestLanguageFileL(iEnv->FsSession(), *dllDrive,
            *resourceFile);
    CleanupStack::PopAndDestroy(dllDrive);
    iResourceOffset = iEnv->AddResourceFileL(*resourceFile);
    CleanupStack::PopAndDestroy(resourceFile);

    iRtorProcessor = CLpdRequestorProcessor::NewL();
    iPeriodicProcessor = CLpdPeriodicProcessor::NewL( *this );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdVerifierPlugin* CLpdVerifierPlugin::NewL()
    {
    CLpdVerifierPlugin* self = new (ELeave) CLpdVerifierPlugin;

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CLpdVerifierPlugin::~CLpdVerifierPlugin()
    {
    // Destruction of this plugin should only occur only in shutdown
    // or in severe problem situation.

    // A very special scenario is that base class construction leaves and
    // this destructor is called. In that case CompleteAllRequests() causes
    // access violation (noticed this by checking source code of base class).
    if (NotifierBase())
        { // base class has been fully constructed, method call is safe
        CompleteAllRequests(KErrGeneral);
        }

    // It is enough to delete queries so when don't get callbacks.
    FreeQueryResources();
    iPeriodicNotQue.Close();
    delete iPeriodicProcessor;
    delete iRequestActiveObject;
    delete iRtorProcessor;
    iEnv->DeleteResourceFile(iResourceOffset);
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNewRequestL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNewRequestL(TPosQNRequestId aRequestId)
    {
    EnqueIfPeriodicL(aRequestId);
    // Check whether the notifier is already handling a request
    // If yes, do nothing for now.
    if (iRequestActiveObject)
        {
        return;
        }
    else
        {
        iRequestActiveObject = CLpdRequestAO::NewL(*this);
        iRequestActiveObject->ScheduleRequest();
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::EnqueIfPeriodicL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::EnqueIfPeriodicL(TPosQNRequestId aRequestId)
    {
    SetCurrentRequestL(aRequestId);
    TRequestType requestType = RequestTypeL(aRequestId);
    CPosRequestor::TRequestType requestorType = CheckRequestTypeL();

    if (requestType == ENotification && requestorType
            == CPosRequestor::ERequestPeriodic)
        {
        TInt64 sessionId = -1;
        GetSessionIdL( sessionId );

        TLpdPeriodicReqInfo newReq(aRequestId, sessionId);

        if (iPeriodicProcessor)
            {
            if (iPeriodicProcessor->GetSessionId() == sessionId)
                {
                CompleteRequest(aRequestId, KErrNone);
                }
            return;
            }
        else
            {
            TIdentityRelation<TLpdPeriodicReqInfo> matcher(
                    TLpdPeriodicReqInfo::MatchSession);
            TInt index = iPeriodicNotQue.Find(newReq, matcher);

            // Remove if it was present in Que
            if (index == KErrNotFound)
                {
                iPeriodicNotQue.Append(newReq);
                }
            else
                {
                CompleteRequest(aRequestId, KErrNone);
                }
            }
        }

    if (KPosNullQNRequestId != iCurrentRequest)
        SetCurrentRequestL(iCurrentRequest);
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleRequestCancelled
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleRequestCancelled(TPosQNRequestId aRequestId)
    {
    // Check whether the request is coming from Uikon Server. 
    // If not reject this request.
    if (!CheckClientSecureId(KUikonSrvSecureId))
        {
        return;
        }

    // This method is called for Verification Query Cancellation.
    // There are 2 scenarios when the cancellation can come.
    // 1. The verification query for which the cancellation is done is 
    //         currently is running
    // 2. The verification query is not running currently but is in the 
    //         requests Queue maintained by the base class.
    //
    // Case 1 :
    //		
    // If there is a Verification query currently running and the requestId
    //         matches then the cancel notificaiton has to be popped up.
    //
    // Case 2 :
    //		
    //     1. A Verification query was popped up.
    //     2. The network timed out and sent a cancel request which popped 
    //           up a notification request.
    //     3. The network sends another verification query in the mean time
    //           and also cancels it. The resulting notification dialog will
    //           not be run since there is still a notification that has not
    //           been dismissed by the user.
    //
    // Hence in case 2 the notification information is maintained in a Que 
    // in the iRequestActiveObject. Once the current notification dialog 
    // closes and there are no more verification dialogs pending, the 
    // notification dialogs are run one after the other.

    if (aRequestId != iCurrentRequest)
        {
        if (aRequestId == KPosNullQNRequestId)
            {
            return;
            }
        //Put this new Notification Request on the queue and then process
        //it later from iRequestActiveObject(CLpdRequestAO class) RunL.		
        RPosRequestorStack* requestors = NULL;
            TRAP_IGNORE(
                    SetCurrentRequestL(aRequestId);
                    // requestors is allocated in iRtorProcessor and ownership is finally
                    // transferred to iRequestActiveObject.
                    requestors = iRtorProcessor->RetrieveRequestorsL( *this );
                    // Enqueue the request to iRequestActiveObject
                    iRequestActiveObject->EnqueueRequestL(RequestSource(),
                            CancelReason(),
                            QueryTimeoutStrategy(),
                            requestors););
        return;
        }

    iCancelInfo.iCancelled = ETrue;
    iCancelInfo.iReason = CancelReason();
    iCancelInfo.iDecision = QueryTimeoutStrategy();
    iCancelInfo.iSource = RequestSource();

    if (iCurrentRequestType == EQuery)
        { // Verification was cancelled
        __ASSERT_DEBUG( iVerifierQuery, HandleDebugAssertError() );
        __ASSERT_DEBUG( !iNotifier, HandleDebugAssertError() );
        iVerifierQuery->Cancel();
        }
    else
        {
        // It must be a notification then, this case is not probable but
        // we can cancel the dialog if this would happen.
        __ASSERT_DEBUG( iCurrentRequestType == ENotification,
                HandleDebugAssertError() );
        __ASSERT_DEBUG( !iVerifierQuery, HandleDebugAssertError() );
        __ASSERT_DEBUG( iNotifier, HandleDebugAssertError() );
        iNotifier->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleAllRequestCancelled
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleAllRequestCancelled()
    {
    if (iCurrentRequest != KPosNullQNRequestId)
        { // current request requires some specific behavior
        HandleRequestCancelled(iCurrentRequest);
        }
    // Note that ScheduleRequest in the end of HandleRequestCancelled() allows
    // current call chain run to completion and resources are released after
    // that. Pending requests have been completed so they won't be processed.
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleVerificationResultL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleVerificationResultL(TInt aResultCode)
    {
    iVerifyResult = aResultCode;

        LOCVERIFIERDLGDEBUG1( "CLpdVerifierPlugin::HandleVerificationResultL(%d)",
                iVerifyResult );

    switch (iVerifyResult)
        {
        case KErrNone: // fall through
        case KErrAccessDenied:
            {
            // No need for rules now in 3.0
            break;
            }
        case KErrTimedOut:
            { // UI's internal timer expired
            break;
            }
        case KErrCancel:
            {
            __ASSERT_DEBUG( iCancelInfo.iCancelled, HandleAssertErrorL() );
            NotifyCancellationL(iCancelInfo.iSource, iCancelInfo.iReason,
                    iCancelInfo.iDecision);

            return; // don't handle next req. yet
            }
        case KErrAbort: // This is used for emergency call support
            {
            CompleteAllRequests(iVerifyResult);
            // ScheduleRequest() allows the current call chain
            // run to completion and resources are released after that.
            iRequestActiveObject->ScheduleRequest();
            return;
            }
        default:
            {
            iEnv->HandleError(iVerifyResult);
            break;
            }
        }

    CompleteCurrentAndContinue(iVerifyResult); // this request was handled
    }
// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNotificationResultL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNotificationResultL(TInt aResultCode)
    {

    TLpdPeriodicReqInfo newReq(iCurrentRequest, -1);
    TIdentityRelation<TLpdPeriodicReqInfo> matcher(
            TLpdPeriodicReqInfo::MatchPrivacy);
    TInt index = iPeriodicNotQue.Find(newReq, matcher);

    // Remove if it was present in Que
    if (index != KErrNotFound)
        {
        iPeriodicNotQue.Remove(index);
        }
    CompleteRequest(iCurrentRequest, aResultCode);
    iCurrentRequest = KPosNullQNRequestId;

    switch (aResultCode)
        {
        case KErrNone: // fall through
        case KErrTimedOut: // fall through
            {
            break;
            }
        case KErrCancel:
            {
            break;
            }
        case KErrAbort: // This is used for emergency call support
            {
            CompleteAllRequests(aResultCode);
            // ScheduleRequest() -> allows the current call chain
            // run to completion and resources are released after that.
            break;
            }
        default:
            {
            iEnv->HandleError(aResultCode);
            break;
            }
        }

    iRequestActiveObject->ScheduleRequest(); // handle next req.
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleLeave
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleLeave(TInt aError)
    {
        LOCVERIFIERDLGDEBUG1( "CLpdVerifierPlugin::HandleLeave(%d)", aError);
    // In this case user needs feedback about the error situation:
    iEnv->HandleError(aError);

    // In case of leave current request is completed with
    // iVerifyResult, but queue handling is still continued.
    // iVerifyResult is better completion code for request than aError.
    CompleteCurrentAndContinue(iVerifyResult);
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextRequest()
    {
    TRAPD( err, HandleNextRequestL() );
    if (err)
        {
        // In case of leave current request is completed with
        // error code, but queue handling is still continued.

        // If we couldn't start handling the request it is
        // better not to confuse user with an error note.

        CompleteCurrentAndContinue(iVerifyResult);
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextRequestL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextRequestL()
    {

    // It is better to free previous query resources here, because
    // now all callback methods have finished (active object allowed
    // run to completion)
    this->FreeQueryResources();

    // Read the next request:
    RArray<TPosQNRequestId> requests;
    CleanupClosePushL(requests);
    GetRequestsL(requests);

    if (requests.Count() == 0)
        { // No more requests to handle
        CleanupStack::PopAndDestroy(); // requests
        // This is a very important step, it allows new requests
        // to flow in ( see HandleNewRequestL() ):
        delete iRequestActiveObject;
        iRequestActiveObject = NULL;
        return;
        }
    iCurrentRequest = requests[0];
    CleanupStack::PopAndDestroy(); // requests
    SetCurrentRequestL(iCurrentRequest);

    // Check whether the request is coming from Uikon Server. If not reject this request.
    if (!CheckClientSecureId(KUikonSrvSecureId))
        {
        CompleteCurrentAndContinue(KErrPermissionDenied);
        return;
        }
   
    // Check the request type
    iCurrentRequestType = RequestTypeL(iCurrentRequest);

    if ((CheckRequestTypeL() == CPosRequestor::ERequestPeriodic)
            && (iCurrentRequestType == ENotification))
        iRtorProcessor->SetRequestType(KPeriodicRequest);
    else
        iRtorProcessor->SetRequestType(KNonPeriodicRequest);

    iRtorProcessor->ReadRequestorsL(*this);

    if (iCurrentRequestType == EQuery)
        {
        HandleNextVerificationL();
        }
    else if (iCurrentRequestType == ENotification)
        {
        HandleNextNotificationL();
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextVerificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextVerificationL()
    {
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    __ASSERT_DEBUG( !iVerifierQuery, HandleAssertErrorL() );
    TInt suplRequest;
    if (CheckRequestTypeL() == CPosRequestor::ERequestPeriodic)
        suplRequest = 1;
    else
        suplRequest = 0;

    TPosRequestSource source(RequestSource());
    if (source == EPosRequestSourceNotAvailable)
        {
        CompleteCurrentAndContinue(KErrNone);
        }
    else
        {
        if(suplRequest)
        {	
        iPeriodicProcessor->SetRequestorIdL( CurrentRequest() );     
        iPeriodicProcessor->SetRequestTypeL(iCurrentRequestType);
        
        CPosRequestor* req = NULL;
        if( RequestorCountL() )
        {
          req = RequestorLC( 0 );
          CleanupStack::Pop();
          }
        iPeriodicProcessor->SetRequestorL( req );

      }
      
      iVerifierQuery = CLpdVerifierQueryLauncher::NewL(*this);
      TPosRequestDecision decision(QueryTimeoutStrategy());
      // Note that item is left in cleanupstack until StartQuery():
      if (suplRequest)
          iVerifierQuery->PrepareSuplVerificationResourcesL();
      else
          iVerifierQuery->PrepareVerificationResourcesL(source, decision);

       // Ownership of text is immediatelly transferred:
       CLpdBaseModel* requestors = iRtorProcessor->RtorNamesForVerifNotifL(
                iVerifierQuery->ListBoxL());
       iVerifierQuery->SetQueryTextArray(requestors);

        // Start the Verification query
        StartQueryDialogL(iVerifierQuery, EVerificationRequest, decision);
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextNotificationL()
    {
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    
    if (CheckRequestTypeL() == CPosRequestor::ERequestPeriodic)
        HandleNextPeriodicNotificationL();
    else
        HandleNextNonPeriodicNotificationL();

    }
// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextNonPeriodicNotificationL()
    {
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );
    __ASSERT_DEBUG( !iNotifier, HandleAssertErrorL() );

    if (RequestSource() != EPosRequestSourceNetwork)
        { // Notifications are supported only for network requests
        User::Leave(KErrNotSupported);
        }

    TPosNotificationReason reason = NotificationReason();
    switch (reason)
        {
        // Decision without user's consent:
        case EPosDecisionByRequestSource:
            // Policy conflict, network timeout occurred before user's
            // response was received
            {
          
            TPosRequestDecision decision = LocationRequestDecision();
            if(!iNotifier)
            iNotifier = CLpdNotifierQueryLauncher::NewL(*this);
            iNotifier->PrepareNotificationResourcesL(reason, decision);
            CLpdBaseModel* requestors =
                    iRtorProcessor->RtorNamesForVerifNotifL(
                            iNotifier->ListBoxL());

            iNotifier->SetQueryTextArray(requestors);

            // Start the notification query
            StartQueryDialogL(iNotifier, ENotification, decision);
            break;
            }
        case EPosVerificationTimeout:
            {
            TPosRequestDecision decision = LocationRequestDecision();
            if(!iNotifier)
            iNotifier = CLpdNotifierQueryLauncher::NewL(*this);
            iNotifier->PrepareNotificationResourcesL(reason, decision);
            CLpdBaseModel* requestors =
                    iRtorProcessor->RtorNamesForVerifNotifL(
                            iNotifier->ListBoxL());
            iNotifier->SetQueryTextArray(requestors);

            // Start the notification query
            StartQueryDialogL(iNotifier, ENotificationTimeout, decision);

            break;
            }
        case EPosNotificationReasonNotAvailable: // fall through
        default: // Future extensions -> EPosNotificationReasonNotAvailable
            {
            User::Leave(KErrNotSupported);
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleNextPeriodicNotificationL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleNextPeriodicNotificationL()
    {
    __ASSERT_DEBUG( iCurrentRequest != KPosNullQNRequestId,
            HandleAssertErrorL() );

    if (RequestSource() != EPosRequestSourceNetwork)
        { // Notifications are supported only for network requests
        User::Leave(KErrNotSupported);
        }

    TPosNotificationReason reason = NotificationReason();

    switch (reason)
        {
        // Decision without user's consent:
        case EPosDecisionByRequestSource:
            // Policy conflict, network timeout occurred before user's
            // response was received
            {
            iPeriodicProcessor->SetRequestorIdL( CurrentRequest() );     
            iPeriodicProcessor->SetRequestTypeL(iCurrentRequestType);
            CPosRequestor* req = NULL;
            if( RequestorCountL() )
            {
            req = RequestorLC( 0 );
            CleanupStack::Pop();
            }
            iPeriodicProcessor->SetRequestorL( req );
        
            TPosRequestDecision decision = LocationRequestDecision();
            
            if(!iNotifier)
            iNotifier = CLpdNotifierQueryLauncher::NewL(*this);

            TInt64 sessionId = -1;
            sessionId = iRtorProcessor->GetSessionId();
            iPeriodicProcessor->SetSessionIdL(sessionId);

            HBufC* requestorName = NULL;
            if (iRtorProcessor->Requestors().Count() == 0)
                {
                requestorName = StringLoader::LoadL(R_LPD_UNKNOWN_REQUESTER);
                }
            else
                {
                const CPosRequestor& requestor =
                        *(iRtorProcessor->Requestors())[1];
                iUtils = CLocRequestorUtilsResolver::NewL();
                requestorName = iUtils->RequestorNameL(requestor);
                if ((requestorName->Des()).CompareC(_L("Unknown")) == 0)
                    {
                    requestorName = NULL;
                    requestorName = StringLoader::LoadL(
                            R_LPD_UNKNOWN_REQUESTER);
                    }
                delete iUtils;
                iUtils = NULL;
                }
            iPeriodicProcessor->SetRequestorNameL(requestorName);

            delete requestorName;
            iNotifier->PrepareSuplNotificationResourcesL(reason);

            HBufC* notifyMessage = HBufC::NewLC(KNotifyMessageLength);
            TRAPD( err, iPeriodicProcessor->NotificationMessageTextL(notifyMessage));
  
            if (err == KErrNotFound)
                {
                    CleanupStack::PopAndDestroy(); // notifyMessage
                    CompleteCurrentAndContinue(err);
                }
            else
                {
                iNotifier->SetMessageQueryTextL(notifyMessage->Des(),
                        iPeriodicProcessor->LinkCallBack());

                CleanupStack::PopAndDestroy(); // notifyMessage
                // Start the notification query
                StartQueryDialogL(iNotifier, ESuplPeriodicNotification,
                        decision);
                }
            break;
            }
        case EPosVerificationTimeout:
            {
            break;
            }
        case EPosNotificationReasonNotAvailable: // fall through
        default: // Future extensions -> EPosNotificationReasonNotAvailable
            {
            User::Leave(KErrNotSupported);
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::NotifyCancellationL
// Helper method
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::NotifyCancellationL(TPosRequestSource aSource,
        TPosVerifyCancelReason aReason, TPosRequestDecision aDecision)
    {
    __ASSERT_DEBUG( iCurrentRequestType == EQuery, HandleAssertErrorL() );

    if (aSource == EPosRequestSourceNetwork)
        { // Notifications supported only for network-originated requests

        switch (aReason)
            {
            case EPosCancelReasonTimeout:
                {
                __ASSERT_DEBUG( !iNotifier, HandleAssertErrorL() );
				if(aDecision != EPosDecisionNotAvailable)
					{
	                iNotifier = CLpdNotifierQueryLauncher::NewL( *this );
	                iNotifier->PrepareCancelNotifResourcesL( aDecision );
	                CLpdBaseModel* requestors =
	                                iRtorProcessor->RtorNamesForVerifNotifL(
	                                                    iNotifier->ListBoxL() );
	                iNotifier->SetQueryTextArray( requestors );

                    // Start the notification query
                    StartQueryDialogL( iNotifier,
                                       ECancelNotification,
                                       aDecision );
                
	                return;
					}
				break;
                }
            case EPosCancelReasonNotAvailable: // fall through
            default: // future extensions -> EPosCancelReasonNotAvailable
                {
                break;
                }
            }
        }

    // If notification dialog was not launched, handle next request:
    iRequestActiveObject->ScheduleRequest();

    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CompleteCurrentAndContinue
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::CompleteCurrentAndContinue(TInt aResultCode)
    {
    if (iCurrentRequest != KPosNullQNRequestId)
        {
        CompleteRequest(iCurrentRequest, aResultCode);
        }
    iCurrentRequest = KPosNullQNRequestId;

    __ASSERT_DEBUG( iRequestActiveObject, HandleDebugAssertError() );
    iRequestActiveObject->ScheduleRequest(); // handle next req.
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::FreeQueryResources
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::FreeQueryResources()
    {
    if(iPeriodicProcessor)
    iPeriodicProcessor->ResetParameters();
    iCurrentRequest = KPosNullQNRequestId;
    delete iVerifierQuery;
    iVerifierQuery = NULL;
    iVerifyResult = KErrGeneral;
    iCancelInfo.iCancelled = EFalse;
    delete iNotifier;
    iNotifier = NULL;
    if (iRtorProcessor)
        { // if already construction fails iRtorProcessor may
        // be NULL, otherwise it points to an instance.
        iRtorProcessor->ResetAndDestroyRequestors();
        }
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleAssertErrorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleAssertErrorL() const
    {
#ifdef _DEBUG        
    User::Panic(KPanicText, KLpdErrGeneral);
#else
    User::Leave( KErrCorrupt );
#endif
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::HandleDebugAssertError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::HandleDebugAssertError() const
    {
#ifdef _DEBUG
    User::Panic(KPanicText, KLpdErrGeneral);
#endif
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::NotifyTimeoutL
// Helper method
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::NotifyCancellationL(
    TPosRequestSource aSource,
    TPosVerifyCancelReason aReason,
    TPosRequestDecision aDecision,
    const RPosRequestorStack& aRequestors )
    {
    if ( aSource == EPosRequestSourceNetwork )
        { // Notifications supported only for network-originated requests

        switch ( aReason )
            {
            case EPosCancelReasonTimeout:
                {
                __ASSERT_DEBUG( !iNotifier, HandleAssertErrorL() );
				if(aDecision != EPosDecisionNotAvailable)
					{
	                iNotifier = CLpdNotifierQueryLauncher::NewL( *this );
	                iNotifier->PrepareCancelNotifResourcesL( aDecision );
	                iRtorProcessor->ReadRequestorsL(aRequestors);
	                CLpdBaseModel* requestors =
	                                iRtorProcessor->RtorNamesForVerifNotifL(
	                                                    iNotifier->ListBoxL() );
	                iNotifier->SetQueryTextArray( requestors );
                    
                    // Start the notification query
                    StartQueryDialogL( iNotifier,
                                       ECancelNotification,
                                       aDecision );
                
	                return;
					}
				break;
                }
            case EPosCancelReasonNotAvailable: // fall through
            default: // future extensions -> EPosCancelReasonNotAvailable
                {
                break;
                }
            }
        }

    // If notification dialog was not launched, handle next request:
    iRequestActiveObject->ScheduleRequest();
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::StartQueryDialogL
// Starts the Query dialog
// @param aQueryDialog Query dialog that has to be started
// @param aDialogType  Dialog type
// @param aDecision    Decision Type
// -----------------------------------------------------------------------------
void CLpdVerifierPlugin::StartQueryDialogL(
        CLpdQueryLauncherBase* aQueryDialog, TDialogType aDialogType,
        TPosRequestDecision aDecision)
    {
    // Set the Command ID
    TInt coverUiCmd = CoverUICommandL(aDialogType, aDecision);

    aQueryDialog->SetCoverUICommand(coverUiCmd);
    // Set the Requestor buffer                               
    CBufFlat* reqBuffer = PackRequestorBufferL();
    CleanupStack::PushL(reqBuffer);

    TPtr8 reqBufferPtr(reqBuffer->Ptr(0));
    aQueryDialog->SetRequestorBuffer(reqBufferPtr);

    if (aDialogType == ESuplPeriodicNotification)
        aQueryDialog->StartSuplPeriodicQueryL();
    else
        aQueryDialog->StartQueryL();
    
    CleanupStack::PopAndDestroy(reqBuffer);


    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::PackRequestorBufferL
// Packs the Requestor buffer to send across to Cover UI. The 
// ownership of the buffer is transferred back to the callee
// @return CBufFlat*    Flat buffer containing the list of requestors    
// -----------------------------------------------------------------------------
//
CBufFlat* CLpdVerifierPlugin::PackRequestorBufferL()
    {
    // Construct the conversion buffers and the package buffer
    CBufFlat* requestorBuffer = CBufFlat::NewL(KReqBufferIncrSize);
    CleanupStack::PushL(requestorBuffer);

    // Opening a Write stream to assist in packing data onto CBufFlat
    RBufWriteStream writeStream;
    writeStream.Open(*requestorBuffer);
    CleanupClosePushL(writeStream);

    // Obtain the list of requestors from which the individual requestor
    // Ids can be obtained
    const RPointerArray<CPosRequestor>& requestors =
            iRtorProcessor->Requestors();

    // The buffer is packed according to the below mentioned
    // format.
    // 
    // | Count | Length(1) | String(1) | ..... | Length( Count ) | String( Count ) |
    // 
    // where,
    // Count   - ( 4 bytes ) - The number of requestor strings. 
    // Length(n) - ( 4 bytes ) - Length of the nth requestor string.
    // String(n) - ( Length(n) * 2 bytes ) - String for the nth requestor.
    // 
    // Each entry is packed one after another without any delimters between them
    //

    // Packing the count
    writeStream.WriteInt32L(requestors.Count());

    for (TInt i = 0; i < requestors.Count(); i++)
        {
        const CPosRequestor& req = *(requestors[i]);

        // Obtain the Requestor Id string
        TPtrC reqStr = req.RequestorIdString();

        // We need a 16 bit buffer for converting. Since the requestor strings that
        // need to be packed have to always be of the UCS-2 ( UNICODE ) format.
        // The buffer would be allocated just before copying the requestor content
        HBufC16* conversionBuf = NULL;
        TPtr16 conBufPtr(0, 0);

        // If the string is a phone number then it needs to be grouped and the
        // converted to a UNICODE string. If not, then simpy convert it to
        // a UNICODE string
        if (CPosRequestor::EIdFormatPhoneNumber == req.RequestorIdFormat())
            {
            // Creating the Phone number formatter for grouping
            CLocPhoneNumberFormat* formatter = CLocPhoneNumberFormat::NewL();
            CleanupStack::PushL(formatter);

            HBufC* groupedName = NULL;
            TRAPD( error, groupedName = formatter->PhoneNumberGroupL( reqStr ));
            if (error || !groupedName)
                {
                // If there was a Leave in the grouping function or the 
                // grouping failed then pack the original name

                // Allocate the conversion buffer and copy the Requestor string
                conversionBuf = HBufC16::NewL(reqStr.Length());
                conBufPtr.Set(conversionBuf->Des());

                // Copy the original string to the conversion buffer
                conBufPtr.Copy(reqStr);
                }
            else
                {
                // Take ownership of the grouped phone number buffer and 
                // copy it to the conversion buffer
                CleanupStack::PushL(groupedName);

                TPtr groupedNamePtr = groupedName->Des();

                // Allocate the conversion buffer and copy the Requestor string
                conversionBuf = HBufC16::NewL(groupedNamePtr.Length());
                conBufPtr.Set(conversionBuf->Des());

                // Copy the grouped string to the conversion buffer
                conBufPtr.Copy(groupedNamePtr);

                CleanupStack::PopAndDestroy(groupedName);
                }

            CleanupStack::PopAndDestroy(formatter);
            }
        else
            {
            // Copy the Requestor ID string

            // Allocate the conversion buffer and copy the Requestor string
            conversionBuf = HBufC16::NewL(reqStr.Length());
            conBufPtr.Set(conversionBuf->Des());

            // Copy the original string to the conversion buffer
            conBufPtr.Copy(reqStr);
            }

        // Push the Conversion buffer to the CleanupStack
        CleanupStack::PushL(conversionBuf);

        // The conversion buffer now contains the string in the UNICODE 
        // format. First pack the length of the string and then the 
        // actual string itself
        writeStream.WriteInt32L(conBufPtr.Length());

        TUint8* buffer = (TUint8*) (conBufPtr.Ptr());
        writeStream.WriteL(buffer, conBufPtr.Size());

        // Destroying the Conversion buffer before it goes out of scope
        CleanupStack::PopAndDestroy(conversionBuf);
        }

    // flush stream data to our buffer
    writeStream.CommitL();
    CleanupStack::PopAndDestroy(&writeStream);

    // The ownership of the requestor buffer should be transferred to the callee
    CleanupStack::Pop(requestorBuffer);

    return requestorBuffer;
    }

// -----------------------------------------------------------------------------
// Determines the Cover UI command corresponding to a paricular
// request
// @param  TRequestType         Request Type
// @param  TPosRequestDecision  Decision Type
// @return TInt                 Cover UI Command ID.
// -----------------------------------------------------------------------------
//
TInt CLpdVerifierPlugin::CoverUICommandL(TDialogType aRequestType,
        TPosRequestDecision aDecision)
    {
    TInt result = 0;

    if (EVerificationRequest == aRequestType)
        {
        switch (aDecision)
            {
            case EPosDecisionAccepted:
                {
                result = ECmdDefaultAccept;
                break;
                }
            case EPosDecisionRejected:
                {
                result = ECmdDefaultReject;
                break;
                }
            default:
                {
                result = ECmdDefaultNone;
                break;
                }
            }
        }
    else if ((ENotification == aRequestType) || (ESuplPeriodicNotification
            == aRequestType))
        {
        result = ECmdNotifyAccept;
        }
    else if (ENotificationTimeout == aRequestType)
        {
        switch (aDecision)
            {
            case EPosDecisionAccepted:
                {
                result = ECmdNotifyRejectFailure;
                break;
                }
            case EPosDecisionRejected:
                {
                result = ECmdNotifyAcceptFailure;
                break;
                }
            default:
                {
                User::Leave(KErrNotFound);
                break;
                }
            }
        }
    else if (ECancelNotification == aRequestType)
        {
        switch (aDecision)
            {
            case EPosDecisionAccepted:
                {
                result = ECmdNotifyAcceptTimeout;
                break;
                }
            case EPosDecisionRejected:
                {
                result = ECmdNotifyRejectTimeout;
                break;
                }
            default:
                {
                User::Leave(KErrNotFound);
                break;
                }
            }
        }
    else
        {
        User::Leave(KErrNotFound);
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::CheckRequestTypeL
// Checks the type of request and helps to know
// if a given request is of SUPL periodic type
// @param aCurrentRequest request id
// @return TRequestType type of request enum
// -----------------------------------------------------------------------------
CPosRequestor::TRequestType CLpdVerifierPlugin::CheckRequestTypeL()
    {
    CPosRequestor::TRequestType reqType = CPosRequestor::ENetworkTypeUnknown;
    if (RequestorCountL() > 0)
        {
        CPosRequestor* requestor = RequestorLC(0);
        reqType = requestor->RequestType();
        CleanupStack::PopAndDestroy(requestor);
        }
    return reqType;
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::GetSessionIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::GetSessionIdL( TInt64& aSessionId )
    {
    CPosRequestor* curRequestor = RequestorLC( 0 );
    TInt length = curRequestor->RequestorIdString().Length();
    TPtrC reqString(curRequestor->RequestorIdString().Mid( length - 3 ));
    TLex temp(reqString);
    temp.Val(aSessionId);
    CleanupStack::PopAndDestroy( curRequestor );
    }

// -----------------------------------------------------------------------------
// CLpdVerifierPlugin::UpdateCurrentNotifierL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdVerifierPlugin::UpdateCurrentNotifierL()
    {
    // Confirm if the current request type is Notification before
    // processing/updating.

    __ASSERT_DEBUG( iCurrentRequestType == ENotification,
            HandleDebugAssertError() );
    __ASSERT_DEBUG( iPeriodicProcessor, HandleDebugAssertError() );
    if(iNotifier)
    iNotifier->CancelQuietly();
    HandleNextPeriodicNotificationL();
    
    }

//  End of File
