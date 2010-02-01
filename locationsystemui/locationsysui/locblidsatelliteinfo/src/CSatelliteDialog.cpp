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
* Description: BLID Satellite Info dialog definition
*
*/


// INCLUDE FILES
#include <aknlists.h>
#include <aknPopup.h>
#include <AknQueryDialog.h>
#include <SatInfo.rsg>
#include <avkon.rsg>
#include <aknlayoutscalable_apps.cdl.h> // For LAF
#include <lbssatellite.h>
#include <aknborders.h>
#include <AknsFrameBackgroundControlContext.h>
#include <aknappui.h>
#include <skinlayout.cdl.h>
using namespace SkinLayout;

#include "Debug.h"
#include "MSatelliteEng.h"
#include "CSatelliteDialog.h"
#include "satellite.hrh"
#include "satellitecontrol.h"

// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// CSatelliteDialog::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CSatelliteDialog* CSatelliteDialog::NewL( MSatelliteEng& aEngine )
    {
    CSatelliteDialog* self = new( ELeave ) CSatelliteDialog(aEngine);
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop(); // self
    return self;
    }


// ----------------------------------------------------------------------------
// CSatelliteDialog::CSatelliteDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CSatelliteDialog::CSatelliteDialog(MSatelliteEng& aEngine): iEngine(aEngine) 
    {
    }

// ---------------------------------------------------------
// CSatelliteDialog::~CSatelliteDialog
// ---------------------------------------------------------
//
CSatelliteDialog::~CSatelliteDialog()
    {
    delete iBgContext;    
    }

// ---------------------------------------------------------
// CSatelliteDialog::ConstructL
// ---------------------------------------------------------
//
void CSatelliteDialog::ConstructL( )
    {
    TRect windowRect = Rect();

	TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, Popup_windows_skin_placing__frame_general__Line_2());
	
	//Determine the outer and inner rects of the dialog for drawing the frames.
	TAknLayoutRect bottomRight;
	bottomRight.LayoutRect(windowRect, ELayoutEmpty, 169, ELayoutEmpty, ELayoutEmpty, 2, 7,7);
    bottomRight.LayoutRect(windowRect, Popup_windows_skin_placing__frame_general__Line_5());
        
    TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
	TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
	
	//Background context responsible for drawing the frame and the
	//background of the frame
    iBgContext = CAknsFrameBackgroundControlContext::NewL(
			            KAknsIIDQsnFrPopup, outerRect, innerRect, ETrue );
	
	//Create satellite control responsible for all the drawing on the dialog.
	iSatelliteControl = CSatelliteControl::NewL(iEngine);
    }

// ---------------------------------------------------------
// CSatelliteDialog::SetLaunchView
// ---------------------------------------------------------
//
void CSatelliteDialog::SetLaunchView(TSatDialogView aView)
	{
	//Satellite controls is responsible for the initial launch 
	//view of the dialog. Pass the initial launch view of the dialog
	//to the control
	iSatelliteControl->SetLaunchView(aView);
	}

// ---------------------------------------------------------
// CSatelliteDialog::OkToExitL
// ---------------------------------------------------------
//
TBool CSatelliteDialog::OkToExitL(TInt aButtonId)
	{
	if(aButtonId == ESatelliteChangeView)
		{
		// Change view soft key pressed.Notify the control to change 
		// the view and draw.
		iSatelliteControl->ChangeView();		
		DrawNow();
		return EFalse;
		}
	else
		{
		return CAknDialog::OkToExitL(aButtonId);
		}
	}

// ----------------------------------------------------------------------------
// CSatelliteDialog::CreateCustomControlL()
// ----------------------------------------------------------------------------
//
SEikControlInfo CSatelliteDialog::CreateCustomControlL(TInt aControlType)
    {
    // This function is invoked by the dialog frame work when if 
    // finds an unknow id in the dialog resouce.This function is
    // expected to return the control which is reponsible for all
    // the drawings that will happen in the dialog.
    SEikControlInfo controlInfo;
    controlInfo.iControl = NULL;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iFlags = 0;
    
    switch(aControlType)
        {
        case ESatelliteControl:
        	// This dialog takes ownerwhip of the control and
        	// wil also delete the control once the dialog is
        	// destroyed.
            controlInfo.iControl = iSatelliteControl;
            break;
            
        default:
            break;
        }
    return controlInfo;
    }


// ----------------------------------------------------------------------------
// CSatelliteDialog::SetSizeAndPosition
// ----------------------------------------------------------------------------
//
void CSatelliteDialog::SetSizeAndPosition(const TSize& /*aSize*/)
    {
    TRect rect = iSatelliteControl->CalculateDialogRect();
    SetRect( rect );
    }

// ----------------------------------------------------------------------------
// CSatelliteDialog::SizeChanged()
// ----------------------------------------------------------------------------
//
void CSatelliteDialog::SizeChanged()
	{
	CAknDialog::SizeChanged();
	TRect windowRect = Rect();

	TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, Popup_windows_skin_placing__frame_general__Line_2());

	TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, Popup_windows_skin_placing__frame_general__Line_5());

	TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
	TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);

	iBgContext->SetFrameRects( outerRect, innerRect );
	iBgContext->SetParentPos( PositionRelativeToScreen() );
	TRAP_IGNORE(UpdateL());
	}

// ----------------------------------------------------------------------------
// CSatelliteDialog::SupplyMopObject()
// ----------------------------------------------------------------------------
//
TTypeUid::Ptr CSatelliteDialog::MopSupplyObject(TTypeUid aId)
	{	
    return MAknsControlContext::SupplyMopObject(aId, iBgContext );
	}

// ----------------------------------------------------------------------------
// CSatelliteDialog::UpdateL
// ----------------------------------------------------------------------------
//
void CSatelliteDialog::UpdateL()
    {    
    CEikButtonGroupContainer& buttonContainer = ButtonGroupContainer();
    
    // Dim the "ChangeView" command when satellite data is not 
    // available
    if(!iEngine.IsSatelliteDataAvailable())
    	{
		buttonContainer.SetCommandSetL(R_AVKON_SOFTKEYS_CLOSE);
		buttonContainer.DrawNow();
    	}
    else
    	{
    	buttonContainer.SetCommandSetL(R_SATELLITE_DIALOG_CBA);
    	buttonContainer.DrawNow();
    	}
    DrawNow();
    }

// ----------------------------------------------------------------------------
// CSatelliteDialog::OfferKeyEventL
// ----------------------------------------------------------------------------
//
TKeyResponse CSatelliteDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType)
    {
    if ( aType == EEventKey && aKeyEvent.iCode == EKeyEnter ||
         aType == EEventKey && aKeyEvent.iScanCode == EStdKeyEnter  )
		{
        iSatelliteControl->ChangeView();
        DrawNow();
		return EKeyWasConsumed;
		}
    return CAknDialog::OfferKeyEventL( aKeyEvent, aType );
    }
// End of File
