/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Postioning Method Settings view class methods.
*
*/


// System Include
#include  <aknViewAppUi.h>					// AknViewAppUi
#include  <featmgr.h> 						// FeatureManager
#include  <eikmenup.h>						// Menu pane
#include  <eikmenub.h>						// Menu Bar
#include  <locpsysettingsuiview.rsg>		// Resource File
#include  <avkon.rsg>
#include  <avkon.hrh>
#include  <hlplch.h>						// Help
#include <AknQueryDialog.h>             // For Confirmation query

// User Includes
#include  "locpsysettingsview.h"
#include  "locpsysettingscontainer.h" 
#include  "locpsysettingsui.hrh"
#include  "locpsyappdebug.h"
#include  "locpsyeng.h"
#include  "locpsyinfo.h"
#include  "locpsysettings.h"
#include  "locpsyconfigclientimpl.h"
#include  "locpsycommandhandler.h"
#include  "locpsysettingsuiuid.hrh"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLocPsySettingsView::CLocPsySettingsView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CLocPsySettingsView::CLocPsySettingsView( MLocPsyCommandHandler& 	aCommandHandler ):
    iCommandHandler( aCommandHandler )
    {
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::~CLocPsySettingsView
// -----------------------------------------------------------------------------
CLocPsySettingsView::~CLocPsySettingsView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    
    FeatureManager::UnInitializeLib();
    
    // Delete the Postioning Method Settings Engine
	delete iLocationEngine;
	
    // Close the Handle to the RPostionServer
    iPositionServer.Close();
    }
    
// -----------------------------------------------------------------------------
// CLocPsySettingsView::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
CLocPsySettingsView* CLocPsySettingsView::NewL( MLocPsyCommandHandler& 	aCommandHandler )
    {
    CLocPsySettingsView* self = CLocPsySettingsView::NewLC( aCommandHandler );
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
CLocPsySettingsView* CLocPsySettingsView::NewLC( MLocPsyCommandHandler& aCommandHandler )
    {
    CLocPsySettingsView* self = new (ELeave) CLocPsySettingsView( aCommandHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CLocPsySettingsView::ConstructL()
    {
    FeatureManager::InitializeLibL();
    BaseConstructL( R_LOCATION_HOME_VIEW );
    
    // Create the Postioning Method Settings Engine
	iLocationEngine = CLocPsyEng::NewL();
	
    // Open a handle to the RPostionServer. This is required to determine
    // whether a PSY is currently enabled or disabled
    User::LeaveIfError( iPositionServer.Connect());
	
    }

// -----------------------------------------------------------------------------
// TUid CLocPsySettingsView::Id
// -----------------------------------------------------------------------------
TUid CLocPsySettingsView::Id() const
    {
    return TUid::Uid( LOCPSYSETTINGSUI_UID3 );
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::HandleCommandL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::HandleCommandL(TInt aCommand)
    {
    MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
        
    TInt listIndex = iContainer->SelectedRow();
    
    switch ( aCommand )
        {
        case ELocCmdEnablePsy:
			{
			TRAPD( err, psySettings->EnablePsyL( listIndex ));
        	if ( err == KErrNone )
        		{
				iContainer->HandleMarkCommandL( EAknCmdMark );
        		}
			break;
			}
        case ELocCmdDisablePsy:
			{
			
            TBool popUpConfirmation = EFalse;
            
            // Obtain the PSY specific information
            CLocPsyInfo* info = NULL;
            TRAPD( error, info = psySettings->GetPsyL( listIndex ));
            if( error )
                {
                // Unable to obtain PSY info. Don't continue with the operation
                break;
                }
            
            // Obtain the module status
            TPositionModuleStatus moduleStatus;
            error = iPositionServer.GetModuleStatus( moduleStatus,
                                                     info->PsyId());
             if( error )
                {
                // Unable to obtain module status. Don't continue with the operation
                break;
                }                                               

            // Set the  popUpConfirmation flag if the PSY is being used
            if( TPositionModuleStatus::EDeviceActive == moduleStatus.DeviceStatus() ||
                TPositionModuleStatus::EDeviceReady == moduleStatus.DeviceStatus() ||
                TPositionModuleStatus::EDeviceInitialising == moduleStatus.DeviceStatus() || 
                TPositionModuleStatus::EDeviceStandBy == moduleStatus.DeviceStatus())
                {
                popUpConfirmation = ETrue;
                }
                
            
            if( popUpConfirmation )
                {
                // The user should not be allowed to disable a PSY that is currently
                //being used. Hence, a confirmation dialog is popped up
                iConfirmationQuery = CAknQueryDialog::NewL();
            	TBool result = 
            	    iConfirmationQuery->ExecuteLD( R_PSYDISABLE_CONFIG_QUERY );
                iConfirmationQuery = NULL;            	    
                if( !result )
                    {
                    break;
                    } 
                }      
            
			TRAPD( err, psySettings->DisablePsyL( listIndex ));
        	if ( err == KErrNone )
        		{
				iContainer->HandleMarkCommandL( EAknCmdUnmark );
        		}
			break;
			}
        case ELocCmdConfigurePsy:
        	{
			psySettings->ConfigurePsyL( listIndex, *this );
        	break;
        	}
        case EAknCmdHelp:
        	{
           	HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(),
           										   AppUi()->AppHelpContextL());
        	break;	
        	}
        case EAknSoftkeyBack:
        	{
        	iCommandHandler.HandleCommandL( EAknSoftkeyBack );
        	break;
        	}
        case EAknSoftkeyExit:
            {
            AppUi()->HandleCommandL( EEikCmdExit );
            break;
            } 
        case ELocCmdNone:
        	{
        	break;
        	}
        default: // EAknCmdHelp or other cmd
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::ProcessCommandL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::ProcessCommandL( TInt aCommand )
    {
    MenuBar()->SetContextMenuTitleResourceId(R_LOCATION_CONTEXT_MENUBAR_HOME_VIEW);
    // Close popup menu if open
    MenuBar()->StopDisplayingMenuBar();
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::DynInitMenuPaneL( TInt          aResourceId, 
                                            CEikMenuPane* aMenuPane )
    {
    if ( aResourceId == R_LOCATION_APP_MENU )
        {
        User::LeaveIfNull( aMenuPane );
        //Handle Help Fature
        HandleHelpFeature( *aMenuPane );
        //Context sensitive menu items
        }
    else if ( aResourceId == R_LOCATION_HOME_VIEW_MENU )
    	{
        MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
        
        TInt listIndex = iContainer->SelectedRow();
        if( listIndex >= 0 )
        	{
        	TBool isPsyEnabled = psySettings->IsPsyEnabled( listIndex ); 

        	//Enable is present only if the current selected PSY is disabled.
        	aMenuPane->SetItemDimmed( ELocCmdEnablePsy,isPsyEnabled );

        	//Disable is present only if the current selected PSY is enabled
        	aMenuPane->SetItemDimmed( ELocCmdDisablePsy,!isPsyEnabled );
        
        	// Method setting is present only if the current selected PSY 
        	//is configurable.
			aMenuPane->SetItemDimmed( ELocCmdConfigurePsy, 
								      !psySettings->IsPsyConfigurable( listIndex ));
        	}
        else   
        	{

        	//Disable the menu items when there are no PSYS installed
        	aMenuPane->SetItemDimmed( ELocCmdEnablePsy, ETrue );
        	aMenuPane->SetItemDimmed( ELocCmdDisablePsy, ETrue );
			aMenuPane->SetItemDimmed( ELocCmdConfigurePsy, ETrue );
        	}
        }
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::NotifyL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::NotifyL()
    {
    MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
    // Disable the Middle soft Key if there are no PSYs
    if( psySettings && !psySettings->PSYCount())
        {
        Cba()->SetCommandSetL( R_LOCATION_OPTIONS_EMPTY_BACK );
        }
    else
        {
        MenuBar()->SetContextMenuTitleResourceId(R_LOCATION_CONTEXT_MENUBAR_HOME_VIEW);	
        Cba()->SetCommandSetL( R_LOCATION_OPTIONS_SELECTION_BACK );   
        } 
     
    // If the confirmation query is currently being displayed then check whether 
    // its valid to continue to display the query. If not then dismiss the 
    // query    
    if( iConfirmationQuery )
        {
        if( !iContainer )
            {
            // The is no container. There is nothing to update. Just return
            return;
            }
        TInt listIndex = iContainer->SelectedRow();
                  
        // Obtain the PSY specific information
        CLocPsyInfo* info = NULL;
        TRAPD( error, info = psySettings->GetPsyL( listIndex ));
        if( !error )
            {
            // Obtain the module status
            TPositionModuleStatus moduleStatus;
            error = iPositionServer.GetModuleStatus( moduleStatus,
                                                     info->PsyId());
             if( !error )
                {
                if( TPositionModuleStatus::EDeviceDisabled == moduleStatus.DeviceStatus() ||
                    TPositionModuleStatus::EDeviceInactive == moduleStatus.DeviceStatus())
                    {
                    // If the PSY for which the confirmation query is being displayed 
                    // is currently disabled then the query needs to be dismissed
                    delete iConfirmationQuery;
                    iConfirmationQuery = NULL;
                    }
                }                                               

            
            }    
        }
    	
    // Settings view implements the MLocEngObserver observer interface
    // which contains this leavable method.
    if ( iContainer )
        {
    	LOCAPPDEBUG("CLocPsySettingsView::NotifyL() container update called");
        iContainer->UpdateL();
        }
    }

// -----------------------------------------------------------------------------
//  CLocPsySettingsView::NotifyError
// -----------------------------------------------------------------------------
void CLocPsySettingsView::NotifyError( TInt aErrorCode )
    {
    LOCAPPDEBUG("CLocPsySettingsView::NotifyError() called");
    if ( aErrorCode != KErrNone )
        {
        CCoeEnv::Static()->HandleError( aErrorCode );
        }
    }


// -----------------------------------------------------------------------------
//  CLocPsySettingsView::ProcessKeyEventL
// -----------------------------------------------------------------------------
TBool CLocPsySettingsView::ProcessKeyEventL( const TKeyEvent& aKeyEvent, 
									  TEventCode aType )
	{
    MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
	if( psySettings && !psySettings->PSYCount())
		{
		return EKeyWasNotConsumed;
	  }
	TInt listIndex = iContainer->SelectedRow();   

	// Popup the Menu when OK Selection key is pressed
    if ( aType == EEventKey && aKeyEvent.iCode == EKeyOK )
    	{
        //Check if the current positioning method is configurable or not
        //If yes then launch the context specific value 
        //else toggle status of the current positioning method (changes for single click)
        if(!psySettings->IsPsyConfigurable( listIndex ))
            {
            if(!psySettings->IsPsyEnabled( listIndex ))
                {
                //Enable
               HandleCommandL(ELocCmdEnablePsy);
                }
            else
                {
                //disable
                HandleCommandL(ELocCmdDisablePsy);
                }
            }
        else
            {
            MenuBar()->TryDisplayContextMenuBarL();
            }
    	return EKeyWasConsumed;
    	}
    return EKeyWasNotConsumed;
	}

// -----------------------------------------------------------------------------
//  CLocPsySettingsView::HandleServerAppExit
// -----------------------------------------------------------------------------
void CLocPsySettingsView::HandleServerAppExit(TInt /*aReason*/)
	{
    MLocPsySettings* psySettings = iLocationEngine->SettingsModel();

	/* Once the application exits the application handle in the Engine has to 
	 * be closed. Also this callback is triggered when the PSY configuration UI 
	 * is running but it has to be closed for some reason. For example, if 
	 * the PSY for which the configuration UI is running, is uninstalled then
	 * the configuration application has to be closed.
	 */
    psySettings->ClosePsyConfigurationApp();
	}

// -----------------------------------------------------------------------------
// CLocPsySettingsView::HandleScreenSizeChange
// -----------------------------------------------------------------------------
//
void CLocPsySettingsView::HandleScreenSizeChange() 
	{
	if( iContainer )
		{
		iContainer->SetRect( ClientRect());		
		}
	}
            	
// -----------------------------------------------------------------------------
// CLocPsySettingsView::DoActivateL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/ )
    {
    LOCAPPDEBUG("CLocPsySettingsView::DoActivateL() called");
    if ( !iContainer )
        {
    	iLocationEngine->CreateModelL( ELocPsySettings );
    	MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
    	psySettings->SetObserver(*this);
        iContainer = CLocPsySettingsContainer::NewL( this,
        						 					 this, 
        						 					 this, 
        						 					 psySettings, 
        						 					 ClientRect());
        AppUi()->AddToStackL( *this, iContainer );
        } 
        
	MLocPsySettings* psySettings = iLocationEngine->SettingsModel();
    // Disable the Middle soft Key if there are no PSYs
    if( psySettings && !psySettings->PSYCount())
        {
        Cba()->SetCommandSetL( R_LOCATION_OPTIONS_EMPTY_BACK );
        }
    else
        {
        MenuBar()->SetContextMenuTitleResourceId(R_LOCATION_CONTEXT_MENUBAR_HOME_VIEW );
        Cba()->SetCommandSetL( R_LOCATION_OPTIONS_SELECTION_BACK );   
        } 
   }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::DoDeactivate
// -----------------------------------------------------------------------------
void CLocPsySettingsView::DoDeactivate()
	{
    iLocationEngine->SettingsModel()->RemoveModelObserver();
    iLocationEngine->RemoveModel( ELocPsySettings );
    if ( iContainer )
    	{
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    iContainer = NULL;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::ActivateLocalViewL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::ActivateLocalViewL( TUid aViewId,
    TUid aCustomMessageId, const TDesC8& aCustomMessage )
    {
    CAknViewAppUi* appui = AppUi();
    appui->ActivateLocalViewL(aViewId, aCustomMessageId, aCustomMessage);
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsView::ActivateLocalViewL
// -----------------------------------------------------------------------------
void CLocPsySettingsView::ActivateLocalViewL( TUid aViewId )
    {
    TBuf8<1> customMessage;
    customMessage.Zero();
    // Custom message Id value contains view id of caller
    ActivateLocalViewL( aViewId, Id(), customMessage );
    }



// -----------------------------------------------------------------------------
// CLocPsySettingsView::HandleHelpFeature
// -----------------------------------------------------------------------------
void CLocPsySettingsView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
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
