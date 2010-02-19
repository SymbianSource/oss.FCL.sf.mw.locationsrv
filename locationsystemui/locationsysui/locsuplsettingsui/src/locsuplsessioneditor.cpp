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
* Description:  Implements the CLocSUPLSessionEditor class
*
*/



#include <aknappui.h>
#include <AknIconArray.h>       // Icon Array
#include <AknsUtils.h>
#include <AknsSkinInstance.h>
#include <akntitle.h>       	// CAknTitlePane
#include <avkon.rsg>
#include <avkon.hrh>
#include <e32math.h>
#include <eikapp.h>
#include <eikaufty.h>
#include <eikcapc.h>
#include <eikedwin.h>
#include <eikenv.h>
#include <eikmenup.h>
#include <eikmenub.h>
#include <eikspane.h>       	// Status pane
#include <barsread.h>           // For TResourceReader
#include <bautils.h> 
#include <AknQueryDialog.h>
#include <data_caging_path_literals.hrh>
#include <gulicon.h>            // Gul Icon
#include <StringLoader.h>
#include <hlplch.h>

#include <epos_csuplsettingsconstants.h>
#include <epos_csuplsettingparams.h>

#include <locsuplsettingsui.rsg>
#include "locsuplsettings.hrh"
#include "locsuplsessioneditor.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"

// Constants
const TInt KMinToSecs = 60;
const TInt KHrToSecs  = 3600;
const TInt KdayToSecs = 86400;
const TInt KRealWidth = 50;

// ============================ LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::CLocSUPLSessionEditor
// -----------------------------------------------------------------------------
//
CLocSUPLSessionEditor::CLocSUPLSessionEditor( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
		):iEngine( aEngine ), iSessionId( aSessionId )
    {  	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::ConstructL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::ConstructL()
    {
	DEBUG( + CLocSUPLSessionEditor::ConstructL );
    CAknForm::ConstructL( R_SUPLSESSION_EDITOR_MENU_BAR );

    // Save original title
    CEikStatusPane* statusPane = iAvkonAppUi->StatusPane();
    iTitlePane = ( CAknTitlePane* ) statusPane->ControlL(
        TUid::Uid( EEikStatusPaneUidTitle ) );
    iOriginalTitle = iTitlePane->Text()->AllocL();

    iSessionName = HBufC::NewL( KMaxTriggerSessionNameLen ); 
    
    // Set Session Observer
    iEngine.SetSessionObserver( this );
	
	DEBUG( - CLocSUPLSessionEditor::ConstructL );
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::NewL
// -----------------------------------------------------------------------------
//
CLocSUPLSessionEditor* CLocSUPLSessionEditor::NewL( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
		)
	{	
    CLocSUPLSessionEditor* self = NewLC( aEngine, aSessionId );
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::NewLC
// -----------------------------------------------------------------------------
//
CLocSUPLSessionEditor* CLocSUPLSessionEditor::NewLC( 
                            CLocSUPLSettingsUiEngine&       aEngine,
        					TInt64 aSessionId
		)
    {
    CLocSUPLSessionEditor* self = new ( ELeave ) 
    			CLocSUPLSessionEditor( aEngine, aSessionId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::~CLocSUPLSessionEditor
// -----------------------------------------------------------------------------
//
CLocSUPLSessionEditor::~CLocSUPLSessionEditor()
    {
	DEBUG( + CLocSUPLSessionEditor::~CLocSUPLSessionEditor );
	
	// Delete Buffer
	delete iSessionName;

	// Remove the Observer set
	iEngine.RemoveSessionObserver();
	
    // Restore title pane. Ownership of iTitlePane transferred.
    if ( iTitlePane && iOriginalTitle )
        {
        iTitlePane->SetText( iOriginalTitle );
        }
	
	DEBUG( - CLocSUPLSessionEditor::~CLocSUPLSessionEditor );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
	DEBUG( + CLocSUPLSessionEditor::DynInitMenuPaneL );
    CAknForm::DynInitMenuPaneL( aResourceId, aMenuPane );

    if ( aResourceId == R_AVKON_FORM_MENUPANE )
        {
        // We do not want to allow the user to modify our Form therefore
        // Disable the Label, Add, Delete, and Save Field
        aMenuPane->SetItemDimmed( EAknFormCmdLabel, ETrue );
        aMenuPane->SetItemDimmed( EAknFormCmdAdd, 	ETrue );
		aMenuPane->SetItemDimmed( EAknFormCmdDelete,ETrue );
        aMenuPane->SetItemDimmed( EAknFormCmdSave, 	ETrue );
        aMenuPane->SetItemDimmed( EAknFormCmdEdit, 	ETrue );
		
    	aMenuPane->SetItemDimmed( ELocSessionEditorTerminate, 	EFalse );	 	       			    	
        				
		switch( IdOfFocusControl() )
			{
			case ELocSuplSessionNotificationStatus:
				{
				aMenuPane->SetItemDimmed( ELocSessionEditorChange, 	EFalse );
				break;
				}
			case ELocSuplSessionType:
			case ELocSuplSessionTimePeriod:
			case ELocSuplSessionTriggerInterval:
			case ELocSuplSessionOutstandingTrigger:
			default:
				{
				aMenuPane->SetItemDimmed( ELocSessionEditorChange, 	ETrue );
				break;
				}
			}			     
        }
	DEBUG( - CLocSUPLSessionEditor::DynInitMenuPaneL );
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::ProcessCommandL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::ProcessCommandL( TInt aCommandId )
	{
	DEBUG1( + CLocSUPLSessionEditor::ProcessCommandL aCommandId=%d, aCommandId );
	CAknForm::ProcessCommandL( aCommandId );

	switch ( aCommandId )
       	{
       	case ELocSessionEditorChange:
       		{
       		TBool status = ETrue;
            CAknPopupFieldText* notificationStatus =
                            (CAknPopupFieldText*)Control( ELocSuplSessionNotificationStatus );
            if(notificationStatus)
                {          
                if( notificationStatus->CurrentValueIndex() == 0 )
                    {
                    status = EFalse;
                    }
                }
                  
       	    iEngine.ChangeNotificationStatusL( iSessionId, status );					
       		break;
       		}
       	case ELocSessionEditorTerminate:
       		{
       		if( DeleteConfirmationQueryL() )
       			{
       			iEngine.RemoveTriggerSessionL( iSessionId );
        		   // Return back to Session view
	       		   TryExitL( EAknSoftkeyBack );
      			}       			
       		break;
       		}
       	case EAknCmdHelp:
       		{
          HlpLauncher::LaunchHelpApplicationL( iCoeEnv->WsSession(),
                                              iAvkonAppUi->AppHelpContextL() );			
       		break;
       		}
       	case EEikCmdExit:
       		{
       		// The Exit is handled by the Application UI
       		TryExitL( EEikCmdExit );
       		break;
       		}
		default:
			break;
       	}
	DEBUG( - CLocSUPLSessionEditor::ProcessCommandL );
	}
	
// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CLocSUPLSessionEditor::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                             TEventCode aType )
    {
	DEBUG( CLocSUPLSessionEditor::OfferKeyEventL );
	if ( aType == EEventKey )
		{
		if( aKeyEvent.iCode == EKeyBackspace )		
            {
        	ProcessCommandL( ELocSessionEditorTerminate );            	
        	return EKeyWasConsumed;
            }      

		if ( aKeyEvent.iCode == EKeyEnter || aKeyEvent.iScanCode == EStdKeyEnter ) 
			{
		    if ( IdOfFocusControl() == ELocSuplSessionNotificationStatus )
		    	{
				ProcessCommandL( ELocSessionEditorChange );  	
        	    return EKeyWasConsumed;  	
		    	}
			}         
		}
		
    return CAknForm::OfferKeyEventL( aKeyEvent, aType );    
    }
    
// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor ::OkToExitL()
// ---------------------------------------------------------------------------
//
TBool CLocSUPLSessionEditor::OkToExitL( TInt aButtonId )
    {
	DEBUG1( + CLocSUPLSessionEditor::OkToExitL aButtonId=%d, aButtonId );
    TBool retVal = EFalse;
    	
    switch( aButtonId )
        {
        case EAknSoftkeyChange:
        	{
        	ProcessCommandL( ELocSessionEditorChange );
        	break;
        	}
        case EAknSoftkeyOptions: //when LSK is pressed
            {
            //display menu specific to editor
            DisplayMenuL();
            break;
            }            
        case EAknSoftkeyBack: 
        case EEikCmdExit: 
            {
            //close editor            
            retVal = ETrue;
            break;
            }
        case ELocServerEditorNoCmd:
        default: // close editor in all other cases
            {
            break;
            }
        }
	DEBUG1( - CLocSUPLSessionEditor::OkToExitL retVal=%d, retVal );
    return retVal;
    }
    
// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor ::PreLayoutDynInitL()
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::PreLayoutDynInitL()
    {    
	DEBUG( + CLocSUPLSessionEditor::PreLayoutDynInitL );
    CAknForm::PreLayoutDynInitL();
    
    //Load the values into the editor fields  
    LoadFormValuesFromDataL();
    
	DEBUG( - CLocSUPLSessionEditor::PreLayoutDynInitL );
    }

// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor ::PostLayoutDynInitL()
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::PostLayoutDynInitL()
    {
	DEBUG( + CLocSUPLSessionEditor::PostLayoutDynInitL );
    CAknForm::PostLayoutDynInitL();   

    HandleMSKCaptionL();			    	
	
	DEBUG( - CLocSUPLSessionEditor::PostLayoutDynInitL );
    }
    
// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor ::LineChangedL()
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::LineChangedL(TInt /*aControlId*/)
    {
    HandleMSKCaptionL(); 
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::ExecuteLD
// -----------------------------------------------------------------------------
//
TInt CLocSUPLSessionEditor::ExecuteLD()
    {    
	return CAknForm::ExecuteLD( R_SUPLSESSION_EDITOR_FORM_DIALOG );    	
    }

// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor ::LoadFormValuesFromDataL()
// Called when the form is executed and when the user chooses to discard changes
// in QuerySaveChangesL (via  DoNotSaveFormDataL).
// ---------------------------------------------------------------------------
// 
void CLocSUPLSessionEditor::LoadFormValuesFromDataL()
    {
	DEBUG( + CLocSUPLSessionEditor::LoadFormValuesFromDataL );  
   	
    TUint64 outstandingTriggers = 0;
    TUint64 timeInterval = 0;
    TUint64 timePeriod = 0;
    TBool notificationPresent = EFalse;
    TBool triggerNotificationStatus = EFalse; 
    CTriggerParams::TTriggerType triggerType; 
    CTriggerParams::TRequestType requestType;
   	
    iSessionName->Des().Zero();
    
   	CTriggerParams *triggerParams = CTriggerParams::NewL();
   	CleanupStack::PushL( triggerParams );
   	
   	TInt err = KErrNone;
   	TRAP( err, iEngine.GetTriggerParamsL( iSessionId, triggerParams ) );
	    		    
    if( !err )
        {
        triggerParams->Get(
		    							iSessionId, 
		    							iSessionName->Des(), 
		    							notificationPresent, 
		    							triggerNotificationStatus, 
		    							triggerType,
		    							requestType, 
		    							outstandingTriggers, 
		    							timeInterval);
    
        timePeriod = timeInterval * outstandingTriggers;
        }
    							
   	CleanupStack::PopAndDestroy( triggerParams );
   	
    // update the title pane caption
    if( !iSessionName->Length() )
        {
        HBufC* unknownString = StringLoader::LoadLC( R_LOC_SERVICES_TITLE_UNKNOWN ); 
        iSessionName->Des().Copy( *unknownString );
        CleanupStack::PopAndDestroy( unknownString );
        }

    ChangeTitlePaneTextL( *iSessionName ); 

    // Update the Editor Fields
    CAknPopupFieldText* notificationStatusPopupFieldText = 
    			( CAknPopupFieldText* ) Control( ELocSuplSessionNotificationStatus );
    if( notificationStatusPopupFieldText )
        {  
        if( triggerNotificationStatus  )
        	notificationStatusPopupFieldText->SetCurrentValueIndex( 0 );                  	
        else
        	notificationStatusPopupFieldText->SetCurrentValueIndex( 1 );    
        } 

    CAknPopupFieldText* typePopupFieldText = 
    			( CAknPopupFieldText* ) Control( ELocSuplSessionType );
    if( typePopupFieldText )
        {  
        typePopupFieldText->SetCurrentValueIndex( 0 );          
        }   

    CAknIntegerEdwin* outstandingTriggersEdwin = 
    			static_cast < CAknIntegerEdwin* > ( ControlOrNull( ELocSuplSessionOutstandingTrigger ) );           
    if( outstandingTriggersEdwin )
        { 
        outstandingTriggersEdwin->SetValueL( outstandingTriggers );
    	
    	outstandingTriggersEdwin->DrawNow();
        } 
   	
   	// Calculate the Time period
	HBufC* periodString = NULL;
	HBufC* intervalString = NULL;
	GetTimeStringL( intervalString, timeInterval );
	GetTimeStringL( periodString, timePeriod );
	CleanupStack::PushL( periodString );
	CleanupStack::PushL( intervalString );

    CEikEdwin* timePeriodEdwin = 
    			static_cast < CEikEdwin* > ( ControlOrNull( ELocSuplSessionTimePeriod ) );           
    if( timePeriodEdwin )
        { 
        timePeriodEdwin->SetTextL( periodString );
    	
    	timePeriodEdwin->DrawNow();
        }     

    CEikEdwin* timeIntervalEdwin = 
    			static_cast < CEikEdwin* > ( ControlOrNull( ELocSuplSessionTriggerInterval ) );           
    if( timeIntervalEdwin )
        { 
        timeIntervalEdwin->SetTextL( intervalString );
    	
    	timeIntervalEdwin->DrawNow();
        }                                 	
        
	CleanupStack::PopAndDestroy( );	//	intervalString
	CleanupStack::PopAndDestroy( );	//	periodString
		
	TBool menuShowing = MenuShowing();
	if( menuShowing )
	    HideMenu();
	
    UpdatePageL( ETrue );
    
    if( menuShowing )
        DisplayMenuL();
	DEBUG( - CLocSUPLSessionEditor::LoadFormValuesFromDataL );
    }
    
// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor::ChangeTitlePaneTextL()
// Set the Title Text
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::ChangeTitlePaneTextL( TDesC& aText )
    {
	DEBUG( + CLocSUPLSessionEditor::ChangeTitlePaneTextL );
    // Obtain the title from the Status Pane
    CAknTitlePane* title = static_cast < CAknTitlePane* >( iEikonEnv->AppUiFactory()->
    	StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
       
    // Set the Title's buffer    
    HBufC* buf = aText.AllocL();
    title->SetText( buf ); // Takes ownership of buf
    title->DrawNow();
	DEBUG( - CLocSUPLSessionEditor::ChangeTitlePaneTextL );
    }     

// ---------------------------------------------------------------------------
// CLocSUPLSessionEditor::TogglePopupFieldControlL()
// it toggles  value of popupfield control
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::TogglePopupFieldControlL( TInt aControlId )
    {
	DEBUG( + CLocSUPLSessionEditor::TogglePopupFieldControlL );
    CAknPopupFieldText* popupFieldText =
                    		( CAknPopupFieldText* ) Control( aControlId );
                    		
	if( popupFieldText )
		{          
		if( popupFieldText->CurrentValueIndex() == 0 )
		  	popupFieldText->SetCurrentValueIndex( 1 );
		else
		  	popupFieldText->SetCurrentValueIndex( 0 );    

		UpdatePageL( ETrue );
		}
	DEBUG( - CLocSUPLSessionEditor::TogglePopupFieldControlL );
    }
 
// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::HandleMSKCaptionL()
// It changes caption of MSK key
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::HandleMSKCaptionL()
    {
	DEBUG( + CLocSUPLSessionEditor::HandleMSKCaptionL );
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    CleanupStack::PushL( cba );
    
    //attribute specific cba
	switch ( IdOfFocusControl() )
       	{
		case ELocSuplSessionNotificationStatus:
			{
       		cba->SetCommandSetL( R_SUPLSESSION_EDITOR_OPTIONS_CHANGE_BACK );
			break;
			}
		case ELocSuplSessionType:
		case ELocSuplSessionTimePeriod:
		case ELocSuplSessionTriggerInterval:
		case ELocSuplSessionOutstandingTrigger:
			{
       		cba->SetCommandSetL( R_SUPLSESSION_EDITOR_OPTIONS_NOMSK_BACK );
			break;
			}
		default:
			{
       		cba->SetCommandSetL( R_SUPLSESSION_EDITOR_OPTIONS_CHANGE_BACK );
			break;
			}
       	}	    	
    cba->DrawDeferred();    
    CleanupStack::Pop( cba );    
	DEBUG( - CLocSUPLSessionEditor::HandleMSKCaptionL );
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::DeleteConfirmationQueryL
// ----------------------------------------------------------------------------
//
TInt CLocSUPLSessionEditor::DeleteConfirmationQueryL()
    {    
    HBufC* noteText = NULL;    
    noteText = StringLoader::LoadLC( R_LOC_SUPL_QUERY_TERMINATE, *iSessionName, iEikonEnv );
    CAknQueryDialog* query = new( ELeave ) CAknQueryDialog( *noteText );    
    TInt ret = query->ExecuteLD( R_LOC_SUPL_DELETE_QUERY );
    CleanupStack::PopAndDestroy( noteText );   
    return ret;
    }
	
// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::UpdateSessionL
// Calls function to load the values into the editor fields  
// ----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::UpdateSessionL( TUpdateType aType )
    {   	
    if( ESessionDeleted == aType )
        {
        TryExitL( EAknSoftkeyBack );
        }
    else
        {
        LoadFormValuesFromDataL();
        }
	}
	
// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::SessionId
// Returns the session id
// ----------------------------------------------------------------------------
//
TInt64 CLocSUPLSessionEditor::SessionId()
    {  
	return iSessionId;
	}
	
// -----------------------------------------------------------------------------
// CLocSUPLSessionEditor::GetTimeStringL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionEditor::GetTimeStringL(HBufC*& aTimeString, TUint aSeconds)
    {  
    TBuf<KRealWidth> tempString;

	if( !aSeconds )
		{
        tempString.Num(aSeconds);
        aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_MINUTES, tempString ); 
        return;
		}
		
	TReal hrs = TReal(aSeconds)/KHrToSecs;
    TReal approxValue;
    TRealFormat fmt(KDefaultRealWidth, 2);
	if( hrs < 1 )
		{
	    TReal mins = TReal(aSeconds)/KMinToSecs;
		Math::Round( approxValue, mins, 2 );
        tempString.Num(approxValue, fmt);
        aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_MINUTES, tempString ); 
		}
	else if( hrs == 1 )
		{
        aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_ONEHOUR ); 
		}
	else
		{
	    TReal days = TReal(aSeconds)/KdayToSecs;
    	if( days < 1 )
    		{
    		Math::Round( approxValue, hrs, 2 );
        	tempString.Num(approxValue, fmt);
            aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_HOURS, tempString ); 
    		}
    	else if( days == 1 )
    		{
            aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_ONEDAY ); 
    		}
    	else
    		{
    		Math::Round( approxValue, days, 2 );
        	tempString.Num(approxValue, fmt);
            aTimeString = StringLoader::LoadL( R_LOC_SUPL_TRIGGER_TIME_DAYS, tempString ); 
    		}
		}
	}

// End of file
