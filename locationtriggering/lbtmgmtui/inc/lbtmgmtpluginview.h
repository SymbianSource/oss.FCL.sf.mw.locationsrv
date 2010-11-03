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
* Description:  Location Triggering Management Plugin view
*
*/

#ifndef CLBTMGMTPLUGINVIEW_H_
#define CLBTMGMTPLUGINVIEW_H_

//  System Includes
#include <data_caging_path_literals.hrh>
#include <e32base.h>
#include <aknview.h>
#include <aknViewAppUi.h>
#include <ConeResLoader.h>

// User Includes
#include "lbtmgmtpluginuid.hrh"
#include "lbtmgmtplugincmdhdlr.h"
#include "lbtmgmtengobserver.h"

// Forward Declarations
class CLbtMgmtPluginContainer;
class MLbtMgmtEngObserver;
class MLocSettingsUiObserver;
class CLbtMgmtPluginEngine;
    
// Constant Declarations
const TUid KLbtMgmtViewPluginUid = { KLbtMgmtPluginUID3 };
_LIT( KLbtMgmtViewResFileName, "lbtmgmtpluginview.rsc" );

// Class Definition

/**
 * Location Triggering Management Plugin view class. 
 */
class CLbtMgmtPluginView : public CAknView,
                              public MLbtMgmtPluginCmdHdlr,
                              public MLbtMgmtEngObserver
    {
    public:
        /**
         * Creates new Location Triggering Management plug-in view.
         *
         * @param  aSettingsObsrv           Observer to the Settings UI
         * @return CLbtMgmtPluginView&   Reference to the application view
         */                                                 
        static CLbtMgmtPluginView* NewL( MLocSettingsUiObserver& aSettingsObsrv );

        /**
         * Creates new Location Triggering Management plug-in view. 
         * Leaves the object on the Clean up stack
         *
         * @param  aSettingsObsrv           Observer to the Settings UI         
         * @return CLbtMgmtPluginView&   Reference to the application view
         */                                                 
        static CLbtMgmtPluginView* NewLC( MLocSettingsUiObserver& aSettingsObsrv );
        
        /**
         * Destructor
         */
        ~CLbtMgmtPluginView ();

        /**
         * Close the Triggering Management Settings
         */
        void CloseSettings();
        
        /**
         * From CAknView. 
         */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                                TUid        aCustomMessageId,
                          const TDesC8&     aCustomMessage );
        
        /**
         * From CAknView. 
         */
        void DoDeactivate();
 
        /**
         * From CAknView. Returns the View Id. Should be chosen to reflect 
         * the Implementation UID of the Plugin
         */
        TUid Id() const;

        /**
         * From CAknView. 
         */
        void HandleCommandL(TInt aCommand);
        

       /**
        * For CBA Change
        */
				void SetCba( TInt aResourceId );
    /*
     * returns CLbtMgmtPluginEngine
     */
    CLbtMgmtPluginEngine* GetLbtMgmtPluginEngine();

    public:
        /**
         * Inherited from MLbtMgmtPluginCmdHdlr
         * Handles commands from the container
         *
         * @param  aCommand Command ID
         */
        void HandleCmdL( TLbtMgmtPluginCmd aCommand );

    public:
        /**
         * Inherited from MLbtMgmtEngObserver
         */
        void HandleSettingsChangeEventL();
                
    private:
        /**
         * Constructor
         */
        CLbtMgmtPluginView( MLocSettingsUiObserver& aSettingsObsrv );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
            
        /**
         * Shows or dims help option, depending wheather help feature
         * is supported or not.
         * @param aMenuPane
         */
        void HandleHelpFeature( CEikMenuPane& aMenuPane ) const;
              
        /**
         * Handle Settings Change Event
         * 
         * @param aCmd Settings change command
         */
        void SettingsChangeL( TInt aCmd );
        
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
        void OpenLocalizedResourceFileL( const TDesC&                aResourceFileName, 
                                               RConeResourceLoader&  aResourceLoader );
        
    private:        
        /**
         * Reference to the Settings UI Observer. Used to notify the dismissal of the 
         * plug-in
         */
        MLocSettingsUiObserver&         iSettingsObserver;
        
        /**
         * Resource loader handle
         * Owns
         */
        RConeResourceLoader             iResourceLoader;
        
        /**
         * Location Trigger Management Container
         * Owns
         */
        CLbtMgmtPluginContainer*     iContainer;
        
        /**
         * Location Trigger Management engine
         * Owns
         */
        CLbtMgmtPluginEngine*        iEngine;
       
    };          

#endif // CLBTMGMTPLUGINVIEW_H_

