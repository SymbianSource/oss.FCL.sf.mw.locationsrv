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
* Description:  Source file for Location System UI view.
*
*
*/


// System Include
#include <barsread.h> 					// For TResourceReader
#include <featmgr.h> 				    // FeatureManager
#include <bautils.h>
#include <StringLoader.h>				// String Loader
#include <aknViewAppUi.h>				// App Ui
#include <hlplch.h>						// Help
#include <gulicon.h>					// Gul Icon
#include <eikmenup.h>					// Menu pane
#include <eikmenub.h>					// Menu Bar
#include <bldvariant.hrh>               // KFeatureHelpId
#include <data_caging_path_literals.hrh>
#include <locsysuiviewrsc.rsg>
#include <locsysuiview.mbg>	

// User Include
#include "locsysuiview.hrh"
#include "locsysuiview.h"
#include "locsysuicontainer.h"
#include "locationsettings.h"
#include "locsettingsuiengine.h"
#include "locsettingsuiclient.h"
#include "locpossettings.h"

// Literal Definitions
_LIT( KLocationSystemUiViewIconFileName, "\\resource\\apps\\locsysuiview.mif" );

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocSysUiView::CLocSysUiView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSysUiView::CLocSysUiView()
	:iResourceLoader( *CCoeEnv::Static())
	{
		
	}

// ---------------------------------------------------------------------------
// CLocSysUiView::~CLocSysUiView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSysUiView::~CLocSysUiView()
	{
	// Delete the View engine	
	delete iEngine;

	// Close the Resource loader
	iResourceLoader.Close();
	
	// Release all the resources associated with Feature Manager
	FeatureManager::UnInitializeLib();
	}
	
// ---------------------------------------------------------------------------
// CLocSysUiView* CLocSysUiView::NewL
// Creates new Location Sys Ui plugin. Internally calls NewLC
//
// @return CLocSysUiView*	Reference to the application view
// ---------------------------------------------------------------------------    
//
EXPORT_C CLocSysUiView* CLocSysUiView::NewL()
	{	
    CLocSysUiView* self = CLocSysUiView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSysUiView* CLocSysUiView::NewLC
// Creates new Location Sys Ui plugin. Leaves the object on the Cleanup stack
// 
// @return CLocSysUiView*	Reference to the application view
// ---------------------------------------------------------------------------
//
CLocSysUiView* CLocSysUiView::NewLC()
	{	
    CLocSysUiView* self = new (ELeave) CLocSysUiView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CLocSysUiView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocSysUiView::ConstructL()
    {
    // Initialize the Feature Manager library
    FeatureManager::InitializeLibL();
    
    if ( FeatureManager::FeatureSupported( KFeatureIdFfLocationsysuiHidePositioningMenu ) )
        {
        // To Do : changes for ou1cimx1#228781	Location: Positioning view does not open
        // Commenting the leaving part as the hide positioning feature flag is get enabled for all configuration by default
        // so till the flag issue resolved, the leaving part will be commented as this issue affected the BAT. 	
        // User::Leave( KErrNotSupported );
        }

    // Open the Resource File
    OpenLocalizedResourceFileL( KLocSysUiViewResFileName, iResourceLoader );
    
    // Call AknView's BaseConstrutL
    BaseConstructL( R_LOCSYSUIVIEW_APP_VIEW );
    
    // Create the View Engine
    iEngine = CLocSettingsUiEngine::NewL( *AppUi());
    
    // Load all the plug-ins. The creation of the engine does not load the
    // plugins and they have to be explicitly loaded. The return value need
    // not be stored here. Hence, the value is ignored
    iEngine->CreateAvailableSettingsUiL();
    
    }

EXPORT_C void CLocSysUiView::LaunchedfromPosSettingsApp()
    {
    isLaunchedFromPosSettingsApp = ETrue;
    }

// ---------------------------------------------------------------------------
// void CLocSysUiView::HandleClientRectChange
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::HandleClientRectChange()
	{
	if ( iContainer )
		{
		iContainer->SetRect( ClientRect());		
		}
	}

// ---------------------------------------------------------------------------
// void CLocSysUiView::ActivateView
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::ActivateViewL()
    {
    // Activate the Current view in the default mode.
    AppUi()->ActivateLocalViewL(Id());
    }

// ---------------------------------------------------------------------------
// void CLocSysUiView::DeActivateView
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::DeActivateView()
    {
        
    // Close all the sub-settings views if they are Open
    // Retrieve the Settings Description Array
	RPointerArray<CLocationSettings>& settingsArray = iEngine->SettingsUiArray();
    
    for( TInt i = 0; i < settingsArray.Count(); i ++ )
        {
        // Retrieve each settings UI
        CLocationSettings* settings = settingsArray[i];
        
        // Close the Settings UI
        settings->Close();
        }
    // Stop displaying the Menubar if its being displayed.
    StopDisplayingMenuBar();    
    }
 
// ---------------------------------------------------------------------------
// void CLocSysUiView::SetObserver
//
// ---------------------------------------------------------------------------	
//    
void CLocSysUiView::SetObserver( MLocationUIObserver*       aObserver )
    {
    iObserver = aObserver;
    }
        	
// ---------------------------------------------------------------------------
// void CLocSysUiView::DoActivateL
// 
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::DoActivateL( const TVwsViewId& 	    aPrevViewId,
											   TUid		/* aCustomMessageId*/,
						  				 const TDesC8&	/* aCustomMessage */ )
	{
	if ( !iSettingsUiDismissalPending )
		{
		// Assign the previous view Id
		iPrevViewId = aPrevViewId;
		
		// Set the Highlighted item to 0
		iHighlightedIndex = 0;	
		}
	else
		{
		iSettingsUiDismissalPending = EFalse;
		}	
	
	// Destroy the existing container if it exists
	if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer=NULL;
        }						
    
    // Retrieve the Settings Description Array
	RPointerArray<CLocationSettings>& settingsArray = iEngine->SettingsUiArray();
	 
    // Create new Container	
	iContainer = CLocSysUiContainer::NewL( *this, settingsArray, ClientRect());
	iContainer->SetMopParent( this );
	AppUi()->AddToViewStackL( *this, iContainer );
	iContainer->SetFocussedElement( iHighlightedIndex );
	
	if ( isLaunchedFromPosSettingsApp && !iEikonEnv->StartedAsServerApp())
		{
		Cba()->SetCommandSetL( R_LOCSYSUIVIEW_SOFTKEYS_OPTION_OPEN_EXIT );	
		}
	// Start the ECom notification for observing changes in the plug-in loading
	// and unloading for ECom plug-ins
	iEngine->StartSettingsEComNotificationL( this );
	}

// ---------------------------------------------------------------------------
// void CLocSysUiView::DoDeactivate
//
// ---------------------------------------------------------------------------	
//	
void CLocSysUiView::DoDeactivate()
	{
	// Stop the ECom notifcation
	iEngine->StopSettingsEComNotification();
	
	// Destroy Container
	if ( iContainer )
        {
        iHighlightedIndex = iContainer->CurrentFocussedElement();
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
	}

// ---------------------------------------------------------------------------
// TUid CLocSysUiView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//	
TUid CLocSysUiView::Id() const
	{
	return KLocSysUiViewPluginUid;
	}

// ---------------------------------------------------------------------------
// void CLocSysUiView::HandleCommandL
//
// ---------------------------------------------------------------------------	
void CLocSysUiView::HandleCommandL( TInt aCommand )	
	{
	switch ( aCommand )
        {
        case ELocSysUiCmdOpen:
        	{
			// Launch the corresponding Settings UI
			TRAPD( error, 
			       iContainer->GetFocussedSettingsUiL().
			                        LaunchLocationUIL( KDefaultParamValue,
			                                           this ));
			if ( KErrNone == error )
				{
				iSettingsUiDismissalPending = ETrue;	
				}
			else if( KErrNoMemory == error )
				{
				User::Leave( error );
				}
        	break;
        	}
        case EAknCmdHelp:
        	{
        	// Launch Help file
    		HlpLauncher::LaunchHelpApplicationL( 
    		                CEikonEnv::Static()->WsSession(),
       			            AppUi()->AppHelpContextL());
        	break;
        	}
        case EAknSoftkeyBack:
       		{
       		if( iPrevViewId.iViewUid.iUid )
       			{
	       		// Activate the Parent view
	       		AppUi()->ActivateLocalViewL( iPrevViewId.iViewUid );	       		    				
	       		
	       		// If someone is listening for the notification of the termination
	       		// event hand it over to the observer
	       		if( iObserver )
	       		    {
	       		    iObserver->LocationUIDismissed( KErrNone );
	       		    }       			
       			}
			else
				{
				AppUi()->HandleCommandL( EAknSoftkeyExit );
       		    }
            break;	
       		}
        default:
        	{
        	AppUi()->HandleCommandL( aCommand );
            break;	
        	}
        }
	}

// -----------------------------------------------------------------------------
// CLocSysUiView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
void CLocSysUiView::DynInitMenuPaneL( TInt          aResourceId, 
                                      CEikMenuPane* aMenuPane )
    {   
    if( R_LOCSYSUIVIEW_MENU_ITEM_HELP == aResourceId )
        {
        User::LeaveIfNull( aMenuPane );
        
        //Handle Help Fature
        HandleHelpFeature( *aMenuPane );
        }
    }
// ---------------------------------------------------------------------------
// void CLocSysUiView::GetCaptionL
// Method for getting caption of this plugin. This should be the 
// localized name of the settings view to be shown in parent view.

// @param aCaption pointer to Caption variable
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::GetCaptionL( TDes& aCaption ) const	
	{
	HBufC* caption = StringLoader::LoadL( R_LOCSYSUIVIEW_CAPTION );
    aCaption.Copy( *caption );
    delete caption;
	}

// ---------------------------------------------------------------------------
// Inherited from CGSPluginInterface.
// Creates a new icon of desired type. Ownership of the created icon 
// is transferred to the caller.
// 
// Icon type UIDs (use these defined constants):
// KGSIconTypeLbxItem  -   ListBox item icon.
// KGSIconTypeTab      -   Tab icon.
//  
// @param aIconType UID Icon type UID of the icon to be created.
// @return CGulIcon* Pointer of the icon. NOTE: Ownership of this
// 					icon is transferred to the caller.
// ---------------------------------------------------------------------------
//
CGulIcon* CLocSysUiView::CreateIconL( const TUid aIconType )
	{
	CGulIcon* icon;

    if ( aIconType == KGSIconTypeLbxItem )
        {
		icon = AknsUtils::CreateGulIconL(
                                    AknsUtils::SkinInstance(),
                                    KAknsIIDQgnPropSetGeneLoc,
                                    KLocationSystemUiViewIconFileName,
                                    EMbmLocsysuiviewQgn_prop_set_gene_loc,
                                    EMbmLocsysuiviewQgn_prop_set_gene_loc_mask );
        }
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
	}
         
// ---------------------------------------------------------------------------
// TInt CLocSysUiView::PluginProviderCategory()
// Inherited from CGSPluginInterface
// Method for reading the ID of the plugin provider category.
//
// @return Plugin provider category ID defined by 
//		   TGSPluginProviderCategory
// ---------------------------------------------------------------------------
//
TInt CLocSysUiView::PluginProviderCategory() const
	{
	return EGSPluginProviderOEM;
	}
        
// ---------------------------------------------------------------------------
// void CLocSysUiView::LocationUIDismissed
// Inherited from MLocationUIObserver
// Notifies the termination of the Settings UI
//
// @param aErrorCode The termination reason. KErrNone for normal 
//					terminations. In case of error or pre-mature
//					aborting System wide Error codes.
// ---------------------------------------------------------------------------
//
void CLocSysUiView::LocationUIDismissed( TInt /*aErrorCode*/ )
	{
	// Check if the Location System UI View is the active view
	// If not Activate it
	TVwsViewId activeViewId;
	if ( KErrNone != AppUi()->GetActiveViewId( activeViewId ) ||
	    activeViewId.iViewUid != Id())
		{
		TRAP_IGNORE( AppUi()->ActivateLocalViewL( Id()));
		}
	}

// --------------------------------------------------------------------------- 
// void CLocSysUiView::SessionStateChanged
// Inherited from MLocSettingsUiEcomNotifer
// Notifies any change in the state of the underlying Settings UI
// session
//
// @param aErrorCode Error Code denoting the state change
// ---------------------------------------------------------------------------
//
void CLocSysUiView::EComSessionStateChanged( TInt /*aErrorCode*/ )
	{
	if ( iContainer )
		{
		// Update the Settings UI contents
		TRAP_IGNORE( iEngine->UpdateAvailableSettingsUisL());
		
		// Obtain the latest Settings descriptions and update
		// Update the list box contents
		TRAP_IGNORE( iContainer->UpdateListBoxL( iEngine->SettingsUiArray()));
		}
	}
		 	
// ---------------------------------------------------------------------------
// void CLocSysUiView::ListBoxItemSelected
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiView::ListBoxItemSelectedL()
	{
	if ( iContainer )
		{
		// Launch the corresponding Settings UI
		TRAPD( error, 
		    iContainer->GetFocussedSettingsUiL().LaunchLocationUIL( 
		                                            KDefaultParamValue,
		                                            this ));
		if ( KErrNone == error )
			{
			iSettingsUiDismissalPending = ETrue;	
			}
		else if( KErrNoMemory == error )
			{
			User::Leave( error );
			}
		}
	}
	
// ---------------------------------------------------------------------------
// void CLocSysUiView::HandleScreenSizeChange
//
// ---------------------------------------------------------------------------
//	
void CLocSysUiView::HandleScreenSizeChange()
	{
	if (iContainer)
		{
		iContainer->SetRect( ClientRect() );		
		}
	}

// ---------------------------------------------------------------------------
// void CLocSysUiView::OpenLocalizedResourceFileL
// Opens the nearest localized resourcefile using aResourceLoader. 
// aResourceLoader should be opened only once before closing it. 
// Otherwise it will raise a panic. Therefore it is better to have 
// multiple resource loaders.
// 
// @param aResourceFileName Drive and name of resource file in format 
//                          <path>:<rsc_file_name>
// @param aResourceLoader Resource loader for the resource. The loader 
//                        must not be opened allready or it will raise 
//                        a panic. It is caller's responsibility to 
//                        close the resource loader after using the 
//                        resource.
// ---------------------------------------------------------------------------
//	
void CLocSysUiView::OpenLocalizedResourceFileL( 
                        const TDesC& 			   aResourceFileName, 
   							  RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect());
    
    // Add the RFs session to the CleanupStack
    CleanupClosePushL( fsSession );
    
	TFileName* tmpName = new(ELeave) TFileName;
    CleanupStack::PushL( tmpName );
    
    // Append the Resource Files Directory
    tmpName->Append( KDC_RESOURCE_FILES_DIR );
    
    // Append the Ressource File Name
    tmpName->Append( aResourceFileName );
   
    // Obtain the drive where the DLL is installed
    TFileName* dllDrive = new (ELeave) TFileName;
    CleanupStack::PushL( dllDrive );
    Dll::FileName( *dllDrive );
    
    // Obtain the Complete path for the Resource File
    TParse parse;
	User::LeaveIfError( parse.Set( *dllDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), tmpName, NULL ) );
	TFileName* fileName = new(ELeave) TFileName;
	CleanupStack::PushL( fileName );
	fileName->Append( parse.FullName());
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( fsSession, *fileName );
    
    // Open resource file    
    aResourceLoader.OpenL( *fileName );
    
    // Pop all the fields
    CleanupStack::PopAndDestroy( fileName );
    CleanupStack::PopAndDestroy( dllDrive );
    CleanupStack::PopAndDestroy( tmpName );
   
    CleanupStack::PopAndDestroy(); // fsSession
    }

// -----------------------------------------------------------------------------
// CLocSysUiView::HandleHelpFeature
// -----------------------------------------------------------------------------
void CLocSysUiView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ))
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, EFalse );
        }
    else
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, ETrue );
        }
    }
