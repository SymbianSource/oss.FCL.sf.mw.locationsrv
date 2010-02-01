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
* Description:  SUPL Settings UI implementation.
*
*/


// System Include
#include <aknViewAppUi.h>
#include <barsread.h> 		        // For TResourceReader
#include <bautils.h>    
#include <AknQueryDialog.h>         // For list query dialog
#include <StringLoader.h>
#include <locsuplsettingsui.rsg>
#include <featmgr.h> 						// FeatureManager

// User Includes
#include "locsuplsettings.hrh"
#include "locsuplsettings.h"
#include "locsettingsuiobserver.h"
#include "locsuplsettingsview.h"
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingslbmodel.h"
#include "locsettingsuiclient.h"   // For the Launch parameter values

#include "locsuplserverview.h"
#include "locsuplserverlbmodel.h"
#include "locsuplsessionview.h"
#include "locsuplsessionlbmodel.h"
#include "locsupldebug.h"


CLocSUPLAO* CLocSUPLAO::NewL( 
        MLocSettingsUiObserver&     aSettingsUiObserver,
        CLocSUPLSettingsUiEngine&   aSettingsEngine,
        TInt                        aLaunchParams  )
    {
    CLocSUPLAO* self = new (ELeave) CLocSUPLAO( aSettingsUiObserver,
                                                aSettingsEngine,
                                                aLaunchParams );
    return self;
    }

CLocSUPLAO::~CLocSUPLAO()
    {
    DoCancel();
    }

CLocSUPLAO::CLocSUPLAO( MLocSettingsUiObserver&     aSettingsUiObserver,
                        CLocSUPLSettingsUiEngine&   aSettingsEngine,
                        TInt                        aLaunchParams  )
: CActive( EPriorityStandard ),
 iSettingsUiObserver( aSettingsUiObserver ),
 iSettingsEngine( aSettingsEngine ),
 iLaunchParams( aLaunchParams )
    {
    CActiveScheduler::Add(this);
    }
        
void CLocSUPLAO::RunL()
    {
    iSettingsEngine.OpenSessionL( iLaunchParams );
    iSettingsUiObserver.SettingClosed( KErrNone );
    }
        
void CLocSUPLAO::DoCancel()
    {
    }

void CLocSUPLAO::CompleteRequest()
    {
    if( !IsActive())
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        SetActive();
        }
    }
// ---------------------------------------------------------------------------
// CLocSUPLSettings::CLocSUPLSettings
// C++ default constructor.
// ---------------------------------------------------------------------------
//
CLocSUPLSettings::CLocSUPLSettings( MLocSettingsUiObserver&	aSettingsUiObserver,
					   		    	CAknViewAppUi&			aAppUi )
	: CLocSettingsUi( aSettingsUiObserver, aAppUi ),
	iResourceLoader( *CCoeEnv::Static() ),
	iLaunchOutStanding( EFalse )
	{
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettings::~CLocSUPLSettings
// Destructor.
// ---------------------------------------------------------------------------	
//
CLocSUPLSettings::~CLocSUPLSettings()	
	{
	DEBUG( + CLocSUPLSettings::~CLocSUPLSettings );
	
	delete iLaunchAO;
	iLaunchAO = NULL;
	
	delete iSettingsEngine;
	iSettingsEngine = NULL;
	
	// Close the Resource loader
	iResourceLoader.Close();

    FeatureManager::UnInitializeLib();  		
	DEBUG( - CLocSUPLSettings::~CLocSUPLSettings );
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettings* CLocSUPLSettings::NewL
// Symbian OS two-phased constructor
// 
// @return CLocSUPLSettings* Reference to the Settings UI object
// ---------------------------------------------------------------------------	
//
CLocSUPLSettings* CLocSUPLSettings::NewL( TAny* aParam )
	{
	DEBUG( + CLocSUPLSettings::NewL );
	CLocSettingsUiParams* 	param = 
	                reinterpret_cast<CLocSettingsUiParams *>( aParam );
	MLocSettingsUiObserver& observer = param->SettingsUiObserver();
	CAknViewAppUi& 		 appUi = param->AppUi();
	CLocSUPLSettings* self = new ( ELeave )CLocSUPLSettings( observer,
													         appUi );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettings::NewL );
	return self; 
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::ConstructL
// Second phase constructor.
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettings::ConstructL()
	{
	DEBUG( + CLocSUPLSettings::ConstructL );
	// Feature manager required for SUPL Feature flag checking and 
	// Help feature check
    FeatureManager::InitializeLibL();

 	// Feature flag check provided for Product profile bits specific variation
 	// If the SUPL feature flag is not defined then the SUPL Settings UI
 	// would not be supported
 	if ( !FeatureManager::FeatureSupported( KFeatureIdSuplFramework ) )
        {
        User::Leave( KErrNotSupported );
        }
       
	// Open the Resource file for the plugin. The resource file is not 
	// explicity opened by system. Hence, the opening and closind if tied
	// down to the time when the DLL is active
	OpenLocalizedResourceFileL( KSUPLUiResFileName, 
							    iResourceLoader );
	
	// Create the SUPL Settings UI Engine
	iSettingsEngine = CLocSUPLSettingsUiEngine::NewL( *this );
	
    // Create the SUPL Settings UI View
	iSettingsView = CLocSUPLSettingsView::NewL( *iSettingsEngine,
	                                    *this );
	// Transferring the view's control to AppUi. The settings Ui no longer
	// has any control to the view handle
	iAppUi.AddViewL( iSettingsView );
	
	// Obtaining the View Id for reference
	iSettingsViewId = iSettingsView->Id();
							    
    // Create the SUPL Settings UI View
	iServerView = CLocSUPLServerView::NewL( *iSettingsEngine,
	                                    *this );
	
	
	// Transferring the view's control to AppUi. The settings Ui no longer
	// has any control to the view handle
	iAppUi.AddViewL( iServerView );
	
	// Obtaining the View Id for reference
	iServerViewId = iServerView->Id();
	
	// Create the SUPL Session UI View
	iSessionView = CLocSUPLSessionView::NewL( *iSettingsEngine,
	                                    *this );
	
	
	// Transferring the view's control to AppUi. The sessions Ui no longer
	// has any control to the view handle
	iAppUi.AddViewL( iSessionView );
	
	// Obtaining the View Id for reference
	iSessionViewId = iSessionView->Id();
	
	DEBUG( - CLocSUPLSettings::ConstructL );
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::LaunchL
// Inherited from CSettingsUi
// Launches the Settings UI
//
// @param aUiLaunchParams	Flags that define specific configuration of
//							if the Settings UI, that the user of the API
//							wants to launch. If more than one functionality
//                          is requested then the various combinations
//                          are to be ORed.
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettings::LaunchL( TInt		aUiLaunchParams )
	{
	DEBUG( + CLocSUPLSettings::LaunchL );
	// If there is any outstanding request on this UI then leave immediately
	if( iLaunchOutStanding )
	    {
	    User::Leave( KErrInUse );
	    }
    
    iLaunchOutStanding = ETrue;
    
    // Set the Param type
    iLaunchParamType = ELaunchParamInt;
    
    iLaunchParams = aUiLaunchParams;
    
    // Initalize the Engine. Only after the initialization is complete can we 
    // modify the Setting contents. Hence, the actual launching would be 
    // deferred to the initialization period.
    iSettingsEngine->Initalize();
	DEBUG( - CLocSUPLSettings::LaunchL );
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::LaunchL
// Inherited from CSettingsUi
// Launches the Settings UI
//
// @param aUiLaunchParams   Flags that define specific configuration of
//                          if the Settings UI, that the user of the API
//                          wants to launch. If more than one functionality
//                          is requested then the various combinations
//                          are to be ORed.
// ---------------------------------------------------------------------------  
//
void CLocSUPLSettings::LaunchL( const TDesC&      aUiLaunchParams )
    {
    DEBUG( + CLocSUPLSettings::LaunchL );
    // If there is any outstanding request on this UI then leave immediately
    if( iLaunchOutStanding )
        {
        User::Leave( KErrInUse );
        }
    
    iLaunchOutStanding = ETrue;
    
    // Set the Param type
    iLaunchParamType = ELaunchParamString;
    
    // Read the Param from the Descriptor
    // Currently this reads the SUPL triggering Session id.
    TLex16 input( aUiLaunchParams );
    User::LeaveIfError( input.Val(iLaunchParams) );
    
    // Initalize the Engine. Only after the initialization is complete can we 
    // modify the Setting contents. Hence, the actual launching would be 
    // deferred to the initialization period.
    iSettingsEngine->Initalize();
    DEBUG( - CLocSUPLSettings::LaunchL );
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::Close
// Inherited from CSettingsUi
// Closes the running Settings UI prematurely. The UI can be closed 
// permanently or could be temporarily suspended and relaunched. Incase of 
// tenporary suspension it is left to the implementation to decide to
// either store its current state or discard the same
//
// @param aResume  Boolean variable to indicate whether the UI is being 
//				   temporarily suspended or completely shut down. In case
// 				   it is temporarily suspended then its the UIs responsibility
//				   to store its internal state
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettings::Close()
	{
	DEBUG( + CLocSUPLSettings::Close );
    // Reset the flag for an outstanding launch request 
	iLaunchOutStanding = EFalse;
	
	// Reset the settings View
	iSettingsView->Reset();
	
	// Hand over the close event to the Engine
	iSettingsEngine->Close();
	DEBUG( - CLocSUPLSettings::Close );
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::GetCaptionL
// Inherited from CSettingsUi
// Method for getting caption of this plugin. This should be the 
// localized name of the settings view to be shown in parent view.
//
// @param aCaption pointer to Caption variable
// ---------------------------------------------------------------------------
//
void CLocSUPLSettings::GetCaptionL( TDes& aCaption ) const
	{
	// Load the SUPL setting caption from the resource file and hand it back
	// to the callee. It is assuemed here that the calling function has 
	// allocated enough memory for the aCaption string. If not the fucntion
	// would leave
	HBufC* caption = StringLoader::LoadL( R_SUPLSETTINGS_CAPTION );
    aCaption.Copy( *caption );
    delete caption;
	}

// ---------------------------------------------------------------------------
// TInt CLocSUPLSettings::GetPriority
// Inherited from CSettingsUi
// Returns the priority of the UI module. 
// 
// @return Priority of the Settings UI
// ---------------------------------------------------------------------------
//
TInt CLocSUPLSettings::GetPriority()
	{
	// As defined by the Location System UI UI specification, the SUPL settings
	// plug-in should be inserted at the second level
	const TInt KSUPLPluginPriority = 0x00000002;
	return KSUPLPluginPriority;
	}

// ---------------------------------------------------------------------------
// TBool CLocSUPLSettings::IsVisible
// Inherited from CSettingsUi
// Accessor Function for determine whether the UI presence 
// should be shown to the user or should be hidden
//
// @return ETrue  If the presence of the UI be shown to the usr
//	   EFalse If the UI should funtion as a non visible UI
// ---------------------------------------------------------------------------
//
TBool CLocSUPLSettings::IsVisible()
	{
	return ETrue;	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::HandleSettingsEventL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettings::HandleSettingsEventL( TSettingsEventType    aEvent )
    {
	DEBUG1( + CLocSUPLSettings::HandleSettingsEventL aEvent=%d, aEvent );
    switch( aEvent )
        {
        case EExitEvent:            
            {
            // Reset the flag for an outstanding launch request 
	        iLaunchOutStanding = EFalse;
	        
	        iAppUi.HandleCommandL( EEikCmdExit );
            break;    
            }
        case EEngineInitialized:
        case EEngineInitializeFailed:
            {
            // Launch the SUPL UI
            TRAPD( error, LaunchSUPLUiL() );
            if( error && iLaunchOutStanding )
                {
                iLaunchOutStanding = EFalse;
                // The UI is intended to be closed. Pass the event 
		    	// to the observer
			    iSettingsUiObserver.SettingClosed( error );               
                }
            break;    
            }
        case ESUPLSettingsChange:
            {
            // There is a change in Settings. Update the views
            iSettingsView->UpdateView();
            iServerView->UpdateView();
            iSessionView->UpdateView();
            break;    
            }
        case EIAPSelectionComplete:
            {
            if( iLaunchOutStanding )
                {
                iLaunchOutStanding = EFalse;
                // The UI is intended to be closed. Pass the event 
		    	// to the observer
			    iSettingsUiObserver.SettingClosed( KErrNone );
                }
            break;
            }
        case EIAPSelectionCancelled:
            {
            if( iLaunchOutStanding )
                {
                iLaunchOutStanding = EFalse;
                // The UI is intended to be closed. Pass the event 
		    	// to the observer
			    iSettingsUiObserver.SettingClosed( KErrAbort );
                }    
            break;
            }
        default:
            {
            break;    
            }
        }       
	DEBUG( - CLocSUPLSettings::HandleSettingsEventL );
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettings::HandleScreenSizeChange
// Inherited from MLocSUPLSettingsEventHandler
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLSettings::HandleScreenSizeChange()
    {
    iSettingsView->HandleScreenSizeChange();
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettings::HandleCommandL
// Inherited from MLocSUPLSettingsEventHandler
// Handles commands from the views
// 
// @param  aCommand Command ID
// ---------------------------------------------------------------------------
//
void CLocSUPLSettings::HandleCommandL( TInt aCommand )
	{
	DEBUG1( + CLocSUPLSettings::HandleCommandL aCommand=%d, aCommand );
	switch ( aCommand )
		{		

		case MLocSUPLSettingsEventHandler::ELaunchViewComplete:
		    {
		    if ( iLaunchParamType == ELaunchParamString )
		        {
	            delete iLaunchAO;
	            iLaunchAO = NULL;
	            
	            iLaunchAO = CLocSUPLAO::NewL( iSettingsUiObserver,
	                                          *iSettingsEngine,                                          
	                                          iLaunchParams );
	                        
	            iLaunchAO->CompleteRequest();		        
		        }
		    break;
		    }
		// Handle any plug-in specific internal commands here
		case EAknSoftkeyBack:
			{			
			// Reset the flag for an outstanding launch request 
	        iLaunchOutStanding = EFalse;
	    
			// The UI is intended to be closed. Pass the event 
			// to the observer
			iSettingsUiObserver.SettingClosed( KErrNone );		
			break;
			}
		case EAknCmdExit:
		case EEikCmdExit:
		    {		    
		    // Reset the flag for an outstanding launch request 
	        iLaunchOutStanding = EFalse;
	        
	        iAppUi.HandleCommandL( aCommand );
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
	DEBUG( - CLocSUPLSettings::HandleSettingsEventL );
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettings::LaunchSUPLUiL
// Launches the SUPL UI
//
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettings::LaunchSUPLUiL()
    {
	DEBUG( + CLocSUPLSettings::LaunchSUPLUiL );
    // Launch the UI
    // The Launch paramters should be of type TLocSUPLSettingsParams
	// This is an internal agreement between the Settings Ui plug-in and the 
	// user of the plug-in. If the paramter is of any other type, then the 
	// behaviour is undefined for the pluging will still take it as 
	// TLocSUPLSettingsParams type
	
	// Additionaly the Launch param can be a String type, which currently
	// holds the session id.
	// If String paramater contains any other info, then it Leaves with
	// KErrNotSupported. This is basically used for Launching the SUPL
	// Triggering Session details. 
	
	switch( iLaunchParamType )
	    {
	    case ELaunchParamInt:
	        {
	        // If the User has requested for the default behaviour, the SUPL Settings
	        // default view is popped up.       
	        if( ELocSUPLDefault == iLaunchParams )
	            {
	            // Activate the actual SUPL configuration view
	            iAppUi.ActivateLocalViewL( iSettingsViewId );
	            }
	        else if ( ELocSuplSettingsPage == iLaunchParams )
	            {
	            iSettingsView->StartedFromAppServer();
	            // Activate the actual SUPL configuration view
	            iAppUi.ActivateLocalViewL( iSettingsViewId );               
	            }
	        else if( ELocSUPLIAPSelection == iLaunchParams )
	            {    
	            // Check if the User intended to Launch IAP Selection 
	            iSettingsEngine->SelectConnectionL();
	            }
	        else
	            {
	            // The falg was not supported
	            User::Leave( KErrNotSupported );   
	            }  
	        break;
	        }
        case ELaunchParamString:
            {
			// For the Title pane to be visible in View Architecture, we need
			// to activate atleast one view which sets iIsReadyToDraw flag.
			// So we activate the Session view.
			iAppUi.ActivateLocalViewL( iSessionViewId );    
            break;
            }
	    }        
 	DEBUG( - CLocSUPLSettings::LaunchSUPLUiL );
   }
        	
// ---------------------------------------------------------------------------
// void CLocSUPLSettings::OpenLocalizedResourceFileL
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
void CLocSUPLSettings::OpenLocalizedResourceFileL( 
                                		const TDesC&    		aResourceFileName, 
   									  	RConeResourceLoader& 	aResourceLoader )
    {
	DEBUG( + CLocSUPLSettings::OpenLocalizedResourceFileL );
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
	User::LeaveIfError( parse.Set( *dllDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), tmpName, NULL ) );
	TFileName* fileName = new ( ELeave ) TFileName;
	CleanupStack::PushL( fileName );
	fileName->Append( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( fsSession, *fileName );
        
    // Open resource file    
    TRAPD( error , aResourceLoader.OpenL( *fileName ) );
    if( KErrNone != error ) 
        {
        User::Leave( error );
        }
    
    // Pop all the fields
    CleanupStack::PopAndDestroy( fileName );
    CleanupStack::PopAndDestroy( dllDrive );
    CleanupStack::PopAndDestroy( tmpName );
   
    CleanupStack::Pop(); // fsSession
               
    //If leave occurs before this, close is called automatically when the thread exits.
    fsSession.Close();
	DEBUG( - CLocSUPLSettings::OpenLocalizedResourceFileL );   
    }

// End of File
