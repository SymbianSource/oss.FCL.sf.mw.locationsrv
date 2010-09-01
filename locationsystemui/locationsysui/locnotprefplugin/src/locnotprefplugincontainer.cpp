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
* Description:  Location Notation Preferences Plugin View's container
*
*/


// System Includes
#include <eikspane.h>       // Status pane
#include <akntitle.h>       // CAknTitlePane
#include <StringLoader.h>   // String Loader
#include <locnotprefpluginview.rsg>
#include <aknlists.h>       // Settings Style list box
#include <csxhelp/locnota.hlp.hrh>
#include <eiklbv.h>

// User Includes
#include "locnotprefplugincontainer.h"
#include "locnotprefpluginview.h"
#include "locnotprefplugin.hrh"
#include "locnotprefpluginuid.hrh"

// Constant defintions
const TInt KNoofComponentControls = 1;


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer::CLocNotPrefPluginContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLocNotPrefPluginContainer::CLocNotPrefPluginContainer( 
                                    MDesCArray&               aListBoxModel,
                                    MLocNotPrefPluginCmdHdlr& aCmdHandler)
    :iListBoxModel( aListBoxModel ),
    iCmdHandler( aCmdHandler )
    {
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer::~CLocNotPrefPluginContainer
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocNotPrefPluginContainer::~CLocNotPrefPluginContainer()
    {
    // Delete the List box model only after deleting the list box
    delete iListBox;    
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer* CLocNotPrefPluginContainer::NewL
// Static Two phase contructor that instantiates the CLocNotPrefPluginContainer
//
// @param aRect                         Client rectangle
// @param aListBoxModel                 Model for the container's list box
// @param aCmdHandler                   List box command handler 
// @return CLocNotPrefPluginContainer*  Reference to the object created
// --------------------------------------------------------------------------- 
//   
CLocNotPrefPluginContainer* CLocNotPrefPluginContainer::NewL( 
                    const TRect&                    aRect,
                          MDesCArray&               aListBoxModel,
                          MLocNotPrefPluginCmdHdlr& aCmdHandler )
    {   
    CLocNotPrefPluginContainer* self = 
                    NewLC( aRect, aListBoxModel, aCmdHandler );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer* CLocNotPrefPluginContainer::NewLC
// Static Two phase contructor that instantiates the CLocNotPrefPluginContainer
//
// @param aRect                         Client rectangle
// @param aListBoxModel                 Model for the container's list box
// @param aCmdHandler                   List box command handler 
// @return CLocNotPrefPluginContainer*  Reference to the object created
// --------------------------------------------------------------------------- 
//  
CLocNotPrefPluginContainer* CLocNotPrefPluginContainer::NewLC(
                    const TRect&                    aRect,
                          MDesCArray&               aListBoxModel,
                          MLocNotPrefPluginCmdHdlr& aCmdHandler  )
    {   
    CLocNotPrefPluginContainer* self = 
            new(ELeave) CLocNotPrefPluginContainer( aListBoxModel, aCmdHandler );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// --------------------------------------------------------------------------- 
// void CLocNotPrefPluginContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aRect              Client rectangle
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::ConstructL( const TRect&   aRect )
    {
    //Make this control a window-owning control
    CreateWindowL(); 
    
    // Create a New Title for the View
    MakeTitleL( R_LOC_TITLE_NOTATIONS );
    
    CreateListboxL();
    
    // Get the Help context
    iContextName = KLOC_HLP_NOTATIONS();
        
    SetRect( aRect );
    ActivateL();
    }
 
// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer::GetFocussedItem
// Returns the currentlt focussed element.
//
 // @return TInt Index of the item selected
// ---------------------------------------------------------------------------
//
TInt CLocNotPrefPluginContainer::GetFocussedItem()
    {
    return iListBox->CurrentItemIndex();
    }
        
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------
//  
void CLocNotPrefPluginContainer::HandleResourceChange(TInt aType)
    {
    // Pass the event to the base class
    CCoeControl::HandleResourceChange(aType);
    switch( aType )
        {
        // Dynamic Layout switch
        case KEikDynamicLayoutVariantSwitch:
            {
            // Handle the screen change event
            // Trap and Ignore the error
            TRAP_IGNORE(
                iCmdHandler.HandleCmdL( MLocNotPrefPluginCmdHdlr::EScreenSizeChanged ));
            break;
            }
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// TInt CLocNotPrefPluginContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
//
TInt CLocNotPrefPluginContainer::CountComponentControls() const
    {
    return KNoofComponentControls;
    }

// ---------------------------------------------------------------------------
// CCoeControl* CLocNotPrefPluginContainer::ComponentControl
//
// ---------------------------------------------------------------------------
//
CCoeControl* CLocNotPrefPluginContainer::ComponentControl(TInt /*aIndex*/) const
    {
    return iListBox;    
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------  
//
TKeyResponse CLocNotPrefPluginContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
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
    return iListBox->OfferKeyEventL( aKeyEvent, aType ); 
    }
        
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::HandleListBoxEventL( 
                                        CEikListBox*                      /* aListBox */, 
                                        MEikListBoxObserver::TListBoxEvent aEventType )
    {
    switch (aEventType)
        {
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            // Settings element has been selected. Command has to be issued to
            // change the settings configuration
            iCmdHandler.HandleCmdL( MLocNotPrefPluginCmdHdlr::EListBoxItemSelected );
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
void CLocNotPrefPluginContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( KLocNotPrefPluginUID3 );
    }

// -----------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::HandlePointerEventL(const TPointerEvent &aPointerEvent)
    {
    // Check if touch is enabled or not
    if( !AknLayoutUtils::PenEnabled() || !iListBox )
        {
        return;
        }
    iListBox->HandlePointerEventL( aPointerEvent ); 
   }
   	
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::FocusChanged(TDrawNow aDrawNow)
    {
    CCoeControl::FocusChanged( aDrawNow );
    // The focus event has to be explicitly handed over to all the compound
    // controls since CCoeControl does not do that implicitly
    iListBox->SetFocus( IsFocused(), aDrawNow );
    }
        
// ---------------------------------------------------------------------------
// CLocNotPrefPluginContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::SizeChanged()
    {
    iListBox->SetRect( Rect()); 
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
//
// ---------------------------------------------------------------------------  
//
void CLocNotPrefPluginContainer::CreateListboxL()
    {
    
    // Create the List box
    iListBox = new ( ELeave ) CAknSettingStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListBox->SetContainerWindowL( *this ); 
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
                                                         CEikScrollBarFrame::EAuto ); 
                                                         
    // The ownership of the list box model is retained with the Container.
    // This is because the model contains the additional functionality of
    // changing the settings values in addition to retreiving it.                                                        
    iListBox->Model()->SetItemTextArray( &iListBoxModel );  
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );                                 
    iListBox->ActivateL();
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginContainer::MakeTitleL( TInt aResourceText )
    {
    // Obtain the title from the Status Pane
    CAknTitlePane* title = static_cast<CAknTitlePane*>( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle )));
       
    // Set the Title's buffer    
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLocNotPrefPluginContainer::StatusPane()
    {
    return iEikonEnv->AppUiFactory()->StatusPane(); 
    }
