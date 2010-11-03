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
* Description: This  is a header file for view class of Location settings Ui
*
*/

#ifndef C_LOCSYSUIVIEW_H_
#define C_LOCSYSUIVIEW_H_

//  System Includes
#include <e32base.h>
#include <aknview.h>
#include <eikclb.h>
#include <ConeResLoader.h>
#include <gsplugininterface.h>

// User Includes
#include "locsysuiviewuid.hrh"
#include "locsysuicontainereventhandler.h"
#include "locationui.h"
#include "locsettingsuiecomnotifier.h"

// Forward Declarations
class CGulIcon;
class CLocSysUiContainer;
class CLocSettingsUiEngine;

// Global Constant Declarations

/** 
 * Plug-In Impletation and View ID 
 */
const TUid KLocSysUiViewPluginUid = { LOCSYSUIVIEW_UID3 };

/**
 * Name of the Plug-in resource file
 */
_LIT( KLocSysUiViewResFileName, "locsysuiviewrsc.rsc" );

// Class Declaration

/**
* Location System UI plugin. Extends the CGSPluginInterface to implement the 
* Location Specific Settings. 
*
* @lib locsysuiview.lib
* @since S60 v3.1  
*/
class CLocSysUiView : public CGSPluginInterface,
					  public MLocationUIObserver,
					  public MLocSettingsUiEcomNotifer,
					  public MLocSysUiContainerEventHandler 
	{        	
	public:
        /**
         * Destructor
         */
        virtual ~CLocSysUiView();
        
        /**
         * Creates new Location Sys Ui plugin.
         *
         * @return CLocSysUiView*	Reference to the application view
         */                                                 
        IMPORT_C static CLocSysUiView* NewL();

        /**
         * Creates new Location Sys Ui plugin. Leaves the object on the 
         * Clean up stack
         *
         * @return CLocSysUiView*	Reference to the application view
         */                                                 
        static CLocSysUiView* NewLC();

        /**
         * Sets the flag that the view has been launched from Positioning
         * Settings Application. This is necessary to variate the CBA for
         * stand-alone launches.
         */
        IMPORT_C void LaunchedfromPosSettingsApp();
        
        /**
         * Handles any change in the client rectangle
         */
        void HandleClientRectChange();

        /**
         * Open Postioning View. 
         * Opens the Postioning View in the default mode.
         */
        void ActivateViewL();
        
        /**
         * Closes the Positioning View and all the associated sub-settings
         * views
         */
        void DeActivateView();
       
        /**
         * Sets an External Observer for notification about the termination
         * of Postioning Setting UI.
         *
         * @param aObserver Observer to Postioning Settings UI.
         */
        void SetObserver( MLocationUIObserver*       aObserver );
         
    public:
        /**
         * From CAknView. 
         */
        void DoActivateL( const TVwsViewId&  aPrevViewId,
        					    TUid		 aCustomMessageId,
        			      const TDesC8&	     aCustomMessage );

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
    
    public:
        /**
         * From MEikMenuObserver
         */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
           
    public: 
        /**
         * Inherited from CGSPluginInterface. 
         * Method for getting caption of this plugin. This should be the 
         * localized name of the settings view to be shown in parent view.
         *
         * @param aCaption pointer to Caption variable
         */
        void GetCaptionL( TDes& aCaption ) const;

        /**
         * Inherited from CGSPluginInterface.
         * Creates a new icon of desired type. Ownership of the created icon 
         * is transferred to the caller.
         *
         * Icon type UIDs (use these defined constants):
         * KGSIconTypeLbxItem  -   ListBox item icon.
         * KGSIconTypeTab      -   Tab icon.
         * 
         * @param aIconType UID Icon type UID of the icon to be created.
         * @return CGulIcon* Pointer of the icon. NOTE: Ownership of this
         *					icon is transferred to the caller.
         */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
         * Inherited from CGSPluginInterface
         * Method for reading the ID of the plugin provider category. 
         *
         * @return Plugin provider category ID defined by 
         *         TGSPluginProviderCategory
         */
        TInt PluginProviderCategory() const;

    public: // Inherited from MLocationUIObserver
        /**
         * Inherited from MLocationUIObserver
         * Notifies the termination of the Settings UI
         *
         * @param aErrorCode The termination reason. KErrNone for normal 
         *					terminations. In case of error or pre-mature
         *					aborting System wide Error codes.
         */
        void LocationUIDismissed( TInt aErrorCode );

    public: // Inherited from MLocSettingsUiEcomNotifer
        /** 
         * Inherited from MLocSettingsUiEcomNotifer
         * Notifies any change in the state of the underlying Settings UI
         * session
         *
         * @param aErrorCode Error Code denoting the state change
         */
        void EComSessionStateChanged( TInt aErrorCode );
    
    public:
        /**
         * Handles the Item selection event from the container.
         * Inherited from MLocSysUiContainerEventHandler
         */
        void ListBoxItemSelectedL();

        /**
         * Handles the Screen Size update
         * Inherited from MLocSysUiContainerEventHandler
         */
        void HandleScreenSizeChange();
               
	private:
        /**
         * Constructor
         */
        CLocSysUiView();
        
        /**
         * Second Phase Constructor
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
	    /**
         * Shows or dims help option, depending wheather help feature
         * is supported or not.
         * @param aMenuPane
         */
        void HandleHelpFeature( CEikMenuPane& aMenuPane ) const;
        	 
	private:
	    /**
	     * View of the View that was active before this view was launched
	     */
		TVwsViewId 						iPrevViewId;
		
	    /**
	     * Resource Loader
	     */
		RConeResourceLoader 			iResourceLoader;
		
	    /**
	     * Reference to the Location System UI Engine
	     * Owns
	     */
		CLocSettingsUiEngine*		    iEngine;
		
	    /**
	     * Reference to the Location System UI Container
	     * Owns
	     */
		CLocSysUiContainer*		        iContainer;
		
	    /**
	     * Boolean flag to denote that a UI is waiting for its dismissal
	     */
		TBool							iSettingsUiDismissalPending;
		
		/**
		 * Current Highlighted item index. Used to refresh the list box when 
		 * we return from the Settings UIs
		 */
		TUint                           iHighlightedIndex;
		 
        /**
         * Postioning Settings Observer.
         */
        MLocationUIObserver*            iObserver;		 
        
        /**
         * Boolean flag to denote whether the view is launched from
         * Positioning Settings Application
         */
        TBool                           isLaunchedFromPosSettingsApp;
	};

#endif // C_LOCSYSUIVIEW_H_

