/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Source file for Location System UI container.
*
*
*/


// System Includes
#include <eikspane.h> 						// Status pane
#include <akntitle.h>   					// CAknTitlePane
#include <barsread.h>    					// For TResourceReader
#include <aknlists.h>						// List box
#include <locsysuiviewrsc.rsg>		
#include <csxhelp/locset.hlp.hrh>

// User Includes
#include "locsysuicontainer.h"
#include "locsysuiview.h"
#include "locationsettings.h"
#include "locsysuilbmodel.h"
#include "locsysuiviewuid.hrh"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSysUiContainer::CLocSysUiContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSysUiContainer::CLocSysUiContainer( 
								MLocSysUiContainerEventHandler&	aEventHandler )
	:iEventHandler( aEventHandler )
	{
	}
	
// ---------------------------------------------------------------------------
// CLocSysUiContainer::~CLocSysUiContainer
// Destructor
//
// ---------------------------------------------------------------------------
//	
CLocSysUiContainer::~CLocSysUiContainer()
	{
	// Delete the list box
	delete iListBox;
	
	// Delete the List box model
	delete iListBoxModel;
	}
	
// ---------------------------------------------------------------------------
// CLocSysUiContainer* CLocSysUiContainer::NewL
// Static Two phase contructor that instantiates the CLocationSystemUiUIContainer
//
// @param aEventHandler		 Reference to the Container's event handler
// @param aSettingsArray	 Reference to the Settings UI array.
// @param aRect				 Client rectangle.
// @return CLocSysUiContainer*	Reference to the object created.
// --------------------------------------------------------------------------- 
//   
CLocSysUiContainer* CLocSysUiContainer::NewL( 
        		MLocSysUiContainerEventHandler&	    aEventHandler,
        		RPointerArray<CLocationSettings>&   aSettingsArray,
        		const TRect& 					    aRect )
	{	
    CLocSysUiContainer* self = CLocSysUiContainer::NewLC( aEventHandler,
                                                          aSettingsArray,
                                                          aRect );
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSysUiContainer* CLocSysUiContainer::NewL
// Static Two phase contructor that instantiates the CLocationSystemUiUIContainer
//
// @param aEventHandler		 Reference to the Container's event handler
// @param aSettingsArray	 Reference to the Settings UI array.
// @param aRect				 Client rectangle.
// @return CLocSysUiContainer*	Reference to the object created.
// --------------------------------------------------------------------------- 
//
CLocSysUiContainer* CLocSysUiContainer::NewLC( 
        		MLocSysUiContainerEventHandler&	    aEventHandler,
        		RPointerArray<CLocationSettings>&   aSettingsArray,
        		const TRect& 					    aRect )
	{	
    CLocSysUiContainer* self = new(ELeave) CLocSysUiContainer( aEventHandler );
	CleanupStack::PushL( self );
	self->ConstructL( aSettingsArray, aRect );
	return self;
	}

// --------------------------------------------------------------------------- 
// void CLocSysUiContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aSettingsArray    Reference to the Settings UI array.
// @param aRect				Client rectangle
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::ConstructL( 
                         RPointerArray<CLocationSettings>&  aSettingsArray,
        			     const TRect& 						aRect )
	{
	//Make this control a window-owning control
	CreateWindowL(); 
	
	// Create a New Title for the View
	MakeTitleL(R_LOCSYSUIVIEW_TITLE);
	 
	CreateListboxL( aSettingsArray );
	
	iContextName = KLOC_HLP_SETTINGS();
    SetRect( aRect );
    ActivateL();
	}	

// ---------------------------------------------------------------------------
// CLocationSettings&	CLocSysUiContainer::GetFocussedSettingsUiDescL
// Returns the Description for the Settings Ui item that is currently focussed
//
// @return CLocSettingsUiDesc*	Reference to the description object of
//							the item that is currently focussed
// 		   NULL				If there is no list box
// ---------------------------------------------------------------------------
//
CLocationSettings&	CLocSysUiContainer::GetFocussedSettingsUiL()
	{		
	TInt selectedItem = iListBox->CurrentItemIndex();
	return iListBoxModel->SettingsUiL( selectedItem );
	}

// ---------------------------------------------------------------------------
// void CLocSysUiContainer::UpdateListBoxL
// Calls upon the container to update the list bix contents.
//
// @param aSettingsArray	Reference to the Settings UI array.
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::UpdateListBoxL(
						RPointerArray<CLocationSettings>& aSettingsArray )
	{
	iListBoxModel->UpdateListBoxContentsL( aSettingsArray );
    iListBox->HandleItemAdditionL();
    iListBox->HandleItemRemovalL();
	DrawDeferred();
	}

// ---------------------------------------------------------------------------
// TUint CLocSysUiContainer::CurrentFocussedElement
// Returns the currently focussed element index
//
// @return TUint    Currently focussed element index    
// ---------------------------------------------------------------------------
//
TUint CLocSysUiContainer::CurrentFocussedElement()
    {
    TInt ret = iListBox->CurrentItemIndex();
    if( ret < 0 )
        {
        ret = 0;
        }
    return ret;
    }
   
// ---------------------------------------------------------------------------
// Sets the current highlighted element
//
// @param aCurrentElement    Currently focussed element index    
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::SetFocussedElement( TUint      aCurrentElement )
    {
    iListBox->SetCurrentItemIndex( aCurrentElement );
    }
                	
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------	
//
TKeyResponse CLocSysUiContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
										   					   TEventCode aType )
	{
	switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
        	{
        	// Listbox takes all event even if it doesn't use them
            return EKeyWasNotConsumed;	
        	}
        default:
        	{
        	break;	
        	}
        }
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL(aKeyEvent, aType); 
	}

// ---------------------------------------------------------------------------
// void CLocSysUiContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------	
//
void CLocSysUiContainer::HandleResourceChange(TInt aType)
	{
	// Pass the event to the base class
	CCoeControl::HandleResourceChange(aType);
    switch ( aType )
    	{
    	// Dynamic Layout switch
    	case KEikDynamicLayoutVariantSwitch:
    		{
			iEventHandler.HandleScreenSizeChange();
			break;
    		}
    	default:
    		{
    		break;
    		}
    	}
	}

// ---------------------------------------------------------------------------
// TInt CLocSysUiContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CLocSysUiContainer::CountComponentControls() const
	{
	return 1;
	}

// ---------------------------------------------------------------------------
// CCoeControl* CLocSysUiContainer::ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CLocSysUiContainer::ComponentControl(TInt /*aIndex*/) const
	{
	return iListBox;	
	}
	
// -----------------------------------------------------------------------------
// CLocSysUiContainer::GetHelpContext
// Overridden from CCoeControl. Retruns the help context
//
// @param aContext The requested Help context
// -----------------------------------------------------------------------------
//
void CLocSysUiContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( LOCSYSUIVIEW_UID3 );
    }
    
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::HandleListBoxEventL( CEikListBox*  /* aListBox */, 
		 						   					  TListBoxEvent aEventType )
	{
	switch (aEventType)
        {
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            iEventHandler.ListBoxItemSelectedL();
            break;	
            }
        default:
           break;
        }
	}
    		 	
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::FocusChanged(TDrawNow aDrawNow)
	{
	CCoeControl::FocusChanged( aDrawNow );
	if ( iListBox )
		{
		iListBox->SetFocus( IsFocused(), aDrawNow );				
		}
	}
		
// ---------------------------------------------------------------------------
// CLocSysUiContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::SizeChanged()
	{
	if ( iListBox )
		{
		iListBox->SetRect( Rect());
		}
	}
	
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
// 
// @param aSettingsArray	 Reference to the Settings UI array.
// ---------------------------------------------------------------------------	
//
void CLocSysUiContainer::CreateListboxL( 
						 RPointerArray<CLocationSettings>&  aSettingsArray )
	{
	// Create the Listbox model
	iListBoxModel = CLocSysUiLBModel::NewL( aSettingsArray );
	
	// Create the List box
	iListBox = new( ELeave )CAknSettingStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListBox->SetContainerWindowL( *this ); 
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
    													 CEikScrollBarFrame::EAuto );
    													 
    iListBox->Model()->SetItemTextArray( iListBoxModel );	
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );									
    iListBox->ActivateL();
	}
	
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLocSysUiContainer::StatusPane()
	{
	return iEikonEnv->AppUiFactory()->StatusPane();	
	}
        
// ---------------------------------------------------------------------------
// void CLocSysUiContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLocSysUiContainer::MakeTitleL( TInt aResourceText )
	{
	// Obtain the title from the Status Pane
	CAknTitlePane* title = static_cast<CAknTitlePane*>( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle )));
       
    // Set the Title's buffer    
    TResourceReader rReader;
    iCoeEnv->CreateResourceReaderLC( rReader, aResourceText );
    title->SetFromResourceL( rReader );
    CleanupStack::PopAndDestroy(); //rReader
	}

