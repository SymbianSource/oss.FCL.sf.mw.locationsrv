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
* Description:  SUPL server UI View's container
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
#include <avkon.mbg>						// for select server (mark) icon
#include <avkon.rsg>
#include <eiklbx.h>
#include <ErrorUI.h>
#include <AknsUtils.h> 
#include <aknapp.h>
#include <locsuplsettings.mbg>				// for server disable icon

// User Includes
#include "locsuplsettings.hrh"
#include "locsuplsettingsuid.hrh"
#include "locsuplservercontainer.h"
#include "locsuplserverview.h"
#include "locsuplserverlbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingseventhandler.h"
#include "locsupldebug.h"

// CONSTANT DEFINITIONS
const TInt KIconArrayGranularity = 2; // The number of default icons
_LIT( KLocSuplSettingsIconFileName, "locsuplsettings.mif" );


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLServerContainer::CLocSUPLServerContainer
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//	
CLocSUPLServerContainer::CLocSUPLServerContainer( CLocSUPLServerView* aView )
    : iView( aView )
	{
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLServerContainer::~CLocSUPLServerContainer
// Destructor
//
// ---------------------------------------------------------------------------	
//
CLocSUPLServerContainer::~CLocSUPLServerContainer()
	{
	DEBUG( + CLocSUPLServerContainer::~CLocSUPLServerContainer );	
	// Delete the List box model only after deleting the list box
	delete iListBox;
	iListBox = NULL;
	
	delete iListBoxModel;
	iListBoxModel = NULL;
	DEBUG( - CLocSUPLServerContainer::~CLocSUPLServerContainer );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLServerContainer* CLocSUPLServerContainer::NewL
// Static Two phase contructor that instantiates the CLocSUPLServerContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param aEventHandler                 Reference to the parent view
// @return CLocSUPLServerContainer*	Reference to the object created
// ---------------------------------------------------------------------------    
//
CLocSUPLServerContainer* CLocSUPLServerContainer::NewL( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLServerView* 		aView )
	{	
	DEBUG( + CLocSUPLServerContainer::NewL );	
    CLocSUPLServerContainer* self = NewLC( aRect, 
                                           aEngine,
                                           aView );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLServerContainer::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerContainer* CLocSUPLServerContainer::NewLC
// Static Two phase contructor that instantiates the CLocSUPLServerContainer
//
// @param aRect							Client rectangle
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// @param aEventHandler                 Reference to the parent view
// @return CLocSUPLServerContainer*	Reference to the object created
// ---------------------------------------------------------------------------  
//  
CLocSUPLServerContainer* CLocSUPLServerContainer::NewLC( 
                    		const TRect&               	aRect,
                          	CLocSUPLSettingsUiEngine&	aEngine,
                          	CLocSUPLServerView* 		aView )
	{	
    CLocSUPLServerContainer* self = new ( ELeave ) CLocSUPLServerContainer(
                                                        aView );
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aEngine );
	return self;
	}

// --------------------------------------------------------------------------- 
// void CLocSUPLServerContainer::ConstructL
// Second phase of the two phase Construction process
//
// @param aRect				 Client rectangle
// @param aEngine            Reference to the SUPL Settings engine
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::ConstructL( 
                                    const TRect& 	aRect ,
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLServerContainer::ConstructL );	
	//Make this control a window-owning control
	CreateWindowL(); 
	
	// Create a New Title for the View
	MakeTitleL( R_LOC_SUPL_SERVER_DETAIL_TITLE );
	
	CreateListboxL( aEngine );
	
	// Get the Help context
	iContextName = KLOC_HLP_SUPL();
	
    SetRect( aRect );
    ActivateL();
	DEBUG( - CLocSUPLServerContainer::ConstructL );	
	}	

// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::UpdateScreenL
// Refreshes the screen so that the list box reflects the latest contents
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::UpdateScreenL()
    {
	DEBUG( + CLocSUPLServerContainer::UpdateScreenL );	
	
	TInt oldCount 	= SlpIds().Count();	
	TInt64 slpId;
	if( iSelectedIndex != KErrNotFound && SlpIds().Count() > iSelectedIndex )
		{
		slpId = SlpIds()[ iSelectedIndex ];		
		}
	
	if ( iListBoxModel )
		{
		iListBoxModel->UpdateSlpIdsL();
		}    
		
    TInt newCount 	= SlpIds().Count();
    
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
				TInt newIndex = SlpIds().Find( slpId );
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
	DEBUG( - CLocSUPLServerContainer::UpdateScreenL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::HandleResourceChange
//
// ---------------------------------------------------------------------------	
void CLocSUPLServerContainer::HandleResourceChange(TInt aType)
	{
	DEBUG( + CLocSUPLServerContainer::HandleResourceChange );	
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
	DEBUG( - CLocSUPLServerContainer::HandleResourceChange );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerContainer::GetHelpContext
//
// -----------------------------------------------------------------------------
//
void CLocSUPLServerContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
	DEBUG( + CLocSUPLServerContainer::GetHelpContext );	
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( KLOCSUPLSETTINGSUIUID3 );
	DEBUG( - CLocSUPLServerContainer::GetHelpContext );	
    }
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLServerContainer::CountComponentControls
//
// ---------------------------------------------------------------------------
TInt CLocSUPLServerContainer::CountComponentControls() const
	{
	return 1;
	}

// ---------------------------------------------------------------------------
// CCoeControl* CLocSUPLServerContainer::ComponentControl
//
// ---------------------------------------------------------------------------
CCoeControl* CLocSUPLServerContainer::ComponentControl( TInt /*aIndex*/ ) const
	{
	return iListBox;	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::OfferKeyEventL
//
// ---------------------------------------------------------------------------	
//
TKeyResponse CLocSUPLServerContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
										   					  TEventCode aType )
	{
    if ( ( aType == EEventKey || aType == EEventKeyUp) )
        {
        if( aKeyEvent.iCode == EKeyBackspace )
            {
            iView->HandleCommandL( ELocSuplServerDelete );            
            return EKeyWasConsumed;
            }    
        }
    // now it's iListBox's job to process the key event
    return iListBox->OfferKeyEventL( aKeyEvent, aType ); 
	}
	    
// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::HandleListBoxEventL( CEikListBox*  /* aListBox */, 
		 						   					 TListBoxEvent aEventType )
	{
	DEBUG( + CLocSUPLServerContainer::HandleListBoxEventL );	
	switch ( aEventType )
        {
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            if(iListBox->SelectionIndexes()->Count() > 0)
                {
                ShowContextMenuL();
                }
            else
                {
                iView->HandleCommandL(ELocSuplServerOpen);
                }
			break;	
			}
        default:
           break;
        }
	DEBUG( - CLocSUPLServerContainer::HandleListBoxEventL );	
	}
	         		 	
// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::FocusChanged
//
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::FocusChanged( TDrawNow aDrawNow )
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
// CLocSUPLServerContainer::SizeChanged
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::SizeChanged()
	{
	if ( iListBox )
		{
		iListBox->SetRect( Rect() );
		}
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::MakeTitleL
// Sets the Title text
//
// @param aResourceText Resource to create title
// ---------------------------------------------------------------------------
//
void CLocSUPLServerContainer::MakeTitleL( TInt aResourceText )
	{
	DEBUG( + CLocSUPLServerContainer::MakeTitleL );	
	// Obtain the title from the Status Pane
	CAknTitlePane* title = static_cast < CAknTitlePane* >( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
       
    // Set the Title's buffer    
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
	DEBUG( - CLocSUPLServerContainer::MakeTitleL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::StatusPane
// Returns the Appui's status pane
//
// @return CEikStatusPane* Statuspane pointer
// ---------------------------------------------------------------------------
//
CEikStatusPane* CLocSUPLServerContainer::StatusPane()
	{
	return iEikonEnv->AppUiFactory()->StatusPane();	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLServerContainer::CreateListboxL
// Creates the List box and the Listbox model. Associates the list box model 
// with the list box
//
// @param aEngine                       Reference to the SUPL Settings
//                                      engine 
// ---------------------------------------------------------------------------	
//
void CLocSUPLServerContainer::CreateListboxL( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLServerContainer::CreateListboxL );	
	// Create the Listbox model
    iListBoxModel = CLocSUPLServerLBModel::NewL( aEngine );
	
	// CAknSingleNumberStyleListBox "2\tTextLabel\t0\t1"
	
    iListBox = new ( ELeave ) CAknSingleNumberStyleListBox;
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
 	
 	CleanupStack::PopAndDestroy( iconFile );
	
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    CleanupStack::Pop( icons );        
    
    // Set the model & observer
    iListBox->SetListBoxObserver( this  );

    iListBox->Model()->SetItemTextArray( iListBoxModel );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );									
    iListBox->ActivateL();   
	DEBUG( - CLocSUPLServerContainer::CreateListboxL );	
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerContainer::AddIconL
// -----------------------------------------------------------------------------
void CLocSUPLServerContainer::AddIconL( CAknIconArray&  aIcons,
                                           const TDesC&    aIconFileWithPath,
                                           TAknsItemID     aSkinID,
                                           TInt            aIconGraphicsIndex,
                                           TInt            aIconGraphicsMaskIndex )
    {
	DEBUG( + CLocSUPLServerContainer::AddIconL );	
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
	DEBUG( - CLocSUPLServerContainer::AddIconL );	
    }

// -----------------------------------------------------------------------------
// CLocSUPLServerContainer::IconFileNameLC
// -----------------------------------------------------------------------------
TFileName* CLocSUPLServerContainer::IconFileNameLC() const
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
// CLocSUPLServerContainer::ListBoxL()
// ----------------------------------------------------------------------------
//
CEikListBox& CLocSUPLServerContainer::ListBoxL()
    {
    User::LeaveIfNull( iListBox );
    return *iListBox;
    }

// -----------------------------------------------------------------------------
// TInt CLocSUPLServerContainer::SelectedItemIndex
// Returns the Selected element. The list box model defines the enumeration that
// would determine the current element that has been selected
//
// @return TInt Index of the item selected
// -----------------------------------------------------------------------------
//
TInt CLocSUPLServerContainer::SelectedItemIndex()
    {
    return iListBox->CurrentItemIndex();
    }
			       
// ----------------------------------------------------------------------------
// CLocSUPLServerContainer::CurrentSlpId()
// ----------------------------------------------------------------------------
//
TInt64 CLocSUPLServerContainer::CurrentSlpId()
	{
	if( SelectedItemIndex() != KErrNotFound && SelectedItemIndex() < ServerCount() )
		return iListBoxModel->SlpIds()[ SelectedItemIndex() ];
	else
		return KErrNotFound;
	}

// ----------------------------------------------------------------------------
// CLocSUPLServerContainer::SelectedCount()
// ----------------------------------------------------------------------------
//
TInt CLocSUPLServerContainer::SelectedCount()
	{
	return iListBox->SelectionIndexes()->Count();
	}

// ----------------------------------------------------------------------------
// CLocSUPLServerContainer::SelectedCount()
// ----------------------------------------------------------------------------
//
TInt CLocSUPLServerContainer::ServerCount()
	{
	return iListBox->Model()->NumberOfItems();
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerContainer::SlpIds()
// returns server id array
//
// ---------------------------------------------------------------------------
//  
RArray<TInt64>& CLocSUPLServerContainer::SlpIds()
	{
	return iListBoxModel->SlpIds();
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerContainer::SetIndex()
// sets the current selected item
//
// ---------------------------------------------------------------------------
//  
void CLocSUPLServerContainer::SetSelectedIndex()
	{
	iSelectedIndex = SelectedItemIndex();
	}

// -----------------------------------------------------------------------------
// CLocSUPLServerContainer::ShowContextMenuL
// -----------------------------------------------------------------------------
//
void CLocSUPLServerContainer::ShowContextMenuL()
    {
	DEBUG( + CLocSUPLServerContainer::ShowContextMenuL );	
	
	// Switch to Context specific options menu,
	// Show it and switch back to main options menu.
	CEikMenuBar* menuBar = iView->MenuBar();
	// TRAP displaying of menu bar.
	// If it fails, the correct resource is set back before leave.
	TRAPD( err, menuBar->TryDisplayContextMenuBarL() );
	User::LeaveIfError( err );
	
	DEBUG( - CLocSUPLServerContainer::ShowContextMenuL );	
    }
// -----------------------------------------------------------------------------
// CLocSUPLServerContainer::HandlePointerEventL
// -----------------------------------------------------------------------------
//
void CLocSUPLServerContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CCoeControl::HandlePointerEventL( aPointerEvent ); 
    if(aPointerEvent.iType == TPointerEvent::EButton1Down && iListBox->Model()->NumberOfItems() == 0)
        {
        ShowContextMenuL();
        }
    }

// End of file
