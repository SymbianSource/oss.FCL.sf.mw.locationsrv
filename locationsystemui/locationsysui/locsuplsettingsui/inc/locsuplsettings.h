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
* Description:  SUPL Settings UI implementation.
*
*/


#ifndef CLOCSUPLSETTINGS_H_
#define CLOCSUPLSETTINGS_H_

// User Includes
#include "locsettingsui.h"
#include "locsuplsettingsuiengobserver.h"
#include "locsuplsettingseventhandler.h"

// System Includes
#include <ConeResLoader.h>
#include <data_caging_path_literals.hrh>


// Forward Declarations
class CAknQueryDialog;
class MLocSettingsUiObserver;
class CLocSUPLSettingsView;
class CLocSUPLServerView;
class CLocSUPLSessionView;
class CLocSUPLSettingsUiEngine;

// Global constants

/**
 * SUPL Settings RSC file name
 */
_LIT( KSUPLUiResFileName, "locsuplsettingsui.rsc" );

class CLocSUPLAO : public CActive
    {               
    public:
        static CLocSUPLAO* NewL(
                MLocSettingsUiObserver&     aSettingsUiObserver,
                CLocSUPLSettingsUiEngine&   aSettingsEngine,
                TInt                        aLaunchParams  );
        
        virtual ~CLocSUPLAO();
        
        void CompleteRequest();

    private:
        CLocSUPLAO( MLocSettingsUiObserver&     aSettingsUiObserver,
                    CLocSUPLSettingsUiEngine&   aSettingsEngine,
                    TInt                        aLaunchParams );
        
        void RunL();
        
        void DoCancel();
        
        
        
    private:
        MLocSettingsUiObserver&         iSettingsUiObserver;
        CLocSUPLSettingsUiEngine&       iSettingsEngine;
        TInt                            iLaunchParams;  
    };

/**
 * CLocSUPLSettings
 *
 * ECom Plugin implementation for SUPL Settings UI. The Settings UI provides 
 * two major functionalities i.e., Launching a Settings UI plug-in and 
 * cancelling the existing launched UI.
 * The Launch request depending on the parameters is expected to pop up a 
 * query dialog to request the user whether he intends to configure SUPL
 * settings currently. The default behaviour should be to pop up the configuration
 * dialog for SUPL settings.
 */
class CLocSUPLSettings : public CLocSettingsUi,
                         public MLocSUPLSettingsUiEngObserver,
                         public MLocSUPLSettingsEventHandler
    {
    
    public:
        
        /**
         * Symbian OS two-phased constructor
         * @return 
         */
        static CLocSUPLSettings* NewL( TAny* aParam );
    
        /**
         * Destructor.
         */
        ~CLocSUPLSettings();

    public: // Inherited from CLocSettingsUi
         
    	/**
    	 * Inherited from CLocSettingsUi
		 * Launches the Settings UI
		 *
		 * @param aUiLaunchParams	Flags that define specific configuration of
         *                          if the Settings UI, that the user of the API
         *                          wants to launch. If more than one functionality
         *                          is requested then the various combinations
         *                          are to be ORed.
		 */
		 void LaunchL( TInt		aUiLaunchParams );
         
        /**
         * Inherited from CLocSettingsUi
         * Launches the Settings UI
         *
         * @param aUiLaunchParams   Flags that define specific configuration of
         *                          if the Settings UI, that the user of the API
         *                          wants to launch. If more than one functionality
         *                          is requested then the various combinations
         *                          are to be ORed.
         */
         void LaunchL( const TDesC&       aUiLaunchParams );

		/**
		 * Inherited from CLocSettingsUi
		 * Closes the running Settings UI prematurely.
		 */
		 void Close();

		/**
		 * Inherited from CLocSettingsUi
         * Method for getting caption of this plugin. This should be the 
         * localized name of the settings view to be shown in parent view.
         *
         * @param aCaption pointer to Caption variable
         */
         void GetCaptionL( TDes& aCaption ) const;

		/**
		 * Inherited from CLocSettingsUi
         * Returns the priority of the UI module. 
         * 
         * @return Priority of the Settings UI
         */
         TInt GetPriority();

		/**
		 * Inherited from CLocSettingsUi
		 * Accessor Function for determine whether the UI presence 
		 * should be shown to the user or should be hidden
		 *
		 * @return ETrue  If the presence of the UI be shown to the usr
				   EFalse If the UI should funtion as a non visible UI
		 */
		 TBool	IsVisible();
	
	public: // Inherited from MLocSUPLSettingsUiEngObserver
		
		/**
		 * Inherited from MLocSUPLSettingsUiEngObserver
		 */
		void HandleSettingsEventL( TSettingsEventType    aEvent );
	
	public: // Inherited from MLocSUPLSettingsEventHandler
		
		/**
		 * Inherited from MLocSUPLSettingsEventHandler
		 */
		void HandleScreenSizeChange();
		 
		/**
         * Inherited from MLocSUPLSettingsEventHandler
         * Handles commands from the views
         *
         * @param  aCommand Command ID
         */
        void HandleCommandL( TInt aCommand );
    
	private:
	    enum TLaunchParamType
	        {
	        ELaunchParamInt = 0,
	        ELaunchParamString
	        };
        
    private:
        /**
         * C++ default constructor.
         */
        CLocSUPLSettings( MLocSettingsUiObserver&		aSettingsUiObserver,
					 	   CAknViewAppUi&				aAppUi );

        /**
         * Second phase constructor.
         */
        void ConstructL();
  
        /**
         * Launches the SUPL UI
         */
        void LaunchSUPLUiL();
        
        /**
         * Opens the nearest localized resourcefile using aResourceLoader. 
         * aResourceLoader should be opened only once before closing it. 
         * Otherwise it will raise a panic. Therefore it is better to have 
         * multiple resource loaders.
         * 
         * @param aResourceFileName Drive and name of resource file in format 
         *                          <path>:<rsc_file_name>
         * @param aResourceLoader Resource loader for the resource. The loader 
         *                        must not be opened allready or it will raise 
         *                        a panic. It is caller's responsibility to 
         *                        close the resource loader after using the 
         *                        resource.
         */
        void OpenLocalizedResourceFileL( const TDesC& 				aResourceFileName, 
        								       RConeResourceLoader& aResourceLoader );
          								       
    private:
	    /**
	     * Resource Loader handle for handling SUPL Settings UI specific resources
	     */
    	RConeResourceLoader 			iResourceLoader;
              								       
    	/** 
    	 * Reference to the SUPL Settings View
    	 * Owns
    	 */
    	CLocSUPLSettingsView*			iSettingsView;
    	
    	/**
    	 * View Id for SUPL Settings UI View
    	 */
    	TUid							iSettingsViewId;              								       

    	/** 
    	 * Reference to the SUPL Server View
    	 * Owns
    	 */
    	CLocSUPLServerView*				iServerView;
    	
    	/**
    	 * View Id for SUPL Server UI View
    	 */
    	TUid							iServerViewId;              								       

        /**
         * Settings UI Engine
         * Owns
         */
         CLocSUPLSettingsUiEngine*      iSettingsEngine;
         
         /** 
    	 * Reference to the SUPL Session View
    	 * Owns
    	 */
         CLocSUPLSessionView*           iSessionView;
         
    	/**
    	 * View Id for SUPL Session UI View
    	 */
    	TUid							iSessionViewId;   
         								       
        /**
         * Denotes whether a Launch request is outstanding. 
         * Necessary to handle the closing of the configuration dialog ( if it
         * exists ) gracefully. If the dialog is currenlty being shown to
         * the user and we get a Cancel request, then we shouldnot complete
         * the request with KErrAbort. This is done to ensure that we complete
         * the request only if a Launch call is outstanding
         */
         TBool                          iLaunchOutStanding;
        
        /**
         * Launch Paramters
         */
         TInt                           iLaunchParams;    
         
        /**
         * Launch Param Type
         */
         TLaunchParamType               iLaunchParamType;
         
         CLocSUPLAO*                    iLaunchAO;
    };

#endif // CLOCSUPLSETTINGS_H_

