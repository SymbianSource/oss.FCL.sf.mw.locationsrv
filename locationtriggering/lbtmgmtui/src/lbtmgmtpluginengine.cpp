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
* Description:  Location Triggering Management Plugin Engine
*
*/


// System Include
#include <e32keys.h>        // Tab Keys
#include <StringLoader.h>   // String Loader
#include <lbtmgmtpluginview.rsg>
#include <aknradiobuttonsettingpage.h>
#include <aknslidersettingpage.h> 
#include <aknmessagequerydialog.h> 


// User Include
#include "lbtmgmtpluginengine.h"
#include "lbtmgmtengobserver.h"
#include "lbtmgmtlbtadapter.h"
#include "lbtmgmtdebug.h"

// Constant Defintions
const TInt KNumberofSettingsItems = 2;
const TInt KMaxConversionBufferLength = 0x200;
const TInt KNoofTriggerStatus = 2;

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLbtMgmtPluginEngine::CLbtMgmtPluginEngine
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLbtMgmtPluginEngine::CLbtMgmtPluginEngine(  
                                MLbtMgmtEngObserver& aEngineObserver )
    :iObserver( aEngineObserver ),
    iSettingsDialogOutstanding( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPluginEngine::~CLbtMgmtPluginEngine
// Destructor
//
// ---------------------------------------------------------------------------
//
CLbtMgmtPluginEngine::~CLbtMgmtPluginEngine()
    {
    // Delete the list box strings
    delete iTriggeringTitle;
    
    delete iActiveTitle;
    
    delete iNoActiveTitle;
    
    delete iInfoMessage;
    
    delete iOnSettings;
    
    delete iOffSettings;
    
    delete iConversionBuffer;
        
    // Delete the Lbt settings handle
    delete iLbtAdapter;
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtPluginEngine* CLbtMgmtPluginEngine::NewL
// Creates new Location Trigger Management plug-in engine instance.
//
// @param  aEngineObserver           Observer to the Trigger Management engine
// @return CLbtMgmtPluginEngine* Reference to the application view
// ---------------------------------------------------------------------------
//    
CLbtMgmtPluginEngine* CLbtMgmtPluginEngine::NewL( 
                            MLbtMgmtEngObserver& aEngineObserver )
    {   
    CLbtMgmtPluginEngine* self = 
                        CLbtMgmtPluginEngine::NewLC( aEngineObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPluginEngine* CLbtMgmtPluginEngine::NewLC
// Creates new Location Trigger Management plug-in engine instance. Leaves
// the object on the Cleanup stack
//
// @param  aEngineObserver           Observer to the Trigger Management engine
// @return CLbtMgmtPluginEngine* Reference to the application view
// ---------------------------------------------------------------------------
// 
CLbtMgmtPluginEngine* CLbtMgmtPluginEngine::NewLC( 
                            MLbtMgmtEngObserver& aEngineObserver )
    {   
    CLbtMgmtPluginEngine* self = 
                        new(ELeave) CLbtMgmtPluginEngine( aEngineObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginEngine::ConstructL()
    {
    // Create Lbt Adapter handle
    iLbtAdapter = CLbtMgmtLbtAdapter::NewL( *this );
    
    // Load the list box strings
    // Allocate and set the System of measurement heading
    iTriggeringTitle = StringLoader::LoadL( R_LOCTR_LIST_LBT_TRIGGERING );
    
    // Allocate and set the server ip address title field
    iActiveTitle = StringLoader::LoadL( R_LOCTR_LIST_ACTIVE_TRIGGER );
    
    // Allocate and set the server ip address title field
    iNoActiveTitle = StringLoader::LoadL( R_LOCTR_LIST_NO_ACTIVE_TRIGGER );
     
    //Allocate and set the Info message text
    iInfoMessage = StringLoader::LoadL( R_LOCTR_INFO_POPUP_TEXT );
      
    // Allocate and set the Imperial settings text
    iOnSettings = StringLoader::LoadL( R_LOCTR_LIST_VALUE_TRIGGER_ON );
    
    // Allocate and set the Metric settings text
    iOffSettings = StringLoader::LoadL( R_LOCTR_LIST_VALUE_TRIGGER_OFF );
    
    // Allocate the Conversion Buffer
    iConversionBuffer = HBufC16::NewL( KMaxConversionBufferLength );    
   }
    
// ---------------------------------------------------------------------------
// TInt CLbtMgmtPluginEngine::MdcaCount
// Inherited from MDesCArray
//
// ---------------------------------------------------------------------------
//
TInt CLbtMgmtPluginEngine::MdcaCount() const
    {
    return KNumberofSettingsItems;
    }

// ---------------------------------------------------------------------------
// TInt CLbtMgmtPluginEngine::MdcaPoint
// Inherited from MDesCArray
//
// ---------------------------------------------------------------------------
//
TPtrC16 CLbtMgmtPluginEngine::MdcaPoint( TInt aIndex ) const
    {
    // Zero the internal buffer
    TPtr16 ptr( iConversionBuffer->Des());
    ptr.Zero();
    
    // Format "\t%S\t\t%S"
    
    // Append the first tab
    ptr.Append( EKeyTab );
            
    switch( aIndex )
        {
        case ETriggeringStatus:
            {
            // Append the Title text
            ptr.Append( iTriggeringTitle->Des() );
            
            // Append the second tab
            ptr.Append( EKeyTab ); 
                            
            // Append the third tab
            ptr.Append( EKeyTab );
            
            // Obtain the current value of system of measurement and append
            // to the buffer
           CLbtMgmtLbtAdapter::TTriggeringStatus state = CLbtMgmtLbtAdapter::EOn;
           TRAPD( err, state = iLbtAdapter->GetCurrentTriggeringStatusL() );
            if( !err && state == CLbtMgmtLbtAdapter::EOn )
                {
          		ptr.Append( iOnSettings->Des() );
                }
            else if ( state == CLbtMgmtLbtAdapter::EOff )
                {
                ptr.Append( iOffSettings->Des());
                }  
            break;    
            }
        case EActiveTriggers:
            {
            // Append the Title text
            //XXX: Optimization::should be called  when there is a change in lbt database
            TInt activeTriggers = iLbtAdapter->ActiveTriggers();
        	
           	if( activeTriggers == 0 )
                ptr.Append( iNoActiveTitle->Des() );
           	else
           		{
        		HBufC* activeTriggersString = NULL;
        		if( activeTriggers == 1 )
        		    {
        		    TRAP_IGNORE( activeTriggersString = StringLoader::LoadL( R_LOCTR_ONE_ACTIVE_TRIGGER ) ); 
        		    }
        		else
        		    {
        		    TRAP_IGNORE( activeTriggersString = StringLoader::LoadL( R_LOCTR_LIST_ACTIVE_TRIGGER, activeTriggers ) ); 
				    }
				if( activeTriggersString )
					{
					ptr.Append( activeTriggersString->Des() );
					delete activeTriggersString;
					}
            	}
            	
            // Append the second tab
            ptr.Append( EKeyTab ); 
            
            // Append the third tab
            ptr.Append( EKeyTab );            
        
            break;    
            }            
        default:
            {
            // Append the second tab
            ptr.Append( EKeyTab );          
            // Append the third tab
            ptr.Append( EKeyTab );
            break;    
            }
        }  
    return iConversionBuffer->Des();
    }
 
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::HandleSettingsChangeL
// Inherited from MLocNotPrefAdapterObserver
//
// ---------------------------------------------------------------------------
//   
void CLbtMgmtPluginEngine::HandleSettingsChangeL()
    {
    iObserver.HandleSettingsChangeEventL();
    }
 
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::Close
// Closes the running Settings UI prematurely. This would result in dismissing
// any active dialogs currently running
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginEngine::Close()
    {
    // If any of the dialogs are active then issue a cancel event on the 
    // dialogs
    if( iSettingsDialogOutstanding )
        {
        TKeyEvent   keyEvent;
        keyEvent.iCode          = EKeyCBA2;
        keyEvent.iScanCode      = EStdKeyDevice1;         
        keyEvent.iModifiers     = EAllModifiers;
        keyEvent.iRepeats       = 0;
        CCoeEnv* cCoeEnv = CCoeEnv::Static();
        
        // Simulate a Cancel event on the running dialogs. The Simulated event
        // is for the Left Soft Key.
        // Ignore the error incase of a leave
        
        TRAP_IGNORE( cCoeEnv->SimulateKeyEventL( keyEvent, EEventKey ) );
        }
    iSettingsDialogOutstanding = EFalse;                
    }
            
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ChangeTiggerStatusL
// Change Trigger settings
// 
// ---------------------------------------------------------------------------
// 
void CLbtMgmtPluginEngine::ChangeTiggerStatusL()
    {
    LBTMGMTUIDEBUG("+ CLbtMgmtPluginEngine::ChangeTiggerStatusL()");
    if( iSettingsDialogOutstanding )
        {
        User::Leave( KErrInUse );
        }
    // Allocate the descriptor array for Text settings for System
    // of measurement
    CDesCArrayFlat* items = new( ELeave ) CDesCArrayFlat( KNoofTriggerStatus );
    CleanupStack::PushL( items );

    // Append the radio-button list items
    items->AppendL( iOnSettings->Des());
    items->AppendL( iOffSettings->Des());   
 
     
    // Obtain the current value for current system of measurement
    // This would be used for setting the default value for 
    // the text settings page               
    
    TInt currentSettings = iLbtAdapter->GetCurrentTriggeringStatusL();
    TInt prevSettings = currentSettings;
        
    CAknRadioButtonSettingPage* dlg = 
                        new ( ELeave )CAknRadioButtonSettingPage( R_LOCTR_LIST_LBT_TRIGGERING_SETTING,
                                                                  currentSettings, 
                                                                  items );
    // Settings Outstanding flag is marked True to enable dismissal incase
    // of a Cancel event                                                                  
    iSettingsDialogOutstanding = ETrue;                                                                  
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ))
        {           
	    if(prevSettings != currentSettings)
    	    {
            switch(currentSettings)
                {
                case CLbtMgmtLbtAdapter::EOff: //Off
                    {
                    //Confirmation is required if any active trigger exists                
                    if( 0 != ActiveTriggers() )
                        {
                        HBufC* msg = StringLoader::LoadLC( R_LOCTR_NOTE_TURNOFF );
                        CAknQueryDialog* confirmationQuery = CAknQueryDialog::NewL();     
                        if(confirmationQuery->ExecuteLD(R_LOCTR_CONFIRMATION_QUERY, *msg))
                            {
                            iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOff  );     
                            }   
                        CleanupStack::PopAndDestroy(msg);
                        }
                    else
                        {
                        iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOff  );      
                        }
                    }
                    break;
                case CLbtMgmtLbtAdapter::EOn: //On
                    {
                    //No Warning note is required.               
                    iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOn );
                    }
                    break;
                default:
                    break;                
                } 
    	    }      
        } 
     iSettingsDialogOutstanding = EFalse;       
    // Free the items resource
    CleanupStack::PopAndDestroy( items );

    LBTMGMTUIDEBUG("- CLbtMgmtPluginEngine::ChangeTiggerStatusL()");
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ToggleTiggerStatusL
// Toggle Trigger settings
// 
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginEngine::ToggleTiggerStatusL()
    {
    LBTMGMTUIDEBUG("+ CLbtMgmtPluginEngine::ToggleTiggerStatusL()");
    if( iSettingsDialogOutstanding )
        {
        User::Leave( KErrInUse );
        }    
	CLbtMgmtLbtAdapter::TTriggeringStatus status = iLbtAdapter->GetCurrentTriggeringStatusL();
	
	if( CLbtMgmtLbtAdapter::EOn == status )
		{
        //Confirmation is required if any active trigger exists                
        if( 0 != ActiveTriggers() )
            {
            HBufC* msg = StringLoader::LoadLC( R_LOCTR_NOTE_TURNOFF );
            CAknQueryDialog* confirmationQuery = CAknQueryDialog::NewL();     
            if(confirmationQuery->ExecuteLD(R_LOCTR_CONFIRMATION_QUERY, *msg))
                {
                iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOff  );     
                }   
            CleanupStack::PopAndDestroy(msg);
            }
        else
            {
            //No Warning note is required.   
            iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOff  );      
            } 
		}
	else
		{
        iLbtAdapter->SetCurrentTriggeringStatusL( CLbtMgmtLbtAdapter::EOn ); 
		}
    LBTMGMTUIDEBUG("- CLbtMgmtPluginEngine::ToggleTiggerStatusL()");
    }


// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ShowInfoDialog
// Show Info dialog for active triggers
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginEngine::ShowInfoDialogL()
    {   
    LBTMGMTUIDEBUG("+ CLbtMgmtPluginEngine::ShowInfoDialogL()");
    CAknMessageQueryDialog*  info = CAknMessageQueryDialog::NewL(*iInfoMessage);
    info->ExecuteLD(R_LOCTR_MESSAGE_QUERY_DIALOG);
    LBTMGMTUIDEBUG("- CLbtMgmtPluginEngine::ShowInfoDialogL()");
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ClearAllTriggersL
// It will clear all triggers
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginEngine::ClearAllTriggersL()
    {    
    LBTMGMTUIDEBUG("+ CLbtMgmtPluginEngine::ClearAllTriggersL()");
    HBufC* msg = StringLoader::LoadLC( R_LOCTR_NOTE_CLEARTRIGGERS );
    CAknQueryDialog* confirmationQuery = CAknQueryDialog::NewL();     
    if(confirmationQuery->ExecuteLD(R_LOCTR_CONFIRMATION_QUERY, *msg))
        {
		iLbtAdapter->DeleteActiveTriggersL();           
        }   
    CleanupStack::PopAndDestroy(msg);
    LBTMGMTUIDEBUG("- CLbtMgmtPluginEngine::ClearAllTriggersL()");
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginEngine::ShowInfoDialog
// It will return total active triggers
// ---------------------------------------------------------------------------
//
TInt CLbtMgmtPluginEngine::ActiveTriggers()
    {
    return iLbtAdapter->ActiveTriggers();
    }
