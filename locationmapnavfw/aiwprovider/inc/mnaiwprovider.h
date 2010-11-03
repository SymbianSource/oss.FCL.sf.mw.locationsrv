/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Map and Navigation AIW Provider menu class
*
*/


#ifndef MN_AIW_PROVIDER_H
#define MN_AIW_PROVIDER_H

#include <e32base.h>
#include <AiwServiceIfMenu.h>
#include "mnaiwcommandhandlerbase.h"

/**
 * CMnAiwProvider implements CAiwServiceIfMenu interface to
 * allow Map and Navigation functionality to be called via AIW.
 */
class CMnAiwProvider : 
	public CAiwServiceIfMenu, 
	public MMnAsyncAiwCmdHandlerCallback,
	public MMnProviderExitCallback
    {
    public:
        static CMnAiwProvider* NewL();
        ~CMnAiwProvider();
        
    protected:
        
        /** Retrieves command id from client's parameters list */
        TInt GetAiwServiceCommandIdL( const CAiwGenericParamList& aInParamList );

        /** Verifies if there is at least one provider application for
         *  requested command
         *  @param aAiwCommandId AIW service command ID
         *  @return ETrue if any provider is found for the command
         */
        TBool IsServiceAvailableL( TInt aAiwCommandId );
        
        /** Searches for already open handler for specified command, 
         *  provider and execution mode. This function returns
         *  pointer to command handler, which matches all given criteria.
         *  @param aAiwCommandId Command ID criterion.
         *  @param aProvider Provider criterion.
         *  @param aRunMode Execution mode criterion.
         */ 
        CMnAiwCommandHandlerBase* FindWorkingHandler( 
            TInt aAiwCommandId,
            const CMnProvider& aProvider, 
            CMnAiwCommandHandlerBase::TRunMode aRunMode );
        
        /** Deletes command handler from internal list 
         *  @param[in] aHandler The handler to remove */
        void CloseHandler( CMnAiwCommandHandlerBase* aHandler );

        /** Updates internal list of known provider applications */
        void UpdateProvidersListL();

		/** Closes and removes all handlers working with 
		 *  specified provider.
		 *  @param aProvider Handlers using this provider will be closed. */
		void CloseHandlers( const CMnProvider* aProvider );

		/** Closes and removes all handlers. This method is used by destructor. */
		void CloseAllHandlers();

        /** Find suitable providers for a given command */
        void GetSuitableProvidersL( 
            TInt aAiwCommandId, 
            RPointerArray<CMnProvider>& aProviders );

        /** Finds amount of providers available for given command */
        TInt NumSuitableProviders( TInt aAiwCommandId );

        /** Returns preferred provider application for a given command
         *  This may involve asking user via list query popup if several 
         *  suitable providers found.
         *  @leave KErrCancel if user cancels selection
         *  @leave KErrNotFound if no providers available
         */
        CMnProvider& SelectProviderL( 
            TInt aAiwCommandId,
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams );
        
        /** Pops up list query dialog for user to select preferred Provider Application */
        void ExecuteSelectionDialogL( 
            TInt& aIndex, 
            RPointerArray<CMnProvider>& aProviders );

        /** Verifies result of a command. Checks whether command may be completed. 
         *  Pops up error note if error happened and restarts command if more than one
         *  provider is available for this command
         */
        void ProcessAiwCommandResultL( 
            TInt aAiwCommandId, 
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
            TInt aResult, TBool& aStop );
        
        /** Executes synchronous command */
        void ProcessSyncAiwCommandL(
            TInt aCommandId, 
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams );
            
        /** Attempts to start execution of asynchronous command until success
         *  or user refuses to continue
         */
        void StartAsyncAiwCommandL(
            TInt aCommandId,    
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams );

        /** Starts execution of asynchronous command */
        CMnAiwCommandHandlerBase* DoStartAsyncAiwCommandL(
            TInt aCommandId,    
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
            TBool& aProviderFound );

        /** Finds running handler, or creates new */
        CMnAiwCommandHandlerBase* HandlerL(
            TInt aCommandId,
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams );

        /** Informs AIW observer about completion of async command */
        void NotifyAiwObserver( 
            TInt aCommandId,
            CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams, 
            TInt aResult );

        /** Shows error note with text specified by parameter */            
        void ErrorNote( TInt aResourceId );
        void DoErrorNoteL( TInt aResourceId );
        
    private:
        /**
        * C++ default constructor.
        */
        CMnAiwProvider();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
    public: // from CAiwServiceIfBase

        void InitialiseL( 
            MAiwNotifyCallback& aFrameworkCallback, 
            const RCriteriaArray& aInterest );

        void HandleServiceCmdL(
            const TInt& aCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL);

    public: // from CAiwServiceIfMenu

        void InitializeMenuPaneL(
            CAiwMenuPane& aMenuPane,
            TInt aIndex,
            TInt aCascadeId,
            const CAiwGenericParamList& aInParamList);

        void HandleMenuCmdL(
            TInt aMenuCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL);
            
    public: // from MMnAsyncAiwCmdHandlerCallback
    
        void HandleAsyncAiwCmdCompletedL( CMnAiwCommandHandlerBase* aHandler, TInt aResult );

    public: // from MMnChainedAiwCmdHandlerCallback
    
        void HandleProviderExit( CMnAiwCommandHandlerBase* aHandler );

    private:
        RPointerArray<CMnAiwCommandHandlerBase> iHandlers;
        RPointerArray<CMnProvider>  			iMnProviders;
        
        CAiwGenericParamList*       			iEventList;
        
        // not-owned shortcut
        CEikonEnv*                  			iEikon;
        TInt                        			iResourceOffset;
        TFileName                   			iResourceFilename;
    };
    

#endif  // MN_AIW_PROVIDER_H

