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
* Description: 
*     Implements the control for responsible for all drawings
*     in the satellite dialog
*
*/


// INCLUDE FILES
#include <skinlayout.cdl.h>
#include <AknsSkinInstance.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h>
#include <StringLoader.h>
#include <AknsFrameBackgroundControlContext.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknViewAppUi.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <SatInfo.rsg>
#include <AknBidiTextUtils.h>
#include <lbssatellite.h>
#include <satellite.mbg>
#include <eikapp.h>
#include <AknLayout2Def.h>
#include <layoutmetadata.cdl.h>
//===HEADER FILE INCLUDED FOR HACK=======
#include <AknLayoutFont.h>
#include <aknlocationed.h>
// header file added for multiple drive support.
#include <pathinfo.h>

#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif //RD_TACTILE_FEEDBACK

#include "satellitecontrol.h"

#include  <centralrepository.h>
#include <locationvariationdomaincrkeys.h>
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSatelliteControl::CSatelliteControl
// ---------------------------------------------------------
//
CSatelliteControl::CSatelliteControl(MSatelliteEng& aEngine): iEngine(aEngine)               
    {
    //By defaul start with signal strength view
    iCurrentSatelliteView = ESignalStrengthView;
    iSatelliteSignalHeading = NULL;
    iSatelliteGrapgicHeading = NULL;
    }

// ---------------------------------------------------------
// CSatelliteControl::~CSatelliteControl
// ---------------------------------------------------------
//
CSatelliteControl::~CSatelliteControl()
    {    
    AknsUtils::DeregisterControlPosition(this);
    delete iEditorContext;
    delete iSatelliteSignalHeading;
    delete iSatelliteGrapgicHeading;
    delete iCompassViewHeading;
    delete iNoSatelliteData;
    delete iSatActiveBmp;
    delete iSatActiveBmpMask;
    delete iSatInActiveBmp;
    delete iSatInActiveBmpMask;
    delete iSatFrame;
    delete iSatFrameMask;
    delete iFirmamentBmp;
    delete iFirmamentBmpMask;
    delete iFirmaActiveBmp;
    delete iFirmaActiveBmpMask;
    delete iFirmaInActiveBmp;
    delete iFirmaInActiveBmpMask;
    delete iSunBmp;
    delete iSunBmpMask;
    delete iMoonBmp;
    delete iMoonBmpMask;
    delete iFirmamentCompassBmp;
    delete iFirmamentCompassBmpMask;
    delete iFixBmp;
    delete iFixBmpMask;
	delete iAltitudeWindowHeading;
	delete iSpeedWindowHeading;
	delete iDirectionWindowHeading;
	delete iSpeedFormatKmph;
	delete iSpeedFormatMph;
	delete iAltitudeFormatM;
	delete iAltitudeFormatFt;
	delete iDirectionFormat;
    delete iCompassNorth;
    delete iCompassSouth;
    delete iCompassWest;
	delete iCompassEast;
    delete iNorth;
    delete iSouth;
    delete iWest;
	delete iEast;
	delete i2DString;
	delete i3DString;
    iSignalBarRects.Reset();
    iSatelliteNumberLayouts.Reset();
    }

// ---------------------------------------------------------
// CSatelliteControl::NewL
// ---------------------------------------------------------
//
CSatelliteControl* CSatelliteControl::NewL(MSatelliteEng& aEngine)
    {
    CSatelliteControl* self =
        new(ELeave) CSatelliteControl(aEngine);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    // self    
    return self;
    }

// ---------------------------------------------------------
// CSatelliteControl::CountComponentControls
// ---------------------------------------------------------
//
TInt CSatelliteControl::CountComponentControls() const
    {
    //No additional controls. All the drawing is done
    //by one control. Helps reduce flickering
    return 0;
    }

// ---------------------------------------------------------
// CSatelliteControl::ComponentControl
// ---------------------------------------------------------
//
CCoeControl* CSatelliteControl::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// ---------------------------------------------------------
// CSatelliteControl::FocusChanged
// ---------------------------------------------------------
//
void CSatelliteControl::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    //No implemenation here for focus change
    }

// ---------------------------------------------------------
// CSatelliteControl::MinimumSize
// ---------------------------------------------------------
//
TSize CSatelliteControl::MinimumSize()
    {    
	TRect rect = CalculateDialogRect();
    TSize size = rect.Size();
    return size;
    }

// ---------------------------------------------------------
// CSatelliteControl::DialogRect
// ---------------------------------------------------------
//
TRect CSatelliteControl::DialogRect() const
    {    
    return CalculateDialogRect();
    }

// ---------------------------------------------------------
// CSatelliteControl::DialogSize
// ---------------------------------------------------------
//
TSize CSatelliteControl::DialogSize() const
    {
	TRect rect = CalculateDialogRect();
	return (TSize(rect.Size()));
    }

// ---------------------------------------------------------
// CSatelliteControl::WindowLayout
// ---------------------------------------------------------
//
TAknWindowLineLayout CSatelliteControl::WindowLayout() const
    {
    //Get the dialogs layout and return it.
    AknLayoutUtils::TAknCbaLocation cbaLocation( AknLayoutUtils::CbaLocation() );
    TInt variety(0);
    
    //Varient for LAF changes based on the location of the softkeys
    if(Layout_Meta_Data::IsLandscapeOrientation())		
        {
        if ( cbaLocation == AknLayoutUtils::EAknCbaLocationRight )
    		{
    		variety = 1;
    		}
    	else if ( cbaLocation == AknLayoutUtils::EAknCbaLocationLeft )
    		{
    		variety = 2;
    		}
        }
    else
        {
        variety = 0;
        }        
	
	//Return the layout line of the dialog from LAF
    return AknLayoutScalable_Apps::popup_blid_sat_info2_window(variety).LayoutLine();
    }

// ----------------------------------------------------------------------------
// CSatelliteControl::CalculateDialogRect
// Calculates rectangle according to current LAF information.
// ----------------------------------------------------------------------------
//
TRect CSatelliteControl::CalculateDialogRect() const
    {
    TRect mainPaneRect;
    TRect screenRect;
    TInt variety(0);
    TAknLayoutRect mainPane;
    
    AknLayoutUtils::TAknCbaLocation cbaLocation( AknLayoutUtils::CbaLocation() );
    if( Layout_Meta_Data::IsLandscapeOrientation() )	 	
        {
        if ( cbaLocation == AknLayoutUtils::EAknCbaLocationRight )
    		{
    		screenRect = iAvkonAppUi->ApplicationRect();
    		variety = 1;
    		mainPane.LayoutRect( screenRect, AknLayoutScalable_Apps::main_pane( 13 ) );           
		mainPaneRect = mainPane.Rect();
    		}
    	else if ( cbaLocation == AknLayoutUtils::EAknCbaLocationLeft )
    		{
    		variety = 2;
    		mainPaneRect = iAvkonAppUi->ClientRect();
    		}
    	else if ( cbaLocation == AknLayoutUtils::EAknCbaLocationBottom )
    		{
	        variety = 0;
	    	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
    		}    		
        }
    else
        {
        variety = 0;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
        }    
    
    TAknLayoutRect mainSatPane;
    mainSatPane.LayoutRect( mainPaneRect, AknLayoutScalable_Apps::popup_blid_sat_info2_window( variety ) );
      
    return mainSatPane.Rect();
    
    }
// ---------------------------------------------------------
// CBlidManualWayPointCtrl::Layout
// ---------------------------------------------------------
//
void CSatelliteControl::Layout()
    {
    SizeChanged();
    }

// ---------------------------------------------------------
// CBlidManualWayPointCtrl::ChangeView
// ---------------------------------------------------------
//
void CSatelliteControl::ChangeView()
	{
    // Change the view variable and update the rects
	switch ( iCurrentSatelliteView )
	    {
	    case ESignalStrengthView:
	        {
	        iCurrentSatelliteView = EGraphicalView;
	        break;
	        }	    
	    case EGraphicalView:
	        {
	        TInt settingsValue = 0;
            CRepository* repository = CRepository::NewL( TUid::Uid( KCRUidLocLocalVariation) );
            CleanupStack::PushL( repository );
            User::LeaveIfError( repository->Get( KLocHideCoordinates,
                                                      settingsValue ));  
            CleanupStack::PopAndDestroy( repository );
            
            if ( ELocCoordinatesHidden == settingsValue)
            {
            	 iCurrentSatelliteView = ESignalStrengthView;
            }
            else
           {
           		iCurrentSatelliteView = ECompassView;
           }
	        break;
	        }
	    case ECompassView:
	        {
	        iCurrentSatelliteView = ESignalStrengthView;
	        break;
	        }
	    default:
	        {
	        break;
	        }
	    }	
	}

// ---------------------------------------------------------
// CSatelliteControl::SetLaunchView
// ---------------------------------------------------------
//
void CSatelliteControl::SetLaunchView(TSatDialogView aView)
	{
	iCurrentSatelliteView = aView;
	}

// ---------------------------------------------------------
// CSatelliteControl::PrepareGraphicalViewIcons
// ---------------------------------------------------------
//
void CSatelliteControl::PrepareGraphicalViewIcons()
	{
	AknIconUtils::SetSize(iFirmamentBmp, iFirmamentRect.Size());
	AknIconUtils::SetSize(iFirmamentBmpMask, iFirmamentRect.Size());
	
	AknIconUtils::SetSize(iFirmaActiveBmp, iSatelliteRect.Size());
	AknIconUtils::SetSize(iFirmaActiveBmpMask, iSatelliteRect.Size());
	AknIconUtils::SetSize(iFirmaInActiveBmp, iSatelliteRect.Size());
	AknIconUtils::SetSize(iFirmaInActiveBmpMask, iSatelliteRect.Size());	
	
	AknIconUtils::SetSize(iMoonBmp, iSunMoonRect.Size());
	AknIconUtils::SetSize(iMoonBmpMask, iSunMoonRect.Size());
	AknIconUtils::SetSize(iSunBmp, iSunMoonRect.Size());
	AknIconUtils::SetSize(iSunBmpMask, iSunMoonRect.Size());
	}

// ---------------------------------------------------------
// CSatelliteControl::PrepareSignalStrengthViewIcons
// ---------------------------------------------------------
//
void CSatelliteControl::PrepareSignalStrengthViewIcons()
	{	
    if(!iSignalBarRects.Count())
    	{
    	return;
    	}
    	    
	TRect rect = iSignalBarRects[1];
	AknIconUtils::SetSize(iSatFrame, rect.Size());
	AknIconUtils::SetSize(iSatFrameMask, rect.Size());
	}

// ---------------------------------------------------------
// CSatelliteDialog::PrepareCompassViewIcons
// ---------------------------------------------------------
//    
void CSatelliteControl::PrepareCompassViewIcons()
    {  	
	AknIconUtils::SetSize(iFirmamentCompassBmp, iFirmamentCompassRect.Size());
	AknIconUtils::SetSize(iFirmamentCompassBmpMask, iFirmamentCompassRect.Size());
	
	AknIconUtils::SetSize(iFixBmp, iTypeRect.Size());
	AknIconUtils::SetSize(iFixBmpMask, iTypeRect.Size());
    }

// ---------------------------------------------------------
// CSatelliteControl::PrepareSignalStrengthViewRectsAndIcons
// ---------------------------------------------------------
//
void CSatelliteControl::PrepareSignalStrengthViewRectsAndIcons()
	{
	iSignalBarRects.Reset();
    iSatelliteNumberLayouts.Reset();
	TAknLayoutScalableParameterLimits paramLimits;
	paramLimits = AknLayoutScalable_Apps::cell_popup_blid_pane_ParamLimits();
	
	TInt numberOfRows = paramLimits.LastRow() - paramLimits.FirstRow();	
	TInt numberOfColumns = paramLimits.LastColumn() - paramLimits.FirstColumn();
	
	TAknLayoutRect layoutWindow;
    layoutWindow.LayoutRect(Rect(), AknLayoutScalable_Apps::grid_popup_blid_pane().LayoutLine());	
    TRect gridRect = layoutWindow.Rect();
	
	iNumberOfRows = paramLimits.LastRow() - paramLimits.FirstRow() + 1;
	iNumberOfColumns = paramLimits.LastColumn() - paramLimits.FirstColumn() + 1;
	
	for(TInt row = paramLimits.FirstRow() ; row <= paramLimits.LastRow(); ++row)
		{
		for(TInt column = paramLimits.FirstColumn() ; column <= paramLimits.LastColumn(); ++column)
			{
			// First get and save the rects of the signal bars
			layoutWindow.LayoutRect(gridRect, AknLayoutScalable_Apps::cell_popup_blid_pane(column, row).LayoutLine());
			TRect cellRect = layoutWindow.Rect();
			
			layoutWindow.LayoutRect(cellRect, AknLayoutScalable_Apps::cell_popup_blid_pane_g1().LayoutLine());
			TRect barRect = layoutWindow.Rect();
			iSignalBarRects.Append(barRect);
			
			// Save the layouts of the satellite numbers
			TAknLayoutText satNumberLayout;
			satNumberLayout.LayoutText(cellRect, AknLayoutScalable_Apps::cell_popup_blid_pane_t1().LayoutLine());			
			iSatelliteNumberLayouts.Append(satNumberLayout);
			}
		}
	PrepareSignalStrengthViewIcons();
	}

// ---------------------------------------------------------
// CSatelliteControl::PrepareGraphicalViewRectsAndIcons
// ---------------------------------------------------------
//
void CSatelliteControl::PrepareGraphicalViewRectsAndIcons()
	{
	TAknLayoutRect layoutWindow;
    layoutWindow.LayoutRect(Rect(), 
    						AknLayoutScalable_Apps::blid_firmament_pane().LayoutLine());
	    						
	// Rect to draw the firmament
    iFirmamentRect = layoutWindow.Rect();
    
    layoutWindow.LayoutRect(iFirmamentRect,
    						AknLayoutScalable_Apps::aid_firma_cardinal().LayoutLine());
        
    //Layout to draw the cardinal point W
    iCardinalWLayout.LayoutText(iFirmamentRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t1().LayoutLine());    
    
    //Layout to draw the cardinal point E
    iCardinalELayout.LayoutText(iFirmamentRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t2().LayoutLine());

	//Layout to draw the cardinal point N
	iCardinalNLayout.LayoutText(iFirmamentRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t3().LayoutLine());

	//Layout to draw the cardinal point S		
	iCardinalSLayout.LayoutText(iFirmamentRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t4().LayoutLine());

	layoutWindow.LayoutRect(iFirmamentRect, 
    						AknLayoutScalable_Apps::blid_sat_info_pane().LayoutLine());
    
    // Parent rect to get the rects of satellites, sun and moon
    TRect parentRect = layoutWindow.Rect();
    
    layoutWindow.LayoutRect(parentRect,
    						AknLayoutScalable_Apps::blid_sat_info_pane_g1().LayoutLine());
    
    iSatelliteRect = layoutWindow.Rect();
    
    layoutWindow.LayoutRect(parentRect, 
    						AknLayoutScalable_Apps::blid_sat_info_pane_g2().LayoutLine());
	
	iSunMoonRect = layoutWindow.Rect();	
	PrepareGraphicalViewIcons();
	}

// ---------------------------------------------------------
// CSatelliteDialog::PrepareCompassViewRectsAndIcons
// ---------------------------------------------------------
//    
void CSatelliteControl::PrepareCompassViewRectsAndIcons()
    {

	TAknLayoutRect layoutWindow;
	TRect parenRect = Rect();
	
    layoutWindow.LayoutRect(parenRect, 
    						AknLayoutScalable_Apps::blid_firmament_pane(1).LayoutLine());
	    						
	// Rect to draw the firmament
    iFirmamentCompassRect = layoutWindow.Rect();
    
    layoutWindow.LayoutRect(iFirmamentCompassRect,
    						AknLayoutScalable_Apps::aid_firma_cardinal(1).LayoutLine());
        
    //Layout to draw the cardinal point W
    iCardinalCompassWLayout.LayoutText(iFirmamentCompassRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t1(1).LayoutLine());    
    
    //Layout to draw the cardinal point E
    iCardinalCompassELayout.LayoutText(iFirmamentCompassRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t2(1).LayoutLine());

	//Layout to draw the cardinal point N
	iCardinalCompassNLayout.LayoutText(iFirmamentCompassRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t3(1).LayoutLine());

	//Layout to draw the cardinal point S		
	iCardinalCompassSLayout.LayoutText(iFirmamentCompassRect, 
    					  		AknLayoutScalable_Apps::blid_firmament_pane_t4(1).LayoutLine());

	// Rect to draw the Type
    layoutWindow.LayoutRect(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_g1(0).LayoutLine());
	    			
	// Rect to draw the Fix Type graphic			
    iTypeRect = layoutWindow.Rect();
    
	// Rect to draw the Fix Type Text - 2D/3D
    iFixTypeLayout.LayoutText(parenRect, 
    					  		AknLayoutScalable_Apps::popup_blid_sat_info2_window_t11(0).LayoutLine());
    
	// Rect to draw the Latitude
    iLatitudeWindowLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t3(0).LayoutLine());

	// Rect to draw the Longitude
    iLongitudeWindowLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t4(0).LayoutLine());

	// Rect to draw the Heading. Altitude.
    iAltitudeWindowHeadingLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t5(0).LayoutLine());

	// Rect to draw the Value. Altitude.
    iAltitudeWindowValueLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t6(0).LayoutLine());

	// Rect to draw the Heading. Speed.
    iSpeedWindowHeadingLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t7(0).LayoutLine());

	// Rect to draw the Value. Speed.
    iSpeedWindowValueLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t8(0).LayoutLine());

	// Rect to draw the Heading. Direction.
    iDirectionWindowHeadingLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t9(0).LayoutLine());

	// Rect to draw the Value. Direction.
    iDirectionWindowValueLayout.LayoutText(parenRect, 
    						AknLayoutScalable_Apps::popup_blid_sat_info2_window_t10(0).LayoutLine());
 
    PrepareCompassViewIcons();    
    }
    
// ---------------------------------------------------------
// CSatelliteControl::ConstructL
// ---------------------------------------------------------
//
void CSatelliteControl::ConstructL( )
	{    
    iSatelliteSignalHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_HEADING_SIGNALS );
    iSatelliteGrapgicHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_HEADING_GRAPHIC );
	iCompassViewHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_HEADING_COMPASS );
	
	iNoSatelliteData = iEikonEnv->AllocReadResourceL( R_BLID_NOTE_NO_SATELLITE_DATA );

	// Altitude,Speed and Direction Strings for Compass View
    iAltitudeWindowHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_COMPASS_ALTITUDE );
    iSpeedWindowHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_COMPASS_SPEED );
	iDirectionWindowHeading = iEikonEnv->AllocReadResourceL( R_BLID_SAT_COMPASS_DIRECTION );

    iSpeedFormatKmph = iEikonEnv->AllocReadResourceL( R_BLID_SAT_METRIC_SPEED );
    iSpeedFormatMph = iEikonEnv->AllocReadResourceL( R_BLID_SAT_IMPERIAL_SPEED );
    iAltitudeFormatM = iEikonEnv->AllocReadResourceL( R_BLID_SAT_METRIC_METER );
    iAltitudeFormatFt = iEikonEnv->AllocReadResourceL( R_BLID_SAT_IMPERIAL_FOOT );
    iDirectionFormat = iEikonEnv->AllocReadResourceL( R_BLID_SAT_DEGREE );


	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
		
	/* Icons for signal strength view */
	// Code added foe multiple drive support.
	TBuf<KMaxFileName> iconFileName;
	iconFileName.Zero();
	iconFileName.Append( PathInfo::RomRootPath()[0]);
	iconFileName.Append(KBlidSatSystemIconPath);
	
	// Satellite Active signal bar
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnPropBlidActive,		                    
		                    iSatActiveBmp,
		                    iSatActiveBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_active,
		                    EMbmSatelliteQgn_prop_blid_active_mask );
	
	// Satellite In-Active signal bar
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnPropBlidInactive,		                    
		                    iSatInActiveBmp,
		                    iSatInActiveBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_inactive,
		                    EMbmSatelliteQgn_prop_blid_inactive_mask );
	
	// Satellite Frame bar
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnPropBlidBg,		                    
		                    iSatFrame,
		                    iSatFrameMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_bg,
		                    EMbmSatelliteQgn_prop_blid_bg_mask );

	/* Icons for firmament view */
	
	// Icon for the firmament
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnIndiBlidSatFirmaCenter,		                    
		                    iFirmamentBmp,
		                    iFirmamentBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_sat_firma,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_mask );
	
	// Icon for active satellite
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnIndiBlidSatFirmaActive,		                    
		                    iFirmaActiveBmp,
		                    iFirmaActiveBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_active,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_active_mask );
	
	// Icon for In-Active satellite
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnIndiBlidSatFirmaInact,		                    
		                    iFirmaInActiveBmp,
		                    iFirmaInActiveBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_inact,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_inact_mask );
	
	// Icon for the sun
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnIndiBlidSatFirmaSun,		                    
		                    iSunBmp,
		                    iSunBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_sun,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_sun_mask );
	
	// Icon for the moon
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnIndiBlidSatFirmaMoon,		                    
		                    iMoonBmp,
		                    iMoonBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_moon,
		                    EMbmSatelliteQgn_prop_blid_sat_firma_moon_mask );
	
	/* Icons for Compass view */
	
	// Icon for the firmament
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnGrafBlid,		                    
		                    iFirmamentCompassBmp,
		                    iFirmamentCompassBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_graf_blid,
		                    EMbmSatelliteQgn_graf_blid_mask );
		                    
	// Icon for the Type of Fix
	AknsUtils::CreateIconL( skin,
                            KAknsIIDQgnGrafBlidRingfix,		                    
		                    iFixBmp,
		                    iFixBmpMask,
		                    iconFileName,
		                    EMbmSatelliteQgn_graf_blid_ringfix,
		                    EMbmSatelliteQgn_graf_blid_ringfix_mask );

	//Type of Fix to be display on the Fix Ring
	i2DString = StringLoader::LoadL( R_BLID_SAT_COMPASS_2D, iEikonEnv );
    i3DString = StringLoader::LoadL( R_BLID_SAT_COMPASS_3D, iEikonEnv );

	//Cardinal points text to be display on the Compass firmament
	iCompassNorth = StringLoader::LoadL( R_BLID_SAT_COMPASS_NORTH, iEikonEnv );
    iCompassSouth = StringLoader::LoadL( R_BLID_SAT_COMPASS_SOUTH, iEikonEnv );
    iCompassWest = StringLoader::LoadL( R_BLID_SAT_COMPASS_WEST, iEikonEnv );
    iCompassEast = StringLoader::LoadL( R_BLID_SAT_COMPASS_EAST, iEikonEnv );

	//Cardinal points text to be display on the Graphic firmament
	iNorth = StringLoader::LoadL( R_BLID_SAT_SYMBOL_NORTH, iEikonEnv );
    iSouth = StringLoader::LoadL( R_BLID_SAT_SYMBOL_SOUTH, iEikonEnv );
    iWest = StringLoader::LoadL( R_BLID_SAT_SYMBOL_WEST, iEikonEnv );
    iEast = StringLoader::LoadL( R_BLID_SAT_SYMBOL_EAST, iEikonEnv );
    }

// ---------------------------------------------------------
// CSatelliteControl::ConstructFromResourceL
// ---------------------------------------------------------
//
void CSatelliteControl::ConstructFromResourceL(TResourceReader& /*aReader*/)
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
	iEditorContext = CAknsFrameBackgroundControlContext::NewL(
        KAknsIIDQsnFrInput, TRect(0,0,1,1), TRect(0,0,1,1), EFalse );
    ActivateL();
    }

// ---------------------------------------------------------
// CSatelliteControl::SizeChanged
// ---------------------------------------------------------
//
void CSatelliteControl::SizeChanged()
    {    
    AknsUtils::RegisterControlPosition(this);
    TAknLayoutRect topLeft;
    TAknLayoutRect bottomRight;
    
    topLeft.LayoutRect(Rect(), SkinLayout::Input_field_skin_placing__general__Line_2());                
    bottomRight.LayoutRect(TRect(Rect().iBr, Rect().iBr), SkinLayout::Input_field_skin_placing__general__Line_5());

    TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);

    iEditorContext->SetFrameRects( outerRect, innerRect );
    // Chain with the background (since the frame doesn't occupy the entire
    // layout and it may even be transparent)
    iEditorContext->SetParentContext( AknsDrawUtils::ControlContextOfParent( this ) );        
    PrepareSignalStrengthViewRectsAndIcons();
    PrepareGraphicalViewRectsAndIcons();
    PrepareCompassViewRectsAndIcons();
    DrawNow();
    }

// ---------------------------------------------------------
// CSatelliteControl::MopSupplyObject
// ---------------------------------------------------------
//
TTypeUid::Ptr CSatelliteControl::MopSupplyObject( TTypeUid aId )
    {
    CCoeControl::MopSupplyObject( aId );
    return MAknsControlContext::SupplyMopObject( aId,  iEditorContext );
    }

// ---------------------------------------------------------
// CSatelliteControl::Draw
// ---------------------------------------------------------
//
void CSatelliteControl::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc=SystemGc();
    TRect rect(Rect());
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    AknsDrawUtils::Background( skin, cc, this, gc, rect, KAknsDrawParamNoClearUnderImage);
    
    //First draw the heading
    TRAP_IGNORE(DrawHeadingL());
    
    if(!iEngine.IsSatelliteDataAvailable())
    	{
    	//Draw the empty string "No satellite data" string in the dialog
    	TRAP_IGNORE(DrawEmptyDialogL());
    	return;
    	}
    
	switch ( iCurrentSatelliteView )
	    {
	    case ESignalStrengthView:
	        {
    		//Draw the signal strength view
    		TRAP_IGNORE(DrawSignalStrengthViewL());
	        break;
	        }	    
	    case EGraphicalView:
	        {
    	    //Draw the graphical view
    	    TRAP_IGNORE(DrawGraphicalViewL());
	        break;
	        }
	    case ECompassView:
	        {
    		//Draw the compass view
    		TRAP_IGNORE(DrawCompassViewL());
	        break;
	        }
	    default:
	        {
	        break;
	        }
	    }
    }
    
// ---------------------------------------------------------
// CSatelliteControl::DrawHeadingL
// ---------------------------------------------------------
//
void CSatelliteControl::DrawHeadingL() const
	{
	//Get the rect for the heading frame from LAF
	TAknLayoutRect layoutWindow;
	layoutWindow.LayoutRect(Rect(), AknLayoutScalable_Apps::heading_pane_cp05(2).LayoutLine());	
	TRect headingFrameRect = layoutWindow.Rect();
	
	//Get the rect for the heading text. Parent of the heading text rect
	//is the rect of the heading frame
	TAknLayoutText headingTextLayout;
	headingTextLayout.LayoutText(headingFrameRect, AknLayoutScalable_Avkon::heading_pane_t1(0).LayoutLine());
		
	CWindowGc& gc=SystemGc();
		
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = AknsDrawUtils::ControlContext( this ); // get parent's cc

    TRgb rgb;
    AknsUtils::GetCachedColor( skin,
                               rgb,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG1 );
    gc.SetPenStyle( CGraphicsContext::ESolidPen );                               
    gc.SetPenColor( rgb ); 
        
    //Skin the heading frame
	AknsDrawUtils::DrawFrame(skin, 
							 gc, 
							 headingFrameRect, 
							 headingTextLayout.TextRect(), 
                             KAknsIIDQsnFrPopupHeading,
                             KAknsIIDQsnFrPopupHeadingCenter);
    
	switch ( iCurrentSatelliteView )
	    {
	    case ESignalStrengthView:
	        {
    	    headingTextLayout.DrawText( gc, 
    	                                iSatelliteSignalHeading->Des(),
    	                                ETrue,
    	                                rgb );
	        break;
	        }	    
	    case EGraphicalView:
	        {
    	    headingTextLayout.DrawText( gc, 
    	                                iSatelliteGrapgicHeading->Des(),
    	                                ETrue,
    	                                rgb );
	        break;
	        }
	    case ECompassView:
	        {
    	    headingTextLayout.DrawText( gc, 
    	                                iCompassViewHeading->Des(),
    	                                ETrue,
    	                                rgb );
	        break;
	        }
	    default:
	        {
	        break;
	        }
	    }
	}

// ---------------------------------------------------------
// CSatelliteControl::DrawEmptyDialogL
// ---------------------------------------------------------
//
void CSatelliteControl::DrawEmptyDialogL() const
	{
	TAknLayoutText textRect1;

	textRect1.LayoutText(Rect(), 
    					 AknLayoutScalable_Apps::popup_blid_sat_info2_window_t1());
	
	
	TAknLayoutText textRect2;

	textRect2.LayoutText(Rect(), 
    					 AknLayoutScalable_Apps::popup_blid_sat_info2_window_t2());   					 

    
	CArrayFix<TInt>* wrapWidthArray =
        new( ELeave ) CArrayFixFlat<TInt>(10);

    CleanupStack::PushL( wrapWidthArray );
    
    TInt lineLength1 = textRect1.TextRect().Width();
    TInt lineLength2 = textRect2.TextRect().Width();

    wrapWidthArray->AppendL(lineLength1);
    wrapWidthArray->AppendL(lineLength2);
    
    TPtrC text = *iNoSatelliteData;
    TBuf<200> buffer;
    
    const CFont *font = textRect1.Font();
    AknBidiTextUtils::ConvertToVisualAndWrapToStringL(text,
												      *wrapWidthArray,
												      *font,
												      buffer,
												      ETrue );

    CleanupStack::PopAndDestroy(); // wrapWidthArray
    
    TPtrC line1= buffer;
    TPtrC line2= buffer;
    TInt off = line1.Locate('\n');
    if (off>=0)
	    {
	    line1.Set(line2.Left(off));
	    line2.Set(line2.Mid(off+1));
	    }
    else
	    {
	    line2.Set(_L(""));
	    }

    TInt off2 = line2.Locate('\n');
    if (off2>=0)
	    {
	    line2.Set(line2.Left(off2));
	    }
    
    CWindowGc& gc=SystemGc();
    gc.UseFont(font);
        
    const CAknLayoutFont* layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( font );
    TInt textPaneHeight;
    TInt textPaneAscent;
    if ( layoutFont ) 
    	{
    	textPaneHeight = layoutFont->TextPaneHeight();     
    	textPaneAscent = layoutFont->TextPaneTopToBaseline();
    	}
    else 
    	{
    	textPaneHeight = font->HeightInPixels();         
    	textPaneAscent = font->AscentInPixels(); 
    	}
    //draw text    
    TInt extraVerticalSpace=(textRect1.TextRect().Height()-textPaneHeight);
    TInt baseLineOffset=extraVerticalSpace/2+textPaneAscent;
    gc.DrawText( line1, 
                 textRect1.TextRect(), 
                 baseLineOffset,
                 CGraphicsContext::ECenter  );
                  
    if(line2.Length())
    	{
    	gc.DrawText( line2, 
	                 textRect2.TextRect(), 
	                 baseLineOffset,
	                 CGraphicsContext::ECenter );
    	}    
    }

// ---------------------------------------------------------
// CSatelliteControl::DrawGraphicalViewL
// ---------------------------------------------------------
//
void CSatelliteControl::DrawGraphicalViewL() const
	{
	CWindowGc& gc=SystemGc();
	TSatelliteData satelliteData;
	for(TInt i = 0 ; i < iEngine.NumberOfSatellites(); ++i)
		{
		iEngine.GetSatelliteData(i, satelliteData);
		TInt signalstrength = satelliteData.SignalStrength();
		}	
		
	// Drawing the firmament
	gc.BitBltMasked(iFirmamentRect.iTl,
					iFirmamentBmp,
					iFirmamentBmp->SizeInPixels(),
					iFirmamentBmpMask,
					ETrue);	
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb rgb;
    AknsUtils::GetCachedColor( skin,
                               rgb,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG1 );
    gc.SetPenStyle( CGraphicsContext::ESolidPen );                               
    gc.SetPenColor( rgb );
	// Draw the cardinal points
	iCardinalNLayout.DrawText(gc, iNorth->Des(), ETrue, rgb );
	iCardinalSLayout.DrawText(gc, iSouth->Des(), ETrue, rgb );
	iCardinalWLayout.DrawText(gc, iWest->Des(), ETrue, rgb );
	iCardinalELayout.DrawText(gc, iEast->Des(), ETrue, rgb );
					
	//Draw the moon
    TReal moonAzmiuth, moonElevation;			
	iEngine.GetMoonAzimuthElevation(moonAzmiuth, moonElevation);
	TRect moonRect = CalculateCelestialBodyRectL(moonAzmiuth, moonElevation, iSunMoonRect.Size());
	gc.BitBltMasked(moonRect.iTl,
					iMoonBmp,
					iMoonBmp->SizeInPixels(),
					iMoonBmpMask,
					ETrue);

	// Draw the sun
	TReal sunAzmiuth, sunElevation;
	iEngine.GetSunAzimuthElevation(sunAzmiuth, sunElevation);
	TRect sunRect = CalculateCelestialBodyRectL(sunAzmiuth, sunElevation, iSunMoonRect.Size());
	
	gc.BitBltMasked(sunRect.iTl,
					iSunBmp,
					iSunBmp->SizeInPixels(),
					iSunBmpMask,
					ETrue);
	
	//Draw all the available satellites
	for(TInt i = 0;i<iEngine.NumberOfSatellites();++i)
		{
		TSatelliteData satData;

		if(iEngine.GetSatelliteData(i, satData) != KErrNotFound)
			{
			TRect rect;
			TInt satId = satData.SatelliteId();	
			rect = CalculateCelestialBodyRectL(satData.Azimuth(), satData.Elevation(), iSatelliteRect.Size());			
			if(satData.IsUsed())
				{
				gc.BitBltMasked(rect.iTl,
								iFirmaActiveBmp,
								iFirmaActiveBmp->SizeInPixels(),
								iFirmaActiveBmpMask,
								ETrue);
				}
			else
				{
				gc.BitBltMasked(rect.iTl,
								iFirmaInActiveBmp,
								iFirmaInActiveBmp->SizeInPixels(),
								iFirmaInActiveBmpMask,
								ETrue);
				}
			TAknLayoutText textLayout;
			textLayout.LayoutText(rect, 
    							  AknLayoutScalable_Apps::blid_sat_info_pane_t1().LayoutLine());
			    							  
			HBufC* satNumber = StringLoader::LoadL( R_BLID_SATELLITE_ID, satData.SatelliteId() );
			HBufC* visualBuffer = HBufC::NewL( satNumber->Size() + KAknBidiExtraSpacePerLine );
		    TPtr visualPtr( visualBuffer->Des() );
		    visualPtr.Zero();
		    TPtr ptr(satNumber->Des());
		    AknTextUtils::LanguageSpecificNumberConversion( ptr );
		    //This portion of code is commented now.We have to check it for language
		    //Like Farsi,Urdu and then it will be removed or uncomented.
		    /*AknBidiTextUtils::ConvertToVisualAndClip( satNumber->Des(),
		                                              visualPtr,
		                                              *textLayout.Font(),
		                                              textLayout.TextRect().Size().iWidth,
		                                              textLayout.TextRect().Size().iWidth );
			textLayout.DrawText(gc, visualPtr);*/
						
			const CFont* fontUsed = iCardinalNLayout.Font();
			const CAknLayoutFont* layoutFont = CAknLayoutFont::AsCAknLayoutFontOrNull( fontUsed );
		    TInt textPaneHeight;
		    TInt textPaneAscent;
		    if ( layoutFont )
		    	{
		    	textPaneHeight = layoutFont->TextPaneHeight();     
		    	textPaneAscent = layoutFont->TextPaneTopToBaseline();
		    	}
		    else 
		    	{
		    	textPaneHeight = fontUsed->HeightInPixels();         
		    	textPaneAscent = fontUsed->AscentInPixels(); 
		    	}
		    //draw text    
		    TInt extraVerticalSpace=(rect.Height()-textPaneHeight);
		    TInt baseLineOffset=extraVerticalSpace/2+textPaneAscent;
		    CGraphicsContext::TTextAlign context;    
    		context = textLayout.Align();   
    		gc.UseFont(fontUsed);
    		//gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    
		    // Set the rgb color to black
		    rgb.SetBlue(0);
		    rgb.SetGreen(0);
		    rgb.SetRed(0);
		    gc.SetPenColor(rgb);
		        		
		    gc.DrawText( satNumber->Des(), 
		                 rect, 
		                 baseLineOffset,
		                 context );

			delete satNumber;
			delete visualBuffer;
			}
		}	
	}

// ---------------------------------------------------------
// CSatelliteControl::CalculateCelestialBodyRectL
// ---------------------------------------------------------
//
TRect CSatelliteControl::CalculateCelestialBodyRectL(const TReal aAzimuth, 
													 const TReal aElevation,
													 const TSize aIconSize) const
	{
	TPoint point;	
    TReal cosElevation;
    TReal sinAzimuth;
    TReal cosAzimuth;
    TInt pointX(0);
    TInt pointY(0);
    TPoint center = iFirmamentRect.Center();
    
    User::LeaveIfError(
	        Math::Cos( cosElevation, KDegToRad * (aElevation) ));
	        
	User::LeaveIfError(
	        Math::Sin( sinAzimuth, KDegToRad * (aAzimuth)));

	User::LeaveIfError(
	        Math::Cos( cosAzimuth, KDegToRad * (aAzimuth)));	        

	User::LeaveIfError( 
        		TRealToTInt((iFirmamentRect.Height()/2) * cosElevation * sinAzimuth, pointX));
        
    User::LeaveIfError( 
      			TRealToTInt((iFirmamentRect.Height()/2) * cosElevation * cosAzimuth, pointY) );
    
    point = TPoint( (center.iX + pointX ) , (center.iY -  pointY ));        
        
    TPoint boxCenterPoint;    
    TInt half;
    
    TRealToTInt(aIconSize.iWidth/2, half);
    boxCenterPoint.iX = point.iX - half;
    TRealToTInt(aIconSize.iHeight/2, half);
    boxCenterPoint.iY = point.iY - half;
    TRect returnRect(boxCenterPoint, aIconSize);
    
    return returnRect;
	}

// ---------------------------------------------------------
// CSatelliteControl::TRealToTInt
// Rounds the supplied real number into the nearest integer
// ---------------------------------------------------------
//
TInt CSatelliteControl::TRealToTInt( const TReal aSrc, TInt& aResult ) const
    {
    TReal resultTReal(0);    
    TInt error(0);

    error = Math::Round( resultTReal, aSrc, 0 );
    if ( error != KErrNone )
    	{
        aResult = 0;
        return error;
        }

    TInt32 resultTInt(0);
    error = Math::Int( resultTInt, resultTReal );  
    if ( error != KErrNone )
        {        
        aResult = 0;
        return error;
        }    
    aResult = resultTInt;
    return error;
    }

// ---------------------------------------------------------
// CSatelliteControl::DrawSignalStrengthViewL
// ---------------------------------------------------------
//
void CSatelliteControl::DrawSignalStrengthViewL() const
	{
	CWindowGc& gc=SystemGc();
	TInt numberofsatellites = iEngine.NumberOfSatellites();
    
	for(TInt i = 0 ; i < numberofsatellites; ++i)
		{
		TSatelliteData satData;

		if(iEngine.GetSatelliteData(i, satData) != KErrNotFound)			
			{
			TInt satelliteId = satData.SatelliteId();
			TInt signalStrength = satData.SignalStrength();
			if( (satelliteId > KMaxSatellites) || (signalStrength == 0) )
				{
				continue;
				}
			TRect rect = iSignalBarRects[satelliteId-1];			
			CFbsBitmap* bitmap;
			CFbsBitmap* bitmapMask;			
			TInt barHeight = rect.Height() * signalStrength / KBlidMaxSatelliteSignalStrength;
			if(barHeight < rect.Height())
				{
				TInt tmp = rect.Height() - barHeight;
				rect.iTl.iY += tmp;
				}
						
			if(satData.IsUsed())
				{
				bitmap = iSatActiveBmp;
				bitmapMask = iSatActiveBmpMask;
				}
			else
				{
				bitmap = iSatInActiveBmp;
				bitmapMask = iSatInActiveBmpMask;
				}
			
			AknIconUtils::SetSize(bitmap,
								  rect.Size());
            
            AknIconUtils::SetSize(bitmap,
								  rect.Size());
			
			
			gc.BitBltMasked(rect.iTl,
							bitmap,
							bitmap->SizeInPixels(),
							bitmapMask,
							ETrue);
							
			rect = iSignalBarRects[satelliteId-1];
			gc.BitBltMasked(rect.iTl,
							iSatFrame,
							iSatFrame->SizeInPixels(),
							iSatFrameMask,
							ETrue);
			}	
		}
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb rgb;
    AknsUtils::GetCachedColor( skin,
                               rgb,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG1 );
    gc.SetPenStyle( CGraphicsContext::ESolidPen );                               
    gc.SetPenColor( rgb );
			
    for(TInt i = 1; i <= KMaxSatellites; ++i)
    	{
    	TSatelliteData satelliteData;
    	iEngine.GetSatelliteData(i, satelliteData);
    	    	
    	HBufC* satNumber = StringLoader::LoadL( R_BLID_SATELLITE_ID, 
    											i );
												
    	
    	HBufC* visualBuffer = HBufC::NewL( satNumber->Size() + 
                                 	   	   KAknBidiExtraSpacePerLine );
	    TPtr visualPtr( visualBuffer->Des() );
	    visualPtr.Zero();
	    AknBidiTextUtils::ConvertToVisualAndClip( satNumber->Des(),
	                                              visualPtr,
	                                              *iSatelliteNumberLayouts[i-1].Font(),
	                                              iSatelliteNumberLayouts[i-1].TextRect().Size().iWidth,
	                                              iSatelliteNumberLayouts[i-1].TextRect().Size().iWidth );
		gc.SetPenStyle(CGraphicsContext::ESolidPen);		
	    iSatelliteNumberLayouts[i-1].DrawText(gc, visualPtr, ETrue, rgb );
	    delete visualBuffer;
    	delete satNumber;
    	}
	}

// ---------------------------------------------------------
// CSatelliteDialog::DrawCompassViewL
// ---------------------------------------------------------
//
void CSatelliteControl::DrawCompassViewL() const
    {    
    CWindowGc& gc=SystemGc();
    
	// Drawing the firmament
	gc.BitBltMasked( iFirmamentCompassRect.iTl,
					 iFirmamentCompassBmp,
					 iFirmamentCompassBmp->SizeInPixels(),
					 iFirmamentCompassBmpMask,
					 ETrue );	

	// Drawing the Fix Ring
	gc.BitBltMasked( iTypeRect.iTl,
					 iFixBmp,
					 iFixBmp->SizeInPixels(),
					 iFixBmpMask,
					 ETrue );
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRgb rgb;
    AknsUtils::GetCachedColor( skin,
                               rgb,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG1 );
    gc.SetPenStyle( CGraphicsContext::ESolidPen );                               
    gc.SetPenColor( rgb );
	// Draw the cardinal points
	iCardinalCompassNLayout.DrawText( gc, iCompassNorth->Des(), ETrue, rgb );
	iCardinalCompassSLayout.DrawText( gc, iCompassSouth->Des(), ETrue, rgb );
	iCardinalCompassWLayout.DrawText( gc, iCompassWest->Des(), ETrue, rgb );
	iCardinalCompassELayout.DrawText( gc, iCompassEast->Des(), ETrue, rgb );
    
	// Drawing the Fix Type
	if( iEngine.NumberOfSatellitesUsed() <= K2dFix )
		{
		iFixTypeLayout.DrawText( gc, i2DString->Des(), ETrue, rgb );
		}
	else
		{
		iFixTypeLayout.DrawText( gc, i3DString->Des(), ETrue, rgb );
		}
					
    TPosition pos;
    TCourse course;
    iEngine.GetPosition( pos );
    iEngine.GetCourse( course );   
    TBuf<KSatInfoBufferSize> tempString;
    TBuf<KSatInfoBufferSize> speedString;
  	HBufC* formatter = NULL;     
  	
  	// Display Latitude
  	if ( !Math::IsNaN( pos.Latitude() ) )
  		{
	    TRAP_IGNORE( formatter = 
	                       CAknLocationEditor::DisplayableLocationL( pos,CAknLocationEditor::ELatitudeOnly ) );
	    if( formatter )  
	        {
	        tempString.Copy( formatter->Des() );             
	        delete formatter;
	        }
		iLatitudeWindowLayout.DrawText( gc, tempString, ETrue, rgb );
  		}

  	// Display Longitude
  	if ( !Math::IsNaN( pos.Longitude() ) )
  		{
	    TRAP_IGNORE( formatter = 
	                       CAknLocationEditor::DisplayableLocationL( pos,CAknLocationEditor::ELongitudeOnly ) );
	    if( formatter )  
	        {
	        tempString.Copy( formatter->Des() );             
	        delete formatter;
	        }
		iLongitudeWindowLayout.DrawText( gc, tempString, ETrue, rgb );
  		}

  	// Display Altitude
    TReal altitude = pos.Altitude();
		iAltitudeWindowHeadingLayout.DrawText( gc, iAltitudeWindowHeading->Des(), ETrue, rgb );
    TInt result(0);
  	if ( !Math::IsNaN( altitude ) && TRealToTInt( altitude , result ) == KErrNone )
  		{
        if ( iEngine.SysOfMeasurementL() == ELocMetric )
            {
            StringLoader::Format( tempString, *iAltitudeFormatM, 
                                  0, result );
            }
        else
        	{
        	TInt imperialAlt( 0 );
        	if ( TRealToTInt( ( altitude*KMetreToFoot ) , imperialAlt ) == KErrNone )
	        	{
	            StringLoader::Format( tempString, *iAltitudeFormatFt, 
	                                  0, imperialAlt );
	        	}
        	}
		iAltitudeWindowValueLayout.DrawText( gc, tempString, ETrue, rgb );
  		}
    
  	// Display Speed
    TReal speed = course.Speed();
    iSpeedWindowHeadingLayout.DrawText( gc, iSpeedWindowHeading->Des(), ETrue, rgb );
  	if ( !Math::IsNaN( speed ) )
  		{
        Math::Round( speed, speed, KDecimals3 );
        TReal speedInKmph = speed * KSpeedConvertion;
        
        if ( iEngine.SysOfMeasurementL() == ELocMetric )
            {
            speedString.Format( KSpeedFormat, speedInKmph );
            formatter = iSpeedFormatKmph;
            }
        else
        	{
            speedInKmph *= KKilometreToMiles;
            speedString.Format( KSpeedFormat, speedInKmph );
            formatter = iSpeedFormatMph;   
        	}
		StringLoader::Format( tempString, *formatter, 0, speedString );
		iSpeedWindowValueLayout.DrawText( gc, tempString, ETrue, rgb );
  		}
    
 	// Display Direction
    TReal heading = course.Heading();
    iDirectionWindowHeadingLayout.DrawText( gc, iDirectionWindowHeading->Des(), ETrue, rgb );
    if ( !Math::IsNaN( heading ) )
        {
				StringLoader::Format( tempString, *iDirectionFormat, 0, heading );  
				iDirectionWindowValueLayout.DrawText( gc, tempString, ETrue, rgb );
        }   
    
    }
    
// ---------------------------------------------------------
// CSatelliteControl::InputCapabilities
// ---------------------------------------------------------
//
TCoeInputCapabilities CSatelliteControl::InputCapabilities() const
	{
	TCoeInputCapabilities inputCapabilities(TCoeInputCapabilities::ENone);	
	return inputCapabilities;
	}

#ifdef RD_SCALABLE_UI_V2
// ---------------------------------------------------------
// CSatelliteDialog::HandlePointerEventL
// ---------------------------------------------------------
//    
void CSatelliteControl::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CCoeControl::HandlePointerEventL(aPointerEvent);
    if ( AknLayoutUtils::PenEnabled() )
        {
    	// Display the context specific menu on double tapping on the fields.
    	if(aPointerEvent.iType == TPointerEvent::EButton1Down && 
    	   iEngine.IsSatelliteDataAvailable() )
    		{
#ifdef RD_TACTILE_FEEDBACK
            MTouchFeedback* feedback = MTouchFeedback::Instance();
            if (feedback)
                {
                feedback->InstantFeedback(ETouchFeedbackBasic);
                }
#endif //RD_TACTILE_FEEDBACK
    		// Change view soft key pressed.Notify the control to change 
    		// the view and draw.
    		ChangeView();		
    		DrawNow();
    		}	
        }
    }
#endif // RD_SCALABLE_UI_V2
// End of file

    
