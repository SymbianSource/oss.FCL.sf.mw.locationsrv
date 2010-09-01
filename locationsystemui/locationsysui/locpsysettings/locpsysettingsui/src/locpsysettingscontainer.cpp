/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides Postioning Method Settings view's container class methods.
*
*/


// System Includes

#include <eikapp.h>				// CEikApplication		
#include <aknlists.h>   		// CAknSingleLargeStyleListBox
#include <AknIconArray.h>		// Icon Array
#include <eikclbd.h>			// Column Listbox
#include <StringLoader.h> 		// StringLoader
#include <barsread.h>   		// TResourceReader
#include <akntitle.h>   		// CAknTitlePane
#include <featmgr.h> 			// FeatureManager
#include <AknsConstants.h> 		// Skin Ids
#include <gulicon.h>			// Gul Icon
#include <avkon.hrh> 
#include <data_caging_path_literals.hrh>
#include <locpsysettingsuiview.rsg>
#include <avkon.mbg>
#include <locpsysettings.mbg>
#include <eikenv.h>
#include <csxhelp/locpsys.hlp.hrh>
// User Includes
#include "locpsysettingscontainer.h"
#include "locpsysettingsview.h"
#include "locpsysettingsui.hrh"
#include "locpsysettingsuiuid.hrh"
#include "locpsyappdebug.h"
#include "locpsyeng.h"
#include "locpsysettingsinterface.h"
#include "locpsylbmodel.h"
#include "locpsykeyprocessor.h"
#include "locpsysettingsuiconsts.h"
#include <bldvariant.hrh> 
#include <AknDef.h>

// CONSTANTS
const TInt KViewListboxGranularity = 2;
const TInt KMinSelectedItemNumber = 0;

#ifdef _DEBUG

_LIT( KPosMethodSettingsContainerPanicMsg, "CLocPsySettingsContainer");
enum TPanicCode
    {
    KLocErrHelpContext
    };

// ==================== LOCAL FUNCTIONS ====================

void Panic(TPanicCode aReason)
    {
    User::Panic( KPosMethodSettingsContainerPanicMsg, aReason );
    }
    
#endif


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer* CLocPsySettingsContainer::NewL
// Two-phased constructor
// -----------------------------------------------------------------------------
CLocPsySettingsContainer* CLocPsySettingsContainer::NewL( 
										MLocPsyKeyProcessor* 	aKeyProcessor, 
										MObjectProvider* 	    aParent,
										CLocPsySettingsView* 	aView,
										MLocPsySettings* 	    aSettingsModel,
										const TRect& 		    aRect )
    {
    CLocPsySettingsContainer* view = 
        new (ELeave) CLocPsySettingsContainer( aKeyProcessor, aView );
    CleanupStack::PushL( view );
    view->SetMopParent( aParent );
    view->ConstructL( aSettingsModel, aRect );
    CleanupStack::Pop( view );
    return view;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::CLocPsySettingsContainer
// C++ Constrcutor
// -----------------------------------------------------------------------------
CLocPsySettingsContainer::CLocPsySettingsContainer( MLocPsyKeyProcessor* 	aKeyProcessor,
													CLocPsySettingsView* 	aView ) 
	: iKeyProcessor(aKeyProcessor),
	  iParentView(aView)
    {
 
 	
     __ASSERT_DEBUG( KLOC_HLP_PSYS().Length() <= iContextName.MaxLength(),
                    Panic( KLocErrHelpContext ) );
    iContextName = KLOC_HLP_PSYS();
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::ConstructL(const TRect& aRect)
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::ConstructL( MLocPsySettings* aSettingsModel, 
									       const TRect&     aRect )
    {
    CreateWindowL();
    CreateListboxL( aSettingsModel );
    MakeTitleL( R_LOC_MAIN_TITLE );
    SetRect( aRect );
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::~CLocPsySettingsContainer
// Destructor
// -----------------------------------------------------------------------------
CLocPsySettingsContainer::~CLocPsySettingsContainer()
    {    
    delete iListBox;
    }
    
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::OfferKeyEventL
// -----------------------------------------------------------------------------
TKeyResponse CLocPsySettingsContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    if ( iKeyProcessor->ProcessKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
    	{
    	return EKeyWasConsumed;
    	}
    	
    TInt count = CountComponentControls();
    for ( TInt i( 0 ) ; i < count; i++ )
        {
        if( ComponentControl(i)->OfferKeyEventL( aKeyEvent, aType ) ==
            EKeyWasConsumed )
            {
            return EKeyWasConsumed;
            }
        }
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::HandleResourceChange
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::HandleResourceChange(TInt aType)
    {
    CCoeControl::HandleResourceChange(aType);
    switch( aType )
    	{
    	case KAknsMessageSkinChange:
        	{
        	TRAPD( error, UpdateIconsL() );
        	if ( error != KErrNone )
            	{
            	iEikonEnv->HandleError( error );
            	}                 
        	DrawDeferred();
        	   	
        	break;
        	}
    	case KEikDynamicLayoutVariantSwitch:
    		{
			iParentView->HandleScreenSizeChange();
			DrawDeferred();
			break;
    		}
    	default:
    		{
    		break;
    		}
    	}
    }

// ---------------------------------------------------------------------------
// void CLocPsySettingsContainer::HandleListBoxEventL
//
// ---------------------------------------------------------------------------
//
void CLocPsySettingsContainer::HandleListBoxEventL( CEikListBox*  /* aListBox */, 
		 						   					TListBoxEvent aEventType )
	{
	switch (aEventType)
        {
        // List box Item Selection
        case EEventEnterKeyPressed:
        case EEventItemSingleClicked:
            {
            // PSY has been selected. The event has to be propagated to the 
            // command handler to pop-up the Context Specific menu
            
            // Set the Key event to represent EKeyOK            
            TKeyEvent   keyevent;
            keyevent.iCode = EKeyOK;
            
            // Set the Key event to Event Key
            TEventCode  keyCode( EEventKey );
            
            iKeyProcessor->ProcessKeyEventL( keyevent, keyCode );
            break;	
            }
        default:
           break;
        }
	}
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::GetHelpContext
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iContext = iContextName;
    aContext.iMajor = TUid::Uid( LOCPSYSETTINGSUI_UID3 );
    }
   
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::SelectedRow
// -----------------------------------------------------------------------------
TInt CLocPsySettingsContainer::SelectedRow() const
    {
    return iListBox->CurrentItemIndex();
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::SetSelectedRow
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::SetSelectedRow( TInt aIndex ) const
    {
    iListBox->SetCurrentItemIndex( aIndex );
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::HandleMarkCommandL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::HandleMarkCommandL( TInt aCommand )
	{
	AknSelectionService::HandleMarkableListProcessCommandL( aCommand, iListBox ); 
	} 

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::UpdateL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::UpdateL()
    {   
    iListBox->HandleItemAdditionL();
    iListBox->HandleItemRemovalL();
   
    TInt currentItem = iListBox->CurrentItemIndex();   
    MDesCArray* array = iListBox->Model()->ItemTextArray();
    if( KErrNotFound == currentItem || currentItem >= array->MdcaCount() )
        {
        TInt newSelected = array->MdcaCount() - 1;
        if( newSelected >= KMinSelectedItemNumber )
            {
            iListBox->SetCurrentItemIndex( newSelected );
            }
        }

    DrawDeferred();
    }
  
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::FocusChanged
// -----------------------------------------------------------------------------   
void CLocPsySettingsContainer::FocusChanged( TDrawNow aDrawNow )
	{
	if( iListBox )
		{
		iListBox->SetFocus( IsFocused(), aDrawNow );				
		}
	}


// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::SizeChanged
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::SizeChanged()
    {
    iListBox->SetRect( Rect());
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::CountComponentControls
// -----------------------------------------------------------------------------
TInt CLocPsySettingsContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::ComponentControl
// -----------------------------------------------------------------------------
CCoeControl* CLocPsySettingsContainer::ComponentControl( TInt /*aIndex*/ ) const
    {    
    return iListBox;
    }
 	
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::AddIconL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::AddIconL( CAknIconArray*  aIcons,
                                         const TDesC&    aIconFileWithPath,
                                         TAknsItemID     aSkinID,
                                         TInt            aIconGraphicsIndex,
                                         TInt            aIconGraphicsMaskIndex )
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* bitmapMask = NULL;
    AknsUtils::CreateIconL( skin, aSkinID, bitmap, bitmapMask,
                            aIconFileWithPath, aIconGraphicsIndex,
                            aIconGraphicsMaskIndex );
    CleanupStack::PushL( bitmap );
    CleanupStack::PushL( bitmapMask );
    
    CGulIcon* icon = CGulIcon::NewL( bitmap, bitmapMask );
    
    // Remove the Bitmap and the Bitmap Icon from the Clean up stack since 
    // the ownership is taken over by the icon
    CleanupStack::Pop( bitmapMask );
    CleanupStack::Pop( bitmap ); 
    
    CleanupStack::PushL( icon );
    aIcons->AppendL( icon );
    CleanupStack::Pop( icon ); // icon
    }
  

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::LocIconFileNameLC
// -----------------------------------------------------------------------------
TFileName* CLocPsySettingsContainer::LocIconFileNameLC() const
    {
    TFileName* fileName = new(ELeave) TFileName;
    CleanupStack::PushL( fileName );
    
	TFileName* tmpName = new(ELeave) TFileName;
    CleanupStack::PushL( tmpName );
	
    tmpName->Append( KDC_BITMAP_DIR );
    tmpName->Append( KPosMethodSettingsIconFileName );

    // Obtain the drive where the DLL is installed
    TFileName* dllDrive = new (ELeave) TFileName;
    CleanupStack::PushL( dllDrive );
    Dll::FileName( *dllDrive );
    
    TParse parse;
    User::LeaveIfError( parse.Set( *dllDrive, NULL, NULL ) );
    User::LeaveIfError( parse.Set( parse.Drive(), tmpName, NULL ) );
    
	fileName->Copy(parse.FullName());

    CleanupStack::PopAndDestroy( dllDrive );
    CleanupStack::PopAndDestroy( tmpName );

    return fileName;
    }

// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::AvkonIconFileNameLC
// -----------------------------------------------------------------------------
TFileName* CLocPsySettingsContainer::AvkonIconFileNameLC() const
    {
    TFileName* fileName = new(ELeave) TFileName;
	CleanupStack::PushL( fileName );
    *fileName = AknIconUtils::AvkonIconFileName();
    return fileName;
    }

//  Makes the title of the view
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::MakeTitleL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::MakeTitleL( TInt aResourceText )
    {
    // title pane is owned by the framework
    CAknTitlePane* title = static_cast<CAknTitlePane*>( StatusPane()->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    HBufC* buf = StringLoader::LoadL( aResourceText );
    title->SetText( buf ); // Takes ownership of buf
    }

// -----------------------------------------------------------------------------
// CLocBaseContainer::MakeEmptyTextListBoxL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::MakeEmptyTextListBoxL( CEikListBox&  aListBox,
                                                      TInt          aResourceText )
    {
    HBufC* textFromResourceFile;
    textFromResourceFile = iCoeEnv->AllocReadResourceLC( aResourceText );
    // Set text for empty list
    aListBox.View()->SetListEmptyTextL( *textFromResourceFile );
    CleanupStack::PopAndDestroy( textFromResourceFile );
    }

// -----------------------------------------------------------------------------
// CLocBaseContainer::StatusPane
// -----------------------------------------------------------------------------
CEikStatusPane* CLocPsySettingsContainer::StatusPane()
    {
    return iEikonEnv->AppUiFactory()->StatusPane();
    }
  
// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::CreateListboxL
// 
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::CreateListboxL(MLocPsySettings* aSettingsModel)
    {
    delete iListBox;
    iListBox = NULL;    

    iListBox = new(ELeave) CAknSingleGraphicStyleListBox();
    iListBox->SetContainerWindowL( *this );
    iListBox->SetListBoxObserver( this );
    
	TResourceReader reader;
    iEikonEnv->CreateResourceReaderLC( reader, R_LOCATION_PSY_LISTBOX );
    iListBox->ConstructFromResourceL( reader );
    CleanupStack::PopAndDestroy(); //reader
    
    UpdateIconsL();

    CLocPsyLBModel* lbmodel = CLocPsyLBModel::NewL( aSettingsModel );
	iListBox->Model()->SetItemTextArray( lbmodel );
	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	MakeEmptyTextListBoxL( *iListBox, R_LOC_PSY_METHODS_LIST_EMPTY );
	
    // Make scroll arrows
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,
                                                         CEikScrollBarFrame::EAuto );
    iListBox->ActivateL();
    }


// -----------------------------------------------------------------------------
// CLocPsySettingsContainer::UpdateIconsL
// -----------------------------------------------------------------------------
void CLocPsySettingsContainer::UpdateIconsL()
    {
    CAknIconArray* icons = new( ELeave ) CAknIconArray( KViewListboxGranularity );
    CleanupStack::PushL( icons );

	TFileName *locIconFile = LocIconFileNameLC();

    AddIconL( icons, 
    		  *locIconFile,
    		  KAknsIIDQgnPropPslnActive, 
    		  EMbmLocpsysettingsQgn_prop_psln_active , 
    		  EMbmLocpsysettingsQgn_prop_psln_active_mask );

    CleanupStack::PopAndDestroy(locIconFile);

    TFileName* aknIconFile = AvkonIconFileNameLC();

    AddIconL( icons,
    		  *aknIconFile,
    		  KAknsIIDQgnPropEmpty, 
    		  EMbmAvkonQgn_prop_empty , 
    		  EMbmAvkonQgn_prop_empty_mask );

    CleanupStack::PopAndDestroy(aknIconFile);

    //get old icons and delete those
    CAknIconArray* Oldicons = static_cast<CAknIconArray*>(
        iListBox->ItemDrawer()->ColumnData()->IconArray() );    
    delete Oldicons;

    //set new icons to array
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );  
    CleanupStack::Pop(icons);
    }

// End of File
