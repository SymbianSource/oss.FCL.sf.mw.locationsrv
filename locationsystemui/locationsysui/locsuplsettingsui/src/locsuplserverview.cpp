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
* Description:  Source file for SUPL server UI view
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
#include "locsuplserverview.h"
#include "locsuplservercontainer.h"
#include "locsettingsui.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"
#include "locsuplsettingsview.h"


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocSUPLServerView::CLocSUPLServerView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLServerView::CLocSUPLServerView( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	:iEventHandler( aEventHandler ),
	iEngine( aEngine )
	{
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerView::~CLocSUPLServerView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLServerView::~CLocSUPLServerView()
	{
	DEBUG( + CLocSUPLServerView::~CLocSUPLServerView );	
	
	// deleting the new contaier used to while prioritizing the servers
    if ( iNewContainer )
         {
         AppUi()->RemoveFromViewStack( *this, iNewContainer );
         delete iNewContainer;
         iNewContainer = NULL;
         }
    
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    iContainer = NULL;
	DEBUG( - CLocSUPLServerView::~CLocSUPLServerView );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLServerView* CLocSUPLServerView::NewL
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLServerView* CLocSUPLServerView::NewL( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
	DEBUG( + CLocSUPLServerView::NewL );	
    CLocSUPLServerView* self = NewLC( aEngine, aEventHandler );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLServerView::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerView* CLocSUPLServerView::NewLC
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLServerView* CLocSUPLServerView::NewLC( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
    CLocSUPLServerView* self = 
                new( ELeave ) CLocSUPLServerView( aEngine,                          
    										        aEventHandler );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLServerView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocSUPLServerView::ConstructL()
	{   	
	// Call AknView's BaseConstrutL
	BaseConstructL( R_SUPLSETTINGS_SERVER_VIEW );	 
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::HandleScreenSizeChange
// -----------------------------------------------------------------------------
//
void CLocSUPLServerView::HandleScreenSizeChange() 
	{
	if( iContainer )
		{
		iContainer->SetRect( ClientRect());		
		}
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::UpdateView
// -----------------------------------------------------------------------------
//	
void CLocSUPLServerView::UpdateView()
    {
	DEBUG( + CLocSUPLServerView::UpdateView );	
    if( iContainer )
		{
		TRAP_IGNORE( iContainer->UpdateScreenL() );	
		}	
	TRAP_IGNORE( ChangeMSKL() );		
	DEBUG( - CLocSUPLServerView::UpdateView );	
    }

// -----------------------------------------------------------------------------
// TInt CLocSUPLServerView::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLServerView::SelectedItemIndex()
    {
    if (iNewContainer)
        {
        return iNewContainer->SelectedItemIndex();
        }
    else if( iContainer )
        {
        return iContainer->SelectedItemIndex();
        }
    else
        {
        return KErrNotFound;
        }
    }    
    
// ---------------------------------------------------------------------------
// void CLocSUPLServerView::DoActivateL
// 
// ---------------------------------------------------------------------------	
//
void CLocSUPLServerView::DoActivateL( const TVwsViewId& 	/* PrevViewId*/,
											   TUid		   	/* aCustomMessageId*/,
						  				 const TDesC8&		/* aCustomMessage */)
	{
	DEBUG( + CLocSUPLServerView::DoActivateL );	
	// Destroy the existing container
	if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer=NULL;
        }						
    
    // Create new Container	
	iContainer = CLocSUPLServerContainer::NewL( ClientRect(), 
	                                              iEngine,
	                                              this );
	iContainer->SetMopParent( this );
	AppUi()->AddToViewStackL( *this, iContainer );
	MenuBar()->SetContextMenuTitleResourceId( R_SUPLSETTINGS_SERVER_CONTEXTMENUBAR );
	ChangeMSKL();
	DEBUG( - CLocSUPLServerView::DoActivateL );	
	}

// ---------------------------------------------------------------------------
// void CLocationSystemUiView::DoDeactivate
//
// ---------------------------------------------------------------------------
//		
void CLocSUPLServerView::DoDeactivate()
	{
	DEBUG( + CLocSUPLServerView::DoDeactivate );	
	// Destroy Container
	if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
	DEBUG( - CLocSUPLServerView::DoDeactivate );	
	}

// ---------------------------------------------------------------------------
// TUid CLocSUPLServerView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//	
TUid CLocSUPLServerView::Id() const
	{
	return KLocSUPLServerUiViewId;
	}

// ---------------------------------------------------------------------------
// void CLocSUPLServerView::HandleCommandL
//
// ---------------------------------------------------------------------------
//	
void CLocSUPLServerView::HandleCommandL( TInt aCommand )	
	{
	DEBUG( + CLocSUPLServerView::HandleCommandL );	
	switch ( aCommand )
        {
        // Handle all the internal view specific commands here
        case ELocSuplServerOpen:
        case ELocSuplServerMSKOpen:
            {
            iEngine.EditServerL( EFalse, iContainer->CurrentSlpId() );
            break;    
            } 
        case ELocSuplServerEnable:
        case ELocSuplServerMSKEnable:
        	{
        	EnableServerL();
        	break;
        	}
        case ELocSuplServerDisable:
        case ELocSuplServerMSKDisable:
        	{
        	DisableServerL();
        	break;
        	}
        case ELocSuplServerNew:
        	{
            iEngine.EditServerL( ETrue, NULL );
            break;    
        	}
        case ELocSuplServerDelete:
        	{
        	iContainer->SetSelectedIndex();
        	DeleteServerL();
        	break;
        	}
        case ELocSuplServerPrioritize:
        	{
        	/** iNewContainer is constructed while prioritizing the servers.
        	 * Since the focus cannot be disabled once it is enabled dynamically, thus 
        	 * prioritizing happens in the new container and the original container is updated.
        	 * The iNewContainer is deleted once prioritizing is over. (TC Id: ou1cimx1#219289)
        	**/
        	if(iNewContainer)
        	    {
                delete iNewContainer;
                iNewContainer = NULL;
        	    }
        	iStartedPrioritize = ETrue;
        	iSelectedIndex = SelectedItemIndex();
        	
        	iNewContainer = CLocSUPLServerContainer::NewL(ClientRect(), iEngine, this);
            iNewContainer->SetMopParent( this );
            AppUi()->AddToViewStackL( *this, iNewContainer );
            MenuBar()->SetContextMenuTitleResourceId( R_SUPLSETTINGS_SERVER_CONTEXTMENUBAR );         
            iSelectedSlpId = iContainer->CurrentSlpId();

        	ChangeMSKL();
        	HandleCommandL( EAknCmdMark );
        	break;
        	}
        case EAknSoftkeyOk:
        	{
        	TInt currentIndex = SelectedItemIndex();        	
        	if ( currentIndex == iSelectedIndex )
        		{
	        	HandleCommandL( EAknUnmarkAll );
	        	UpdateView(); 
        	  // deleting the new contaier used to while prioritizing the servers
	            if ( iNewContainer )
	                {
	                AppUi()->RemoveFromViewStack( *this, iNewContainer );
	                delete iNewContainer;
	                iNewContainer = NULL;
	                }
        		}
        	else
        		{
        		TBool isIncrease;
        		TInt  val;
        		if( currentIndex > iSelectedIndex )
        			{
        			isIncrease = EFalse;
        			val = currentIndex - iSelectedIndex;
        			}
        		else
        			{
        			isIncrease = ETrue;
        			val = iSelectedIndex - currentIndex;        		
        			}
	        	iEngine.ChangePriorityL( 
	        				iSelectedSlpId, 
	        				val,
	        				isIncrease );  
        	  // deleting the new contaier used to while prioritizing the servers
	            if ( iNewContainer )
	                {
	                AppUi()->RemoveFromViewStack( *this, iNewContainer );
	                delete iNewContainer;
	                iNewContainer = NULL;
	                }
        		}
        	break;
        	}
        case EAknSoftkeyCancel:
        	{
        	ChangeMSKL();
        	HandleCommandL( EAknUnmarkAll );
       	   // deleting the new contaier used to while prioritizing the servers
            if ( iNewContainer )
                {
                AppUi()->RemoveFromViewStack( *this, iNewContainer );
                delete iNewContainer;
                iNewContainer = NULL;
                }
        	break;
        	}
        case EAknCmdMark:
        case EAknCmdUnmark:
        case EAknMarkAll:
        case EAknUnmarkAll:
	       	{
	       	// if new container is contructed to prioritize the servers, then mark the item in the new container
	       	if(iNewContainer)
	       	    {
                //Setting the current item in the new container as same as in the original container.
                iNewContainer->ListBoxL().SetCurrentItemIndexAndDraw(iContainer->SelectedItemIndex());	       	
                AknSelectionService::HandleMarkableListProcessCommandL(
	                        aCommand, &( iNewContainer->ListBoxL() ) );   
	       	    }
	       	else
	       	    {
                AknSelectionService::HandleMarkableListProcessCommandL(
						aCommand, &( iContainer->ListBoxL() ) );    
	       	    }
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
        case ELocSuplServerMSKNoCmd:
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
	DEBUG( - CLocSUPLServerView::HandleCommandL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
void CLocSUPLServerView::DynInitMenuPaneL( TInt          aResourceId, 
                                             CEikMenuPane* aMenuPane )
    {
	DEBUG( + CLocSUPLServerView::DynInitMenuPaneL );	
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
        case R_SUPLSETTINGS_SERVER_MENU: 
            {             
	        User::LeaveIfNull( aMenuPane );	     
        	if( iContainer->ServerCount() <= 0 )
        		{	           
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplServerOpen, 		ETrue );
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplServerEnable, 		ETrue );
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplServerDisable, 		ETrue );
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplServerDelete, 		ETrue );
			    aMenuPane->SetItemDimmed( 
			    					ELocSuplServerPrioritize, 	ETrue );
        		}
        	else
        		{
        		if ( iContainer->SelectedCount() > 1 )
        			{
			    	aMenuPane->SetItemDimmed( 
			    					ELocSuplServerOpen, 		ETrue );
			    	aMenuPane->SetItemDimmed( 
			    					ELocSuplServerPrioritize, 	ETrue );  
			    	
			    	TInt disableCount = SelectedDisabledServerCountL();
			    	if( disableCount == iContainer->SelectedCount() )
			    		{
			    		aMenuPane->SetItemDimmed( 
			    					ELocSuplServerDisable, 		ETrue );
			    		}
			    	else if (disableCount == 0 )
			    		{
			    		aMenuPane->SetItemDimmed( 
			    					ELocSuplServerEnable, 		ETrue );
			    		}		    	      			
        			}      		
        		else
        			{
    				TBool enabledFlag;    
        			iEngine.GetServerEnabledFlagL( 
        							iContainer->CurrentSlpId(), enabledFlag );
        							
        			if( enabledFlag )
        				aMenuPane->SetItemDimmed( 
        							ELocSuplServerEnable, 		ETrue );
        			else
        				aMenuPane->SetItemDimmed( 
        							ELocSuplServerDisable,		ETrue );        			
        			}

    			if( iContainer->ServerCount() == 1 )				
			    	aMenuPane->SetItemDimmed( 
			    					ELocSuplServerPrioritize, 	ETrue );        			
        		}        		      		
	        break;
        	}
    	case R_SUPLSETTINGS_SERVER_CONTEXTMENU:
        	{
        	if( iContainer->ServerCount() <= 0 )
        		{
		        aMenuPane->SetItemDimmed( 
		        					ELocSuplServerMSKEnable, 	ETrue );
		        aMenuPane->SetItemDimmed( 
		        					ELocSuplServerMSKDisable,	ETrue );        		
		        aMenuPane->SetItemDimmed( 
		        					ELocSuplServerDelete,		ETrue );        		
        		}
        	else 
        		{
                aMenuPane->SetItemDimmed(ELocSuplServerNew, ETrue);
        		if ( iContainer->SelectedCount() > 0 )
        			{
			    	TInt disableCount = SelectedDisabledServerCountL();
			    	if( disableCount == iContainer->SelectedCount() )
			    		{
			    		aMenuPane->SetItemDimmed( 
			    					ELocSuplServerMSKDisable,	ETrue );
			    		}
			    	else if (disableCount == 0 )
			    		{
			    		aMenuPane->SetItemDimmed( 
			    					ELocSuplServerMSKEnable,	ETrue );
			    		}		    	      			
        			}      	
        		else
        			{
        			TBool enabledFlag;
        			iEngine.GetServerEnabledFlagL( iContainer->CurrentSlpId(), enabledFlag );
        			if( enabledFlag )
        				aMenuPane->SetItemDimmed( 
        							ELocSuplServerMSKEnable, 	ETrue );
        			else
        				aMenuPane->SetItemDimmed( 
        							ELocSuplServerMSKDisable, 	ETrue );
        			}
        		}
        	break;
        	}
        default:
    		{
    		break;
    		}
        }
	DEBUG( - CLocSUPLServerView::DynInitMenuPaneL );	
    }
    
// -----------------------------------------------------------------------------
// CLocSUPLServerView::HandleHelpFeature
// -----------------------------------------------------------------------------
void CLocSUPLServerView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
    {
	DEBUG( + CLocSUPLServerView::HandleHelpFeature );	
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, EFalse );
        }
    else
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, ETrue );
        }
	DEBUG( - CLocSUPLServerView::HandleHelpFeature );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerView::DeletionConfirmationQueryL
// ----------------------------------------------------------------------------
//
TInt CLocSUPLServerView::DeletionConfirmationQueryL( const TInt aAmount )
    {    
    HBufC* noteText = NULL;    
    if ( aAmount > 1 )
        {
        noteText = StringLoader::LoadL( R_LOC_DETAIL_CONF_DELETE_SERVERS, aAmount, iEikonEnv );
        }
    else
        {
        noteText = StringLoader::LoadL( R_LOC_DETAIL_CONF_DELETE_SERVER, iEikonEnv );
        }
    CAknQueryDialog* query = new( ELeave ) CAknQueryDialog( *noteText );    
    TInt ret = query->ExecuteLD( R_LOC_SUPL_DELETE_QUERY );
    delete noteText; //noteText    
    noteText = NULL;
    return ret;
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerView::DeleteServerL
// ----------------------------------------------------------------------------
//
void CLocSUPLServerView::DeleteServerL()
	{
	DEBUG( + CLocSUPLServerView::DeleteServerL );	
	TInt selCount 	= iContainer->SelectedCount();
	TInt severCount = iContainer->ServerCount();
	if( DeletionConfirmationQueryL( selCount ) )
		{
		if ( selCount < 1 && severCount > 0 )
			{
			TRAP_IGNORE( iEngine.RemoveServerL( iContainer->CurrentSlpId() ) );
			}
		else
			{
		    const CArrayFix<TInt>* markedIndexes = NULL;
		    markedIndexes = iContainer->ListBoxL().SelectionIndexes();
				
			RArray<TInt64> markedItemIds;					
			GetSelectedItemIdsL( markedItemIds, *markedIndexes );
			
			for( TInt ctr = 0; ctr < selCount; ctr++ )
				{
				TRAP_IGNORE( iEngine.RemoveServerL( markedItemIds[ ctr ] ) );
				}			
			markedItemIds.Close();			
			}
		}
	DEBUG( - CLocSUPLServerView::DeleteServerL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::DisableServerL
// ----------------------------------------------------------------------------
//
void CLocSUPLServerView::DisableServerL()
	{
	DEBUG( + CLocSUPLServerView::DisableServerL );	
	TInt selCount 	= iContainer->SelectedCount();
	TInt severCount = iContainer->ServerCount();
	
	if ( selCount < 1 && severCount > 0 )
		{
		iEngine.SetServerEnabledFlagL( iContainer->CurrentSlpId(), EFalse );
		}
	else
		{
	    const CArrayFix<TInt>* markedIndexes = NULL;
	    markedIndexes = iContainer->ListBoxL().SelectionIndexes();
			
		RArray<TInt64> markedItemIds;					
		GetSelectedItemIdsL( markedItemIds, *markedIndexes );
		
		for( TInt ctr = 0; ctr < selCount; ctr++ )
			{
			iEngine.SetServerEnabledFlagL( markedItemIds[ ctr ], EFalse );
			}			
		markedItemIds.Close();
		}
	DEBUG( - CLocSUPLServerView::DisableServerL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::EnableServerL
// ----------------------------------------------------------------------------
//
void CLocSUPLServerView::EnableServerL()
	{
	DEBUG( + CLocSUPLServerView::EnableServerL );	
	TInt selCount 	= iContainer->SelectedCount();
	TInt severCount = iContainer->ServerCount();
	
	if ( selCount < 1 && severCount > 0 )
		{
		iEngine.SetServerEnabledFlagL( iContainer->CurrentSlpId(), ETrue );
		}
	else
		{
	    const CArrayFix<TInt>* markedIndexes = NULL;
	    markedIndexes = iContainer->ListBoxL().SelectionIndexes();
			
		RArray<TInt64> markedItemIds;					
		GetSelectedItemIdsL( markedItemIds, *markedIndexes );
		
		for( TInt ctr = 0; ctr < selCount; ctr++ )
			{
			iEngine.SetServerEnabledFlagL( markedItemIds[ ctr ], ETrue );
			}			
		markedItemIds.Close();
		}
	DEBUG( - CLocSUPLServerView::EnableServerL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::ChangeMSKL
// -----------------------------------------------------------------------------
//
void CLocSUPLServerView::ChangeMSKL() 
	{
	DEBUG( + CLocSUPLServerView::ChangeMSKL );	
	// Listbox takes all event even if it doesn't use them
	if( !Cba() )
	    {
	   	return;
	    }
	if( iEngine.SlpCount() <= 0 )
		{
		TRAP_IGNORE( 
			Cba()->SetCommandSetL ( R_SUPLSETTINGS_SERVER_OPTIONS_BACK ) ); 
		}
	else if( iStartedPrioritize )
		{
		//ou1cimx1#352684. to disable first item flicker
	   iNewContainer->ListBoxL().SetCurrentItemIndex(iSelectedIndex);
	   iNewContainer->ListBoxL().DisableSingleClick(ETrue); 
		iStartedPrioritize = EFalse;
		TRAP_IGNORE( 
			Cba()->SetCommandSetL ( R_SUPLSETTINGS_SERVER_OK_CANCEL ) ); 		
		}
	else
		{
		TRAP_IGNORE( 
			Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_CONTEXTOPTIONS_BACK ) ); 		
		}

	Cba()->DrawDeferred();  	
	DEBUG( - CLocSUPLServerView::ChangeMSKL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerView::GetSelectedItemIdsL
// -----------------------------------------------------------------------------
//
void CLocSUPLServerView::GetSelectedItemIdsL(
    RArray<TInt64>& aSelectedItems,
    const CArrayFix<TInt>& aSelectedIndexes ) const
    {
    aSelectedItems.Reset();
    TInt itemCount = iContainer->ServerCount();
    TInt selectionCount = aSelectedIndexes.Count();
    for ( TInt i( 0 ); i < selectionCount; ++i )
        {
        TInt index = aSelectedIndexes[i];
        User::LeaveIfError( aSelectedItems.Append( iContainer->SlpIds()[index] ) );
        }
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerView::SelectedDisabledServerCountL
// -----------------------------------------------------------------------------
//
TInt CLocSUPLServerView::SelectedDisabledServerCountL()
	{
    const CArrayFix<TInt>* markedIndexes = NULL;
    markedIndexes = iContainer->ListBoxL().SelectionIndexes();
		
	RArray<TInt64> markedItemIds;					
	GetSelectedItemIdsL( markedItemIds, *markedIndexes );
	
	TInt disabledServerCount( 0 );
	for( TInt i( 0 ); i < markedItemIds.Count(); ++i )	
		{
		TBool enabledFlag;    
		iEngine.GetServerEnabledFlagL( markedItemIds[i], enabledFlag );
		if( !enabledFlag )
			disabledServerCount++;		
		}
	return disabledServerCount;
	}
	
// End of file
