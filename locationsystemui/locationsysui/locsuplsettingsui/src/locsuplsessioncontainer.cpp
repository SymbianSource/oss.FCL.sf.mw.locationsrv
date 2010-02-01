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
* Description:  SUPL session UI View's container
*
*/


// System Includes
#include <eikspane.h> 						// Status pane
#include <akntitle.h>   					// CAknTitlePane
#include <aknlists.h>
#include <StringLoader.h>
#include <locsuplsettingsui.rsg>
#include <csxhelp/locsupl.hlp.hrh>          // Help File
#include <aknview.h>
#include <eikmenub.h>
#include <gulicon.h>
#include <AknIconUtils.h>
#include <AknIconArray.h>
#include <eikclbd.h>
#include <avkon.mbg>						// for select session (mark) icon
#include <avkon.rsg>
#include <eiklbx.h>
#include <ErrorUI.h>
#include <AknsUtils.h> 
#include <aknapp.h>
#include <locsuplsettings.mbg>				// for session NI/TI icon

// User Includes
#include "locsuplsettings.hrh"
#include "locsuplsettingsuid.hrh"
#include "locsuplsessioncontainer.h"
#include "locsuplsessionview.h"
#include "locsuplsessionlbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingseventhandler.h"
#include "locsupldebug.h"

// CONSTANT DEFINITIONS
const TInt KIconArrayGranularity = 2; // The number of default icons
_LIT( KLocSuplSettingsIconFileName, "locsuplsettings.mif" );


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::CLocSUPLSessionContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLSessionContainer::CLocSUPLSessionContainer( CLocSUPLSessionView* aView )
    : iView( aView )
	{
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::~CLocSUPLSessionContainer
// Destructor
//
// ---------------------------------------------------------------------------	
//
CLocSUPLSessionContainer::~CLocSUPLSessionContainer()
	{
	DEBUG( + CLocSUPLSessionContainer::~CLocSUPLSessionContainer );	
	// Delete the List box model only after deleting the list box
	delete iListBox;
	iListBox = NULL;
	
	delete iListBoxModel;
	iListBoxModel = NULL;
	DEBUG( - CLocSUPLSessionContainer::~CLocSUPLSessionContainer );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer* CLocSUPLSessionContainer::NewL
// Static Two phase constructor that instantiates the CLocSUPLSessionContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param aEventHandler                 Reference to the parent view
// @return CLocSUPLSessionContainer*	Reference to the object created
// ---------------------------------------------------------------------------    
//
CLocSUPLSessionContainer* CLocSUPLSessionContainer::NewL( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLSessionView* 		aView )
	{	
	DEBUG( + CLocSUPLSessionContainer::NewL );	
    CLocSUPLSessionContainer* self = NewLC( aRect, 
                                           aEngine,
                                           aView );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSessionContainer::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer* CLocSUPLSessionContainer::NewLC
// Static Two phase contructor that instantiates the CLocSUPLSessionContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param aEventHandler                 Reference to the parent view
// @return CLocSUPLSessionContainer*	Reference to the object created
// ---------------------------------------------------------------------------  
//  
CLocSUPLSessionContainer* CLocSUPLSessionContainer::NewLC( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLSessionView* 		aView )
	{	
    CLocSUPLSessionContainer* self = new ( ELeave ) CLocSUPLSessionContainer(
                                                        aView );
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aEngine );
	return self;
	}

// --------------------------------------------------------------------------- 
// void CLocSUPLSessionContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aRect				 Client rectangle
// @param aEngine            Reference to the SUPL Settings engine
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::ConstructL( 
                                    const TRect& 	aRect ,
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLSessionContainer::ConstructL );	
	//Make this control a window-owning control
	CreateWindowL(); 
	
	// Create a New Title for the View
	MakeTitleL( R_LOC_SUPL_SERVICES_TITLE );
	
	CreateListboxL( aEngine );
	
	// Get the Help context
	iContextName = KLOC_HLP_SUPL();
	
    SetRect( aRect );
    ActivateL();
	DEBUG( - CLocSUPLSessionContainer::ConstructL );	
	}	

// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::UpdateScreenL
// Refreshes the screen so that the list box reflects the latest contents
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::UpdateScreenL()
    {
	DEBUG( + CLocSUPLSessionContainer::UpdateScreenL );	
	
	TInt oldCount 	= SessionIds().Count();	
	TInt64 sessionId;
	if( iSelectedIndex != KErrNotFound && SessionIds().Count() > iSelectedIndex )
		{
		sessionId = SessionIds()[ iSelectedIndex ];		
		}
	
	if ( iListBoxModel )
		{
		iListBoxModel->UpdateSessionIdsL();
		}    
		
    TInt newCount 	= SessionIds().Count();
    
    if ( iListBox )
        {
    	// Update Listbox items
        if( newCount <= 0 || newCount == oldCount )
        	{
         	TRAP_IGNORE( iListBox->HandleItemAdditionL() );
	    	TRAP_IGNORE( iListBox->HandleItemRemovalL() );       	
        	}
        else if( newCount > oldCount )
        	{
        	// notify the listbox for item addition 
        	TRAP_IGNORE( iListBox->HandleItemAdditionL() );

        	// set focus to newly added item
			TInt index = iListBox->CurrentItemIndex();	
			if( index == 0 && newCount == 1 )
				{
				iListBox->SetCurrentItemIndex( 0 );	
				}
			else
				{
				iListBox->SetCurrentItemIndex( newCount - 1 );	
				}
        	}
        else if( newCount < oldCount )
        	{
        	// notify the listbox for item deletion 
	    	TRAP_IGNORE( iListBox->HandleItemRemovalL() );
			
        	// set focus at last selected item or index			
			if( iSelectedIndex > newCount-1 )
				{
				iListBox->SetCurrentItemIndex( newCount - 1 );
				}
			else
				{
				TInt newIndex = SessionIds().Find( sessionId );
				if( newIndex != KErrNotFound )
					{
					iListBox->SetCurrentItemIndex( newIndex );
					}
				else
					{
					iListBox->SetCurrentItemIndex( iSelectedIndex );				
					}				
				}
        	}
        }        
    DrawDeferred();
	DEBUG( - CLocSUPLSessionContainer::UpdateScreenL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------	
void CLocSUPLSessionContainer::HandleResourceChange(TInt aType)
	{
	DEBUG( + CLocSUPLSessionContainer::HandleResourceChange );	
	// Pass the event to the base class
	CCoeControl::HandleResourceChange(aType);
    switch( aType )
    	{
    	// Dynamic Layout switch
    	case KEikDynamicLayoutVariantSwitch:
    		{
    		SetRect( iView->ClientRect() );
			break;
    		}
    	default:
    		{
    		break;
    		}
    	}
	DEBUG( - CLocSUPLSessionContainer::HandleResourceChange );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionContainer::GetHelpContext
//
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
	DEBUG( + CLocSUPLSessionContainer::GetHelpContext );	
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( KLOCSUPLSERVERUIUID3 );
	DEBUG( - CLocSUPLSessionContainer::GetHelpContext );	
    }
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLSessionContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
TInt CLocSUPLSessionContainer::CountComponentControls() const
	{
	return 1;
	}

// ---------------------------------------------------------------------------
// CCoeControl* CLocSUPLSessionContainer::ComponentControl
//
// ---------------------------------------------------------------------------
CCoeControl* CLocSUPLSessionContainer::ComponentControl( TInt /*aIndex*/ ) const
	{
	return iListBox;	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------	
//
TKeyResponse CLocSUPLSessionContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
										   					  TEventCode aType )
	{
	TKeyResponse keyresponse = EKeyWasNotConsumed;
	
    // Now handle the Key Event based on the Key type
    switch ( aKeyEvent.iScanCode )
        {            
        case EStdKeyUpArrow:
        case EStdKeyDownArrow:
            {
            keyresponse = iListBox->OfferKeyEventL( aKeyEvent, aType );
            iView->ChangeMSKL();
            return keyresponse;         
            }
        case EStdKeyBackspace:
        case EStdKeyDelete:
            {
            if(aType == EEventKey )
                {
                keyresponse = iListBox->OfferKeyEventL( aKeyEvent, aType );
                iView->HandleCommandL( ELocSuplSessionTerminate ); 
                }
            return keyresponse;               
            }
        case EStdKeyHash:
            {
            TBool shiftKeyPressed = ( aKeyEvent.iModifiers & EModifierShift );
            keyresponse = iListBox->OfferKeyEventL( aKeyEvent, aType );
            if( aType == EEventKeyUp && ( EKeyWasConsumed == keyresponse || !shiftKeyPressed ) )
                iView->ChangeMSKL();
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
// void CLocSUPLSessionContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::HandleListBoxEventL( CEikListBox*  /* aListBox */, 
		 						   					 TListBoxEvent aEventType )
	{
	DEBUG( + CLocSUPLSessionContainer::HandleListBoxEventL );	
	switch ( aEventType )
        {
        case EEventItemClicked:
            {
            // The item is just selected here. Change the MSK to reflect
            // the Correct MSK value
            iView->ChangeMSKL();
            break;    
            }
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            if( SelectedCount() )
                {
                //display menu specific to editor
                ShowContextMenuL();
                }
            else
                {
                iView->HandleCommandL( ELocSuplSessionOpen );   
                }
            break;	
            }
        default:
           break;
        }
	DEBUG( - CLocSUPLSessionContainer::HandleListBoxEventL );	
	}
	         		 	
// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::FocusChanged( TDrawNow aDrawNow )
	{
	CCoeControl::FocusChanged( aDrawNow );
	// The focus event has to be explicitly handed over to all the compound
	// controls since CCoeControl does not do that implicitly
	if ( iListBox )
		{
		iListBox->SetFocus( IsFocused(), aDrawNow );				
		}
	}
		
// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::SizeChanged()
	{
	if ( iListBox )
		{
		iListBox->SetRect( Rect() );
		}
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::MakeTitleL( TInt aResourceText )
	{
	DEBUG( + CLocSUPLSessionContainer::MakeTitleL );	
	// Obtain the title from the Status Pane
	CAknTitlePane* title = static_cast < CAknTitlePane* >( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
       
    // Set the Title's buffer    
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
	DEBUG( - CLocSUPLSessionContainer::MakeTitleL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLocSUPLSessionContainer::StatusPane()
	{
	return iEikonEnv->AppUiFactory()->StatusPane();	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
//
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// ---------------------------------------------------------------------------	
//
void CLocSUPLSessionContainer::CreateListboxL( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLSessionContainer::CreateListboxL );	
	// Create the Listbox model
    iListBoxModel = CLocSUPLSessionLBModel::NewL( aEngine );
	
	// CAknSingleGraphicStyleListBox	"0\tTextLabel\t1\t2"
	
    iListBox = new ( ELeave ) CAknSingleGraphicStyleListBox;
	iListBox->ConstructL( 
						this, 
						#ifdef RD_SCALABLE_UI_V2
						EAknListBoxStylusMarkableList
						#else
						EAknListBoxMarkableList
						#endif //RD_SCALABLE_UI_V2
						);
    // Set the container control.
	iListBox->SetContainerWindowL( *this );
    // Add scrollbars to listbox
	iListBox->CreateScrollBarFrameL( ETrue );
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
														 CEikScrollBarFrame::EAuto );
    // Icon array, needed only if listbox style is single number with graphic.
    CAknIconArray* icons = new( ELeave ) CAknIconArray( KIconArrayGranularity );
    CleanupStack::PushL( icons );

    TFileName avkonIconFile = AknIconUtils::AvkonIconFileName();
	
    TRAP_IGNORE( AddIconL(   *icons, 
                avkonIconFile,
                KAknsIIDQgnIndiMarkedAdd, 
                EMbmAvkonQgn_indi_marked_add, 
                EMbmAvkonQgn_indi_marked_add_mask ) );
 	
	TFileName *iconFile = IconFileNameLC();
    
    TRAP_IGNORE( AddIconL(   *icons, 
                *iconFile,
                KAknsIIDQgnPropRadiobuttOff, 
                EMbmLocsuplsettingsQgn_loc_server_disabled, 
                EMbmLocsuplsettingsQgn_loc_server_disabled_mask ) );
    
    TRAP_IGNORE( AddIconL(   *icons, 
                *iconFile,
                KAknsIIDQgnPropRadiobuttOff, 
                EMbmLocsuplsettingsQgn_loc_server_disabled, 
                EMbmLocsuplsettingsQgn_loc_server_disabled_mask ) );
 	
 	CleanupStack::PopAndDestroy( iconFile );
	
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    CleanupStack::Pop( icons );        
    
    // Set the model & observer
    iListBox->SetListBoxObserver( this  );

    iListBox->Model()->SetItemTextArray( iListBoxModel );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );									
    iListBox->ActivateL();   
	DEBUG( - CLocSUPLSessionContainer::CreateListboxL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionContainer::AddIconL
// -----------------------------------------------------------------------------
void CLocSUPLSessionContainer::AddIconL( CAknIconArray&  aIcons,
                                           const TDesC&    aIconFileWithPath,
                                           TAknsItemID     aSkinID,
                                           TInt            aIconGraphicsIndex,
                                           TInt            aIconGraphicsMaskIndex )
    {
	DEBUG( + CLocSUPLSessionContainer::AddIconL );	
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
  
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* bitmapMask = NULL;
    
    TRAPD( err, AknsUtils::CreateIconL( skin,
                            aSkinID,
                            bitmap,
                            bitmapMask,
                            aIconFileWithPath,
                            aIconGraphicsIndex,
                            aIconGraphicsMaskIndex ) );

    if(err != KErrNone)
        {
        TRAP( err, AknIconUtils::CreateIconL( bitmap,
                                bitmapMask,
                                aIconFileWithPath,
                                aIconGraphicsIndex,
                                aIconGraphicsMaskIndex ) );
        }
    User::LeaveIfError( err );
    
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( bitmapMask );
    
    CGulIcon* icon = CGulIcon::NewL( bitmap, bitmapMask );
    
    // Remove the Bitmap and the Bitmap Icon from the Clean up stack since 
    // the ownership is taken over by the icon
    CleanupStack::Pop( bitmapMask );
    CleanupStack::Pop( bitmap ); 
    
    CleanupStack::PushL( icon );
    aIcons.AppendL( icon );
    CleanupStack::Pop( icon ); // icon
	DEBUG( - CLocSUPLSessionContainer::AddIconL );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLSessionContainer::IconFileNameLC
// -----------------------------------------------------------------------------
TFileName* CLocSUPLSessionContainer::IconFileNameLC() const
    {
    TFileName* fileName = new(ELeave) TFileName;
    CleanupStack::PushL( fileName );
    
	TFileName* tmpName = new(ELeave) TFileName;
    CleanupStack::PushL( tmpName );
	
    tmpName->Append( KDC_BITMAP_DIR );
    tmpName->Append( KLocSuplSettingsIconFileName );

    CEikAppUi *appUi = ( CEikAppUi *)( CEikonEnv::Static()->AppUi());
    TFileName appDrive = appUi->Application()->AppFullName();
    
    TParse parse;
    User::LeaveIfError( parse.Set( appDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), tmpName, NULL ) );
    
	fileName->Copy(parse.FullName());

    CleanupStack::PopAndDestroy( tmpName );

    return fileName;
    }    
  
// ----------------------------------------------------------------------------
// CLocSUPLSessionContainer::ListBoxL()
// ----------------------------------------------------------------------------
//
CEikListBox& CLocSUPLSessionContainer::ListBoxL()
    {
    User::LeaveIfNull( iListBox );
    return *iListBox;
    }

// -----------------------------------------------------------------------------
// TInt CLocSUPLSessionContainer::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLSessionContainer::SelectedItemIndex()
    {
    return iListBox->CurrentItemIndex();
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSessionContainer::FocusToSessionDetailL
// Focus move to server detail setting item
//
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::FocusToSessionDetailL( TInt aIndex )
	{
		iListBox->SetCurrentItemIndex( aIndex );
	}
				       
// ----------------------------------------------------------------------------
// CLocSUPLSessionContainer::CurrentSessionId()
// ----------------------------------------------------------------------------
//
TInt64 CLocSUPLSessionContainer::CurrentSessionId()
	{
	if( SelectedItemIndex() != KErrNotFound && SelectedItemIndex() < SessionCount() )
		return iListBoxModel->SessionIds()[ SelectedItemIndex() ];
	else
		return KErrNotFound;
	}

// ----------------------------------------------------------------------------
// CLocSUPLSessionContainer::SelectedCount()
// ----------------------------------------------------------------------------
//
TInt CLocSUPLSessionContainer::SelectedCount()
	{
	return iListBox->SelectionIndexes()->Count();
	}

// ----------------------------------------------------------------------------
// CLocSUPLSessionContainer::SessionCount()
// ----------------------------------------------------------------------------
//
TInt CLocSUPLSessionContainer::SessionCount()
	{
	return iListBox->Model()->NumberOfItems();
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::SessionIds()
// returns server id array
//
// ---------------------------------------------------------------------------
//  
RArray<TInt64>& CLocSUPLSessionContainer::SessionIds()
	{
	return iListBoxModel->SessionIds();
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::SetIndex()
// sets the current selected item
//
// ---------------------------------------------------------------------------
//  
void CLocSUPLSessionContainer::SetSelectedIndex()
	{
	iSelectedIndex = SelectedItemIndex();
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionContainer::CurrentSessionName()
// Get the currently focused session name
//
// ---------------------------------------------------------------------------
//  
TPtrC CLocSUPLSessionContainer::CurrentSessionName()
	{
	if( SelectedCount() == 1 )
		return iListBoxModel->SessionName( iListBox->SelectionIndexes()->At(0) );
	else
		return iListBoxModel->SessionName( SelectedItemIndex() );
	}

// -----------------------------------------------------------------------------
// CLocSUPLSessionContainer::ShowContextMenuL
// Shows context specific options menu
// -----------------------------------------------------------------------------
//
void CLocSUPLSessionContainer::ShowContextMenuL()
    {
	DEBUG( + CLocSUPLSessionContainer::ShowContextMenuL );	
	
	// Switch to Context specific options menu,
	// Show it and switch back to main options menu.
	CEikMenuBar* menuBar = iView->MenuBar();
	// TRAP displaying of menu bar.
	// If it fails, the correct resource is set back before leave.
	TRAPD( err, menuBar->TryDisplayContextMenuBarL() );
	User::LeaveIfError( err );
	
	DEBUG( - CLocSUPLSessionContainer::ShowContextMenuL );	
    }

// End of file
