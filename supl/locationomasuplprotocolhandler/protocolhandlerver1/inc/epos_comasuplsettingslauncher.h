/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Active object wrapper for launching Settings UI
*
*/



#ifndef C_COMASUPLSETTINGSLAUNCHER_H
#define C_COMASUPLSETTINGSLAUNCHER_H

// INCLUDES
#include <e32std.h>
#include <agentdialog.h>    // for RGenConAgentDialogServer

const TUid KNotifier1Uid = { 0x10009C00 };
const TUid KNotifier2Uid = { 0x10009C01 };
const TUid KNotifier3Uid = { 0x10009C02 };
// FORWARD DECLARATIONS
class MOMASuplUISettingsObserver;
class CLocSettingsUiClient;
class COMASuplTrace;
/**
* Active object wrapper for launching Supl Settings UI.
* User of this class needs to implement relevant MOMASuplUISettingsObserver interface
* methods to receive notifications.
*/
class COMASuplSettingsLauncher : public CActive
    {
    public:

		enum TSuplDialogType
			{
				ESUPLYesNoDialog  = 0,
				ESUPLIAPDialog,
				ESUPLUsageDialog,
				ESUPLTimeoutDialog		
			};	
        /**
        * This is a two-phase constructor method that is used to
        * create a new instance for launching settings UI
        *
        * @param aCallback Reference to a callback instance.
        * @return          A pointer to a new instance of the COMASuplSettingsLauncher class.
        */
        static COMASuplSettingsLauncher* NewL(MOMASuplUISettingsObserver& aCallback);

        /**
        * This is a two-phase constructor method that is used to create a new
        * instance for launching settings UI
        * Leaves the constructed instance to cleanup stack.
        *
        * @param aCallback Reference to a callback instance.
        * @return          A pointer to a new instance of the COMASuplSettingsLauncher class.
        */
        static COMASuplSettingsLauncher* NewLC(MOMASuplUISettingsObserver& aCallback);
        /**
        * When this method is called, the COMASuplSettingsLauncher launches the
        * settings UI
        *
        * @throws error code depending on whether launch was successful or failed
        */
        TInt LaunchSettings();

        /**
        * When this method is called, the COMASuplSettingsLauncher cancels the 
        * launch if it has not been already launched
        */
        void CancelLaunch();

        /**
        * When this method is called, the COMASuplSettingsLauncher launches the
        * settings UI
        *
        * @throws error code depending on whether launch was successful or failed
        */
        TInt LaunchSuplUsageSettingsUI(TBool aRoaming);
        /* Launches the Dialog saying one or more location requests were cancelled
        */
        
        TInt LaunchSessionTimeOutDialog();

        /**
        * Destructor.
        */
        virtual ~COMASuplSettingsLauncher();
        
        TInt GetIAPName(TDes& aIAPName,TUint32& aIAPId);	

    protected:

        /**
        * Implements CActive
        * @param none
        * @return none
        */    
        void RunL();
    
        /**
        * Implements CActive
        * @param aError the error returned
        * @return error
        */
        TInt RunError(TInt aError);

        /**
        * Implements CActive
        * @param none
        * @return none
        */    
        void DoCancel();

    private:

        /**
        * Default Constructor
        * @param aCallback Reference to a callback instance.
        * @return None
        */
        COMASuplSettingsLauncher(MOMASuplUISettingsObserver& aCallback);
        
        void ConstructL();                                         
   private:
   
   		TInt LaunchIAPDialog();       
   		
   		void ConvertIAPIdtoNameL( const TUint aIAPId,TDes& aIAPName );                            
    private:
        // not owned by this class
        MOMASuplUISettingsObserver& iCallback; 
        // Settings Launcher
        CLocSettingsUiClient* iSettingsLauncher;

		//Trace Utility
		COMASuplTrace* iTrace;

		TSuplDialogType  iDialogType;

        RNotifier iNotifier;
	    TBuf8<256>	buffer1;
	    TBuf8<256>  buffer2;
        TBool iRoaming;
        
		/**
		* IAP Selector Handle
		*/
	    RGenConAgentDialogServer    	iIapSelector;
	    
		/**
		* Selected IAP ID
		*/
        TUint32                     	iSelectedIap;


    };


/**
* This class provides a callback interface for handling the
* completion of settings.  The Client derives a
* class from this interface and implements the SettingsUICompleted-methods
* that interest it.
*/
class MOMASuplUISettingsObserver
    {

    public:

    /** 
    * This callback method is used to notify the client about
    * completion of settingsUi Launch
    * @param aError error encountered while launching change
    * @return None
    */
    virtual void SettingsUICompletedL(TInt aError) = 0;
   
    /** 
    * This callback method is used to notify the client about
    * completion of settingsUi Launch for SUPL state
    * @param aError error encountered while launching change
    * @return None
    */
   
    virtual void SettingsUsageUICompletedL(TInt aError) = 0;
   
    /** 
    * This callback method is used to notify the client about
    * completion of Timeout UI
    * @param aError error encountered while launching change
    * @return None
    */
   
    virtual void SettingsTimeOutUICompletedL(TInt aError) = 0;
   
    };

#endif      // C_COMASUPLSETTINGSLAUNCHER_H

// End of File
