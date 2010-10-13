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
* Description:  Adapter to the SUPL Settings API
*
*/


#ifndef CLOCSUPLSETTINGSADAPTER_H_
#define CLOCSUPLSETTINGSADAPTER_H_

// System Includes
#include <e32base.h>
#include <epos_csuplsettingparams.h>		// class CServerParams
#include <epos_csuplsettings.h>				// class CSuplSettings
#include <epos_msuplsettingsobserver.h>

// User Includes
#include "epos_msuplsettingsobserver.h"
#include "epos_msuplsessionobserver.h"

// Forward Declarations
class MLocSUPLSettingsAdapterObserver;
class MLocSUPLSettingsSessionObserver;

// Class Declaration

/**
 * Adapter class for the SUPL settings API. Handles all the interfacing
 * functionality to the SUPL Settings API. It also observes for the Settings
 * changes communicated through the MSuplSettingsObserver
 */
class CLocSUPLSettingsAdapter : public CActive,
                         		public MSuplSettingsObserver,
                         		public MSuplSessionObserver
    {
    public:
        /**
         * SUPL Usage enumerations which display the order of display
         * in the User interface
         */
         enum TLocSuplUsage
            {
            ELocSuplAutomatic = 0,
            ELocSuplAutomaticatHome,
            ELocSuplAlwaysAsk,
            ELocSuplDisable
            };               
    public:
        /**
 		 * Static Two phase contructor that instantiates the CLocSUPLSettingsAdapter
		 * 
		 * @param aObserver				    Observer to the SUPL settings adapter
		 * @return CLocSUPLSettingsAdapter*	Reference to the object created
		 */
        static CLocSUPLSettingsAdapter* NewL(
                            MLocSUPLSettingsAdapterObserver&    aObserver );

        /**
         * Destructor
         */
        virtual ~CLocSUPLSettingsAdapter();

    private:
        /**
         * Private Constructor
         */
        CLocSUPLSettingsAdapter( MLocSUPLSettingsAdapterObserver&    aObserver );

        /**
         * Second phase of the two phase constructor
         */
        void ConstructL();
				
    public:
        /**
         * Initializes the SUPL Settings API. This is an asynchronus call, the call
         * back for which is given through the observer
         */
        void Initialize();
        
        /**
         * Cancels the Initialization
         */
        void CancelInitialize();
        
        /**
         * Sets the SUPL Settings usage.
         *
         * @param   aIndex   Index of the SUPL settings usage item.
         */        
        void SetSuplUsageL( const TLocSuplUsage    aIndex );

        /**
         * Obtains the SUPL Settings usage.
         *
         *  @param   aSuplUsage     SUPL settings usage string.
         */  
        void GetSuplUsageL( TDes&    aSuplUsage );
        
        /**
         * Obtains the SUPL usage
         * 
         * @return The SUPL usage
         */
        TInt GetSuplUsage();
        
        /**
         * Obtains the Supl Settings usage index
         * 
         * @return TLocSuplUsage SUPL settings usage index 
         */
        TLocSuplUsage GetSuplUsageIndex();
        

        /***************************************************************************
	     * 
	     * @since S60 9.1TB 
	     * 
         ***************************************************************************/

        /**
	     * Adds the new server into settings. 
	     * After adding server entry, the server identity generated will be retuned with aSlpId..  
	     * 
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aServerAddress Server address in string format 
	     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
	     * used to access the HSLP over the internet.  This can have maximum size of 100 characters.
	     * @param [IN] aUsageInHomeNw Inidcates whether server can be used in the home network or not 
     	 */
		void AddNewServerL( 
                const TDesC& aServerAddress,
                const TDesC& aIapName,
                const TBool aUsageInHomeNw
        );

	    /**
	     * Deletes server entry from SUPL settings storage.  
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId Server identity for which the entery needs to be deleted from the SUPL settings storage 
	     *
	     */
	    void RemoveServerL(
	    		const TInt64 aSlpId
	    );

	    /**
	     * This synchronous method is used to retrieve properties of all servers from SUPL settings storage.  
	     * This method retrieves all properties for each server.
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [OUT] aParamValues Array of retrieved server entries 
	     * @ref CServerParams class
	     *
	     */
	    void GetAllSlpL(
	            RPointerArray<CServerParams>& aParamValues
	    ) const;

	    /**
	     * This method is used to retrieve all properties of server from SUPL settings storage
	     * based on SLP identification.  
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameters needs to be changed 
	     * @param [OUT] aParamValues Parameter for which the value needs to be changed  
	     * @ref CServerParams class
	     */
	    void GetSlpInfoFromIdL(
	            const TInt64 aSlpId, 
	            CServerParams *aParamValues
	    ) const;

	    /**
	     * Sets the priority of the server to the specified priority.  
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId Server identity for which the priority needs to be changed 
         * @param [IN] aPriority Number of positions to move 
         * @param [IN] aDirection Whether to increase or decrease priority. 
	     *
	     */
	    void ChangePriorityL(
	            TInt64 aSlpId, 
		        TInt aPriority,
		        TBool aDirection
	    );

	    /**
	     * Sets the server address parameter in server settings 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
	     * @param [IN] aServerAddres SUPL Server address 
	     *
	     */
	    void SetServerAddressL( 
	            const TInt64 aSlpId, 
	            const TDesC& aServerAddress
	    );

	    /**
	     * Obtains the server address parameter from server settings.  Client
	     * has to allocate memory for server address parameter.
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID of server for which the server address needs to be retrieved 
	     * @param [OUT] aServerAddres SUPL Server address 
	     * 
	     */
	    void GetServerAddressL( 
	            TInt64 aSlpId, 
	            TDes& aServerAddress
	    ) const;

	    /**
	     * Sets the IAP Name in server parameters. IAP name refers to 
	     * the access point which is used to access the HSLP over the internet. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
	     * @param [IN] aIapName The Internet Access Point Name refers to the access point which is 
	     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
	     * 
	     */
	    void SetIapNameL( 
	            const TInt64 aSlpId, 
	            const TDesC& aIapName
	    );

	    /**
	     * Obtains the IAP Name in server parameters. IAP name refers to 
	     * the access point which is used to access the HSLP over the internet. It is client's
	     * responsibility to allocate memory for IAP name before invoking this method.
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
	     * @param [OUT] aIapName The Internet Access Point Name refers to the access point which is 
	     * used to access the HSLP over the internet.  This can have maximum size of 100 characters. 
	     * 
	     */
	    void GetIapNameL( 
	            const TInt64 aSlpId, 
	            TDes& aIapName
	    ) const;


	    /**
	     * Sets the parameter which indicates whether server
	     * can be used for SUPL session or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
	     * @param [IN] aEnable Flag to indicate whether server can be enabled or not 
	     * 
	     */
	    void SetServerEnabledFlagL(
	            const TInt64 aSlpId, 
	            const TBool aEnable 
	    );

	    /**
	     * Obtains the parameter which indicates whether server
	     * can be used for SUPL session or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
	     * @param [OUT] aEnable Flag to indicate whether server can be enabled or not 
	     * 
	     */
	    void GetServerEnabledFlagL(
	            const TInt64 aSlpId, 
	            TBool& aEnable 
	    ) const;

	    /**
	     * Sets the parameter which indicates whether server
	     * can be used out side home network or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be changed 
	     * @param [IN] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
	     * 
	     */
	    void SetUsageInHomwNwFlagL(
	            const TInt64 aSlpId, 
	            const TBool aHomeNwFlag 
	    );

	    /**
	     * Obtains the parameter which indicates whether server
	     * can be used out side home network or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
	     * @param [OUT] aHomeNwFlag Flag to indicate whether server can be used outside home network or not 
	     * 
	     */
	    void GetUsageInHomwNwFlagL(
	            const TInt64 aSlpId, 
	            TBool& aHomeNwFlag 
	    ) const;

	    /**
	     * This method is used to change parameter which indicates whether server
	     * details are editable or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
	     * @param [IN] aEditFlag Flag to indicate whether server details are editable or not 
	     */ 
	    void SetEditableFlagL( 
	            const TInt64 aSlpId, 
	            const TBool aEditFlag 
	    ) const;

	    /**
	     * This method is used to retrieve parameter which indicates whether server
	     * details are editable or not. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSlpId SLP ID for which the parameter needs to be retrieved 
	     * @param [OUT] aEditFlag Flag to indicate whether server details are editable or not 
	     */ 
	    void GetEditableFlagL( 
	            const TInt64 aSlpId, 
	            TBool& aEditFlag 
	    ) const;

	    /**
	     * Obtains the number of server entries present in SUPL settings. 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [OUT] aCount Number of SLP entries 
	     *
	     */
	    void SlpCountL(
	            TInt& aCount 
	    );
	    
	     /**
	     * Obtains the count of active SUPL sessions 
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [OUT] aActiveSessionsCount Count of active SUPL sessions 
	     *
	     */
	    void GetActiveSessionsCountL( TInt& aActiveSessionsCount );
	    
	     /**
	     * This method is used to retrieve properties of all triggers from SUPL settings storage.  
	     * This method retrieves all properties for each trigger.
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [OUT] aParamValues Array of retrieved trigger entries 
	     * @ref CTriggerParams class
	     *
	     */
	    void GetTriggerParamsL( RPointerArray<CTriggerParams>& aParamValues ) const;
	    
	     /**
	     * This method is used to retrieve properties of all triggers from SUPL settings storage.  
	     * This method retrieves all properties for required trigger.
	     *
	     * @since S60 9.2TB 
	     *
	     * @param [IN] aSessionId Sessiond Id 
	     * @param [OUT] aTrigger Array of retrieved trigger entries 
	     * @ref CTriggerParams class
	     *
	     */
	    void GetTriggerParamsL( TInt64 aSessionId, CTriggerParams*& aTrigger ) const;
	    
	     /**
	     * Changes the notification status for a trigger session based on session id
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSessionId Id of session for which the notification status has to be changed
	     *
	     */
	    void ChangeNotificationStatusL( TInt64 aSessionId, TBool aTriggerNotificationStatus ) const;
	    
	     /**
	     * Deletes a trigger session based on session id
	     *
	     * @since S60 9.1TB 
	     *
	     * @param [IN] aSessionId Id of session that needs to be deleted 
	     *
	     */
	    void RemoveTriggerSessionL( TInt64 aSessionId ) const;
	   
	   /**
	     * Set the Observer for the specific Session
	     * 
	     */  
	    void SetSessionObserver( MLocSUPLSettingsSessionObserver* aObserver );
	   
	   /**
	     * Remove the Session observer
	     * 
	     */  
	    void RemoveSessionObserver( );
	    
	    /**
         * Generates IMSI address
         */
        void GenerateHslpAddressFromImsi(TDes& aIMSIAddress );


    public: // Accessor functions for SUPL settings usage strings
        const TDesC& Automatic();
        
        const TDesC& AutomaticAtHome();
        
        const TDesC& AlwaysAsk();
        
        const TDesC& Disable();
              	
	public:
	
	    /** 
	     * Inherited from MSuplSettingsObserver
	     */	
        void HandleSuplSettingsChangeL(
                TSuplSettingsEventType aEvent,
                TInt aSlpId                        
        );
        
        
	    /** 
	     * Inherited from MSuplSessionObserver
	     */	
		void HandleSuplSessionChangeL( 
                TSuplSessionEventType aEvent,
                TInt64 aSessionId                         
        		);		
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
         * Observer for the SUPL Settings UI Adapter
         * Reference
         */
        MLocSUPLSettingsAdapterObserver&    iObserver;
        
        /**
         * SUPL Settings UI
         * Owns
         */
        CSuplSettings*                      iSUPLSettings;
        
        /**
         * SUPL settings usage is automatic
         * Owns
         */
        HBufC*                              iAutomatic;
        
        /**
         * SUPL settings usage is automatic in home network
         * Owns
         */
        HBufC*                              iAutomaticatHome;
        
        /**
         * SUPL settings usage needs a user confirmation
         * Owns
         */
        HBufC*                              iAsk;
        
        /**
         * SUPL is disabled.
         * Owns.
         */
        HBufC*                              iDisable;
        
        /**
         * Observer for the specific SUPL session.
         * Does not own.
         */
		MLocSUPLSettingsSessionObserver* iSessionObserver;
        
    };

#endif      // CLOCSUPLSETTINGSADAPTER_H_

// End of File
