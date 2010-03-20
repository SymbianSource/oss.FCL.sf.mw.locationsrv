/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SUPL Settings UI Engine. The engine is resposible for handling
*                all the UI components required for SUPL Settings UI. In addition
*                to this it also is responsible for interacting with the SUPL
*                Settings API
*
*/


#ifndef C_LOCSUPLSETTINGSUIENGINE_H_
#define C_LOCSUPLSETTINGSUIENGINE_H_

// System Includes
#include <e32base.h>
#include <agentdialog.h>    // for RGenConAgentDialogServer

// User Includes
#include "locsuplsettingsadapterobserver.h"

// Forward Declaration
class CLocSUPLSettingsAdapter;
class MLocSUPLSettingsUiEngObserver;
class CTextResolver;
class CAknGlobalConfirmationQuery;
class CServerParams;
class CTriggerParams;
class CLocSUPLServerEditor;
class MSuplServerEditorObserver;
class MLocSUPLSettingsSessionObserver;

// Class Declaration
/**
 * Engine for SUPL Settings UI
 *
 * Engine class for SUPL Settings UI. It is responsible for interacting with the 
 * SUPL Settings API. Further, it handles all commands for SUPL settings UI. The
 * Engine launches all the dialogs associated with configuring the SUPL Settings.
 * It further observers the SUPL Settings UI for any changes to the Settings
 * values.
 */
class CLocSUPLSettingsUiEngine : public CActive, 
                                 public MLocSUPLSettingsAdapterObserver
    {
    public:
        /**
         * Two-phased constructor
         *
         * @param  aObserver                 Referenece to the Engine observer
         * @return CLocSUPLSettingsUiEngine* Reference to the object created
         */
        static CLocSUPLSettingsUiEngine* NewL( 
                            MLocSUPLSettingsUiEngObserver&      aObserver );
                            
        /**
         * Two-phased constructor. Leaves the object on the Cleanup stack
         *
         * @param  aObserver                 Reference to the Engine observer
         * @return CLocSUPLSettingsUiEngine* Reference to the object created
         */
        static CLocSUPLSettingsUiEngine* NewLC( 
                            MLocSUPLSettingsUiEngObserver&      aObserver );
         
        /**
         * Initializes the Settings Engine. This is an asynchronous call the 
         * completion of which is communicated through the observer
         * 
         */
        void Initalize();
                         
        /**
         * Destructor.
         */
        virtual ~CLocSUPLSettingsUiEngine();
       
        /** 
         * Launches the confirmation query to select IAP
		 *
		 */
		void SelectConnectionL();
        
		/**
		 * Launches the Access Point Configurator dialog
		 *
		 */
		void LaunchApConfiguratorL( TInt64 aSlpId, 
				MSuplServerEditorObserver* aEditorObserver );
		
        /** 
         * Opens the exisitng server with its attributes
		 *
		 */
		void EditServerL( TBool iIsEditable, TInt64 aSlpId );

        /** 
         * returns exisitng server entries count
		 *
		 */
		TInt SlpCount();

		/**
		 * Launches the SUPL Usage configurator
		 */
		void LaunchSuplUsageConfiguratorL();
		
        /**
		 * Closes the running Settings UI prematurely.
		 */
		void Close();
		
        /**
         * Obtains the SUPL Settings usage.
         *
         * @return TPtr16     SUPL settings usage.
         */  
        TPtr16 GetSuplUsageL();
        
        /**
         * Obtains the SUPL usage
         *
         * @return The SUPL Usage value
         */
        TInt   GetSuplUsage();			
		 
        /**
		 * Creates a new server entry in Supl Settings.
		 */
		void AddNewServerL( 
		        const TDesC& aServerAddress,
		        const TDesC& aIapName,
		        const TBool aUsageInHomeNw
		);

	    /**
	     * Deletes server entry from SUPL settings storage.  
	     */
	    void RemoveServerL(
	    		const TInt64 aSlpId
	    );

	    /**
	     * Sets the priority of the server to the specified priority.  
	     */
		void ChangePriorityL(
		        TInt64 aSlpId, 
		        TInt aPriority,
		        TBool aDirection
		);

	    /**
	     * Gets properties of all servers from SUPL settings storage.  
	     * This method retrieves all properties for each server.
	     */
		void GetAllSlpL(
		        RPointerArray<CServerParams>& aParamValues
		) const;
	
	    /**
	     * Gets all properties of server from SUPL settings storage
	     * based on SLP identification.  
	     */
		void GetSlpInfoFromIdL(
	            const TInt64 aSlpId, 
	            CServerParams *aParamValues
		) const;

	    /**
	     * Sets the server address parameter in server settings 
	     */
	    void SetServerAddressL( 
	            const TInt64 aSlpId, 
	            const TDesC& aServerAddress
	    );
	    
	    /**
	     * Obtains the server address parameter from server settings.  Client
	     * has to allocate memory for server address parameter.
	     */
	    void GetServerAddressL( 
	            TInt64 aSlpId, 
	            TDes& aServerAddress
	    ) const;

	    /**
	     * Sets the IAP Name in server parameters. IAP name refers to 
	     * the access point which is used to access the HSLP over the internet. 
	     */
	    void SetIapNameL( 
	            const TInt64 aSlpId, 
	            const TDesC& aIapName
	    );

	    /**
	     * Sets the parameter which indicates whether server
	     * can be used for SUPL session or not. 
	     */
	    void SetServerEnabledFlagL(
	            const TInt64 aSlpId, 
	            const TBool aEnable 
	    ) const;

	    /**
	     * Obtains the parameter which indicates whether server
	     * can be used for SUPL session or not. 
	     */
	    void GetServerEnabledFlagL(
	            const TInt64 aSlpId, 
	            TBool& aEnable 
	    ) const;

	    /**
	     * Sets the parameter which indicates whether server
	     * can be used out side home network or not. 
	     */
	    void SetUsageInHomwNwFlagL(
	            const TInt64 aSlpId, 
	            const TBool aHomeNwFlag 
	    );

		
	    /**
	     * Sets the parameter which indicates whether server
	     * details are editable or not. 
	     */ 
		void SetEditableFlagL( 
	            const TInt64 aSlpId, 
	            const TBool aEditFlag 
	    ) const;

	    /**
	     * Gets the parameter which indicates whether server
	     * details are editable or not. 
	     */ 
	    void GetEditableFlagL( 
	            const TInt64 aSlpId, 
	            TBool& aEditFlag 
	    ) const;
	    
	    /**
	     * Obtains the count of active Supl sessions
	     * 
	     */  
	    void GetActiveSessionsCountL( TInt& aAccessPointCount );
	    
	    /**
	     * Gets properties of all triggers from SUPL settings storage.  
	     * This method retrieves all properties for each trigger.
	     */
		void GetTriggerParamsL(
		        RPointerArray<CTriggerParams>& aParamValues
		) const;
	    
	    /**
	     * Gets properties of all triggers from SUPL settings storage.  
	     * This method retrieves all properties for each trigger based on session id.
	     */
		void GetTriggerParamsL(  TInt64 aSessionId, 
		        CTriggerParams*& aTrigger
		) const;
	   
	   /**
	     * Open the Active Session for the specified session id.
	     * 
	     */  
	    void OpenSessionL( TInt64 aSessionId );
		
	   /**
	     * Deletes a trigger session based on session id
	     * 
	     */  
	    void RemoveTriggerSessionL( TInt64 aSessionId ) const;
	   
	   /**
	     * Deletes a trigger sessions based on session ids
	     * 
	     */  
	    void RemoveTriggerSessionsL( RArray< TInt64 >& aSessionIdList ) const;
	   
	   /**
	     * Deletes all the trigger sessions
	     * 
	     */  
	    void RemoveAllTriggerSessionsL( ) const;
	   
	   /**
	     * Changes the notification status of an active session based on session id
	     * 
	     */  
	    void ChangeNotificationStatusL( TInt64 aSessionId, TBool aTriggerNotificationStatus ) const;
	   
	   /**
	     * Set the Observer for the specific Session
	     * 
	     */  
	    void SetSessionObserver( MLocSUPLSettingsSessionObserver* aObserver ) const;
	   
	   /**
	     * Remove the Session observer
	     * 
	     */  
	    void RemoveSessionObserver( ) const;
	    
	public:     		
		/**
		 * Inherited from MLocSUPLSettingsAdapterObserver
		 */
		void HandleSuplSettingsChangeL( TLocSUPLSettingsEvent aEvent );
				 
    protected:
        /**
         * Inherited from CActive
         */
        void RunL();
        
        /**
         * Inherited from CActive
         */
        void DoCancel();
             
    private:
        /**
         * C++ constructor.
         */
        CLocSUPLSettingsUiEngine( MLocSUPLSettingsUiEngObserver&  aObserver );
        
        /**
         * Second Phase constructor
         */
        void ConstructL();
         
        /**
         * Obtains the index UID corresponding to the Access point name
         *
         * @param   aIAPName     Access point name
         * @return  TUint       UID corresponding to the AP Name. Zero if 
         *                      the name does not exist
         */
        TUint ConvertIAPNameToIdL( const TDesC&      aIAPName );
       
        /**
         * Displays an Error note
         *
         * @param aError Error ID
         */
        void DisplayErrorL( TInt aError );
        
        /**
         * Displays an Info note
         *
         * @param None
         */
        TInt ShowNoteL();                               
        
    private:
        /**
         * Reference to the Engine Observer
         */
        MLocSUPLSettingsUiEngObserver&	iObserver;
        
        /**
         * SUPL Settings API Adapter
         * Owns
         */
        CLocSUPLSettingsAdapter*       	iSUPLSettingsAdapter;
        
        /**
         * Flag to denote whether any dialog is currently active
         * Used in the Cancel method. If there are any requests 
         * outstanding then they have to be cancelled
         */
        TBool                   		iDialogActive;
                        
        /**
         * Temporary Buffer for returning values to the Accessor 
         * functions. The accessor expects a 16 bit variant of the descriptor
         * Owns
         */
        HBufC16*                		iConversionBuffer;
        
        /**
         * Pointer to the Conversion Buffer
         */
        TPtr16                  		iConversionBufferPtr;
        
        /**
         * Temporary Buffer to hand over to the UI components and
         * access data from the Settinga API
         * Owns
         */
        HBufC*                  		iSettingsBuffer;
        
        /**
         * Pointer to the Settings Buffer
         */
        TPtr                    		iSettingsBufferPtr;
        
        /**
         * Standard Text resolver for converting Error codes to 
         * standard texts
         */
        CTextResolver*          		iTextResolver;

        
        /**
         * Selected IAP ID
         */
        TUint32                     	iSelectedIap;
        
        /**
         * Launch Paramters
         */
        TInt                        	iLaunchParams;
        
        /**
         * Akn Global confirmation query
         * Owns
         */
        CAknGlobalConfirmationQuery*    iConfirmQuery;

        /**
         * Flag to denote whether access point selection requested or not
         */
        TBool                   		iRequestIapSelect;

        /**
         * SUPL Servers SLP ID to get and set the server attributes.
         * Owns.
         */
        TInt64							iCurrentSlpId;
                        
        /**
         * Supl Server Editor 
         * Owns
         */
        CLocSUPLServerEditor*			iEditorDlg;

        /**
         * Editor Observer
         * Owns
         */
        MSuplServerEditorObserver*		iEditorObserver;
        
    };
    
#endif // C_LOCSUPLSETTINGSUIENGINE_H_

