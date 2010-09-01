/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CLocBtNotifierPlugIn class implementation
*
*/


// System Includes
#include <cfactionindication.h>
#include <centralrepository.h>
#include <AknGlobalNote.h>


// User Includes
#include "locbtnotifierplugin.h"
#include "locbtnotifierconstantsmanager.h"
#include "locbtnotifierprivatecrkeys.h"
#include "locbtnotifierdebug.h"

// CONSTANTS

// Security Policy
_LIT_SECURITY_POLICY_PASS( KSecurityPolicy );

// LocBtNotifier action
_LIT( KLocBtNotifierAction, "showNote" );

// LocBtNotifier action parameters
_LIT( KPsBatteryKeyState, "BatteryState" );
_LIT( KPsAntennaKeyState, "AntennaState" );
_LIT( KPsPowerKeyState,   "PowerState" );

// BtGpsPsy Battery key state values
_LIT( KBatteryStateNormal, "EBatteryStateNormal" );
_LIT( KBatteryStateNA,     "EBatteryStateNA" );
_LIT( KBatteryStateLow,    "EBatteryStateLow" );
_LIT( KBatteryStateFull,   "EBatteryStateFull" );

// BtGpsPsy Antenna key state values
_LIT( KExtAntennaNA,  "EExtAntennaNA" );
_LIT( KExtAntennaOff, "EExtAntennaOff" );
_LIT( KExtAntennaOn,  "EExtAntennaOn" );

// BtGpsPsy Power key state values
_LIT( KExtPowerNA,  "EExtPowerNA" );
_LIT( KExtPowerOff, "EExtPowerOff" );
_LIT( KExtPowerOn,  "EExtPowerOn" );

// Max length of the text to be displayed in the Information Note
const TInt KMaxInfoTextLength = 256;

// Logging constants
_LIT(KLogFile, "locbtnotifier.txt");

_LIT(KLogDir, "locbtnotifier");




// ======== MEMBER FUNCTIONS ========



// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::CLocBtNotifierPlugIn
// ---------------------------------------------------------------------------
//
 CLocBtNotifierPlugIn::CLocBtNotifierPlugIn(): 
 iBatteryStatus( CLocBtNotifierPlugIn::EBatteryStateUnInitialized ),
 iAntennaStatus( CLocBtNotifierPlugIn::EAntennaStateUnInitialized ),
 iPowerStatus( CLocBtNotifierPlugIn::EPowerStateUnInitialized ),
 iInfoText ( NULL , 0 )
    {
    
    }

// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::NewL
// Two Phased constructor 
// ---------------------------------------------------------------------------
//
CLocBtNotifierPlugIn* CLocBtNotifierPlugIn::NewL()
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::NewL : IN" );
   
    CLocBtNotifierPlugIn* self = new( ELeave ) CLocBtNotifierPlugIn;
    
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::NewL : OUT" );
    
    return self;
    }



// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::~CLocBtNotifierPlugIn
// Destructor
// ---------------------------------------------------------------------------
//
CLocBtNotifierPlugIn::~CLocBtNotifierPlugIn()
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::~CLocBtNotifierPlugIn : IN" );
    
    delete iGlobalNote;
    delete iConstantsManager;
   
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::~CLocBtNotifierPlugIn : OUT" );
    }


// ---------------------------------------------------------------------------
// From class CCFActionPlugIn
// CLocBtNotifierPlugIn::InitializeL
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::InitializeL()
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::InitializeL : IN" );
    
    iGlobalNote = CAknGlobalNote::NewL();
    iConstantsManager = CLocBtNotifierConstantsManager::NewL();
 
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::InitializeL : OUT" );
    }

// ---------------------------------------------------------------------------
// From class CCFActionPlugIn
// CLocBtNotifierPlugIn::ExecuteL
// ---------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CLocBtNotifierPlugIn::ExecuteL(
                                CCFActionIndication* aActionIndication )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ExecuteL : IN" );
    
    CCFActionPlugIn::TExecutionTime time = CCFActionPlugIn::ENone;
    
    // Parse PS key settings
    const RKeyValueArray& parameters = aActionIndication->Parameters();
    
    // Get the key and value of the action parameters
    TPtrC key( KNullDesC );
    TPtrC value( KNullDesC );
    
    key.Set( parameters[0]->Key() );
    value.Set( parameters[0]->Value() );
    
    // Check key value of the action parameter
    if( !( key.CompareF( KPsBatteryKeyState ) ) )
        {
        // Handle the Battery Key State change events
        HandleBatteryKeyStateL( value );
        }	
    
    else if( !( key.CompareF ( KPsAntennaKeyState ) ) )
        {
        // Handle the Antenna key State change events
        HandleAntennaKeyStateL( value );
        }
        
    else if( !( key.CompareF ( KPsPowerKeyState ) ) )
        {
        // Handle the Power key State change events
        HandlePowerKeyStateL( value );
        }
       
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ExecuteL : OUT" );   
    
    return time;
    }
    
// ---------------------------------------------------------------------------
// From class CCFActionPlugIn
// CLocBtNotifierPlugIn::GetActionsL
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::GetActionsL( CDesCArray& aActionList ) const
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::GetActionsL : IN" );
    
    aActionList.AppendL( KLocBtNotifierAction );
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::GetActionsL : OUT" );
    }
    
// ---------------------------------------------------------------------------
// From class CCFActionPlugIn
// CLocBtNotifierPlugIn::SecurityPolicy
// ---------------------------------------------------------------------------
//
const TSecurityPolicy& CLocBtNotifierPlugIn::SecurityPolicy() const
    {
    return KSecurityPolicy;
    }
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::HandleBatteryKeyState
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::HandleBatteryKeyStateL( TDesC& aValue )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandleBatteryKeyState : IN" );
    
    // Current Battery State
    CLocBtNotifierPlugIn::TBtGpsPsyBatteryKeyState batteryState = 
                          CLocBtNotifierPlugIn::EBatteryStateUnInitialized ;
    
        
    // Get the current PS key state value
    if( !( aValue.CompareF( KBatteryStateNA ) ) )
        {    
        batteryState = CLocBtNotifierPlugIn::EBatteryStateNotApplicable;
        }
    else if( !( aValue.CompareF( KBatteryStateNormal ) ) )
        {
        batteryState = CLocBtNotifierPlugIn::EBatteryStateNormal;
        }
    else if( !( aValue.CompareF( KBatteryStateLow ) ) )
        {
    	batteryState = CLocBtNotifierPlugIn::EBatteryStateLow;
        }
    else if( !( aValue.CompareF( KBatteryStateFull ) ) )
        {
        batteryState = CLocBtNotifierPlugIn::EBatteryStateFull;
        }
    else
    	{
    	// Invalid
    	return;
    	}
    
    LOCBTNOTIFIERDEBUG1( "Battery State (%d)",  batteryState );
       
    
    if( iBatteryStatus == CLocBtNotifierPlugIn::EBatteryStateUnInitialized )
        {
        // Initializing the previous battery Status for the first callback   
        // from context framework
        iBatteryStatus = CLocBtNotifierPlugIn::EBatteryStateInitialized;
        
        LOCBTNOTIFIERDEBUG1( "Previous Battery State (%d)",  iBatteryStatus );
       
        return;
        }
    else if ( iBatteryStatus == CLocBtNotifierPlugIn::EBatteryStateInitialized 
              && batteryState != CLocBtNotifierPlugIn::EBatteryStateNotApplicable )
    	{
    	// Initializing the previous battery status to current value of PS key
        // after pairing of BT GPS device happens
    	iBatteryStatus = batteryState;
    	
      LOCBTNOTIFIERDEBUG1( "Previous Battery State (%d)",  iBatteryStatus );
       
    	return;
    	}   
     
     
    LOCBTNOTIFIERDEBUG1( "Previous Battery State (%d)",  iBatteryStatus );
                
    //Check that there is a change in the Battery State before displaying the note
    if( iBatteryStatus != batteryState )
        {
       
        LOCBTNOTIFIERDEBUG1( "Previous Battery State (%d)",  iBatteryStatus );
            
       
        LOCBTNOTIFIERDEBUG1( "Battery State (%d)",  batteryState );
               
    	switch( batteryState )
    	    {
    	    case CLocBtNotifierPlugIn::EBatteryStateNotApplicable :
    	         {
    	         // No note shown
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EBatteryStateNormal :
    	         {
    	         // No note shown
    	         iBatteryStatus = batteryState;
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EBatteryStateLow :
    	         {
    	         iBatteryStatus = batteryState;
    	         ShowBatteryNoteL( batteryState );
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EBatteryStateFull :
    	         {
    	         iBatteryStatus = batteryState;
    	         ShowBatteryNoteL( batteryState );
    	         break;
    	         }
    	         
    	    default :
    	         {
    	         break;
    	         }
    	    }
    	    
    	
    	
        }
      
    LOCBTNOTIFIERDEBUG1( "Previous Battery State (%d)",  iBatteryStatus );
             
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandleBatteryKeyState : OUT" );
    }
    
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::ShowBatteryNoteL
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::ShowBatteryNoteL( TBtGpsPsyBatteryKeyState aState )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowBatteryNoteL : IN" );
	
	// Check if the note can be shown
	if( !( IsBatteryNoteShownL( aState ) ) )
	    {
	    return;
	    }
	    
	switch( aState )
        {
        case CLocBtNotifierPlugIn::EBatteryStateLow : 
        	 {
        	 iInfoText.Set( iConstantsManager->GetBatteryLowDialogText() );
        	 break;            
        	 }
             
        case CLocBtNotifierPlugIn::EBatteryStateFull : 
             {
             iInfoText.Set( iConstantsManager->GetBatteryFullDialogText() );
             break;	
             }
             
        default :
             {
             break;
             } 
        }
    
    iGlobalNote->ShowNoteL( EAknGlobalInformationNote, iInfoText );    
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowBatteryNoteL : OUT" );
    }
    
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::IsBatteryNoteShown
// ---------------------------------------------------------------------------
//
TBool CLocBtNotifierPlugIn::IsBatteryNoteShownL( TBtGpsPsyBatteryKeyState aState )
    {
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsBatteryNoteShown : IN" );
	
	TBool shown = EFalse;
	
	switch( aState )
	    {
	    case CLocBtNotifierPlugIn::EBatteryStateLow :
	         {
	         IsDialogShownL( KBluetoothGpsPsyBatteryLowDialog, shown );
             break;	
             }
             
        case CLocBtNotifierPlugIn::EBatteryStateFull :
             {
             IsDialogShownL( KBluetoothGpsPsyBatteryFullDialog, shown );
             break;	
             }
             
        default :
             {
             break;
             }
	         
	    }
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsBatteryNoteShown : OUT" );
	
	return shown;
    }
  
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::HandleAntennaKeyState
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::HandleAntennaKeyStateL( TDesC& aValue )
    {
   LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandleAntennaKeyState : IN" );
    
    // Current Antenna State
    CLocBtNotifierPlugIn::TBtGpsPsyAntennaKeyState antennaState = 
                          CLocBtNotifierPlugIn::EAntennaStateUnInitialized ;
                          
        
    // Get the current PS key state value
    if( !( aValue.CompareF( KExtAntennaNA ) ) )
        {
    	antennaState = CLocBtNotifierPlugIn::EAntennaStateNotApplicable;
        }
    else if( !( aValue.CompareF( KExtAntennaOff ) ) )
        {
    	antennaState = CLocBtNotifierPlugIn::EExtAntennaOff;
        }
    else if( !( aValue.CompareF( KExtAntennaOn ) ) )
        {
        antennaState = CLocBtNotifierPlugIn::EExtAntennaOn ;
        }
     else
    	{
    	// Invalid
    	return;
    	}
    
   LOCBTNOTIFIERDEBUG1( " Antenna State (%d)",  antennaState );
    
    if( iAntennaStatus == CLocBtNotifierPlugIn::EAntennaStateUnInitialized )
        {
        // Initializing the previous Antenna Status for the first callback   
        // from context framework
        iAntennaStatus = CLocBtNotifierPlugIn::EAntennaStateInitialized;
        
        LOCBTNOTIFIERDEBUG1( "Previous Antenna State (%d)",  iAntennaStatus );

        return;
        }
    else if ( iAntennaStatus == CLocBtNotifierPlugIn::EAntennaStateInitialized
              && antennaState != CLocBtNotifierPlugIn::EAntennaStateNotApplicable )
    	{
    	// Initializing the previous Antenna status to current value of PS key
        // after pairing of BT GPS device happens
    	iAntennaStatus = antennaState;
    	
    	LOCBTNOTIFIERDEBUG1( "Previous Antenna State (%d)",  iAntennaStatus );
        
    	return;
    	}   
            
    	LOCBTNOTIFIERDEBUG1( "Previous Antenna State (%d)",  iAntennaStatus );
         
    //Check that there is a change in the Antenna State before displaying the note
    if( iAntennaStatus != antennaState )
        {
        
        LOCBTNOTIFIERDEBUG1( "Previous Antenna State (%d)",  iAntennaStatus );
 
        
        LOCBTNOTIFIERDEBUG1( " Antenna State (%d)",  antennaState );
         
    	switch( antennaState )
    	    {
    	    case CLocBtNotifierPlugIn::EAntennaStateNotApplicable :
    	         {
    	         // No note shown
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EExtAntennaOff :
    	         {
    	         iAntennaStatus = antennaState;
    	         ShowAntennaNoteL( antennaState );
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EExtAntennaOn :
    	         {
    	         iAntennaStatus = antennaState;
    	         ShowAntennaNoteL( antennaState );
    	         break;
    	         }
    	         
    	    default :
    	         {
    	         break;
    	         }
    	    }
    	    
    	
        }
        
    	LOCBTNOTIFIERDEBUG1( "Previous Antenna State (%d)",  iAntennaStatus );
        
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandleAntennaKeyState : OUT" );
    }

  
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::ShowAntennaNoteL
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::ShowAntennaNoteL( TBtGpsPsyAntennaKeyState aState )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowAntennaNoteL : IN" );
	
	// Check if the note can be shown
	if( !( IsAntennaNoteShownL( aState ) ) )
	    {
		return;
	    }
	    
	switch( aState )
        {
        case CLocBtNotifierPlugIn::EExtAntennaOff : 
        	 {
        	 iInfoText.Set( iConstantsManager->GetExtAntennaDisconnectedDialogText() );
        	 break;            
        	 }
             
        case CLocBtNotifierPlugIn::EExtAntennaOn : 
             {
             iInfoText.Set( iConstantsManager->GetExtAntennaConnectedDialogText() );
             break;	
             }
             
        default :
             {
             break;
             } 
        }
    
    iGlobalNote->ShowNoteL( EAknGlobalInformationNote, iInfoText );    
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowAntennaNoteL : OUT" );
    } 

    
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::IsAntennaNoteShown
// ---------------------------------------------------------------------------
//
TBool CLocBtNotifierPlugIn::IsAntennaNoteShownL( TBtGpsPsyAntennaKeyState aState )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsAntennaNoteShown : IN" );
	
	TBool shown = EFalse;
	
	switch( aState )
	    {
	    case CLocBtNotifierPlugIn::EExtAntennaOff :
	         {
             IsDialogShownL( KBluetoothGpsPsyExtAntennaDisconnectedDialog, shown );
             break;	
             }
             
        case CLocBtNotifierPlugIn::EExtAntennaOn :
             {
             IsDialogShownL( KBluetoothGpsPsyExtAntennaConnectedDialog, shown );
             break;	
             }
             
        default :
             {
             break;
             }
	         
	    }
    
 LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsAntennaNoteShown : OUT" );
	
	return shown;
    }


// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::HandlePowerKeyState
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::HandlePowerKeyStateL( TDesC& aValue )
    {
  LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandlePowerKeyState : IN" );
    
    // Current Power State
    CLocBtNotifierPlugIn::TBtGpsPsyPowerKeyState powerState = 
                          CLocBtNotifierPlugIn::EPowerStateUnInitialized ;
                          
        
    // Get the current PS key state value
    if( !( aValue.CompareF( KExtPowerNA ) ) )
        {
    	powerState = CLocBtNotifierPlugIn::EPowerStateNotApplicable;
        }
    else if( !( aValue.CompareF( KExtPowerOff ) ) )
        {
    	powerState = CLocBtNotifierPlugIn::EExtPowerOff;
        }
    else if( !( aValue.CompareF( KExtPowerOn ) ) )
        {
        powerState = CLocBtNotifierPlugIn::EExtPowerOn;
        }
    else
    	{
    	// Invalid
    	return;
    	}
    
   LOCBTNOTIFIERDEBUG1( "Power State (%d)",  powerState );
 
    if( iPowerStatus == CLocBtNotifierPlugIn::EPowerStateUnInitialized )
        {
        // Initializing the previous Power Status for the first callback   
        // from context framework
        iPowerStatus = CLocBtNotifierPlugIn::EPowerStateInitialized;
        
        	LOCBTNOTIFIERDEBUG1( "Previous Power State (%d)",  iPowerStatus );

        return;
        }
    else if ( iPowerStatus == CLocBtNotifierPlugIn::EPowerStateInitialized
              && powerState != CLocBtNotifierPlugIn::EPowerStateNotApplicable )
    	{
    	// Initializing the previous Power status to current value of PS key
        // after pairing of BT GPS device happens
    	iPowerStatus = powerState;
    	
      LOCBTNOTIFIERDEBUG1( "Previous Power State (%d)",  iPowerStatus );

    	return;
    	}   
    	
    	LOCBTNOTIFIERDEBUG1( "Previous Power State (%d)",  iPowerStatus );
             
    //Check that there is a change in the Power State before displaying the note
    if( iPowerStatus != powerState )
        {
        
        LOCBTNOTIFIERDEBUG1( "Previous Power State (%d)",  iPowerStatus );
 
        LOCBTNOTIFIERDEBUG1( " Power State (%d)",  powerState );
       
    	switch( powerState )
    	    {
    	    case CLocBtNotifierPlugIn::EPowerStateNotApplicable :
    	         {
    	         // No note shown
    	         break;
    	         }
    	         
    	    case CLocBtNotifierPlugIn::EExtPowerOff :
    	         {
    	         iPowerStatus = powerState;
    	         ShowPowerNoteL( powerState );
    	         break;
    	         }
    	         
    	    case EExtPowerOn :
    	         {
    	         iPowerStatus = powerState;
    	         ShowPowerNoteL( powerState );
    	         break;
    	         }
    	         
    	    default :
    	         {
    	         break;
    	         }
    	    }
    	    
    	
        }
      	
      	LOCBTNOTIFIERDEBUG1( "Previous Power State (%d)",  iPowerStatus );
         
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::HandlePowerKeyState : OUT" );
    }

  
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::ShowPowerNoteL
// ---------------------------------------------------------------------------
//
void CLocBtNotifierPlugIn::ShowPowerNoteL( TBtGpsPsyPowerKeyState aState )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowPowerNoteL : IN" );
	
	// Check if the note can be shown
	if( !( IsPowerNoteShownL( aState ) ) )
	    {
		return;
	    }
	    
	switch( aState )
        {
        case CLocBtNotifierPlugIn::EExtPowerOff : 
        	 {
        	 iInfoText.Set( iConstantsManager->GetExtPowerDisconnectedDialogText() );
        	 break;            
        	 }
             
        case CLocBtNotifierPlugIn::EExtPowerOn : 
             {
             iInfoText.Set( iConstantsManager->GetExtPowerConnectedDialogText() );
             break;	
             }
             
        default :
             {
             break;
             } 
        }
    
    iGlobalNote->ShowNoteL( EAknGlobalInformationNote, iInfoText );    
        
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::ShowPowerNoteL : OUT" );
    } 

    
    
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::IsPowerNoteShown
// ---------------------------------------------------------------------------
//
TBool CLocBtNotifierPlugIn::IsPowerNoteShownL( TBtGpsPsyPowerKeyState aState )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsPowerNoteShown : IN" );
	
	TBool shown = EFalse;
	
	switch( aState )
	    {
	    case CLocBtNotifierPlugIn::EExtPowerOff :
	         {
	         IsDialogShownL( KBluetoothGpsPsyExtPowerDisconnectedDialog, shown );
           break;	
             }
             
        case CLocBtNotifierPlugIn::EExtPowerOn :
             {
             IsDialogShownL( KBluetoothGpsPsyExtPowerConnectedDialog, shown  );
             break;	
             }
             
        default :
             {
             break;
             }
	         
	    }
	    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsPowerNoteShown : OUT" );
	
	return shown;
    }
    
        
// ---------------------------------------------------------------------------
// CLocBtNotifierPlugIn::IsDialogShown
// ---------------------------------------------------------------------------
// 
void CLocBtNotifierPlugIn::IsDialogShownL( TUint32 aKey , TBool& aShown )
    {
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsDialogShown : IN" );
    
    // CRepository instance for accessing the BtGpsPsy settings
    CRepository* cenRep = CRepository::NewL( KCRUidLocBtGpsPsy );
    
        
    // Get the value of the specified Cen Rep key
    TInt err = cenRep->Get( aKey, aShown );
    
    if( err != KErrNone )
    // Failed to Get value of cenrep key,return error
        {
        User::Leave( err );
        }
    
    delete cenRep;
    
    LOCBTNOTIFIERDEBUG( "CLocBtNotifierPlugIn::IsDialogShown : OUT" );
    }


