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


// System Include
#include <barsread.h>               // For TResourceReader
#include <bautils.h>   
#include <StringLoader.h>
#include <aknViewAppUi.h>
#include <lbtmgmtpluginview.rsg>
#include <avkon.rsg>
#include <aknnotewrappers.h>        // Error Notes
#include <textresolver.h>           // Text Resolver
#include <hlplch.h>                 // Help
#include <featmgr.h>                // FeatureManager

// User Include
#include "locsettingsui.h"
#include "locsettingsuiobserver.h"
#include "lbtmgmtplugin.hrh"
#include "lbtmgmtpluginview.h"
#include "lbtmgmtplugincontainer.h"
#include "lbtmgmtpluginengine.h"


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLbtMgmtPluginView::CLbtMgmtPluginView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLbtMgmtPluginView::CLbtMgmtPluginView( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    :iSettingsObserver( aSettingsObsrv ),
    iResourceLoader( *CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPluginView::~CLbtMgmtPluginView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLbtMgmtPluginView::~CLbtMgmtPluginView()
    {    
    // Close the resource loader
    iResourceLoader.Close(); 
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtPluginView* CLbtMgmtPluginView::NewL
// Creates new Location Trigger Management view.
//
// @param aSettingsObsrv            Pointer to the Command Handler
// @return CLbtMgmtPluginView*   Reference to the application view
// ---------------------------------------------------------------------------
//   
CLbtMgmtPluginView* CLbtMgmtPluginView::NewL( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    {   
    CLbtMgmtPluginView* self = 
                        CLbtMgmtPluginView::NewLC( aSettingsObsrv );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPluginView* CLbtMgmtPluginView::NewLC
// Creates new Location Trigger Management view.
//
// @param aSettingsObsrv            Pointer to the Command Handler
// @return CLbtMgmtPluginView*   Reference to the application view
// ---------------------------------------------------------------------------
//  
CLbtMgmtPluginView* CLbtMgmtPluginView::NewLC( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    {   
    CLbtMgmtPluginView* self = 
                        new(ELeave) CLbtMgmtPluginView( aSettingsObsrv );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::ConstructL()
    {
        
    // Open the Resource File
    OpenLocalizedResourceFileL( KLbtMgmtViewResFileName, iResourceLoader );
    
    // Call AknView's BaseConstrutL
    BaseConstructL( R_LBTMGMTPLUGIN_VIEW );
 
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::CloseSettings
// Close the Location Triggering Management Settings
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::CloseSettings()
    {
    // Stop displaying the menu bar, if its currently being displayed
    StopDisplayingMenuBar();
    
    // Close all the active dialogs
    if( iEngine )
    		iEngine->Close();
    }
        
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::DoActivateL
// 
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::DoActivateL( const TVwsViewId&  /* PrevViewId*/,
                                               TUid         /* aCustomMessageId*/,
                                         const TDesC8&      /* aCustomMessage */)
    {
    // Destroy the existing container if it exists
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer=NULL;
        }                       
    
    // Create the Location Triggering engine instance
    iEngine = CLbtMgmtPluginEngine::NewL( *this );
    
    // Create new Container 
    iContainer = CLbtMgmtPluginContainer::NewL( ClientRect(),
                                                   *iEngine,
                                                   *this );
    iContainer->SetMopParent( this );
    AppUi()->AddToViewStackL( *this, iContainer );
    
    SetCba( R_LBTMGMTPLUGIN_CBA_CHANGE );
    }

// ---------------------------------------------------------------------------
// void CLocationSystemUiView::DoDeactivate
//
// ---------------------------------------------------------------------------
//      
void CLbtMgmtPluginView::DoDeactivate()
    {
    // Destroy Container
    AppUi()->RemoveFromViewStack( *this, iContainer );
    delete iContainer;
    iContainer = NULL;
    
    // Delete the engine
		delete iEngine;
		iEngine = NULL;
    
    }

// ---------------------------------------------------------------------------
// TUid CLbtMgmtPluginView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//  
TUid CLbtMgmtPluginView::Id() const
    {
    return TUid::Uid( KLbtMgmtPluginUID3 );
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::HandleCommandL
//
// ---------------------------------------------------------------------------
//  
void CLbtMgmtPluginView::HandleCommandL(TInt aCommand)   
    {
    switch( aCommand )
        {
        case ELbtMgmtChange:
        case ELbtMgmtMSKChange:
    		{
    		SettingsChangeL(aCommand);   		
          	break;
          	}
        case ELbtMgmtInfo:
        case ELbtMgmtMSKInfo:
        	{
			iEngine->ShowInfoDialogL();
        	break;
        	}
        case ELbtMgmtClearAll:
    		{
    		iEngine->ClearAllTriggersL();
    		iContainer->Update();
    		HandleCmdL(EListBoxItemFocused);
    		break;
          	}
        case EAknCmdHelp:        
            {
            HlpLauncher::LaunchHelpApplicationL( iCoeEnv->WsSession(),
                                                 AppUi()->AppHelpContextL());
            break;  
          	}           
        // Handle any plug-in specific internal commands here
        case EAknSoftkeyBack:
            {
            // The UI is intended to be closed. Pass the event 
            // to the observer
            iSettingsObserver.SettingClosed( KErrNone );
            break;
            }
        default:
            {
            // Un-handled commands are to be passed to the 
            // App UI
            AppUi()->HandleCommandL( aCommand );
            break;  
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CLbtMgmtPluginView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CLbtMgmtPluginView::DynInitMenuPaneL( TInt          aResourceId, 
                                              CEikMenuPane* aMenuPane )
    {
    if ( aResourceId == R_LBTMGMTPLUGIN_MENU )
        {
        User::LeaveIfNull( aMenuPane );
        //Handle Help Feature

        //Context sensitive menu items
        if( CLbtMgmtPluginEngine::EActiveTriggers == iContainer->GetFocussedItem() )
        	{
        	aMenuPane->SetItemDimmed( ELbtMgmtChange, ETrue );
        	if( iEngine->ActiveTriggers() )
        	    {
        	    aMenuPane->SetItemDimmed( ELbtMgmtClearAll, EFalse );  			            
        	    aMenuPane->SetItemDimmed( ELbtMgmtInfo, EFalse );
        	    }
        	else
        	    {
        	    aMenuPane->SetItemDimmed( ELbtMgmtClearAll, ETrue );                          
        	     aMenuPane->SetItemDimmed( ELbtMgmtInfo, ETrue );
        	    }
        	}
        else
        	{
			aMenuPane->SetItemDimmed( ELbtMgmtChange, EFalse );  
			 aMenuPane->SetItemDimmed( ELbtMgmtClearAll, ETrue ); 
			aMenuPane->SetItemDimmed( ELbtMgmtInfo, ETrue );
        	}
        }
    }   

// -----------------------------------------------------------------------------
// CLbtMgmtPluginView::SetCba
// -----------------------------------------------------------------------------
//
void CLbtMgmtPluginView::SetCba( TInt aResourceId ) 
		{
		if( Cba() )
		    {
		   	TRAP_IGNORE(Cba()->SetCommandSetL(aResourceId));
		    }
	  Cba()->DrawDeferred(); 
		}

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::HandleCommandL
// Inherited from MLbtMgmtPluginCmdHdlr
// Handles Selection events from the container
// 
// @param  aCommand Command ID
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::HandleCmdL( TLbtMgmtPluginCmd aCommand )
    {
    switch( aCommand )
        {
        // Handle any plug-in specific internal commands here
        case EListBoxItemSelected:
	          	{
	          	// Handle List Box Selected Event   
	            TRAPD( error, SettingsChangeL( ELbtMgmtMSKChange ));
	            if( error )
	                {
	                // Show Error Note
	    			CTextResolver* iTextResolver = CTextResolver::NewLC(*iCoeEnv);
	                TPtrC buffer = iTextResolver->ResolveErrorString( error );
	                CAknErrorNote* errorNote = new ( ELeave )CAknErrorNote( ETrue );
	                TRAP_IGNORE( errorNote->ExecuteLD( buffer ));
	    			CleanupStack::PopAndDestroy( iTextResolver );
	                }    
				break;    
	          	}
        case EListBoxItemFocused:
        		{
	        	if( CLbtMgmtPluginEngine::EActiveTriggers == iContainer->GetFocussedItem()   )
	        	    {
	        	    if( iEngine->ActiveTriggers() )
	        			SetCba( R_LBTMGMTPLUGIN_CBA_INFO );
	        	    else
	        	        SetCba( R_LBTMGMTPLUGIN_CBA_NOACTIVETRIGGERS );
	        	    }
	        	else
	        			SetCba( R_LBTMGMTPLUGIN_CBA_CHANGE );
	        	break;
	        	}
        case EScreenSizeChanged:
	          {
            if( iContainer )
                {
                iContainer->SetRect( ClientRect());     
                }
	          break;
	          }
        default:
            {
            break;  
            }
        }
    }

// ---------------------------------------------------------------------------
// Inherited from MLbtMgmtEngObserver
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::HandleSettingsChangeEventL()
    {
    // Observer callback notifies that one/more of the settings values have
    // changed. Hence, need to refresh the container to show the latest
    // values
    if( iContainer )
        {
        iContainer->DrawDeferred();
        }
    
    // Handle any changes to the MSK configuration. 
    HandleCmdL( EListBoxItemFocused );
    }
    
// -----------------------------------------------------------------------------
// CLbtMgmtPluginView::HandleHelpFeature
// -----------------------------------------------------------------------------
//
void CLbtMgmtPluginView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
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
    

// ---------------------------------------------------------------------------
// Handle Settings Change Event
// 
// @param aCmd Settings change command
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginView::SettingsChangeL( TInt aCmd )
    {
    if( !iContainer )
        {
        // No container available. Nothing to do in this case. Hence, returning
        return;
        }
    CLbtMgmtPluginEngine::TSettingsList currentElement;
    currentElement = static_cast<CLbtMgmtPluginEngine::TSettingsList>
                        ( iContainer->GetFocussedItem() );
    
    // Switch based on the currently focussed element
    switch( currentElement )
        {
        case CLbtMgmtPluginEngine::ETriggeringStatus:
            {
            // Different handling needed for Touch handling and Item selection
            // using the OK key.
            // Incase the User has selected the item using the OK key then 
            // normal handling required
            // Incase the User has selected the item using touch selection
            // then the Trigger Status needs to be toggled
            if( ELbtMgmtChange == aCmd )
                {
                iEngine->ChangeTiggerStatusL();
                }
            else
                {
                iEngine->ToggleTiggerStatusL();
                }
            iContainer->Update();
            break;
            }
        case CLbtMgmtPluginEngine::EActiveTriggers:
            {
			// If condition is added for any pointer event from container
            if( iEngine->ActiveTriggers() )
            		iEngine->ShowInfoDialogL();
            break;
            }
        default:
            {
            // Element not found
            User::Leave( KErrNotFound );
            }
        }
    }
                    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginView::OpenLocalizedResourceFileL
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
void CLbtMgmtPluginView::OpenLocalizedResourceFileL( 
                            const TDesC&               aResourceFileName, 
                                  RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect());
    
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
    fileName->Append( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( fsSession, *fileName );
    
    // Open resource file    
    aResourceLoader.OpenL( *fileName );
    
    // Pop all the fields
    CleanupStack::PopAndDestroy( fileName );
    CleanupStack::PopAndDestroy( dllDrive );
    CleanupStack::PopAndDestroy( tmpName );
    
    //If leave occurs before this, close is called automatically when the thread exits.
    fsSession.Close();
    }
