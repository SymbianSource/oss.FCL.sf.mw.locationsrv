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
* Description:  SUPL Settings UI View's container
*
*/


// System Includes
#include <eikspane.h> 		// Status pane
#include <akntitle.h>   	// CAknTitlePane
#include <aknlists.h>
#include <StringLoader.h>
#include <locsuplsettingsui.rsg>
#include <csxhelp/locsupl.hlp.hrh>          // Help File

// User Includes
#include "locsuplsettings.hrh"
#include "locsuplsettingsuid.hrh"
#include "locsuplsettingscontainer.h"
#include "locsuplsettingslbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingsview.h"
#include "locsupldebug.h"


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLSettingsContainer::CLocSUPLSettingsContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLSettingsContainer::CLocSUPLSettingsContainer( 
                                        CLocSUPLSettingsView&         aSettingsView )
    : iSettingsView( aSettingsView )
	{
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSettingsContainer::~CLocSUPLSettingsContainer
// Destructor
//
// ---------------------------------------------------------------------------	
//
CLocSUPLSettingsContainer::~CLocSUPLSettingsContainer()
	{
	DEBUG( + CLocSUPLSettingsContainer::~CLocSUPLSettingsContainer );	
	// Delete the List box model only after deleting the list box
	delete iListBox;
	iListBox = NULL;
	
	delete iListBoxModel;
	iListBoxModel = NULL;
	DEBUG( - CLocSUPLSettingsContainer::~CLocSUPLSettingsContainer );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSettingsContainer* CLocSUPLSettingsContainer::NewL
// Static Two phase constructor that instantiates the CLocSUPLSettingsContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param aSettingsView                 Reference to the container's view
// @return CLocSUPLSettingsContainer*	Reference to the object created
// ---------------------------------------------------------------------------    
//
CLocSUPLSettingsContainer* CLocSUPLSettingsContainer::NewL( 
                    const TRect& 	                    aRect,
                          CLocSUPLSettingsUiEngine&     aEngine,
                          CLocSUPLSettingsView&         aSettingsView )
	{	
 	DEBUG( + CLocSUPLSettingsContainer::NewL );	
   	CLocSUPLSettingsContainer* self = NewLC( aRect, 
                                             aEngine,
                                             aSettingsView );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettingsContainer::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsContainer* CLocSUPLSettingsContainer::NewLC
// Static Two phase contructor that instantiates the CLocSUPLSettingsContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param @param aSettingsView                 Reference to the container's view
// @return CLocSUPLSettingsContainer*	Reference to the object created
// ---------------------------------------------------------------------------  
//  
CLocSUPLSettingsContainer* CLocSUPLSettingsContainer::NewLC( 
                    const TRect& 	                    aRect,
                          CLocSUPLSettingsUiEngine&     aEngine,
                          CLocSUPLSettingsView&         aSettingsView  )
	{	
    CLocSUPLSettingsContainer* self = new ( ELeave ) CLocSUPLSettingsContainer(
                                                        aSettingsView );
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aEngine );
	return self;
	}

// --------------------------------------------------------------------------- 
// void CLocSUPLSettingsContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aRect				 Client rectangle
// @param aEngine            Reference to the SUPL Settings engine
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::ConstructL( 
                                    const TRect& 	aRect ,
                                          CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLSettingsContainer::ConstructL );	
	//Make this control a window-owning control
	CreateWindowL(); 
	
	// Create a New Title for the View
	MakeTitleL( R_SUPLVIEW_TITLE );
	
	CreateListboxL( aEngine );
	
	// Get the Help context
	iContextName = KLOC_HLP_SUPL();
	
    SetRect( aRect );
    ActivateL();
	DEBUG( - CLocSUPLSettingsContainer::ConstructL );	
	}	

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::UpdateScreen
// Refreshes the screen so that the list box reflects the latest contents
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::UpdateScreen()
    {
	DEBUG( + CLocSUPLSettingsContainer::UpdateScreen );	
    if ( iListBox )
        {
    	// Update Listbox items
    	TRAP_IGNORE( iListBox->HandleItemAdditionL() );
    	TRAP_IGNORE( iListBox->HandleItemRemovalL() );
        }
    DrawNow();
	DEBUG( - CLocSUPLSettingsContainer::UpdateScreen );	
    }

// -----------------------------------------------------------------------------
// TInt CLocSUPLSettingsContainer::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLSettingsContainer::SelectedItemIndex()
    {
    return iListBoxModel->ResolveSettingsItem( iListBox->CurrentItemIndex() );
    }
			       
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------	
void CLocSUPLSettingsContainer::HandleResourceChange(TInt aType)
	{
	DEBUG( + CLocSUPLSettingsContainer::HandleResourceChange );	
	// Pass the event to the base class
	CCoeControl::HandleResourceChange(aType);
    switch( aType )
    	{
    	// Dynamic Layout switch
    	case KEikDynamicLayoutVariantSwitch:
    		{
    		iSettingsView.HandleScreenSizeChange();
			break;
    		}
    	default:
    		{
    		break;
    		}
    	}
	DEBUG( - CLocSUPLSettingsContainer::HandleResourceChange );	
	}
   
// -----------------------------------------------------------------------------
// CLocSUPLSettingsContainer::GetHelpContext
//
// -----------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
	DEBUG( + CLocSUPLSettingsContainer::GetHelpContext );	
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( KLOCSUPLSETTINGSUIUID3 );
	DEBUG( - CLocSUPLSettingsContainer::GetHelpContext );	
    }
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLSettingsContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
TInt CLocSUPLSettingsContainer::CountComponentControls() const
	{
	return 1;
	}

// ---------------------------------------------------------------------------
// CCoeControl* CLocSUPLSettingsContainer::ComponentControl
//
// ---------------------------------------------------------------------------
CCoeControl* CLocSUPLSettingsContainer::ComponentControl( TInt /*aIndex*/ ) const
	{
	return iListBox;	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------	
//
TKeyResponse CLocSUPLSettingsContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
										   					  TEventCode aType )
	{
	DEBUG( + CLocSUPLSettingsContainer::OfferKeyEventL );	
	if( aType == EEventKey )
		{
		switch ( aKeyEvent.iScanCode )
	        {
	        case EStdKeyUpArrow:
	        case EStdKeyDownArrow:
	        	{        	
	        	TKeyResponse keyresponse = iListBox->OfferKeyEventL( aKeyEvent, aType );        	
	            iSettingsView.HandleCommandL( ELocSuplOpenChange );
	            return keyresponse;	
	        	}
	        default:
	        	{
	        	break;	
	        	}
	        }		
		}
	DEBUG( - CLocSUPLSettingsContainer::OfferKeyEventL );	
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL( aKeyEvent, aType ); 
	}
	    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::HandleListBoxEventL( CEikListBox*  /* aListBox */, 
		 						   					 TListBoxEvent aEventType )
	{
	DEBUG( + CLocSUPLSettingsContainer::HandleListBoxEventL );	
	switch (aEventType)
        {
        case EEventItemClicked:
            {
            // The item is just selected here. Change the MSK to reflect
            // the Correct MSK value
            iSettingsView.HandleCommandL( ELocSuplOpenChange );
            break;    
            }
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked: //case EEventItemDoubleClicked:
            {
            // Settings element has been selected. Command has to be issued to
            // change the settings configuration
            if ( SelectedItemIndex() == 0 )
                {
                iSettingsView.HandleCommandL( ELocSuplMSKChange );
                }
            else if ( SelectedItemIndex() == 1 || SelectedItemIndex() == 2 )
                {
                iSettingsView.HandleCommandL( ELocSuplMSKOpen );
                }
            break;	
            }
        default:
           break;
        }
	DEBUG( - CLocSUPLSettingsContainer::HandleListBoxEventL );	
	}
	         		 	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::FocusChanged( TDrawNow aDrawNow )
	{
	DEBUG( + CLocSUPLSettingsContainer::FocusChanged );	
	CCoeControl::FocusChanged( aDrawNow );
	// The focus event has to be explicitly handed over to all the compound
	// controls since CCoeControl does not do that implicitly
	if ( iListBox )
		{
		iListBox->SetFocus( IsFocused(), aDrawNow );				
		}
	DEBUG( - CLocSUPLSettingsContainer::FocusChanged );	
	}
		
// ---------------------------------------------------------------------------
// CLocSUPLSettingsContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::SizeChanged()
	{
	DEBUG( + CLocSUPLSettingsContainer::SizeChanged );	
	if ( iListBox )
		{
		iListBox->SetRect( Rect());
		}
	DEBUG( - CLocSUPLSettingsContainer::SizeChanged );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::MakeTitleL( TInt aResourceText )
	{
	DEBUG( + CLocSUPLSettingsContainer::MakeTitleL );	
	// Obtain the title from the Status Pane
	CAknTitlePane* title = static_cast<CAknTitlePane*>( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
       
    // Set the Title's buffer    
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
	DEBUG( - CLocSUPLSettingsContainer::MakeTitleL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLocSUPLSettingsContainer::StatusPane()
	{
	return iEikonEnv->AppUiFactory()->StatusPane();	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
//
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// ---------------------------------------------------------------------------	
//
void CLocSUPLSettingsContainer::CreateListboxL( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLSettingsContainer::CreateListboxL );	
	// Create the Listbox model
    iListBoxModel = CLocSUPLSettingsLBModel::NewL( aEngine );
	
	// Create the List box
	iListBox = new ( ELeave ) CAknSettingStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListBox->SetContainerWindowL( *this ); 
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
    													 CEikScrollBarFrame::EAuto ); 
    													 
    // The ownership of the list box model is retained with the Container.
    // This is because the model contains the additional functionality of
    // changing the settings values in addition to retreiving it.    													 
    iListBox->Model()->SetItemTextArray( iListBoxModel );	
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );									
    iListBox->ActivateL();
	DEBUG( - CLocSUPLSettingsContainer::CreateListboxL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsContainer::FocusToItemL
// Focus move to server detail setting item
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsContainer::FocusToItemL( TInt aIndex )
	{
		iListBox->SetCurrentItemIndex( aIndex );
	}
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLSettingsLBModel::ActiveSessionCount()
// Returns the number of Active sessions.
//
// @return  TInt Returns the number of Active sessions count.
// ---------------------------------------------------------------------------
//	
TInt CLocSUPLSettingsContainer::ActiveSessionCount( ) const
    {
	if( iListBoxModel )
		{
		return iListBoxModel->ActiveSessionCount();
		}
	else
		{
		return 0;
		}
	}

// End of file
