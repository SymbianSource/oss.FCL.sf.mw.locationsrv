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
* Description:  Location Notation Preferences Plugin view
*
*/

#ifndef C_LOCNOTPREFPLUGINVIEW_H_
#define C_LOCNOTPREFPLUGINVIEW_H_

//  System Includes
#include <data_caging_path_literals.hrh>
#include <e32base.h>
#include <aknview.h>
#include <aknViewAppUi.h>
#include <ConeResLoader.h>

// User Includes
#include "locnotprefpluginuid.hrh"
#include "locnotprefplugincmdhdlr.h"
#include "locnotprefengobserver.h"

// Forward Declarations
class CLocNotPrefPluginContainer;
class MLocNotPrefPluginCmdHdlr;
class MLocSettingsUiObserver;
class CLocNotPrefPluginEngine;
class CTextResolver;
    
// Constant Declarations
const TUid KLocNotPrefViewPluginUid = { KLocNotPrefPluginUID3 };
_LIT( KLocNotPrefViewResFileName, "locnotprefpluginview.rsc" );

// Class Definition

/**
 * Location Notation Preferences Plugin view class. 
 */
class CLocNotPrefPluginView : public CAknView,
                              public MLocNotPrefPluginCmdHdlr,
                              public MLocNotPrefEngObserver
    {
    public:
        /**
         * Creates new Location Notation Preferences plug-in view.
         *
         * @param  aSettingsObsrv           Observer to the Settings UI
         * @return CLocNotPrefPluginView&   Reference to the application view
         */                                                 
        static CLocNotPrefPluginView* NewL( MLocSettingsUiObserver& aSettingsObsrv );

        /**
         * Creates new Location Notation Preferences plug-in view. 
         * Leaves the object on the Clean up stack
         *
         * @param  aSettingsObsrv           Observer to the Settings UI         
         * @return CLocNotPrefPluginView&   Reference to the application view
         */                                                 
        static CLocNotPrefPluginView* NewLC( MLocSettingsUiObserver& aSettingsObsrv );
        
        /**
         * Destructor
         */
        ~CLocNotPrefPluginView ();

        /**
         * Close the Notation preferences Settings
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
        * From MEikMenuObserver
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );        
        
    public:
        /**
         * Inherited from MLocNotPrefPluginCmdHdlr
         * Handles commands from the container
         *
         * @param  aCommand Command ID
         */
        void HandleCmdL( TLocNotPrefPluginCmd aCommand );

    public:
        /**
         * Inherited from MLocNotPrefEngObserver
         */
        void HandleSettingsChangeEventL();
                
    private:
        /**
         * Constructor
         */
        CLocNotPrefPluginView( MLocSettingsUiObserver& aSettingsObsrv );

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
         * Location notation preferences Container
         * Owns
         */
        CLocNotPrefPluginContainer*     iContainer;
        
        /**
         * Location noation preferences engine
         * Owns
         */
        CLocNotPrefPluginEngine*        iEngine;
        
        /**
         * Standard Text resolver for converting Error codes to 
         * standard texts
         * Owns
         */
        CTextResolver*                  iTextResolver;        
    };          

#endif // C_LOCNOTPREFPLUGINVIEW_H_

