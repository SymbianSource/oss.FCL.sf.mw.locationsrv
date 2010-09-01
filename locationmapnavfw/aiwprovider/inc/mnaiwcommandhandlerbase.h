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
* Description:  CMnAiwCommandHandlerBase class declaration
*
*/


#ifndef MN_AIWCOMMANDHANDLERBASE_H
#define MN_AIWCOMMANDHANDLERBASE_H

#include <e32base.h>
#include <AiwCommon.h>
#include <AknServerApp.h>
#include <EPos_Landmarks.h>
#include "mnprovider.h"
#include "mnaiwservices.h"

class CMnAiwCommandHandlerBase;
class CPosLandmark;

/** \internal 
 *  Callback for clients of @ref CMnAiwCommandHandlerBase.
 *  Used by CMnAiwProvider to destroy asynchronous handlers.
 */
class MMnAsyncAiwCmdHandlerCallback
    {
    public:
        /** Called by CMnAiwCommandHandlerBase-derived class, 
         *  to notify completion of AIW command async handling.
         *  @param aHandler pointer to completed command's handler
         *  @param aResult result of the command execution
         */
        virtual void HandleAsyncAiwCmdCompletedL( 
            CMnAiwCommandHandlerBase* aHandler, 
            TInt aResult ) = 0;
    };

/** \internal 
 *  Callback for clients of @ref CMnAiwCommandHandlerBase.
 *  Used by CMnAiwProvider to destroy handlers, when provider is shutdown.
 */
class MMnProviderExitCallback
    {
    public:
        /** Called by CMnAiwCommandHandlerBase-derived class, 
         *  to notify provider's exit.
         *  @param aHandler pointer to completed command's handler
         */
        virtual void HandleProviderExit( 
            CMnAiwCommandHandlerBase* aHandler ) = 0;
    };

/** \internal 
 *  Base class for AIW command handlers 
 */
class CMnAiwCommandHandlerBase : public CActive, public MAknServerAppExitObserver
    {
    public:
        /** Holder for AIW parameters */
        class TAiwParameters
            {
            public:
                TAiwParameters();
                TAiwParameters(
                    const CAiwGenericParamList& aInParamList,
                    CAiwGenericParamList& aOutParamList,
                    const MAiwNotifyCallback* aCallback
                    );
                
                const CAiwGenericParamList&     InList() const;
                CAiwGenericParamList&           OutList();
                const MAiwNotifyCallback*       Callback() const;
                
            protected:        
                const CAiwGenericParamList*     iInParamList;
                CAiwGenericParamList*           iOutParamList;
                const MAiwNotifyCallback*       iCallback;
            };
    
        enum TRunMode
            {
            ERunModeDefault,
            ERunModeChained,
            ERunModeStandalone
            };
    
    public:
        /** Returns information about what Provider Application's 
         *  facilities are needed to execute a particular command
         */
        static void AppServiceAndFeatureNeeded(
            TInt aAiwServiceCommand, 
            CMnProvider::TService& aServiceNeeded,
            TInt& aFeaturesNeeded );
            
        /** Returns default execution mode for a command.
         *  @param aAiwServiceCommand Command ID */    
        static TRunMode DefaultRunMode( TInt aAiwServiceCommand );

        /** Creates new command handler.
         *  @param aAiwServiceCommand Command ID
         *  @param[in] aAiwParams Initial AIW parameters. Needed to detect requested
         *                        execution mode.
         *  @return New instance of command handler
         *  @leave KErrNotSupported If given command is not supported
         */
        static CMnAiwCommandHandlerBase* CreateHandlerL( 
            TInt aAiwServiceCommand, 
            CMnProvider& aProvider,
            TRunMode aRunMode );

        /** Analyzes TMnAiwCommonOptionsParam ( if present ) to see 
         *  whether chained execution is needed.
         *  @param aDefaultMode default value to return if parameter is not found in the list.
         *  @return Execution mode requested in AIW parameters as defined in
         *          TMnAiwCommonOptionsParam::iRunChained is ETrue.
         */
        static TRunMode RequestedRunModeL( const TAiwParameters& aAiwParams );

        /** Analyzes TMnAiwCommonOptionsParamExtended ( if present ) to see 
         *  whether provider application is specified.
         *  @param[out] aProviderId provider application UID, if specified.
         *  @return ETrue if provider application UID is specified, otherwise EFalse.
         */
        static TBool IsProviderSpecifiedL( 
            const CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
            TInt& aProviderId );

        /** Analyzes TMnAiwCommonOptionsParamExtended ( if present ) to see 
         *  whether error messages are disabled.
         *  @return ETrue if error messages are disables, otherwise EFalse.
         */
        static TBool IsErrorMessageDisabledL( 
            const CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams );

    public:

        /** Destructor */
        virtual ~CMnAiwCommandHandlerBase();

        /** Executes this command synchronously.
         *  If provider exits, it is reported via MMnProviderExitCallback */
        void StartL();

        /** Starts asynchronous processing of this command. 
         *  Completion of a command is reported via MMnAsyncAiwCmdHandlerCallback 
         *  @param[in] aCallback Object to notify, when request is completed */
        void Start( MMnAsyncAiwCmdHandlerCallback& aCallback );
        
        /** AIW service command id implemented by the handler */
        virtual TInt CommandId() = 0;

        /** Returns whether this command is asynchronous
         *  Async commands must be started by StartL, sync commands - by ExecuteL */
        virtual TBool IsAsyncCommand() = 0;

        /** Returns AIW command parameters */
        TAiwParameters& AiwParameters();
        
        /** Returns execution mode of the command */
        TRunMode RunMode();
        
        /** Sets parameters for this command. */
        void SetParametersL( const TAiwParameters& aParams );
        
        /** Called by before setting new parameters. Not called for the first time
         *  this command is executed. */
        virtual void ResetParametersL() = 0;
        
        virtual void SetExitObserverL( MMnProviderExitCallback& aExitCallback );

        /** Returns reference to provider used by this command handler */
        CMnProvider& Provider();

    protected:
        /** C++ constructor 
         *  @param aProvider Provider Application to use
         *  @param aAiwParameters Parameters sent to AIW Provider class
         *  @param aCallback Observer of this command's completion if command
         *                   is asynchronously executed
         */
        CMnAiwCommandHandlerBase( CMnProvider& aProvider );
    
        /** Symbian constructor. Must be called by derived classes */
        void ConstructL();
        
        /** Reports completion of the command, when executed asynchronously
         *  It should be safe to delete this object after this method is called.
         *  Used by derived classes */
        void Done( TInt aResult );

        /** Starts processing of input AIW parameters. Parameters found are reported
         *  via HandleAiwParam... handlers.
         */
        void ProcessAiwParametersL();

        /** Finds common options parameter in given parameter list */
        static TBool FindCommonOptionsL( 
            const TAiwParameters& aAiwParams, 
            TMnAiwCommonOptionsParamExtended& aCommonOptions );

        /** Finds i-th landmark data in input parameter list */
        TBool FindLandmarkData( TInt aIndex, TAiwGenericParam& aParam );

        /** Reads landmark data from given AIW parameter */
        void ReadLandmarkDataL( const TAiwGenericParam& aParam, TPtrC8& aLandmarkPtr );

        /** Reads landmark database URI from given AIW parameter */
        void ReadDatabaseUriL( const TAiwGenericParam& aParam, TPtrC& aDbUri );

        /** Reads landmark ID from given AIW parameter */
        TPosLmItemId ReadLandmarkId( const TAiwGenericParam& aParam );

        /** Reads landmark ID list from given AIW parameter */
        void ReadLandmarkIdListL( 
            const TAiwGenericParam& aParam,
            const TPosLmItemId*& aListPtr,
            TInt& aListSize );

        /** Appends given landmark to AIW output parameters list */
        void WriteLandmarkL( const CPosLandmark& aLandmark );
        
        /** Appends given landmark to AIW output parameters list */
        void WriteLandmarkL( TPosLmItemId aLandmarkId, const TDesC& aDatabaseUri );
        
    protected: // Virtual callbacks for parameter list parsing
    
        /** Called when ProcessAiwParametersL finds unknown AIW parameter */
        virtual void HandleAiwParamL( const TAiwGenericParam& aParam );        
        /** Called by ProcessAiwParametersL to report found landmarks */
        virtual void HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList );
        /** Called by ProcessAiwParametersL to report found linked landmarks */
        virtual void HandleAiwParamLandmarksListL( const TDesC& aDbUri, RArray<TPosLmItemId>& aLmIds );
        
    public: // from CActive
        virtual void RunL();
        virtual TInt RunError( TInt aError );
        virtual void DoCancel();

	protected: // from MAknServerAppExitObserver
        virtual void HandleServerAppExit( TInt aReason );
        virtual void DoStartL() = 0;

	protected:
        TRunMode    						iRunMode;

    private:
        CMnProvider&                    	iProvider;
        TAiwParameters                  	iAiwParameters;
        MMnAsyncAiwCmdHandlerCallback*   	iCallback;
        MMnProviderExitCallback* 	        iExitCallback;
    };

#endif // MN_AIWCOMMANDHANDLERBASE_H
