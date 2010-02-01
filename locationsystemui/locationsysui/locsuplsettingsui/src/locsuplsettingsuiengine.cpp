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
* Description:  SUPL Settings UI Engine. The engine is resposible for handling
*                all the UI components required for SUPL Settings UI. In addition
*                to this it also is responsible for interacting with the SUPL
*                Settings API
*
*/


// System Includes
#include <e32keys.h>
#include <locsuplsettingsui.rsg>
#include <StringLoader.h>
#include <akntextsettingpage.h>     // Text editor
#include <commdb.h>                 // CommsDatabase                                    
#include <cmapplicationsettingsui.h>// Access Point Configurator
#include <cmplugincsddef.h>			// For UID of CSD BearerType
#include <cmpluginpacketdatadef.h>	// For UID of Packet data BearerType
#include <cmmanager.h>				// For Conversion from UID to Name and Name to UID 
#include <cmconnectionmethod.h> 	// For Conversion from UID to Name and Name to UID
#include <aknnotewrappers.h>        // Error Notes
#include <textresolver.h>
#include <cdbcols.h>
#include <bldvariant.hrh>           // For 3.2 SDK flag
#include <AknGlobalConfirmationQuery.h> // For Global confirmation query
#include <avkon.hrh>                // For the Softkey definition
#include <aknSDData.h>              // Secondary display data for confirmation
                                    // query
#include <badesca.h>                // For Radio button array
#include <aknradiobuttonsettingpage.h>    // Akn Radio button page                                    
#include "locsuplsettingscoverui.h" // Secondary display API
#include <aknViewAppUi.h>
#include <epos_csuplsettingsconstants.h>

// User Includes
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingsuiengobserver.h"
#include "locsuplsettingsadapter.h"
#include "locsuplservereditor.h"
#include "locsuplsessioneditor.h"
#include "locsupldebug.h"

// Constants
const TInt KMaxBufferLength 		= 0x100;
const TInt KNoofUsageSettings 		= 4;

// ---------------------------------------------------------------------------
// CLocSUPLSettingsUiEngine::CLocSUPLSettingsUiEngine
// C++ default constructor.
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsUiEngine::CLocSUPLSettingsUiEngine( 
                            MLocSUPLSettingsUiEngObserver&      aObserver )
	: CActive( EPriorityStandard ), 
	iObserver( aObserver ),
	iConversionBufferPtr( NULL, 0 ),
	iSettingsBufferPtr( NULL, 0),
	iCurrentSlpId( -1 ),
	iEditorObserver( NULL )
	{
	// No implementation
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsUiEngine::~CLocSUPLSettingsUiEngine
// Destructor.
// ---------------------------------------------------------------------------	
//
CLocSUPLSettingsUiEngine::~CLocSUPLSettingsUiEngine()	
    {
	DEBUG( + CLocSUPLSettingsUiEngine::~CLocSUPLSettingsUiEngine );	
    Cancel(); 
    
    // Delete the Confirmation query
    delete iConfirmQuery;
    iConfirmQuery = NULL;
    
	delete iSUPLSettingsAdapter;
	iSUPLSettingsAdapter = NULL;	
	
	// Delete the Conversion buffers
	delete iConversionBuffer;
	iConversionBuffer = NULL;
	
	delete iSettingsBuffer;
	iSettingsBuffer = NULL;
	
	// Delete the Error handling utilities
	delete iTextResolver;
	iTextResolver = NULL;
	DEBUG( - CLocSUPLSettingsUiEngine::~CLocSUPLSettingsUiEngine );	
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsUiEngine* CLocSUPLSettingsUiEngine::NewL
// Symbian OS two-phased constructor
// 
// @return CLocSUPLSettingsUiEngine* Reference to the Settings UI Engine
// ---------------------------------------------------------------------------	
//
CLocSUPLSettingsUiEngine* CLocSUPLSettingsUiEngine::NewL( 
                                 MLocSUPLSettingsUiEngObserver&      aObserver )
	{
	DEBUG( + CLocSUPLSettingsUiEngine::NewL );	
	CLocSUPLSettingsUiEngine* self = CLocSUPLSettingsUiEngine::NewLC(
                                                                aObserver );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettingsUiEngine::NewL );	
	return self; 
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSettingsUiEngine* CLocSUPLSettingsUiEngine::NewLC
// Symbian OS two-phased constructor
// 
// @return CLocSUPLSettingsUiEngine* Reference to the Settings UI Engine
// ---------------------------------------------------------------------------	
//
CLocSUPLSettingsUiEngine* CLocSUPLSettingsUiEngine::NewLC( 
                                 MLocSUPLSettingsUiEngObserver&      aObserver )
	{
	CLocSUPLSettingsUiEngine* self = 
	                new ( ELeave ) CLocSUPLSettingsUiEngine( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self; 
	}	
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::ConstructL
// Second phase constructor.
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettingsUiEngine::ConstructL()
	{ 
	DEBUG( + CLocSUPLSettingsUiEngine::ConstructL );	
    // Allocate the conversion buffers
    iConversionBuffer = HBufC16::NewL( KMaxBufferLength );
    iConversionBufferPtr.Set( iConversionBuffer->Des() );
    
    iSettingsBuffer = HBufC::NewL( KMaxBufferLength );
    iSettingsBufferPtr.Set( iSettingsBuffer->Des());
    
    iTextResolver = CTextResolver::NewL( *( CCoeEnv::Static() ) );
    
    // Create the SUPL Settings API adapter. 
    iSUPLSettingsAdapter = CLocSUPLSettingsAdapter::NewL( *this ); 
    
    CActiveScheduler::Add( this );
	DEBUG( - CLocSUPLSettingsUiEngine::ConstructL );	
     
	}

// ---------------------------------------------------------------------------	
// void CLocSUPLSettingsUiEngine::Initalize
// Initializes the Settings Engine. This is an asynchronous call the
// completion of which is communicated through the observer
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettingsUiEngine::Initalize()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::Initalize );	
    iSUPLSettingsAdapter->Initialize();
	DEBUG( - CLocSUPLSettingsUiEngine::Initalize );	
    }
            
void CLocSUPLSettingsUiEngine::SelectConnectionL()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::SelectConnectionL );	
    if( iDialogActive || IsActive() || iConfirmQuery )
        {
        User::Leave( KErrInUse );
        }
        
    // In the case of a framework launching SUPL UI, the user is ignorant 
    // of the action. Hence, a confirmation dialog is popped up to intimate
    // the user that he needs to configure SUPL settings.
    
    iConfirmQuery = CAknGlobalConfirmationQuery::NewL();

    CAknSDData* secondaryData = CAknSDData::NewL( KCatUidLocationSuplSettingsUi,
                                                  ECmdSuplSettingsMissing,
                                                  KNullDesC8 );
 
    // Send the Confirmation query information to the Secondary display
    // The ownership of 'secondaryData' is taken up by the Global confirmation query
    iConfirmQuery->SetSecondaryDisplayData( secondaryData );
     
    // Load the SUPL IAP confirmation query prompt text from the resource file
	HBufC* promptText = StringLoader::LoadL( R_LOC_SUPL_IAP_QUERY );
		
	// Display the Confirmation query.  
    iConfirmQuery->ShowConfirmationQueryL( iStatus,
                                           *promptText,
                                           R_AVKON_SOFTKEYS_YES_NO__YES,
                                           R_QGN_NOTE_QUERY_ANIM );
                                               
    SetActive();  
    delete promptText; //    
    promptText = NULL;
	DEBUG( - CLocSUPLSettingsUiEngine::SelectConnectionL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::LaunchApConfiguratorL
// Launches the Access Point Configurator dialog
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::LaunchApConfiguratorL( TInt64 aSlpId, 
		MSuplServerEditorObserver* aEditorObserver )
    {     
	DEBUG( + CLocSUPLSettingsUiEngine::LaunchApConfiguratorL );	
	iCurrentSlpId =  aSlpId;
	iEditorObserver = aEditorObserver;

    if( iDialogActive )
        {
        User::Leave( KErrInUse );
        }
        
    // Zeroing the Temporary buffers so that it doesn't contain any
    // left over value from the previous access
    iSettingsBufferPtr.Zero();
    
    // Obtain the Server address value from the SUPL settings
    // API. If the value is set then it has to be shown to the user
    // as the existing value when he tries to configure the UI
    TRAP_IGNORE( iSUPLSettingsAdapter->GetIapNameL( iCurrentSlpId, iSettingsBufferPtr ) );
    	
    // Obtain the UID for the selected Access point so that the configurator
    // can be highlighted
    TUint32 highlightUid( 0 );
 
    TRAPD( error, highlightUid = ConvertIAPNameToIdL( iSettingsBufferPtr ) ); 
    
    if( error == KErrNotFound )
    	{
    	highlightUid = 0;
    	}
   
 	CCmApplicationSettingsUi* apHandler =  CCmApplicationSettingsUi::NewLC();
 	
 	iDialogActive = ETrue;
    TBool ret = EFalse;
    TCmSettingSelection selectionUid;
    selectionUid.iId = highlightUid;
    selectionUid.iResult = CMManager::EConnectionMethod ;
    TBearerFilterArray filter;
	CleanupClosePushL( filter );
	
	// Need access points for bearer type CSD and Packet data only
	filter.AppendL( KUidCSDBearerType );
	filter.AppendL( KUidPacketDataBearerType );

	// Run CCmApplicationSettingsUi dialog only for Access points (Connection methods)
	// selectionUid contains UID to be highlighted, on return it will contain UID of selected CM
    TRAP( error, ret = 
    	apHandler->RunApplicationSettingsL( 
    			selectionUid , CMManager::EShowConnectionMethods, filter 
    ) ); // | CMManager::EShowAlwaysAsk
    
    iDialogActive = EFalse;  
    if( error == CMManager::KErrConnectionNotFound )
        {
        // No AP defined, Show Note                
        ShowNoteL();        
        //User::Leave( error );    
        }   
    CleanupStack::PopAndDestroy( &filter );    
    CleanupStack::PopAndDestroy( apHandler );   
    
    if( ret )
        {            
        RCmManager cmManager;
		cmManager.OpenLC();
	
		RArray< TUint32 > cmArray;
		HBufC* cmName = NULL;
		RCmConnectionMethod method;
		// Get all the CMs into cmArray
		cmManager.ConnectionMethodL( cmArray, ETrue, EFalse );
		CleanupClosePushL( cmArray );
		
		// Get name of selected CM
		TInt count = cmArray.Count();
		for ( TInt i = 0; i < count; i++ )
		    {
		    if (selectionUid.iId == cmArray[i] )
		    	{
		    	method = cmManager.ConnectionMethodL( cmArray[i] );
		    	CleanupClosePushL( method );
		    	cmName = method.GetStringAttributeL( CMManager::ECmName );
		    	CleanupStack::PushL( cmName );
		    	iSettingsBufferPtr.Copy( cmName->Des() );
		    	CleanupStack::PopAndDestroy( cmName );
		    	CleanupStack::PopAndDestroy( &method );
		    	break;
		    	}		    
		    }
		CleanupStack::PopAndDestroy( &cmArray );    
    	CleanupStack::PopAndDestroy( &cmManager );          	

		if( iEditorObserver )
	       	{
	       	iEditorObserver->UpdateIapL( iSettingsBufferPtr );		       	
	       	}
        }
           
	DEBUG( - CLocSUPLSettingsUiEngine::LaunchApConfiguratorL );	
    }  

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::LaunchSuplUsageConfiguratorL
// Launches the SUPL Usage Configurator
//
// ---------------------------------------------------------------------------    
//
void CLocSUPLSettingsUiEngine::LaunchSuplUsageConfiguratorL()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::LaunchSuplUsageConfiguratorL );	
    if( iDialogActive )
        {
        User::Leave( KErrInUse );
        }
        
    // Allocate the descriptor array for Text settings for System
    // of measurement
    CDesCArrayFlat* items = new( ELeave ) CDesCArrayFlat( KNoofUsageSettings );
    CleanupStack::PushL( items );

    // Allocate all the Settings usage string
    
    // Append the radio-button list items
    items->AppendL( iSUPLSettingsAdapter->Automatic() );
    items->AppendL( iSUPLSettingsAdapter->AutomaticAtHome() );
    items->AppendL( iSUPLSettingsAdapter->AlwaysAsk() );
    items->AppendL( iSUPLSettingsAdapter->Disable() );
      
    // Obtain the current value for SUPL settings usage
    // This would be used for setting the default value for 
    // the text settings page               
    TInt currentSettings = iSUPLSettingsAdapter->GetSuplUsageIndex();
        
    CAknRadioButtonSettingPage* dlg = 
                        new ( ELeave )CAknRadioButtonSettingPage( R_LOC_SUPLUSAGE_SETTINGS,
                                                                  currentSettings, 
                                                                  items );
    // Settings Outstanding flag is marked True to enable dismissal incase
    // of a Cancel event                                                                  
    iDialogActive = ETrue;                                                                  
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        CLocSUPLSettingsAdapter::TLocSuplUsage newValue = 
                    static_cast<CLocSUPLSettingsAdapter::TLocSuplUsage>( currentSettings );
                    
        iSUPLSettingsAdapter->SetSuplUsageL( newValue );
        }
    
    iDialogActive = EFalse;           
    
    // Free the items resource
    CleanupStack::PopAndDestroy( items );   
	DEBUG( - CLocSUPLSettingsUiEngine::LaunchSuplUsageConfiguratorL );	
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::Close
// Closes the running Settings UI prematurely.
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::Close()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::Close );	
    Cancel();   
    
    // Cancel the Initalization request. This call Cancels if there is an 
    // outstanding request. If not, does nothing
    iSUPLSettingsAdapter->CancelInitialize();
     
    // If any of the dialogs are active then issue a cancel event on the 
    // dialogs
    if( iDialogActive )
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
	DEBUG( - CLocSUPLSettingsUiEngine::Close );	
    }

// ---------------------------------------------------------------------------
// TPtr16 CLocSUPLSettingsAdapter::GetSuplUsage
// ---------------------------------------------------------------------------
//  
TPtr16 CLocSUPLSettingsUiEngine::GetSuplUsageL()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::GetSuplUsageL );	
    // Zeroing the Temporary buffers so that it doesn't contain any
    // left over value from the previous access
    iSettingsBufferPtr.Zero();
    iConversionBufferPtr.Zero();
    
    // Obtain the Settings values from the SUPL settings API
    iSUPLSettingsAdapter->GetSuplUsageL( iSettingsBufferPtr );
    iConversionBufferPtr.Copy( iSettingsBufferPtr );
	DEBUG( - CLocSUPLSettingsUiEngine::GetSuplUsageL );	
    return iConversionBufferPtr;
    }
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLSettingsAdapter::GetSuplUsage
// ---------------------------------------------------------------------------
//
TInt CLocSUPLSettingsUiEngine::GetSuplUsage()
    {
    return iSUPLSettingsAdapter->GetSuplUsage();
    }
            				
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::HandleSuplSettingsChangeL
// Inherited from MLocSUPLSettingsAdapterObserver
// 
// ---------------------------------------------------------------------------		 		                                                    
//
void CLocSUPLSettingsUiEngine::HandleSuplSettingsChangeL( 
                                    TLocSUPLSettingsEvent aEvent )
    {
	DEBUG( + CLocSUPLSettingsUiEngine::HandleSuplSettingsChangeL );	
    iObserver.HandleSettingsEventL( 
        static_cast< MLocSUPLSettingsUiEngObserver::TSettingsEventType >( aEvent ) );		
	DEBUG( - CLocSUPLSettingsUiEngine::HandleSuplSettingsChangeL );	
    }
                                       
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RunL()
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::RunL()
    {    
	DEBUG( + CLocSUPLSettingsUiEngine::RunL );	
    // If the Confirmation query is present then we have to Pop up the 
    // IAP Selection dialog based on User Action
    if( iConfirmQuery )
        {        
        // Incase there has been a response from the User then the Key pressed
        // is present in the iStatus variable 
        if( EAknSoftkeyYes == iStatus.Int() )
            {
	        iObserver.HandleSettingsEventL( 
	                MLocSUPLSettingsUiEngObserver::EIAPSelectionComplete );                       
            }
        else
            {
            iObserver.HandleSettingsEventL( 
                    MLocSUPLSettingsUiEngObserver::EIAPSelectionCancelled );
            }
        
        // Destroy the IAP confirmation query handle    
        delete iConfirmQuery;
        iConfirmQuery = NULL;
        }
	DEBUG( - CLocSUPLSettingsUiEngine::RunL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::DoCancel()
//
// ---------------------------------------------------------------------------
//    
void CLocSUPLSettingsUiEngine::DoCancel()    
    {
	DEBUG( + CLocSUPLSettingsUiEngine::DoCancel );	
    if( iConfirmQuery )
        {
        // If the confirmation query is running Cancel the query
        iConfirmQuery->CancelConfirmationQuery();
        
        delete iConfirmQuery;
        iConfirmQuery = NULL;
        
        }
	DEBUG( - CLocSUPLSettingsUiEngine::DoCancel );	
    }

// ---------------------------------------------------------------------------
// TUint CLocSUPLSettingsUiEngine::ConvertIAPNameToIdL()
// Obtains the index UID corresponding to the Access point name
// 
// @param   aIAPName     Access point name
// @return  TUint       UID corresponding to the AP Name. Zero if 
//                      the name does not exist
// ---------------------------------------------------------------------------
//
TUint CLocSUPLSettingsUiEngine::ConvertIAPNameToIdL( const TDesC& aIAPName )
    {
	DEBUG( + CLocSUPLSettingsUiEngine::ConvertIAPNameToIdL );	
    TUint32 idVal = 0;
    
    RCmManager cmManager;

	cmManager.OpenLC();
	
	RArray< TUint32 > cmArray;	
	HBufC* name = NULL;
	RCmConnectionMethod method;
	// Get all the CMs into cmArray
	cmManager.ConnectionMethodL( cmArray, ETrue, EFalse );
	CleanupClosePushL( cmArray );
	
	TInt count = cmArray.Count();
	if( !count )
        {
        // No CM found
        User::Leave( KErrNotFound );
        }
    // Get uid for CM name       
	for ( TInt i = 0; i < count; i++ )
        {
        method = cmManager.ConnectionMethodL( cmArray[i] );
        CleanupClosePushL( method );
        name = method.GetStringAttributeL( CMManager::ECmName );
        CleanupStack::PushL( name );
        if (! aIAPName.Compare( name->Des() ))
            {
            idVal = cmArray[i];                               
            CleanupStack::PopAndDestroy( name );
            CleanupStack::PopAndDestroy( &method );
            break; 
            }
        CleanupStack::PopAndDestroy( name );     
        CleanupStack::PopAndDestroy( &method );   
        }
    CleanupStack::PopAndDestroy( &cmArray );    
	CleanupStack::PopAndDestroy( &cmManager ); 
    
    if( !idVal )
        {
        User::Leave( KErrBadName );
        }    
	DEBUG( - CLocSUPLSettingsUiEngine::ConvertIAPNameToIdL );	
    return idVal;
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::DisplayErrorL()
// Displays an Error note
//
// @param aError Error ID
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::DisplayErrorL( TInt aError )
    {
	DEBUG( + CLocSUPLSettingsUiEngine::DisplayErrorL );	
    // Show Error Note
    TPtrC buffer = iTextResolver->ResolveErrorString( aError );
    CAknErrorNote* errorNote = new ( ELeave ) CAknErrorNote;
    TRAP_IGNORE( errorNote->ExecuteLD( buffer ) );
	DEBUG( - CLocSUPLSettingsUiEngine::DisplayErrorL );	
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::ShowNoteL()
// Displays No access point defined note
//
// @param None
// ---------------------------------------------------------------------------
//
TInt CLocSUPLSettingsUiEngine::ShowNoteL()
    {
	DEBUG( + CLocSUPLSettingsUiEngine::ShowNoteL );	
    // Show Info Note "No Access Points Defined"       
    HBufC* tmp = StringLoader::LoadL( R_LOC_NO_AP_DEFINED );
    CleanupStack::PushL( tmp );    
    CAknNoteDialog* dlg =
        new ( ELeave ) CAknNoteDialog
                (
                 REINTERPRET_CAST( CEikDialog**, &dlg ),
                 CAknNoteDialog::ENoTone,
                 CAknNoteDialog::EShortTimeout
                );
    dlg->PrepareLC( R_GENERAL_NOTE );  
    dlg->SetTextL( *tmp );
    TInt retval = dlg->RunLD();
    CleanupStack::PopAndDestroy( tmp );  // temp    
	DEBUG( - CLocSUPLSettingsUiEngine::ShowNoteL );	
    return retval;
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::EditServerL()
// Displays No access point defined note
//
// @param iIsEditable
// @param aSlpId
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::EditServerL( TBool iIsEditable, TInt64 aSlpId )
    {
	DEBUG( + CLocSUPLSettingsUiEngine::EditServerL );		
	if( iEditorDlg )
		{		
		delete iEditorDlg;
		iEditorDlg = NULL;
		}
    iEditorDlg = CLocSUPLServerEditor::NewL( iIsEditable, *this, aSlpId );
    TInt error = iEditorDlg->ExecuteLD();
    if ( EEikCmdExit == error )
        {
        ( ( CAknViewAppUi* ) CEikonEnv::Static()->EikAppUi())->HandleCommandL( EEikCmdExit );
        }        
    iEditorDlg = NULL;                     	
    
	DEBUG( - CLocSUPLSettingsUiEngine::EditServerL );	
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SlpCount()
// returns total no of slp entries
//
// ---------------------------------------------------------------------------
//    
TInt CLocSUPLSettingsUiEngine::SlpCount()
	{
	TInt slpCount = 0;
	TRAP_IGNORE( iSUPLSettingsAdapter->SlpCountL( slpCount ) );
	return slpCount;
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::AddNewServerL()
// Method to add new server entry to SUPL settings with server attributes 
//
// ---------------------------------------------------------------------------
//    
void CLocSUPLSettingsUiEngine::AddNewServerL( 
        const TDesC& aServerAddress,
        const TDesC& aIapName,
        const TBool aUsageInHomeNw
        )
    {
	DEBUG( + CLocSUPLSettingsUiEngine::AddNewServerL );	
    iSUPLSettingsAdapter->AddNewServerL( 
    				aServerAddress, 
    				aIapName, 
    				aUsageInHomeNw );
 	DEBUG( - CLocSUPLSettingsUiEngine::AddNewServerL );	
 	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RemoveServerL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::RemoveServerL(
		const TInt64 aSlpId
		)
	{
	DEBUG( + CLocSUPLSettingsUiEngine::RemoveServerL );	
	iSUPLSettingsAdapter->RemoveServerL( aSlpId );
	DEBUG( - CLocSUPLSettingsUiEngine::RemoveServerL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::ChangePriorityL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::ChangePriorityL(
        TInt64 aSlpId, 
        TInt aPriority,
        TBool aDirection
		)
	{
	DEBUG( + CLocSUPLSettingsUiEngine::ChangePriorityL );	
	iSUPLSettingsAdapter->ChangePriorityL( aSlpId, aPriority, aDirection );		
	DEBUG( - CLocSUPLSettingsUiEngine::ChangePriorityL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetAllSlpL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetAllSlpL(
        RPointerArray<CServerParams>& aParamValues
		) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetAllSlpL );	
	iSUPLSettingsAdapter->GetAllSlpL( aParamValues ) ;
	DEBUG( - CLocSUPLSettingsUiEngine::GetAllSlpL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetSlpInfoFromIdL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetSlpInfoFromIdL(
	            const TInt64 aSlpId, 
	            CServerParams *aParamValues
	    ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetSlpInfoFromIdL );	
	iSUPLSettingsAdapter->GetSlpInfoFromIdL( aSlpId, aParamValues ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::GetSlpInfoFromIdL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetServerAddressL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetServerAddressL(
	            const TInt64 aSlpId, 
	            const TDesC& aServerAddress
	    ) 
	{
	DEBUG( + CLocSUPLSettingsUiEngine::SetServerAddressL );	
	iSUPLSettingsAdapter->SetServerAddressL( aSlpId, aServerAddress ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::SetServerAddressL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetServerAddressL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetServerAddressL( 
        TInt64 aSlpId, 
        TDes& aServerAddress
		) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetServerAddressL );	
	iSUPLSettingsAdapter->GetServerAddressL( aSlpId, aServerAddress );	
	DEBUG( - CLocSUPLSettingsUiEngine::GetServerAddressL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetIapNameL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetIapNameL(
	            const TInt64 aSlpId, 
	            const TDesC& aIapName
	    ) 
	{
	DEBUG( + CLocSUPLSettingsUiEngine::SetIapNameL );	
	iSUPLSettingsAdapter->SetIapNameL( aSlpId, aIapName ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::SetIapNameL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetServerEnabledFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetServerEnabledFlagL(
	            const TInt64 aSlpId, 
	            const TBool aEnable 
	    ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::SetServerEnabledFlagL );	
	iSUPLSettingsAdapter->SetServerEnabledFlagL( aSlpId, aEnable ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::SetServerEnabledFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetServerEnabledFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetServerEnabledFlagL(
        const TInt64 aSlpId, 
        TBool& aEnable 
		) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetServerEnabledFlagL );	
	iSUPLSettingsAdapter->GetServerEnabledFlagL( aSlpId, aEnable );	
	DEBUG( - CLocSUPLSettingsUiEngine::GetServerEnabledFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetUsageInHomwNwFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetUsageInHomwNwFlagL(
	            const TInt64 aSlpId, 
	            const TBool aHomeNwFlag 
	    ) 
	{
	DEBUG( + CLocSUPLSettingsUiEngine::SetUsageInHomwNwFlagL );	
	iSUPLSettingsAdapter->SetUsageInHomwNwFlagL( aSlpId, aHomeNwFlag ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::SetUsageInHomwNwFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetEditableFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetEditableFlagL( 
	            const TInt64 aSlpId, 
	            const TBool aEditFlag 
	    ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::SetEditableFlagL );	
	iSUPLSettingsAdapter->SetEditableFlagL( aSlpId, aEditFlag );
	DEBUG( - CLocSUPLSettingsUiEngine::SetEditableFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetEditableFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetEditableFlagL(
	            const TInt64 aSlpId, 
	            TBool& aEditFlag 
	    ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetEditableFlagL );	
	iSUPLSettingsAdapter->GetEditableFlagL( aSlpId, aEditFlag ) ;	
	DEBUG( - CLocSUPLSettingsUiEngine::GetEditableFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetActiveSessionsCountL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsUiEngine::GetActiveSessionsCountL( TInt& aAccessPointCount )
    {
    DEBUG( + CLocSUPLSettingsUiEngine::GetActiveSessionsCountL );
    iSUPLSettingsAdapter->GetActiveSessionsCountL( aAccessPointCount );
    DEBUG( - CLocSUPLSettingsUiEngine::GetActiveSessionsCountL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetTriggerParamsL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetTriggerParamsL(
        RPointerArray<CTriggerParams>& aParamValues
		) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetTriggerParamsL );	
	iSUPLSettingsAdapter->GetTriggerParamsL( aParamValues ) ;
	DEBUG( - CLocSUPLSettingsUiEngine::GetTriggerParamsL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::GetTriggerParamsL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::GetTriggerParamsL(  TInt64 aSessionId, 
        CTriggerParams*& aTrigger
		) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::GetTriggerParamsL );	
	iSUPLSettingsAdapter->GetTriggerParamsL( aSessionId, aTrigger ) ;
	DEBUG( - CLocSUPLSettingsUiEngine::GetTriggerParamsL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::OpenSessionL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::OpenSessionL( TInt64 aSessionId )
	{
	DEBUG( + CLocSUPLSettingsUiEngine::OpenSessionL );
	
	CLocSUPLSessionEditor* form = CLocSUPLSessionEditor::NewL( *this, aSessionId );  
	TInt error = form->ExecuteLD( ); 
    if ( EEikCmdExit == error )
        {
        ( ( CAknViewAppUi* ) CEikonEnv::Static()->EikAppUi())->HandleCommandL( EEikCmdExit );
        }                       	
    
	DEBUG( - CLocSUPLSettingsUiEngine::OpenSessionL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::ChangeNotificationStatusL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::ChangeNotificationStatusL( TInt64 aSessionId, TBool aTriggerNotificationStatus ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::ChangeNotificationStatusL );	
	iSUPLSettingsAdapter->ChangeNotificationStatusL( aSessionId, aTriggerNotificationStatus ) ;
	DEBUG( - CLocSUPLSettingsUiEngine::ChangeNotificationStatusL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RemoveTriggerSessionL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::RemoveTriggerSessionL( TInt64 aSessionId ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::RemoveTriggerSessionL );	
	iSUPLSettingsAdapter->RemoveTriggerSessionL( aSessionId ) ;
	DEBUG( - CLocSUPLSettingsUiEngine::RemoveTriggerSessionL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RemoveTriggerSessionsL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::RemoveTriggerSessionsL( RArray< TInt64 >& aSessionIdList ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::RemoveTriggerSessionsL );	
	for( TInt i=0; i<aSessionIdList.Count(); i++ )
		{
		iSUPLSettingsAdapter->RemoveTriggerSessionL( aSessionIdList[i] ) ;
		}
	DEBUG( - CLocSUPLSettingsUiEngine::RemoveTriggerSessionsL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RemoveAllTriggerSessionsL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::RemoveAllTriggerSessionsL( ) const
	{
	DEBUG( + CLocSUPLSettingsUiEngine::RemoveAllTriggerSessionsL );	
	RPointerArray<CTriggerParams> paramValues;
	CleanupClosePushL( paramValues );
	
	iSUPLSettingsAdapter->GetTriggerParamsL( paramValues ) ;
	
	for( TInt i=0; i<paramValues.Count(); i++ )
		{
        TInt64 sessionId;
        TUint64 outstandingTrigger;
        TUint64 interval;
        TBool notificationPresent;
        TBool triggerNotificationStatus;  
        CTriggerParams::TTriggerType triggerType; 
        CTriggerParams::TRequestType requestType;
        HBufC* sessionName = HBufC::NewLC( KMaxTriggerSessionNameLen ); 
                
        TInt errParams = paramValues[i]->Get(
    							sessionId, 
    							sessionName->Des(), 
    							notificationPresent, 
    							triggerNotificationStatus, 
    							triggerType,
    							requestType, 
    							outstandingTrigger, 
    							interval);
		iSUPLSettingsAdapter->RemoveTriggerSessionL( sessionId ) ;
		
		CleanupStack::PopAndDestroy( sessionName );
		}
		
    paramValues.ResetAndDestroy();
	CleanupStack::PopAndDestroy( &paramValues ); // paramValues
	DEBUG( - CLocSUPLSettingsUiEngine::RemoveAllTriggerSessionsL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::SetSessionObserver
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::SetSessionObserver( MLocSUPLSettingsSessionObserver* aObserver ) const
	{
	iSUPLSettingsAdapter->SetSessionObserver( aObserver );
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsUiEngine::RemoveSessionObserver
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsUiEngine::RemoveSessionObserver( ) const
	{
	iSUPLSettingsAdapter->RemoveSessionObserver( );
	}
	
// End of File
