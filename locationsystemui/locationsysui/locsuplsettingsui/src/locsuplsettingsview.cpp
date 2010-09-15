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
* Description:  Source file for SUPL Settings UI view
*
*/


// System Include
#include <barsread.h> 		                // For TResourceReader
#include <bautils.h>
#include <StringLoader.h>
#include <aknViewAppUi.h>
#include <locsuplsettingsui.rsg>
#include <avkon.rsg>
#include <hlplch.h>			                // Help
#include <featmgr.h> 						// FeatureManager
#include <eikmenup.h>						// Menu pane
#include <eikmenub.h>						// Menu Bar
#include <AknQueryDialog.h> 

// User Include
#include "locsuplsettings.hrh"
#include "locsuplsettingsview.h"
#include "locsuplsettingscontainer.h"
#include "locsettingsui.h"
#include "locsupldebug.h"
#include "locsuplserverview.h"
#include "locsuplsessionview.h"
#include "locsuplsettingsuiengine.h"

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocSUPLSettingsView::CLocSUPLSettingsView
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLSettingsView::CLocSUPLSettingsView( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	:iEventHandler( aEventHandler ),
	iEngine( aEngine ) 
	{
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsView::~CLocSUPLSettingsView
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsView::~CLocSUPLSettingsView()
	{
	DEBUG( + CLocSUPLSettingsView::~CLocSUPLSettingsView );	
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    iContainer = NULL;
	DEBUG( - CLocSUPLSettingsView::~CLocSUPLSettingsView );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSettingsView* CLocSUPLSettingsView::NewL
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLSettingsView* CLocSUPLSettingsView::NewL( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
	DEBUG( + CLocSUPLSettingsView::NewL );	
    CLocSUPLSettingsView* self = NewLC( aEngine, aEventHandler );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettingsView::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsView* CLocSUPLSettingsView::NewLC
// Creates new Location Sys Ui plugin.
//
// @param aEngine               Reference to the SUPL Settings
//                              engine          
// @param aEventHandler		    Pointer to the Event Handler
// @return CAknViewAppUi&		Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocSUPLSettingsView* CLocSUPLSettingsView::NewLC( 
                              CLocSUPLSettingsUiEngine&     aEngine,
		 					  MLocSUPLSettingsEventHandler&	aEventHandler )
	{	
    CLocSUPLSettingsView* self = 
                new( ELeave ) CLocSUPLSettingsView( aEngine,                          
    										        aEventHandler );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsView::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsView::ConstructL()
	{   	
	DEBUG( + CLocSUPLSettingsView::ConstructL );	
	// Call AknView's BaseConstrutL
	BaseConstructL( R_SUPLSETTINGS_VIEW );	 
	DEBUG( - CLocSUPLSettingsView::ConstructL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::HandleScreenSizeChange
// -----------------------------------------------------------------------------
//
void CLocSUPLSettingsView::HandleScreenSizeChange() 
	{
	DEBUG( + CLocSUPLSettingsView::HandleScreenSizeChange );	
	if( iContainer )
		{
		iContainer->SetRect( ClientRect());		
		}
	DEBUG( - CLocSUPLSettingsView::HandleScreenSizeChange );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::UpdateView
// -----------------------------------------------------------------------------
//	
void CLocSUPLSettingsView::UpdateView()
    {
	DEBUG( + CLocSUPLSettingsView::UpdateView );	
    if( iContainer )
		{
		iContainer->UpdateScreen();		
		}
	  TRAP_IGNORE( ChangeMSKL() );		
	DEBUG( - CLocSUPLSettingsView::UpdateView );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::StartedFromAppServer
// -----------------------------------------------------------------------------
//
void CLocSUPLSettingsView::StartedFromAppServer()
    {
	DEBUG( + CLocSUPLSettingsView::StartedFromAppServer );	
    iStartedFromServer = ETrue;
	DEBUG( - CLocSUPLSettingsView::StartedFromAppServer );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::SwitchedFromServerView
// -----------------------------------------------------------------------------
//
void CLocSUPLSettingsView::Reset()
    {
	DEBUG( + CLocSUPLSettingsView::Reset );	
    iViewLaunched = 0;
    StopDisplayingMenuBar();
	DEBUG( - CLocSUPLSettingsView::Reset );	
    }
    
// -----------------------------------------------------------------------------
// TInt CLocSUPLSettingsView::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLSettingsView::SelectedItemIndex()
    {
	DEBUG( CLocSUPLSettingsView::SelectedItemIndex );	
    if( iContainer )
        {
        DEBUG1( CLocSUPLSettingsView::SelectedItemIndex =%d, iContainer->SelectedItemIndex() );	
        return iContainer->SelectedItemIndex();
        }
    else
        {
        return KErrNotFound;
        }	
    }    
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsView::DoActivateL
// 
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettingsView::DoActivateL( const TVwsViewId& 	/* PrevViewId*/,
											   TUid		   	/* aCustomMessageId*/,
						  				 const TDesC8&		/* aCustomMessage */)
	{
	DEBUG( + CLocSUPLSettingsView::DoActivateL );	
	// Destroy the existing container
	if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer=NULL;
        }						
    
    // Create new Container	
	iContainer = CLocSUPLSettingsContainer::NewL( ClientRect(), 
	                                              iEngine,
	                                              *this );
	iContainer->SetMopParent( this );
	AppUi()->AddToViewStackL( *this, iContainer );
	
	if ( iStartedFromServer )
	    {
	    MenuBar()->SetMenuType( CEikMenuBar::EMenuOptionsNoTaskSwapper );
	    }
	
	if( iViewLaunched )
        {
        iContainer->FocusToItemL(	iViewLaunched );
        iViewLaunched = 0;
        }
	iContainer->DrawNow();
	ChangeMSKL();
	DEBUG( - CLocSUPLSettingsView::DoActivateL );	
	}

// ---------------------------------------------------------------------------
// void CLocationSystemUiView::DoDeactivate
//
// ---------------------------------------------------------------------------
//		
void CLocSUPLSettingsView::DoDeactivate()
	{
	DEBUG( + CLocSUPLSettingsView::DoDeactivate );	
	// Destroy Container
	if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
	DEBUG( - CLocSUPLSettingsView::DoDeactivate );	
	}

// ---------------------------------------------------------------------------
// TUid CLocSUPLSettingsView::Id
// Returns the View Id. Should be chosen to reflect the Implementation UID 
// of the Plugin
//
// @return TUid The View Id
// ---------------------------------------------------------------------------
//	
TUid CLocSUPLSettingsView::Id() const
	{
	return KLocSUPLSettingsUiViewId;
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsView::HandleCommandL
//
// ---------------------------------------------------------------------------
//	
void CLocSUPLSettingsView::HandleCommandL(TInt aCommand)	
	{
	DEBUG( + CLocSUPLSettingsView::HandleCommandL );	
	switch ( aCommand )
        {
		case ELocSuplOpenChange:
			{
			ChangeMSKL();
			break;
			}        

    	case ELocSuplOpen:
    	case ELocSuplMSKOpen:
            {
            TInt index = SelectedItemIndex();
            switch( index )
    	    {
		      case 0:
                    {
                    // Launch the Supl Server List.
                    // The error code is ignored since the necessary UI 
                    // notification would be convveyed by the Engine
                    iViewLaunched = 0;
                    TRAP_IGNORE( AppUi()->ActivateLocalViewL ( KLocSUPLServerUiViewId ) );
                    break;
                    }
            case 1:
               {
                    TInt sessionCount = iContainer->ActiveSessionCount();
                    if (sessionCount)
                        {
                        iViewLaunched = 1;
                        // Launch the Supl sessions view.
                        TRAP_IGNORE( AppUi()->ActivateLocalViewL ( KLocSUPLSessionUiViewId ) );
                        }
                    break;
                    }
             default:
			    break; 
            }
            break;   
            } 
        case ELocSuplClearSession:
        case ELocSuplClearSessions:
            {
			if( iContainer->ActiveSessionCount() && DeletionConfirmationQueryL() )
				{
	            TRAP_IGNORE( iEngine.RemoveAllTriggerSessionsL( ) ); 
	            ChangeMSKL();  
				}
            break;    
            }
		case ELocSuplMSKNo:
			{
			break;
			}
        case EAknCmdHelp:
        	{
           	HlpLauncher::LaunchHelpApplicationL( iCoeEnv->WsSession(),
           										 AppUi()->AppHelpContextL());
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
            // Back command is left to the Settings UI which is the command handler 
            // in this case to handle
            iEventHandler.HandleCommandL( EAknSoftkeyBack );
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
	DEBUG( - CLocSUPLSettingsView::HandleCommandL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::ChangeMSKL
// -----------------------------------------------------------------------------
//
void CLocSUPLSettingsView::ChangeMSKL() 
	{
	DEBUG( + CLocSUPLSettingsView::ChangeMSKL );	
	// Listbox takes all event even if it doesn't use them	
    if( !Cba() || !iContainer )
	    {
	   	return;
	    }
	
    TInt index = SelectedItemIndex();
	TInt sessionCount = iContainer->ActiveSessionCount();
    switch( index )
    	{
		case 0:
			{
			TRAP_IGNORE( 
				Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_CHANGE_BACK ) ); 
			break;
			}
		case 1:
			{
			TRAP_IGNORE( 
				Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_OPEN_BACK ) ); 
			break;
			}
		default:
			{
			if( sessionCount )
				{
				TRAP_IGNORE( 
					Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_OPEN_BACK ) ); 
				}
			else
				{
				TRAP_IGNORE( 
					Cba()->SetCommandSetL ( R_SUPLSETTINGS_OPTIONS_NOMSK_BACK ) ); 
				}
			break;
			}   		
    	}    	
	Cba()->DrawDeferred();  	
	DEBUG( - CLocSUPLSettingsView::ChangeMSKL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::DynInitMenuPaneL
// -----------------------------------------------------------------------------
void CLocSUPLSettingsView::DynInitMenuPaneL( TInt          aResourceId, 
                                             CEikMenuPane* aMenuPane )
    {
	DEBUG( + CLocSUPLSettingsView::DynInitMenuPaneL );	
    if ( aResourceId == R_SUPLSETTINGS_MENU )
        {
        User::LeaveIfNull( aMenuPane );
        //Handle Help Fature
        HandleHelpFeature( *aMenuPane );
        
        //Disable Open/Change/Clear sessions options
		switch( SelectedItemIndex() )
			{
			case 0:
				{
	        	aMenuPane->SetItemDimmed( ELocSuplOpen, EFalse );
	        	aMenuPane->SetItemDimmed( ELocSuplClearSession, ETrue );
	        	aMenuPane->SetItemDimmed( ELocSuplClearSessions, ETrue );
				break;
				}
			case 1:
				{
				TInt sessionCount = iContainer->ActiveSessionCount();
				
				switch( sessionCount )
					{
					case 0:
						{
	        			aMenuPane->SetItemDimmed( ELocSuplOpen, ETrue );
	        			aMenuPane->SetItemDimmed( ELocSuplClearSession, ETrue );
	        			aMenuPane->SetItemDimmed( ELocSuplClearSessions, ETrue );
					break;
						}
					case 1:
						{
	        			aMenuPane->SetItemDimmed( ELocSuplOpen, EFalse );
	        			aMenuPane->SetItemDimmed( ELocSuplClearSession, EFalse );
					aMenuPane->SetItemSpecific(ELocSuplClearSession, ETrue);
	        			aMenuPane->SetItemDimmed( ELocSuplClearSessions, ETrue );
						break;
						}
					default:
						{
	        			aMenuPane->SetItemDimmed( ELocSuplOpen, EFalse );
		        		aMenuPane->SetItemDimmed( ELocSuplClearSession, ETrue );
		        		aMenuPane->SetItemDimmed( ELocSuplClearSessions, EFalse );
					aMenuPane->SetItemSpecific(ELocSuplClearSessions, ETrue);
						break;
						}
					}
				break;
				}
			 default:
			    break;
			}
        //Context sensitive menu items
        }
	DEBUG( - CLocSUPLSettingsView::DynInitMenuPaneL );	
    }
    
// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::HandleHelpFeature
// -----------------------------------------------------------------------------
void CLocSUPLSettingsView::HandleHelpFeature( CEikMenuPane& aMenuPane ) const
    {
	DEBUG( + CLocSUPLSettingsView::HandleHelpFeature );	
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ))
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, EFalse );
        }
    else
        {
        aMenuPane.SetItemDimmed( EAknCmdHelp, ETrue );
        }
	DEBUG( - CLocSUPLSettingsView::HandleHelpFeature );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::DeletionConfirmationQueryL
// ----------------------------------------------------------------------------
//
TInt CLocSUPLSettingsView::DeletionConfirmationQueryL( )
    {    
    HBufC* noteText = NULL;    
    noteText = StringLoader::LoadLC( R_LOC_SUPL_QUERY_TERMINATE_ALL, iEikonEnv );
    CAknQueryDialog* query = new( ELeave ) CAknQueryDialog( *noteText );    
    TInt ret = query->ExecuteLD( R_LOC_SUPL_DELETE_QUERY );
    CleanupStack::PopAndDestroy( noteText );   
    return ret;
    }

#if 0
// -----------------------------------------------------------------------------
// CLocSUPLSettingsView::IsActive
// -----------------------------------------------------------------------------
TBool CLocSUPLSettingsView::IsActive()
	{
	if( iContainer )
	    {
	    return ETrue;		
	    }		
	else
	    {
	    return EFalse;
	    }		
	}
	
#endif

// End of file
