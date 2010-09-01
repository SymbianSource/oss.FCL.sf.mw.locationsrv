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


// System Include
#include <aknViewAppUi.h>
#include <barsread.h> 		// For TResourceReader
#include <bautils.h>
#include <StringLoader.h>
#include <locpsysettingsuiview.rsg>
#include  <eikmenub.h>						// Menu Bar


// User Includes
#include "locpsysettingsui.h"
#include "locsettingsuiobserver.h"
#include "locsettingsuiparams.h"
#include "locpsysettingsview.h"


// ---------------------------------------------------------------------------
// CLocPsySettingsUi::CLocPsySettingsUi
// C++ default constructor.
// ---------------------------------------------------------------------------
CLocPsySettingsUi::CLocPsySettingsUi( MLocSettingsUiObserver&	aSettingsUiObserver,
					 		 	 	  CAknViewAppUi&			aAppUi )
	:CLocSettingsUi( aSettingsUiObserver, aAppUi ),
	iResourceLoader( *CCoeEnv::Static())
	{
	}

// ---------------------------------------------------------------------------
// CLocPsySettingsUi::~CLocPsySettingsUi
// Destructor.
// ---------------------------------------------------------------------------	
CLocPsySettingsUi::~CLocPsySettingsUi()	
	{
	// Close the Resource loader
	iResourceLoader.Close();   
	}

// ---------------------------------------------------------------------------
// CLocPsySettingsUi* CLocPsySettingsUi::NewL
// Symbian OS two-phased constructor
// 
// @return CLocPsySettingsUi* Reference to the Settings UI object
// ---------------------------------------------------------------------------	
CLocPsySettingsUi* CLocPsySettingsUi::NewL( TAny* aParams )
	{
	CLocSettingsUiParams* 	param = reinterpret_cast<CLocSettingsUiParams *>(aParams);
	MLocSettingsUiObserver& observer = param->SettingsUiObserver();
	CAknViewAppUi& 		 appUi = param->AppUi();
	CLocPsySettingsUi* self = new ( ELeave )CLocPsySettingsUi( observer,
										  		   			   appUi );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self; 
	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::ConstructL
// Second phase constructor.
// ---------------------------------------------------------------------------	
void CLocPsySettingsUi::ConstructL()
	{
    
	// Open the Resource file for the plugin
	OpenLocalizedResourceFileL( KPosSettingsUiResFileName, 
							    iResourceLoader );
							    
	// Create the Position Method Settings UI View
	iView = CLocPsySettingsView::NewL( *this );
	
	// Transferring the view's control to AppUi
	iAppUi.AddViewL( iView );
	
	// Obtaining the View Id for reference
	iViewId = iView->Id();

	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::LaunchL
// Inherited from CLocSettingsUi
// Launches the Settings UI
//
// @param aUiLaunchParams	Flags that define specific configuration of
//							if the Settings UI, that the user of the API
//							wants to launch.
// ---------------------------------------------------------------------------	
void CLocPsySettingsUi::LaunchL( TInt		/* aUiLaunchParams */)
	{        
	// Activate self
	iAppUi.ActivateLocalViewL( iViewId );
	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::Close
// Inherited from CLocSettingsUi
// Closes the running Settings UI prematurely.
// ---------------------------------------------------------------------------	
void CLocPsySettingsUi::Close()
	{
	// Close the Menubar if it's being displayed
	iView->StopDisplayingMenuBar();
	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::GetCaptionL
// Inherited from CLocSettingsUi
// Method for getting caption of this plugin. This should be the 
// localized name of the settings view to be shown in parent view.
//
// @param aCaption pointer to Caption variable
// ---------------------------------------------------------------------------
void CLocPsySettingsUi::GetCaptionL( TDes& aCaption ) const
	{
	HBufC* caption = StringLoader::LoadL( R_POSSETTINGSUI_CAPTION );
    aCaption.Copy( *caption );
    delete caption;
	}

// ---------------------------------------------------------------------------
// TInt CLocPsySettingsUi::GetPriority
// Inherited from CLocSettingsUi
// Returns the priority of the UI module. 
// 
// @return Priority of the Settings UI
// ---------------------------------------------------------------------------
TInt CLocPsySettingsUi::GetPriority()
	{
	// As defined by the Location System UI UI specification, the PSY settings
	// plug-in should be inserted at the first level
	const TInt KPsyPluginPriority = 0x00000001;
	return KPsyPluginPriority;
	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::HandleCommandL
// Inherited from MCommandHandler
// Handles commands from the views
// 
// @param  aCommand Command ID
// ---------------------------------------------------------------------------
void CLocPsySettingsUi::HandleCommandL( TInt aCommand )
	{
	switch ( aCommand )
		{
		// Handle any plug-in specific internal commands here
		case EAknSoftkeyBack:
			{
			// The UI is intended to be closed. Pass the event 
			// to the observer
			iSettingsUiObserver.SettingClosed( KErrNone );
			break;
			}
		default:
			{
			// Un-handled commands are to be passed to the 
			// App UI
			iAppUi.HandleCommandL( aCommand );
			break;	
			}
		}
	}

// ---------------------------------------------------------------------------
// void CLocPsySettingsUi::OpenLocalizedResourceFileL
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
void CLocPsySettingsUi::OpenLocalizedResourceFileL( const TDesC& 			   aResourceFileName, 
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
    
    //If leave occurs before this, close is called automatically
    // from by the CleanupStack  
    CleanupStack::PopAndDestroy(); // fsSession
    }
