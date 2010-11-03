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
* Description:  Location Triggering Management Plugin View's container
*
*/


// System Include
#include <aknViewAppUi.h>
#include <StringLoader.h>           // For String Loader
#include <lbtmgmtpluginview.rsg>
#include <barsread.h>               // For TResourceReader
#include <bautils.h>

// User Includes
#include "locsettingsuiobserver.h"
#include "locsettingsuiparams.h"
#include "lbtmgmtplugin.h"
#include "lbtmgmtpluginview.h"

// Constant Definition
/**
 * Position of Triggering Management plug-in the Postioning View.
 * As defined by the Location System UI UI specification, the Location
 * Triggering Management plug-in should be inserted at the Third level
 */
const TInt KLbtMgmtPluginPriority = 0x00000003;

// ---------------------------------------------------------------------------
// CLbtMgmtPlugin::CLbtMgmtPlugin
// C++ default constructor.
// ---------------------------------------------------------------------------
CLbtMgmtPlugin::CLbtMgmtPlugin( MLocSettingsUiObserver&   aSettingsUiObserver,
                                      CAknViewAppUi&            aAppUi )
    :CLocSettingsUi( aSettingsUiObserver, aAppUi ),
    iResourceLoader( *CCoeEnv::Static() )
    {
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPlugin::~CLbtMgmtPlugin
// Destructor.
// ---------------------------------------------------------------------------  
CLbtMgmtPlugin::~CLbtMgmtPlugin() 
    {
    iResourceLoader.Close();
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPlugin* CLbtMgmtPlugin::NewL
// Symbian OS two-phased constructor
// 
// @return CLbtMgmtPlugin* Reference to the Settings UI object
// ---------------------------------------------------------------------------  
CLbtMgmtPlugin* CLbtMgmtPlugin::NewL( TAny* aParam )
    {
    CLocSettingsUiParams*   param = reinterpret_cast<CLocSettingsUiParams *>( aParam );
    MLocSettingsUiObserver& observer = param->SettingsUiObserver();
    CAknViewAppUi&          appUi = param->AppUi();
    CLbtMgmtPlugin* self = new (ELeave )CLbtMgmtPlugin( observer,
                                                              appUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self; 
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPlugin::ConstructL
// Second phase constructor.
// ---------------------------------------------------------------------------  
void CLbtMgmtPlugin::ConstructL()
    {
    // Open the Resource file for the plugin. The resource file is not 
    // explicity opened by system. Hence, the opening and closind if tied
    // down to the time when the DLL is active
    OpenLocalizedResourceFileL( KLbtMgmtUiResFileName, 
                                iResourceLoader );
                                    
    // Creating an Application view
    iView = CLbtMgmtPluginView::NewL( iSettingsUiObserver );
    
    // Transferring the view's control to AppUi
    iAppUi.AddViewL( iView );
    
    // Obtaining the View Id for reference
    iViewId = iView->Id();
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPlugin::LaunchL
// Inherited from CSettingsUi
// Launches the Settings UI
//
// @param aUiLaunchParams   Flags for Launch options
// ---------------------------------------------------------------------------  
void CLbtMgmtPlugin::LaunchL( TInt       /* aUiLaunchParams */)
    {
    // Activate self
    iAppUi.ActivateLocalViewL( iViewId );
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPlugin::Close
// Inherited from CSettingsUi
// Closes the running Settings UI
// ---------------------------------------------------------------------------  
void CLbtMgmtPlugin::Close()
    {
    iView->CloseSettings();
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPlugin::GetCaptionL
// Inherited from CSettingsUi
// Method for getting caption of this plugin. This should be the 
// localized name of the settings view to be shown in parent view.
//
// @param aCaption pointer to Caption variable
// ---------------------------------------------------------------------------
void CLbtMgmtPlugin::GetCaptionL( TDes& aCaption ) const
    {
    // Load the Location Triggering setting caption from the resource file 
    // and hand it back to the callee. It is assuemed here that the calling function has 
    // allocated enough memory for the aCaption string. If not the fucntion
    // would leave
    HBufC* caption = StringLoader::LoadL( R_LOCTR_TITLE_TEXT_LT );
    
    if( aCaption.MaxLength() < caption->Length())
        {
        User::Leave( KErrNoMemory );
        }
    aCaption.Copy( *caption );
    delete caption; 
    }

// ---------------------------------------------------------------------------
// TInt CLbtMgmtPlugin::GetPriority
// Inherited from CSettingsUi
// Returns the priority of the UI module. 
// 
// @return Priority of the Settings UI
// ---------------------------------------------------------------------------
TInt CLbtMgmtPlugin::GetPriority()
    {
    return KLbtMgmtPluginPriority;
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPlugin::OpenLocalizedResourceFileL
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
void CLbtMgmtPlugin::OpenLocalizedResourceFileL( 
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
    fileName->Append( parse.FullName() );
    
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


