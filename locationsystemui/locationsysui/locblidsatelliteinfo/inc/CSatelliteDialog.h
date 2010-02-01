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
* Description: 
*     Implements interface for SatelliteData.
*
*/


#ifndef __SATELLITE_DIALOG_H__
#define __SATELLITE_DIALOG_H__

//  INCLUDES
#include <AknDialog.h>
#include <AknQueryDialog.h>
#include <coecntrl.h>
#include <e32base.h>
#include <s32std.h>
#include <aknlistquerydialog.h>
#include <aknlists.h>
#include "SatInfoConsts.h"

// FORWARD DECLARATION
class CEikonEnv;
class CEikFormattedCellListBox;
class MSatelliteEng;
class CAknPopupList;
class CEikListBox;
class CAknListQueryDialog;
class CSatelliteListBox;
class CAknsFrameBackgroundControlContext;
class CSatelliteControl;

// CLASS DECLARATION
/**
 *  Implements satellite's dialog
 */
NONSHARABLE_CLASS( CSatelliteDialog ) : public CAknDialog
    {
    public:
        /**
         * Two-phased constructor.
         *
         * @return CSatelliteDialog object.
         */
        static CSatelliteDialog* NewL( MSatelliteEng& aEngine );

        /**
         * Destructor.
         */
        ~CSatelliteDialog();

    private:
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL( );

        /**
         * C++ default constructor.
         */
        CSatelliteDialog(MSatelliteEng& aEngine);
        
        void SetSizeAndPosition(const TSize& aSize);
        
    protected:
    	SEikControlInfo CreateCustomControlL(TInt aControlType);	    
	    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
	    void SizeChanged();
	    TBool OkToExitL(TInt aButtonId);

    public: // from CAknDialog
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType);

    public: // new functions
        /**
         * Update dialog
         */
        void UpdateL();
        
        /**
        * Set the launch view of the dialog
        */        
        void SetLaunchView(TSatDialogView aView);
        
    private: // data
    
        //Reference to the engine
        MSatelliteEng& iEngine;
        
        //Own: Object to draw the fame and its background
        CAknsFrameBackgroundControlContext* iBgContext;
        
        //Own: Satellite control responsible for all the drawing in the dialog
        CSatelliteControl* iSatelliteControl;        
    };

#endif // __SATELLITE_DIALOG_H__

// End of File
