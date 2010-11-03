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
* Description: This is a header file for engine class of Location settings Ui
*
*/


#ifndef C_LOCSETTINGSUIENGINE_H_
#define C_LOCSETTINGSUIENGINE_H_

// System Includes
#include <e32base.h>

// User Include

// Forward Declarations
class CLocSettingsUi;
class CAknViewAppUi;
class MLocSettingsUiEcomNotifer;
class CLocSettingsUiPluginLoader;
class CLocationSettings;


// Class Definition

/**
 * Encapulates the logic necessary to launch various sub-settings UIs. 
 * The Location UI AppServer and the Location Systems Plugin to the 
 * General Settings will use this object for manipulating the Settings
 * components. The class contains funtionalities for discovering 
 * various available Settings UIs, launching a UI,cancelling a UI request ,
 * terminating a UI after completion of the request and to hold back
 * the suspended UIs.
 *
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
class CLocSettingsUiEngine : public CBase
	{		   		  
	public: // Exported Functions
        /**
         * Two Phase Constructor. 
         * 
         * @param    aAppUi				     Reference to the Application UI class that 
         *								     launched the Settings Application.
         * @return	CLocSettingsUiEngine*    Reference to the created object.
         */
        IMPORT_C static CLocSettingsUiEngine* NewL( CAknViewAppUi&	aAppUi );

        /**
         * Two Phase Constructor. 
         * 
         * @param    aAppUi				    Reference to the Application UI class that 
         *								    launched the Settings Application.
         * @return	CLocSettingsUiEngine*	Reference to the created object.
         */
        IMPORT_C static CLocSettingsUiEngine* NewLC( CAknViewAppUi&	aAppUi );
        						    	   
        /**
         * Destructor
         */
        virtual ~CLocSettingsUiEngine();

    public: // Other Exported functions
        /**
         * Creates a single Settings UI Object.
         * In case the object already exists, then a reference to the existing object is
         * returned and no new object is created. In either case, the class retains the
         * ownership of the created object.
         *
         * @param aImplementationUid The Implementation UID of the plug-in that
         *                           has to be created.
         * @return CLocationSettings* Reference to the created object.
         */
        IMPORT_C CLocationSettings* CreateSettingsUiL( TUid aImplementationUid );
    
    public: // Non exported functions
        /**
         * Creates all the available settings UI. This function call performs
         * an ECom discovery of all the possible ECom plugins that are 
         * available and creates the Settings UI components as well as the 
         * plug-in UI. The ownership of the created objects is not transferred
         * to the callee but retained with the Engine class. This function needs 
         * that No settings UIs have been already created. Incase, there exists
         * any Settings UIs then the function leaves with KErrAlreadyExists.
         *
         * @return RPointerArray<CLocationSettings>& Reference to the array of Settings
         *                                           objects.
         */
        RPointerArray<CLocationSettings>& CreateAvailableSettingsUiL();
        
        /**
         * Updates the list of available UIs. The new list would contain all the
         * latest additions to the list of UIs in addition to the existing
         * UIs. The interface does not delete any instances if they are not in
         * the current implementation list.
         *
         * @return RPointerArray<CLocationSettings>& Reference to the array of Settings
         *                                           objects.         
         */
        RPointerArray<CLocationSettings>& UpdateAvailableSettingsUisL();
        
        /**
         * Accessor function to obtain the Settings UI structures. The ownership of the 
         * internal objects is not transferred.
         *
         * @return RPointerArray<CLocationSettings>& Reference to the array of Settings
         *                                           objects. 
         */
        RPointerArray<CLocationSettings>& SettingsUiArray();
        
        /**
         * Starts the Ecom notification for observing loading and unloading of the 
         * underlying sub-settings plug-ins
         *
         * @param aSettingsNotifer  Notifier for communication of changes in the 
         *                          Ecom loading and unloading for sub-settings
         *                          plug-ins
         */
        void    StartSettingsEComNotificationL( 
                            MLocSettingsUiEcomNotifer*   aNotifier );
                            
        /**
         * Stops ECom notification. Does nothing in case there was no outstanding
         * request
         */
        void    StopSettingsEComNotification();                            

	private:
	    /**
         * Constructor
         */
        CLocSettingsUiEngine( CAknViewAppUi&    aAppUi );
        			   	 			   		  
        /**
         * Second Phase of construction
         */
        void ConstructL();
		 
	private:
		/**
         * Reference to the Application Ui to which the Settings UI would belong to
         */
		CAknViewAppUi&						iAppUi;
		
		/**
		 * Array of Settings UI classes
		 * Owns
		 */
		RPointerArray<CLocationSettings>	iSettingsArray;
		
		/**
		 * Reference to the Plug-in Loader
		 * Owns
		 */
		CLocSettingsUiPluginLoader*			iPluginLoader;
	};

#endif // C_LOCSETTINGSUIENGINE_H_

