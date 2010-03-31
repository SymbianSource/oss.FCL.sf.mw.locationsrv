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


#ifndef __SATELLITE_CONTROL_H__
#define __SATELLITE_CONTROL_H__

// INCLUDE FILES
#include <aknlayoutdef.h>
#include <AknUtils.h>
#include "SatInfoConsts.h"
#include "MSatelliteEng.h"

// FORWARD DECLARATION
class CAknsFrameBackgroundControlContext;

// CLASS DECLARATION
NONSHARABLE_CLASS( CSatelliteControl ) : public CCoeControl
	{
	public:
		/**
        * Constructor.
        */
        static CSatelliteControl* NewL( MSatelliteEng& aEngine );
        
    public: 
        // Destructor
        ~CSatelliteControl();
                
    private:
        void ConstructL(  );
        // Default Constructor
        CSatelliteControl(MSatelliteEng& aEngine);
    
    
    public: 
    	void ConstructFromResourceL(TResourceReader& aReader);
		TSize MinimumSize();
		TCoeInputCapabilities InputCapabilities() const;
		TAknWindowLineLayout WindowLayout() const;
		TRect CalculateDialogRect() const;
		void Layout();
		void ChangeViewL();
		void SetLaunchView(TSatDialogView aView);	
		

        #ifdef RD_SCALABLE_UI_V2
        //Implementated for stylus pop-up menu implementation
        void HandlePointerEventL(const TPointerEvent& aPointerEvent);
        #endif // RD_SCALABLE_UI_V2
        
    protected: // from CCoeControl
        /**
        * Count component controls.
        * @return Number of component controls.
        */
        TInt CountComponentControls() const;

        /**
        * Get a component control by index.
        * @param aIndex Index of component control to be returned.
        * @return Component control or NULL.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * Handle focus change.
        * @param aDrawNow EDrawNow if draw the control now else ENoDrawNow
        */
        void FocusChanged( TDrawNow aDrawNow );

        /*
        * @see CCoeControl
        */
        
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );

	protected:  // from CCoeControl

        /**
        * Handle size change, lay out components.
        */
        void SizeChanged();
		void Draw(const TRect& aRect) const;
		
	private: //new function
		TSize DialogSize() const;
		TRect DialogRect() const;
		void DrawHeadingL() const;
		void DrawSignalStrengthViewL() const;
		void DrawGraphicalViewL() const;
		void DrawEmptyDialogL() const;
		void PrepareGraphicalViewIcons();		
		void PrepareSignalStrengthViewIcons();
		void PrepareSignalStrengthViewRectsAndIcons();
		void PrepareGraphicalViewRectsAndIcons();
		TRect CalculateCelestialBodyRectL(const TReal aAzimuth, const TReal aElevation, const TSize aIconSize) const;
		TInt TRealToTInt( const TReal aSrc, TInt& aResult ) const;
		
		// New functions added for Compass View
		void DrawCompassViewL() const;
		void PrepareCompassViewIcons();
		void PrepareCompassViewRectsAndIcons();
	
	private: //data
    	CAknsFrameBackgroundControlContext* iEditorContext;
    	TInt iCurrentSatelliteView;
    	MSatelliteEng& iEngine;
    	
    	
    	//Heading text;
    	HBufC* iSatelliteSignalHeading;
    	HBufC* iSatelliteGrapgicHeading;
    	HBufC* iNoSatelliteData;
    	HBufC* iCompassViewHeading;

    	TInt iNumberOfRows;
    	TInt iNumberOfColumns;
    	    	
    	// Private data for Signal Strength view    	
    	RArray<TRect> iSignalBarRects;
    	RArray<TAknLayoutText> iSatelliteNumberLayouts;
    	CFbsBitmap* iSatActiveBmp;
    	CFbsBitmap* iSatActiveBmpMask;
    	CFbsBitmap* iSatInActiveBmp;
    	CFbsBitmap* iSatInActiveBmpMask;
    	CFbsBitmap* iSatFrame;
    	CFbsBitmap* iSatFrameMask;
    	
    	// Private Data for Graphical view
    	TRect iSatelliteRect;
    	TRect iSunMoonRect;    	
    	TRect iFirmamentRect;    	
    	TAknLayoutText iCardinalNLayout;
		TAknLayoutText iCardinalSLayout;
		TAknLayoutText iCardinalWLayout;
		TAknLayoutText iCardinalELayout;
		TRect iSatelliteNoRect;
    	CFbsBitmap* iFirmamentBmp;
    	CFbsBitmap* iFirmamentBmpMask;
    	CFbsBitmap* iFirmaActiveBmp;
    	CFbsBitmap* iFirmaActiveBmpMask;
    	CFbsBitmap* iFirmaInActiveBmp;
    	CFbsBitmap* iFirmaInActiveBmpMask;
		CFbsBitmap* iMoonBmp;
    	CFbsBitmap* iMoonBmpMask;
    	CFbsBitmap* iSunBmp;
    	CFbsBitmap* iSunBmpMask;
    	HBufC* iNorth;
    	HBufC* iSouth;
    	HBufC* iWest;
    	HBufC* iEast;    	
    	
    	// Private Data for Compass view
    	TRect iFirmamentCompassRect;
    	TRect iTypeRect;  
    	TAknLayoutText iCardinalCompassNLayout;
		TAknLayoutText iCardinalCompassSLayout;
		TAknLayoutText iCardinalCompassWLayout;
		TAknLayoutText iCardinalCompassELayout;
		TAknLayoutText iLatitudeWindowLayout;
		TAknLayoutText iLongitudeWindowLayout;
		TAknLayoutText iAltitudeWindowHeadingLayout;
		TAknLayoutText iSpeedWindowHeadingLayout;
		TAknLayoutText iDirectionWindowHeadingLayout;
		TAknLayoutText iAltitudeWindowValueLayout;
		TAknLayoutText iSpeedWindowValueLayout;
		TAknLayoutText iDirectionWindowValueLayout;
		TAknLayoutText iFixTypeLayout;
    	CFbsBitmap* iFirmamentCompassBmp;
    	CFbsBitmap* iFirmamentCompassBmpMask;		
		CFbsBitmap* iFixBmp;
    	CFbsBitmap* iFixBmpMask;
    	HBufC* iAltitudeWindowHeading;
    	HBufC* iSpeedWindowHeading;
    	HBufC* iDirectionWindowHeading;
    	HBufC* iSpeedFormatKmph;
    	HBufC* iSpeedFormatMph;
    	HBufC* iAltitudeFormatM;
    	HBufC* iAltitudeFormatFt;
    	HBufC* iDirectionFormat;
    	HBufC* iCompassNorth;
    	HBufC* iCompassSouth;
    	HBufC* iCompassWest;
    	HBufC* iCompassEast; 
    	HBufC* i2DString;
    	HBufC* i3DString; 
    	
	};

#endif // __SATELLITE_CONTROL_H__




