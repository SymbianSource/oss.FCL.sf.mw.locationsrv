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


// INCLUDES

#include <EPos_CPosPrivacyNotifier.h>
#include <lbs/epos_privacy.h>
#include <lbs/epos_cposrequestor.h>


#include "lpdrequestao.h"
#include "lpdresulthandler.h"
#include "lpdperiodicreqinfo.h"
#include "lpdperiodicprocessor.h"


#ifndef CLPDVERIFIERDLG_H
#define CLPDVERIFIERDLG_H

// SYSTEM INCLUDE
#include <bldvariant.hrh>               // For the 3.2 Flag


// FORWARD DECLARATIONS
class CLpdVerifierQueryLauncher;
class CLpdNotifierQueryLauncher;
class CEikonEnv;
class CLpdRequestorProcessor;
class RPosRequestorStack;
class CLocRequestorUtilsResolver;
class CLpdPeriodicProcessor;


// CLASS DECLARATION

/**
*  Verifier plug-in which derives from CPosPrivacyNotifier.
*  Notifiers are based on RNotifier framework, and this particular
*  notifier is used to launch a privacy query for accepting/rejecting
*  privacy requests. It also shows informational notes and error
*  messages, which are received as notification.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdVerifierPlugin : public CPosPrivacyNotifier, public MLpdResultHandler,
                                             public MLpdPeriodicRequestObserver
    {
    // To allow calls to HandleNextRequest() from active object
    friend class CLpdRequestAO;
    
    private:

        /**
         * Enumeration for defining the type of Verification or notification
         * request
         */
        enum TDialogType
            {
            EVerificationRequest,
            ENotification,
            ENotificationTimeout,
            ECancelNotification,
            ESuplPeriodicNotification,
            };

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdVerifierPlugin* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CLpdVerifierPlugin();

		/**
		* To popup a notification note for verification cancellation event.
		*/
		void NotifyCancellationL(
			TPosRequestSource aSource,
			TPosVerifyCancelReason aReason,
			TPosRequestDecision aDecision,
			const RPosRequestorStack& aRequestors );

        /**
        * Method for cancelling query or freeing
        * resources after finishing query.
        */
        void FreeQueryResources();

    public: // Functions from base classes

        /**
        * From CPosPrivacyNotifier, called when a new request arrives.
        * @param aRequestId identifier of received request
        */
        void HandleNewRequestL( TPosQNRequestId aRequestId );

        /**
        * From CPosPrivacyNotifier, called when request has been cancelled
        * @param aRequestId identifier of cancelled request
        */
        void HandleRequestCancelled( TPosQNRequestId aRequestId );

        /**
        * From CPosPrivacyNotifier, called when all requests are cancelled.
        */
        void HandleAllRequestCancelled();

        /**
        * From MLpdResultHandler, called when verification result is available.
        * @param aResultCode
        */        
        void HandleVerificationResultL( TInt aResultCode );

        /**
        * From MLpdResultHandler, called when notification has been done.
        * @param aResultCode
        */        
        void HandleNotificationResultL( TInt aResultCode );

        /**
        * From MLpdResultHandler, called if leave occurred in callback method
        * @param aError code
        */        
        void HandleLeave( TInt aError );
        
      


    protected:  // New functions

        /**
        * Non-leaving internal method for handling next pending request.
        */
        void HandleNextRequest();
        
        /**
        * Internal method for handling next pending request.
        */
        void HandleNextRequestL();

        /**
        * Internal helper method used by HandleNextRequestL().
        */
        void HandleNextVerificationL();

        /**
        * Internal helper method used by HandleNextRequestL().
        */
        void HandleNextNotificationL();

        /**
          * Internal helper method used by HandleNextNotificationL().
          */
        void HandleNextNonPeriodicNotificationL();

        /**
        * Internal helper method used by HandleNextNotificationL().
        */
        void HandleNextPeriodicNotificationL();        

        /**
        * Internal helper method.
        */
        void NotifyCancellationL( TPosRequestSource aSource, 
                                  TPosVerifyCancelReason aReason,
                                  TPosRequestDecision aDecision );

        /**
        * Internal helper method.
        * Completes current request and continues queue handling.
        */
        void CompleteCurrentAndContinue( TInt aResultCode );

        /**
        * Handles assert errors. Panics in debug build but leaves in release
        * build, as this component runs in EikSrv and must never panic.
        */
        void HandleAssertErrorL() const;

        /**
        * Panics in debug build but does nothing in release build.
        */
        void HandleDebugAssertError() const;
        
        /**
        * Handle the request if it is periodic.
        */
        void EnqueIfPeriodicL(TPosQNRequestId aRequestId);
        
         /**
        * From MLperiodicRequestObserver, called if the currently
        * displayed notifier has to be updated
        * @param aError code
        */        
        void UpdateCurrentNotifierL();
        
        /**
        * Gets session id of request
        * @param aSessionId The session id
        */  
        void GetSessionIdL( TInt64& aSessionId );

    private:

        /**
        * C++ default constructor.
        */
        CLpdVerifierPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
        /**
         * Starts the Query dialog
         * @param aQueryDialog Query dialog that has to be started
         * @param aDialogType  Dialog type
         * @param aDecision    Decision Type
         */
        void StartQueryDialogL( CLpdQueryLauncherBase*  aQueryDialog,
                                TDialogType             aDialogType,
                                TPosRequestDecision     aDecision );
         
        /**
         * Packs the Requestor buffer to send across to Cover UI. The 
         * ownership of the buffer is transferred back to the callee
         * @return CBufFlat*    Flat buffer containing the list of requestors    
         */
        CBufFlat*  PackRequestorBufferL();
                 
        /**
         * Determines the Cover UI command corresponding to a paricular
         * request
         * @param  TDialogType          Request Type
         * @param  TPosRequestDecision  Decision Type
         * @return TInt                 Cover UI Command ID.
         */
        TInt   CoverUICommandL( TDialogType         aRequestType, 
                                TPosRequestDecision aDecision );
        
         /**
          * Checks the type of request and helps to know
          * if a given request is of SUPL periodic type
          * @param aCurrentRequest request id
          * @return CPosRequestor::TRequestType type of request enum
         */
        CPosRequestor::TRequestType CheckRequestTypeL();


    private:    // Data

        // Cancellation info needed if cancel handling is postponed
        class TCancelInfo
            {
            public:
                TBool iCancelled;
                TPosVerifyCancelReason iReason;
                TPosRequestDecision iDecision;
                TPosRequestSource iSource;
            };

        // Cancellation info needed if verification query is cancelled
        // during rule query and handling is therefore postponed
        TCancelInfo iCancelInfo;

        // Identifier of currently handled request
        TPosQNRequestId iCurrentRequest;

        // Type of the current request, valid if iCurrentRequest valid
        TRequestType iCurrentRequestType;

        // Result code of verifier query, needed if user cancels rule query
        TInt iVerifyResult;

        // Resource file needs to be explicitely added
        // to the environment. Adding returns an offset value.
        TInt iResourceOffset;

        // Location request accept/reject query object
        CLpdVerifierQueryLauncher* iVerifierQuery;  // owns

        // Location request accept/reject query object
        CLpdNotifierQueryLauncher* iNotifier;       // owns
        
        // Active object, used to let requests run to
        // completion instead of recursive calls.
        CLpdRequestAO* iRequestActiveObject;        // owns

        CLpdRequestorProcessor* iRtorProcessor;     // owns
        
        CLocRequestorUtilsResolver* iUtils; // owns

        CLpdPeriodicProcessor* iPeriodicProcessor;     // owns
        
        RArray<TLpdPeriodicReqInfo> iPeriodicNotQue; // owns

        CEikonEnv* iEnv;                            // uses
                        
    };

#endif      // CLPDVERIFIERDLG_H   
            
// End of File
