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
* Description:  Manages the E-Comm Plugin handling for the Settings UI.
*
*/

#ifndef C_LOCSETTINGSUIPLUGINLOADER_H_
#define C_LOCSETTINGSUIPLUGINLOADER_H_

// System Include
#include <e32base.h>
#include <ecom/ecom.h>

// Forward Declarations
class CLocationSettings;
class CAknViewAppUi;
class MLocSettingsUiEcomNotifer;

// Class Declarations 

/**
 * Plugin loader class. 
 * Loads all the Settings related ECom plugins and also registers itself for 
 * notfication from the ECOM server. It takes up complete onus of the creation
 * of the CSettingsUi objects.
 *
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
 NONSHARABLE_CLASS( CLocSettingsUiPluginLoader ) : public CActive
	{
	private:
		/** 
		 * Overloaded Constructor 
		 *
		 * @param aAppUi    App UI Reference
		 */
	 	CLocSettingsUiPluginLoader( CAknViewAppUi&	aAppUi );
	public:
		/**
		 * Creates an instance of the Settings UI Plug-in Loader.
		 *
		 * @param  aObserver				    Observer to the CLocSettingsUiPluginLoader
		 *									    class
		 * @param  aAppUi					    App Ui Reference
		 * @return CLocSettingsUiPluginLoader*	Reference to the instantiated class 
		 */
		static CLocSettingsUiPluginLoader* NewL( CAknViewAppUi&	    aAppUi );

        /**
         * Creates an instance of the Settings UI Plug-in Loader.
         *
         * @param  aObserver				Observer to the CLocSettingsUiPluginLoader
         *									class
         * @param  aAppUi					App Ui Reference
         * @return CLocSettingsUiPluginLoader*	Reference to the instantiated class 
         */
        static CLocSettingsUiPluginLoader* NewLC( CAknViewAppUi&    aAppUi );
        						
        /**
         * Destructor 
         */
        virtual ~CLocSettingsUiPluginLoader();

        /** 
         * Creates all the settings UI currently available in the system. The 
         * user if this interface should ensure that he passes an empty
         * Settings Array structure.
         *
         * @param aDescArray 		 Array of Settings UI.
         */
        void CreateAvailableSettingsUisL( 
                        RPointerArray<CLocationSettings>&    aSettingsArray );
        							   
        /**
         * Creates the Settings UI and the corresponding description entry. This
         * function creates a new Setting UI ECOM plug-in and a Location Settings
         * object. the created object is appended to the existing list and a reference to
         * it returned. The ownership of the created Settings object is retained with
         * the Settings array. This function assumes that the object doesnt already exists
         *
         * @param aImplementationUid The Implementation UID of the plug-in that
         *                           has to be created
         * @param aSettingsArray	 Array of Settings UIs
         * @return CLocationSettings& Reference to the created object. The ownership is not
         *                            by the return value.
         */
        CLocationSettings& CreateSettingsUiL( 
                                TUid                                 aImplementationUid,
                                RPointerArray<CLocationSettings>&    aSettingsArray );
        /** 
         * Updates the Settings UI list and the description structures. The new
         * additions are appened in-order. The interface does not
         * delete any instances if they are not in the current implementation 
         * list
         *
         * @param aSettingsArray     Array of Settings UIs.
         */
        void UpdateAvailableSettingsUisL( 
                        RPointerArray<CLocationSettings>&    aSettingsArray );

        /**
         * Initiates a notify request on the ECom Session
         *
         * @param aSettingsNotifer  Notifier for communication of changes in the 
         *                          Ecom loading and unloading for sub-settings
         *                          plug-ins. The default value is used if the
         *                          caller of this API intends to obtain a 
         *                          notification on the previously set observer.        
         */ 
        void StartEComNotifyL( MLocSettingsUiEcomNotifer*   aNotifier = NULL );
        
        /**
         * Stop the outstanding EComNotification
         */
        void StopEComNotify();
        
	protected:
        /** 
         * Inherited from CActive 
         */
        void RunL();

        /** 
         * Inherited from CActive 
         */
        void DoCancel();

        /** 
         * Inherited from CActive 
         */
        TInt RunError( TInt aError );

	private:		
		/**
         * Reference to the Application Ui to which all the Settings UI would
         * belong to
         */
		CAknViewAppUi&					    iAppUi;
		
		/**
		 * Handle to the ECom Session
		 */
		REComSession*					    iSession;
		
		/**
		 * ECom notifier
		 */
		MLocSettingsUiEcomNotifer*          iNotifier;
		
	};

#endif // C_LOCSETTINGSUIPLUGINLOADER_H_

