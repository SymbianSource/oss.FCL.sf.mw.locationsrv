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


// System Include
#include <barsread.h>               // For TResourceReader
#include <bautils.h>    
#include <StringLoader.h>
#include <aknViewAppUi.h>
#include <locnotprefpluginview.rsg>
#include <avkon.rsg>
#include <aknnotewrappers.h>        // Error Notes
#include <textresolver.h>           // Text Resolver
#include <hlplch.h>                 // Help
#include <featmgr.h>                // FeatureManager

// User Include
#include "locsettingsuiobserver.h"
#include "locnotprefpluginview.h"
#include "locnotprefplugincontainer.h"
#include "locsettingsui.h"
#include "locnotprefplugincmdhdlr.h"
#include "locnotprefpluginengine.h"
#include "locnotprefplugin.hrh"


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocNotPrefPluginView::CLocNotPrefPluginView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLocNotPrefPluginView::CLocNotPrefPluginView( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    :iSettingsObserver(aSettingsObsrv),
    iResourceLoader( *CCoeEnv::Static())
    {
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPluginView::~CLocNotPrefPluginView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocNotPrefPluginView::~CLocNotPrefPluginView()
    {
    // Delete the engine
    delete iEngine;
    
    // Delete the Error handling utilities
    delete iTextResolver;
        
    // Close the resource loader
    iResourceLoader.Close();
    
    FeatureManager::UnInitializeLib();  
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefPluginView* CLocNotPrefPluginView::NewL
// Creates new Location notation preferences view.
//
// @param aSettingsObsrv            Pointer to the Command Handler
// @return CLocNotPrefPluginView*   Reference to the application view
// ---------------------------------------------------------------------------
//   
CLocNotPrefPluginView* CLocNotPrefPluginView::NewL( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    {   
    CLocNotPrefPluginView* self = 
                        CLocNotPrefPluginView::NewLC( aSettingsObsrv );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPluginView* CLocNotPrefPluginView::NewLC
// Creates new Location notation preferences view.
//
// @param aSettingsObsrv            Pointer to the Command Handler
// @return CLocNotPrefPluginView*   Reference to the application view
// ---------------------------------------------------------------------------
//  
CLocNotPrefPluginView* CLocNotPrefPluginView::NewLC( 
                            MLocSettingsUiObserver& aSettingsObsrv )
    {   
    CLocNotPrefPluginView* self = 
                        new(ELeave) CLocNotPrefPluginView( aSettingsObsrv );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginView::ConstructL()
    {
    // Feature manager required for Help feature check
    FeatureManager::InitializeLibL();
        
    // Open the Resource File
    OpenLocalizedResourceFileL( KLocNotPrefViewResFileName, iResourceLoader );
    
    // Call AknView's BaseConstrutL
    BaseConstructL( R_LOCNOTPREFPLUGIN_VIEW );
    
    // Create the Location notation preferences engine instance
    iEngine = CLocNotPrefPluginEngine::NewL( *this );
     
    iTextResolver = CTextResolver::NewL(*( CCoeEnv::Static()));  
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginView::CloseSettings
// Close the Notation preferences Settings
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginView::CloseSettings()
    {
    // Stop displaying the menu bar, if its currently being displayed
    StopDisplayingMenuBar();
    
    // Close all the active dialogs
    iEngine->Close();
    }
        
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginView::DoActivateL
// 
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginView::DoActivateL( const TVwsViewId&  /* PrevViewId*/,
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
    
    // Create new Container 
    iContainer = CLocNotPrefPluginContainer::NewL( ClientRect(),
                                                   *iEngine,
                                                   *this );
    iContainer->SetMopParent( this );
    AppUi()->AddToViewStackL( *this, iContainer );
    
    }

// ---------------------------------------------------------------------------
// void CLocationSystemUiView::DoDeactivate
//
// ---------------------------------------------------------------------------
//      
void CLocNotPrefPluginView::DoDeactivate()
    {
    // Destroy Container
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// ---------------------------------------------------------------------------
// TUid CLocNotPrefPluginView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//  
TUid CLocNotPrefPluginView::Id() const
    {
    return TUid::Uid( KLocNotPrefPluginUID3);
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginView::HandleCommandL
//
// ---------------------------------------------------------------------------
//  
void CLocNotPrefPluginView::HandleCommandL(TInt aCommand)   
    {
    switch( aCommand )
        {
        case ELocNotPrefChange:
        case ELocNotPrefMSKChange:
            {
            // Settings Change command
            TRAPD( error, SettingsChangeL( aCommand ));
            if( error )
                {
                // Show Error Note
                TPtrC buffer = iTextResolver->ResolveErrorString( error );
                CAknErrorNote* errorNote = new ( ELeave )CAknErrorNote;
                TRAP_IGNORE( errorNote->ExecuteLD( buffer ));
                }
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
// CLocNotPrefPluginView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CLocNotPrefPluginView::DynInitMenuPaneL( TInt          aResourceId, 
                                              CEikMenuPane* aMenuPane )
    {
    if ( aResourceId == R_LOCNOTPREFPLUGIN_MENU )
        {
        User::LeaveIfNull( aMenuPane );
        //Handle Help Fature
        HandleHelpFeature( *aMenuPane );
        //Context sensitive menu items
        }
    }   

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::HandleCommandL
// Inherited from MLocNotPrefPluginCmdHdlr
// Handles Selection events from the container
// 
// @param  aCommand Command ID
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginView::HandleCmdL( TLocNotPrefPluginCmd aCommand )
    {
    switch( aCommand )
        {
        // Handle any plug-in specific internal commands here
        case EListBoxItemSelected:
            {
            TRAPD( error, SettingsChangeL( ELocNotPrefMSKChange ));
            if( error )
                {
                // Show Error Note
                TPtrC buffer = iTextResolver->ResolveErrorString( error );
                CAknErrorNote* errorNote = new ( ELeave )CAknErrorNote;
                TRAP_IGNORE( errorNote->ExecuteLD( buffer ));
                }           
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
// Inherited from MLocNotPrefEngObserver
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginView::HandleSettingsChangeEventL()
    {
    // Observer callback notifies that one/more of the settings values have
    // changed. Hence, need to refresh the container to show the latest
    // values
    if( iContainer )
        {
        iContainer->DrawDeferred();
        }
    }
    
// -----------------------------------------------------------------------------
// CLocNotPrefPluginView::HandleHelpFeature
// -----------------------------------------------------------------------------
//
void CLocNotPrefPluginView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
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
void CLocNotPrefPluginView::SettingsChangeL( TInt aCmd )
    {
    if( !iContainer )
        {
        // No container available. Nothing to do in this case. Hence, returning
        return;
        }
    CLocNotPrefPluginEngine::TSettingsList currentElement;
    currentElement = static_cast<CLocNotPrefPluginEngine::TSettingsList>
                        ( iContainer->GetFocussedItem());
    
    // Switch based on the currently focussed element
    switch( currentElement )
        {
        case CLocNotPrefPluginEngine::ESystemofMeasurement:
            {
            // Different handling needed for Touch handling and Item selection
            // using the OK key.
            // Incase the User has selected the item using the OK key then 
            // normal handling required
            // Incase the User has selected the item using touch selection
            // then the system of measurement needs to be toggled
            if( ELocNotPrefChange == aCmd )
                {
                iEngine->ChangeSysofMeasSettingsL();
                }
            else
                {
                iEngine->ToggleSysofMeasSettingsL();
                }              
            break;
            }
        case CLocNotPrefPluginEngine::ECoordinateDisplayFormat:
            {
            // Change Co-ordinate display format settings
            iEngine->ChangeCoordinateDisplaySettingsL();
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
// void CLocNotPrefPluginView::OpenLocalizedResourceFileL
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
void CLocNotPrefPluginView::OpenLocalizedResourceFileL( 
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
    fileName->Append( parse.FullName());
    
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
