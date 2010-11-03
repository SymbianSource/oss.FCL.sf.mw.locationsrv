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
* Description:  Location Notation Preferences Plugin View's container
*
*/


// System Include
#include <aknViewAppUi.h>
#include <StringLoader.h>           // For String Loader
#include <locnotprefpluginview.rsg>
#include <barsread.h>               // For TResourceReader
#include <bautils.h>

// User Includes
#include "locnotprefplugin.h"
#include "locsettingsuiobserver.h"
#include "locsettingsuiparams.h"
#include "locnotprefpluginview.h"

// Constant Definition
/**
 * Position of Notation Preferences plug-in the Postioning View.
 * As defined by the Location System UI UI specification, the Location
 *  notation preferences plug-in should be inserted at the fourth level
 */
const TInt KNotPrefPluginPriority = 0x00000004;

// ---------------------------------------------------------------------------
// CLocNotPrefPlugin::CLocNotPrefPlugin
// C++ default constructor.
// ---------------------------------------------------------------------------
CLocNotPrefPlugin::CLocNotPrefPlugin( MLocSettingsUiObserver&   aSettingsUiObserver,
                                      CAknViewAppUi&            aAppUi )
    :CLocSettingsUi( aSettingsUiObserver, aAppUi ),
    iResourceLoader( *CCoeEnv::Static())
    {
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPlugin::~CLocNotPrefPlugin
// Destructor.
// ---------------------------------------------------------------------------  
CLocNotPrefPlugin::~CLocNotPrefPlugin() 
    {
    iResourceLoader.Close();
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPlugin* CLocNotPrefPlugin::NewL
// Symbian OS two-phased constructor
// 
// @return CLocNotPrefPlugin* Reference to the Settings UI object
// ---------------------------------------------------------------------------  
CLocNotPrefPlugin* CLocNotPrefPlugin::NewL( TAny* aParam )
    {
    CLocSettingsUiParams*   param = reinterpret_cast<CLocSettingsUiParams *>( aParam );
    MLocSettingsUiObserver& observer = param->SettingsUiObserver();
    CAknViewAppUi&          appUi = param->AppUi();
    CLocNotPrefPlugin* self = new (ELeave )CLocNotPrefPlugin( observer,
                                                              appUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self; 
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::ConstructL
// Second phase constructor.
// ---------------------------------------------------------------------------  
void CLocNotPrefPlugin::ConstructL()
    {
    // Open the Resource file for the plugin. The resource file is not 
    // explicity opened by system. Hence, the opening and closind if tied
    // down to the time when the DLL is active
    OpenLocalizedResourceFileL( KLocNotPrefUiResFileName, 
                                iResourceLoader );
                                    
    // Creating an Application view
    iView = CLocNotPrefPluginView::NewL( iSettingsUiObserver );
    
    // Transferring the view's control to AppUi
    iAppUi.AddViewL( iView );
    
    // Obtaining the View Id for reference
    iViewId = iView->Id();
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::LaunchL
// Inherited from CSettingsUi
// Launches the Settings UI
//
// @param aUiLaunchParams   Flags for Launch options
// ---------------------------------------------------------------------------  
void CLocNotPrefPlugin::LaunchL( TInt       /* aUiLaunchParams */)
    {
    // Activate self
    iAppUi.ActivateLocalViewL( iViewId );
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::Close
// Inherited from CSettingsUi
// Closes the running Settings UI
// ---------------------------------------------------------------------------  
void CLocNotPrefPlugin::Close()
    {
    iView->CloseSettings();
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::GetCaptionL
// Inherited from CSettingsUi
// Method for getting caption of this plugin. This should be the 
// localized name of the settings view to be shown in parent view.
//
// @param aCaption pointer to Caption variable
// ---------------------------------------------------------------------------
void CLocNotPrefPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // Load the Notation preferences setting caption from the resource file 
    // and hand it back to the callee. It is assuemed here that the calling function has 
    // allocated enough memory for the aCaption string. If not the fucntion
    // would leave
    HBufC* caption = StringLoader::LoadL( R_LOC_SETTINGS_NOTATIONS );
    
    if( aCaption.MaxLength() < caption->Length())
        {
        User::Leave( KErrNoMemory );
        }
    aCaption.Copy( *caption );
    delete caption; 
    }

// ---------------------------------------------------------------------------
// TInt CLocNotPrefPlugin::GetPriority
// Inherited from CSettingsUi
// Returns the priority of the UI module. 
// 
// @return Priority of the Settings UI
// ---------------------------------------------------------------------------
TInt CLocNotPrefPlugin::GetPriority()
    {
    return KNotPrefPluginPriority;
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPlugin::OpenLocalizedResourceFileL
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
void CLocNotPrefPlugin::OpenLocalizedResourceFileL( 
                                const TDesC&               aResourceFileName, 
                                      RConeResourceLoader& aResourceLoader )
    {
    RFs fsSession;
    User::LeaveIfError( fsSession.Connect());
    
    // Add the RFs session to the CleanupStack
    CleanupClosePushL( fsSession );
    
    TFileName* tmpName = new( ELeave ) TFileName;
    CleanupStack::PushL( tmpName );
    
    // Append the Resource Files Directory
    tmpName->Append( KDC_RESOURCE_FILES_DIR );
    
    // Append the Ressource File Name
    tmpName->Append( aResourceFileName );
    
    
    // Obtain the drive where the DLL is installed
    TFileName* dllDrive = new ( ELeave ) TFileName;
    CleanupStack::PushL( dllDrive );
    Dll::FileName( *dllDrive );
    
    // Obtain the Complete path for the Resource File
    TParse parse;
    User::LeaveIfError( parse.Set( *dllDrive, NULL, NULL ));
    User::LeaveIfError( parse.Set( parse.Drive(), tmpName, NULL ));
    TFileName* fileName = new ( ELeave ) TFileName;
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
   
    CleanupStack::Pop(); // fsSession
               
    //If leave occurs before this, close is called automatically when the thread exits.
    fsSession.Close();
   
    }


