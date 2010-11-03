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
* Description:  Source file for SUPL session UI view
*
*/


// System Include
#include <barsread.h> 		                // For TResourceReader
#include <bautils.h>
#include <StringLoader.h>
#include <aknViewAppUi.h>
#include <avkon.rsg>
#include <hlplch.h>			                // Help
#include <featmgr.h> 						// FeatureManager
#include <eikmenup.h>						// Menu pane
#include <eikmenub.h>						// Menu Bar
#include <AknUtils.h>
#include <AknQueryDialog.h> 

#include <locsuplsettingsui.rsg>

// User Include
#include "locsuplsettings.hrh"
#include "locsuplsessionview.h"
#include "locsuplsessioncontainer.h"
#include "locsettingsui.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"
#include "locsuplsettingsview.h"


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocSUPLSessionView::CLocSUPLSessionView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLSessionView::CLocSUPLSessionView( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	:iEventHandler( aEventHandler ),
	iEngine( aEngine )
	{
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionView::~CLocSUPLSessionView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSessionView::~CLocSUPLSessionView()
	{
	DEBUG( + CLocSUPLSessionView::~CLocSUPLSessionView );	
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    iContainer = NULL;
	DEBUG( - CLocSUPLSessionView::~CLocSUPLSessionView );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSessionView* CLocSUPLSessionView::NewL
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLSessionView* CLocSUPLSessionView::NewL( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
	DEBUG( + CLocSUPLSessionView::NewL );	
    CLocSUPLSessionView* self = NewLC( aEngine, aEventHandler );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSessionView::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionView* CLocSUPLSessionView::NewLC
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLSessionView* CLocSUPLSessionView::NewLC( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
    CLocSUPLSessionView* self = 
                new( ELeave ) CLocSUPLSessionView( aEngine,                          
    										        aEventHandler );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSessionView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionView::ConstructL()
	{   	
	// Call AknView's BaseConstrutL
	BaseConstructL( R_SUPLSETTINGS_SESSION_VIEW );	 
	
	// Needed for the Session editor Launch
	ConstructMenuAndCbaEarlyL();
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::HandleScreenSizeChange
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionView::HandleScreenSizeChange() 
	{
	if( iContainer )
		{
		iContainer->SetRect( ClientRect());		
		}
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::UpdateView
// -----------------------------------------------------------------------------
//	
void CLocSUPLSessionView::UpdateView()
    {
	DEBUG( + CLocSUPLSessionView::UpdateView );	
    if( iContainer )
		{
		TRAP_IGNORE( iContainer->UpdateScreenL() );	
		}	
	TRAP_IGNORE( ChangeMSKL() );		
	DEBUG( - CLocSUPLSessionView::UpdateView );	
    }

// -----------------------------------------------------------------------------
// TInt CLocSUPLSessionView::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLSessionView::SelectedItemIndex()
    {
    if( iContainer )
        {
        return iContainer->SelectedItemIndex();
        }
    else
        {
        return KErrNotFound;
        }
    }    
    
// ---------------------------------------------------------------------------
// void CLocSUPLSessionView::DoActivateL
// 
// ---------------------------------------------------------------------------	
//
void CLocSUPLSessionView::DoActivateL( const TVwsViewId& 	/* PrevViewId*/,
											   TUid		   	/* aCustomMessageId*/,
						  				 const TDesC8&		 aCustomMessage )
	{
	DEBUG( + CLocSUPLSessionView::DoActivateL );	

    // Destroy the existing container
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer=NULL;
        }						

    // Create new Container	
    iContainer = CLocSUPLSessionContainer::NewL( ClientRect(), 
                                                  iEngine,
                                                  this );
    iContainer->SetMopParent( this );
    AppUi()->AddToViewStackL( *this, iContainer );
    MenuBar()->SetContextMenuTitleResourceId( R_SUPLSETTINGS_SESSION_CONTEXTMENUBAR );
    ChangeMSKL();
	iEventHandler.HandleCommandL( MLocSUPLSettingsEventHandler::ELaunchViewComplete );	
	DEBUG( - CLocSUPLSessionView::DoActivateL );	
	}

// ---------------------------------------------------------------------------
// void CLocationSystemUiView::DoDeactivate
//
// ---------------------------------------------------------------------------
//		
void CLocSUPLSessionView::DoDeactivate()
	{
	DEBUG( + CLocSUPLSessionView::DoDeactivate );	
	// Destroy Container
	if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
	DEBUG( - CLocSUPLSessionView::DoDeactivate );	
	}

// ---------------------------------------------------------------------------
// TUid CLocSUPLSessionView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//	
TUid CLocSUPLSessionView::Id() const
	{
	return KLocSUPLSessionUiViewId;
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSessionView::HandleCommandL
//
// ---------------------------------------------------------------------------
//	
void CLocSUPLSessionView::HandleCommandL( TInt aCommand )	
	{
	DEBUG( + CLocSUPLSessionView::HandleCommandL );	
	switch ( aCommand )
        {
        // Handle all the internal view specific commands here
        case ELocSuplSessionOpen:
        case EAknSoftkeyOpen:
            {
            TInt itemIndex = iContainer->SelectedItemIndex();	
            iEngine.OpenSessionL( iContainer->CurrentSessionId() );
            TInt accessPointCount;
            TRAP_IGNORE( iEngine.GetActiveSessionsCountL( accessPointCount ) );
            if( accessPointCount == 0 )
            {
             // Launch the Settings View
              TRAP_IGNORE( AppUi()->ActivateLocalViewL ( KLocSUPLSettingsUiViewId ) );
            }
            else
            TRAP_IGNORE( iContainer->FocusToSessionDetailL( itemIndex ) );
            break;    
            } 
        case ELocSuplSessionTerminate:
        case ELocSuplSessionsTerminate:
        case ELocSuplSessionMSKTerminate:
        case ELocSuplSessionsMSKTerminate:
        	{
        	iContainer->SetSelectedIndex();
        	DeleteSessionL();
        	TInt accessPointCount;
          TRAP_IGNORE( iEngine.GetActiveSessionsCountL( accessPointCount ) );
          if( accessPointCount == 0 )
          {
             // Launch the Settings View
             TRAP_IGNORE( AppUi()->ActivateLocalViewL ( KLocSUPLSettingsUiViewId ) );
          }
        	break;
        	}
        case EAknCmdMark:
        case EAknCmdUnmark:
        case EAknMarkAll:
        case EAknUnmarkAll:
	       	{
			AknSelectionService::HandleMarkableListProcessCommandL(
						aCommand, &( iContainer->ListBoxL() ) );    
	        ChangeMSKL();
	       	break;
	       	}
       case EAknCmdHelp:
        	{
           	HlpLauncher::LaunchHelpApplicationL( iCoeEnv->WsSession(),
           										 AppUi()->AppHelpContextL() );
        	break;	
        	}
        case EAknCmdExit:
        case EEikCmdExit:
            {
            // The Exit command has to be transferred back to the application
            iEventHandler.HandleCommandL( EEikCmdExit );
            break;    
            }
        case EAknSoftkeyBack:
            {
            // Launch the Settings View
            TRAP_IGNORE( AppUi()->ActivateLocalViewL ( KLocSUPLSettingsUiViewId ) );
            break;    
            }
        case ELocSuplSessionMSKNoCmd:
        	{
        	// No operation will happen
        	break;
        	}
        default:
    	    {
    	    // View Switiching, View closure and other external
    	    // view related and AppUi related commands
    	    iEventHandler.HandleCommandL( aCommand );
            break;	
    	    }
        }
	DEBUG( - CLocSUPLSessionView::HandleCommandL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
void CLocSUPLSessionView::DynInitMenuPaneL( TInt          aResourceId, 
                                             CEikMenuPane* aMenuPane )
    {
	DEBUG( + CLocSUPLSessionView::DynInitMenuPaneL );	
    AknSelectionService::HandleMarkableListDynInitMenuPane( 
    								aResourceId, 
                                    aMenuPane, 
                                    &( iContainer->ListBoxL() ) );
    switch( aResourceId )
        {                               
        case R_SUPLSETTINGS_SERVER_MAIN_MENU: 
            { 
	        User::LeaveIfNull( aMenuPane );
	        //Handle Help Fature
	        HandleHelpFeature( *aMenuPane );
	        break;
        	}
        case R_SUPLSETTINGS_SESSION_MENU: 
            {             
	        User::LeaveIfNull( aMenuPane );	     
        	if( iContainer->SessionCount() <= 0 )
        		{	           
			   // aMenuPane->SetItemDimmed( 
			   // 					ELocSuplSessionOpen, 		ETrue );
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplSessionTerminate, 	ETrue );
        		}
        	else
        		{
        		if ( iContainer->SelectedCount() )
        			{
			  //  	aMenuPane->SetItemDimmed( 
			   // 					ELocSuplSessionOpen, 		ETrue );
					if( iContainer->SelectedCount() > 1 )
						{
						aMenuPane->SetItemDimmed( 
				    					ELocSuplSessionTerminate, 	ETrue );  	  
						aMenuPane->SetItemDimmed( 
				    					ELocSuplSessionsTerminate, 	EFalse ); 	    	      			
        				}
					else
						{
						aMenuPane->SetItemDimmed( 
				    					ELocSuplSessionTerminate, 	EFalse );  	  
						aMenuPane->SetItemDimmed( 
				    					ELocSuplSessionsTerminate, 	ETrue );  
						}
					}      		
        		else
        			{
			    //	aMenuPane->SetItemDimmed( 
			    	//				ELocSuplSessionOpen, 		EFalse );  
					aMenuPane->SetItemDimmed( 
			    					ELocSuplSessionTerminate, 	EFalse );  	  
					aMenuPane->SetItemDimmed( 
			    					ELocSuplSessionsTerminate, 	ETrue );  	      			
        			}   		
        		}        		      		
	        break;
        	}
    	case R_SUPLSETTINGS_SESSION_CONTEXTMENU:
        	{
        	if( iContainer->SelectedCount() == 1 )
        		{
				aMenuPane->SetItemDimmed( 
		    					ELocSuplSessionMSKTerminate, 	EFalse );  	  
				aMenuPane->SetItemDimmed( 
		    					ELocSuplSessionsMSKTerminate, 	ETrue );  
        		}
        	else
        	    {
				aMenuPane->SetItemDimmed( 
		    					ELocSuplSessionMSKTerminate, 	EFalse );  	  
				aMenuPane->SetItemDimmed( 
		    					ELocSuplSessionsMSKTerminate, 	ETrue );  
        	    }
        	break;
        	}
        default:
    		{
    		break;
    		}
        }
	DEBUG( - CLocSUPLSessionView::DynInitMenuPaneL );	
    }
    
// -----------------------------------------------------------------------------
// CLocSUPLSessionView::HandleHelpFeature
// -----------------------------------------------------------------------------
void CLocSUPLSessionView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
    {
	DEBUG( + CLocSUPLSessionView::HandleHelpFeature );	
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, EFalse );
        }
    else
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, ETrue );
        }
	DEBUG( - CLocSUPLSessionView::HandleHelpFeature );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::DeletionConfirmationQueryL
// ----------------------------------------------------------------------------
//
TInt CLocSUPLSessionView::DeletionConfirmationQueryL( const TInt aAmount )
    {    
    HBufC* noteText = NULL;    
    if ( aAmount > 1 )
        {
        noteText = StringLoader::LoadL( R_LOC_SUPL_QUERY_TERMINATE_MULTIPLE, aAmount, iEikonEnv );
        }
    else
        {
        noteText = StringLoader::LoadL( R_LOC_SUPL_QUERY_TERMINATE, iContainer->CurrentSessionName(), iEikonEnv );
        }
    CAknQueryDialog* query = new( ELeave ) CAknQueryDialog( *noteText );    
    TInt ret = query->ExecuteLD( R_LOC_SUPL_DELETE_QUERY );
    delete noteText; //noteText    
    noteText = NULL;
    return ret;
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::DeleteSessionL
// ----------------------------------------------------------------------------
//
void CLocSUPLSessionView::DeleteSessionL()
	{
	DEBUG( + CLocSUPLSessionView::DeleteSessionL );	
	TInt selCount 	= iContainer->SelectedCount();
	TInt sessionCount = iContainer->SessionCount();
	if( !sessionCount )
		return;
		
	if( selCount )
		{
		if( DeletionConfirmationQueryL( selCount ) )
			{
		    const CArrayFix<TInt>* markedIndexes = NULL;
		    markedIndexes = iContainer->ListBoxL().SelectionIndexes();
				
			RArray<TInt64> markedItemIds;					
			GetSelectedItemIdsL( markedItemIds, *markedIndexes );
		
			TRAP_IGNORE( iEngine.RemoveTriggerSessionsL( markedItemIds ) );		
					
			markedItemIds.Close();	
			}
		}
	else
		{
		if( DeletionConfirmationQueryL( 1 ) )
			{
			TRAP_IGNORE( iEngine.RemoveTriggerSessionL( iContainer->CurrentSessionId() ) );
			}		

		}
	DEBUG( - CLocSUPLSessionView::DeleteSessionL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::GetSelectedItemIdsL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionView::GetSelectedItemIdsL(
    RArray<TInt64>& aSelectedItems,
    const CArrayFix<TInt>& aSelectedIndexes ) const
    {
    aSelectedItems.Reset();
    TInt itemCount = iContainer->SessionCount();
    TInt selectionCount = aSelectedIndexes.Count();
    for ( TInt i( 0 ); i < selectionCount; ++i )
        {
        TInt index = aSelectedIndexes[i];
        User::LeaveIfError( aSelectedItems.Append( iContainer->SessionIds()[index] ) );
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionView::ChangeMSKL
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionView::ChangeMSKL() 
	{
	DEBUG( + CLocSUPLSessionView::ChangeMSKL );	
	// Listbox takes all event even if it doesn't use them	
	if( !Cba() || !iContainer )
	    {
	   	return;
	    }
	if( iContainer->SelectedCount() )
		{
		TRAP_IGNORE( 
			Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_CONTEXTOPTIONS_BACK ) ); 	
		}
	else
		{
		if( iContainer->SessionCount() )
		    {
    		TRAP_IGNORE( 
    			Cba()->SetCommandSetL ( R_SUPLSETTINGS_SESSION_OPTIONS_BACK_OPEN ) ); 	
		    }	
		else
		    {
    		TRAP_IGNORE( 
    			Cba()->SetCommandSetL ( R_SUPLSETTINGS_SESSION_OPTIONS_BACK ) ); 
		    }
		}

	Cba()->DrawDeferred();  	
	DEBUG( - CLocSUPLSessionView::ChangeMSKL );	
	}
	

// End of file
