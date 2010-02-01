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
* Description:  Location Triggering Management Plugin View's container
*
*/


// System Includes
#include <eikspane.h>       // Status pane
#include <akntitle.h>       // CAknTitlePane
#include <StringLoader.h>   // String Loader
#include <lbtmgmtpluginview.rsg>
#include <aknlists.h>       // Settings Style list box
#include <csxhelp/lbt.hlp.hrh> // LBT Help

// User Includes
#include "lbtmgmtplugincontainer.h"
#include "lbtmgmtplugincmdhdlr.h"
#include "lbtmgmtplugin.hrh"
#include "lbtmgmtpluginuid.hrh"

// Constant defintions
const TInt KNoofComponentControls = 1;


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer::CLbtMgmtPluginContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLbtMgmtPluginContainer::CLbtMgmtPluginContainer( 
                                    MDesCArray&               aListBoxModel,
                                    MLbtMgmtPluginCmdHdlr& aCmdHandler )
    :iListBoxModel( aListBoxModel ),
    iCmdHandler( aCmdHandler )
    {
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer::~CLbtMgmtPluginContainer
// Destructor
//
// ---------------------------------------------------------------------------
//
CLbtMgmtPluginContainer::~CLbtMgmtPluginContainer()
    {
    // Delete the List box model only after deleting the list box
    delete iListBox;    
    }
    
// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer* CLbtMgmtPluginContainer::NewL
// Static Two phase contructor that instantiates the CLbtMgmtPluginContainer
//
// @param aRect                         Client rectangle
// @param aListBoxModel                 Model for the container's list box
// @param aCmdHandler                   List box command handler 
// @return CLbtMgmtPluginContainer*  Reference to the object created
// --------------------------------------------------------------------------- 
//   
CLbtMgmtPluginContainer* CLbtMgmtPluginContainer::NewL( 
                    const TRect&                    aRect,
                          MDesCArray&               aListBoxModel,
                          MLbtMgmtPluginCmdHdlr& aCmdHandler )
    {   
    CLbtMgmtPluginContainer* self = 
                    NewLC( aRect, aListBoxModel, aCmdHandler );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer* CLbtMgmtPluginContainer::NewLC
// Static Two phase contructor that instantiates the CLbtMgmtPluginContainer
//
// @param aRect                         Client rectangle
// @param aListBoxModel                 Model for the container's list box
// @param aCmdHandler                   List box command handler 
// @return CLbtMgmtPluginContainer*  Reference to the object created
// --------------------------------------------------------------------------- 
//  
CLbtMgmtPluginContainer* CLbtMgmtPluginContainer::NewLC(
                    const TRect&                    aRect,
                          MDesCArray&               aListBoxModel,
                          MLbtMgmtPluginCmdHdlr& aCmdHandler  )
    {   
    CLbtMgmtPluginContainer* self = 
            new(ELeave) CLbtMgmtPluginContainer( aListBoxModel, aCmdHandler );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// --------------------------------------------------------------------------- 
// void CLbtMgmtPluginContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aRect              Client rectangle
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::ConstructL( const TRect&   aRect )
    {
    //Make this control a window-owning control
    CreateWindowL(); 
    
    // Create a New Title for the View
    MakeTitleL( R_LOCTR_TITLE_TEXT_LT );
    
    CreateListboxL();
    
    // Get the Help context
    iContextName = KLOC_HLP_LBT_SETTINGS();
        
    SetRect( aRect );
    ActivateL();
    }
 
// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer::GetFocussedItem
// Returns the currentlt focussed element.
//
 // @return TInt Index of the item selected
// ---------------------------------------------------------------------------
//
TInt CLbtMgmtPluginContainer::GetFocussedItem()
    {
    return iListBox->CurrentItemIndex();
    }
        
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------
//  
void CLbtMgmtPluginContainer::HandleResourceChange(TInt aType)
    {
    // Pass the event to the base class
    CCoeControl::HandleResourceChange( aType );
    switch( aType )
        {
        // Dynamic Layout switch
        case KEikDynamicLayoutVariantSwitch:
            {
            // Handle the screen change event
            // Trap and Ignore the error
            TRAP_IGNORE(
                iCmdHandler.HandleCmdL( MLbtMgmtPluginCmdHdlr::EScreenSizeChanged ));
            break;
            }
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// TInt CLbtMgmtPluginContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CLbtMgmtPluginContainer::CountComponentControls() const
    {
    return KNoofComponentControls;
    }

// ---------------------------------------------------------------------------
// CCoeControl* CLbtMgmtPluginContainer::ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CLbtMgmtPluginContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;    
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------  
//
TKeyResponse CLbtMgmtPluginContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                               TEventCode aType )
    {
    TKeyResponse keyresponse;
    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
        case EKeyRightArrow:
            {
            // Listbox takes all event even if it doesn't use them
            return EKeyWasNotConsumed;  
            }
        case EKeyUpArrow:
        case EKeyDownArrow:
	        {
	        keyresponse = iListBox->OfferKeyEventL( aKeyEvent, aType );
        	iCmdHandler.HandleCmdL( MLbtMgmtPluginCmdHdlr::EListBoxItemFocused );    
        	return keyresponse;        	
	        }
        default:
            {
            break;  
            }
        }
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL( aKeyEvent, aType ); 
    }
        
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::HandleListBoxEventL( 
                                        CEikListBox*                      /* aListBox */, 
                                        MEikListBoxObserver::TListBoxEvent aEventType )
    {
    switch ( aEventType )
        {
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            // Settings element has been selected. Command has to be issued to
            // change the settings configuration
            iCmdHandler.HandleCmdL( MLbtMgmtPluginCmdHdlr::EListBoxItemSelected );
            break;
            }  
        default:
           break;
        }
    }

// -----------------------------------------------------------------------------
// CLocNotPrefPluginContainer::GetHelpContext
//
// -----------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( KLbtMgmtPluginUID3 );
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::HandlePointerEventL
//
// ---------------------------------------------------------------------------
//       
   void CLbtMgmtPluginContainer::HandlePointerEventL(const TPointerEvent &aPointerEvent)
     {
    // Check if touch is enabled or not
    if( !AknLayoutUtils::PenEnabled() || !iListBox )
        {
        return;
        }
    iListBox->HandlePointerEventL( aPointerEvent );      
   }                  
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::FocusChanged( TDrawNow aDrawNow )
    {
    CCoeControl::FocusChanged( aDrawNow );
    // The focus event has to be explicitly handed over to all the compound
    // controls since CCoeControl does not do that implicitly
    iListBox->SetFocus( IsFocused(), aDrawNow );
    }
        
// ---------------------------------------------------------------------------
// CLbtMgmtPluginContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::SizeChanged()
    {
    iListBox->SetRect( Rect() ); 
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
//
// ---------------------------------------------------------------------------  
//
void CLbtMgmtPluginContainer::CreateListboxL()
    {
    
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
    iListBox->Model()->SetItemTextArray( &iListBoxModel );  
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );  
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );                    
    iListBox->ActivateL();
    }
    
// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::MakeTitleL( TInt aResourceText )
    {
    // Obtain the title from the Status Pane
    CAknTitlePane* title = static_cast<CAknTitlePane*>( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle )));
       
    // Set the Title's buffer    
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLbtMgmtPluginContainer::StatusPane()
    {
    return iEikonEnv->AppUiFactory()->StatusPane(); 
    }

// ---------------------------------------------------------------------------
// void CLbtMgmtPluginContainer::Update
// It will updates listbox model
// ---------------------------------------------------------------------------
//
void CLbtMgmtPluginContainer::Update()
    {
    if(iListBox)
        {
        iListBox->DrawDeferred(); 
        }
	}

