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
*     Implements interface for Satellite.
*
*/


#ifndef __SATELLITE_H__
#define __SATELLITE_H__

//  INCLUDES
#include <e32base.h>
#include <eikenv.h>
#include "MEngObserver.h"
#include "SatInfoConsts.h"

// FORWARD DECLARATION
class MSatelliteEng;
class CSatelliteDialog;
class CAknWaitDialog;

// CLASS DECLARATION
/**
 *  Implements 
 */
NONSHARABLE_CLASS( CSatellite ) : public CBase, public MEngObserver
    {
    public:
        /**
         * Two-phased constructor.
         *
         * @return CSatellite object.
         */
        static CSatellite* NewL();

        /**
         * Destructor.
         */
        ~CSatellite();

    private:
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

        /**
         * C++ default constructor.
         */
        CSatellite();

    public: // from MEngObserver
        void NotifyL(TInt aError);
        void NotifyError( TInt aErrorCode );        

    public: // new functions
        /**
         * Display satellite's information
         * @param aNameOfRule
         * @return TInt variable
         */
        TInt ExecuteL( const TDesC& aNameOfRule );

        /**
         * Dialog switched to foreground
         * @param aForeground ETrue to switch to the foreground. 
         * EFalse to switch from the foreground.
         */
        void HandleForegroundEventL(TBool aForeground);
        
        /**
        * Function used to set the default launch view
        * @param aView EGraphicalView if graphical view and ESignalStrengthView
        * for signal strength view
        */
        void SetLaunchView(TSatDialogView aView);

    private: // new functions
        /**
         * Open resource file
         */
        void OpenResourceFileL();

        /**
         * Close resource file
         */
        void CloseResourceFile();

        /** 
         * Find resource file
         */
        void FindResourceFileL();
        
    private:    // data
        /// Own:
        CEikonEnv* iEnv;

        /// Own: Resource file name and path
        TFileName iResourceFileName;

        /// Own:
        TInt iResourceOffset;

        /// Own: 
        MSatelliteEng* iEngine;

        /// Own:
        CSatelliteDialog* iDialog;

        ///Own: A pointer to CAknWaitDialog
		CAknWaitDialog* iWaitDialog;	
		
		///Own: Launch view of the dialog
		TSatDialogView iLaunchView;	
    };

#endif // __SATELLITE_H__

// End of File
