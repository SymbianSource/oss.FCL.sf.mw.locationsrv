/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Postioning Method Settings UI plugin to the Location System
*				  UI engine
*
*/


#ifndef C_LOCPSYSETTINGSUI_H_
#define C_LOCPSYSETTINGSUI_H_

// User Includes
#include "locsettingsui.h"
#include "locpsycommandhandler.h"

// System Includes
#include <ConeResLoader.h>
#include <data_caging_path_literals.hrh>

// Forward Declarations
class MLocSettingsUiObserver;
class CAknViewAppUi;
class CLocPsySettingsView;

// Global constants

/**
 * Position Method Settings RSC file name
 */
_LIT( KPosSettingsUiResFileName, "locpsysettingsuiview.rsc" );

/**
 * Position Method Settings UI plugin
 *
 * The Positioning Method Settings UI is implemented as an ECom plug-in
 * that plugs into the Location System UI Engine ECom interface. It 
 * implements the settings view and functionalities for configuring PSYs.
 * It also provides a plug-in interface for sub-settings UIs (like PSY 
 * specific Configuration UIs) to get included into the settings hierarchy. 
 * It uses the Location framework for retrieving the location settings. 
 */
class CLocPsySettingsUi : public CLocSettingsUi,
						  public MLocPsyCommandHandler
    { 
    public:
        /**
         * Symbian OS two-phased constructor
         * @return 
         */
        static CLocPsySettingsUi* NewL( TAny* aParams );
 
        /**
         * Destructor.
         */
        virtual ~CLocPsySettingsUi();
        
    public:
        /**
         * Inherited from CLocSettingsUi
         * Launches the Settings UI
         *
         * @param aUiLaunchParams	Flags that define specific configuration of
         *                          if the Settings UI, that the user of the API
         *                          wants to launch.
         */
        void LaunchL( TInt		aUiLaunchParams );

        /**
         * Inherited from CLocSettingsUi
         * Closes the running Settings UI prematurely.
         *
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
         * 
         * @return Priority of the Settings UI
         */
        TInt GetPriority();

        /**
         * Inherited from MLocPsyCommandHandler
         * Handles commands from the views
         *
         * @param  aCommand Command ID
         */
        void HandleCommandL( TInt aCommand );

    private:
        /**
         * C++ default constructor.
         */
        CLocPsySettingsUi( MLocSettingsUiObserver&	aSettingsUiObserver,
        	 		 	   CAknViewAppUi&			aAppUi );  
        	 		 	    
        /**
         * Second phase constructor.
         */
        void ConstructL();

        	
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
	     * View of the View that was active before this view was launched
	     */
    	RConeResourceLoader 			iResourceLoader;
    	
    	/** 
    	 * Reference to the Position Method Settings View
    	 * Owns
    	 */
    	CLocPsySettingsView*			iView;
    	
    	/**
    	 * View Id for Position Method Settings View
    	 */
    	TUid							iViewId;
    	
    };

#endif // C_LOCPSYSETTINGSUI_H_
