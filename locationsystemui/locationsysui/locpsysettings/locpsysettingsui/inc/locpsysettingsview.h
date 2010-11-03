/*
* Copyright (c) 2002,2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  View declaration for Position method settings UI
*
*/

#ifndef CLOCPSYSETTINGSVIEW_H
#define CLOCPSYSETTINGSVIEW_H

// System Include
#include <aknview.h>
#include <AknServerApp.h>
#include  <bldvariant.hrh>

// User Include
#include "locpsyengobserver.h"
#include "locpsykeyprocessor.h"
#include <lbs.h>                        // Location Acquisition API
// CONSTANTS

// FORWARD DECLARATIONS
class CLocPsySettingsContainer;
class CLocPsyEng;
class CEikMenuPane;
class MLocPsyCommandHandler;
class CAknQueryDialog;
	
// CLASS DECLARATION

/**
*  CLocPsySettingsView view class.
* 
*/
class CLocPsySettingsView : public CAknView, 
				 		    public MLocPsyEngObserver, 
				 		    public MLocPsyKeyProcessor,
				 		    public MAknServerAppExitObserver
    {
    private:
        /**
        * C++ constructor
        * @param aKeyProcessor processes key events, ownership not transferred
        */
        CLocPsySettingsView( MLocPsyCommandHandler& 	aCommandHandler );
    public:
        /**
        * Destructor.
        */
        virtual ~CLocPsySettingsView();

        /**
        * C++ default constructor.
        * @param aKeyProcessor processes key events, ownership not transferred
        * @return created object
        */
        static CLocPsySettingsView* NewLC( MLocPsyCommandHandler& 	aCommandHandler );

        /**
        * C++ default constructor.
        * @param aKeyProcessor processes key events, ownership not transferred
        * @return created object
        */
        static CLocPsySettingsView* NewL( MLocPsyCommandHandler& 	aCommandHandler );

        /**
        * Handles the Screen Size update
        */
        void HandleScreenSizeChange();

    public:
        /**
        * From CAknView
        */
        TUid Id() const;

        /**
        * From CAknView
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CAknView
        */
        void ProcessCommandL( TInt aCommand );

    public:
        /**
        * From MEikMenuObserver
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
        
    public:
        /**
        * From MLocPsyEngObserver
        */
        void NotifyL();
        
		/**
        * From MLocPsyEngObserver
        */
        void NotifyError( TInt aErrorCode );

    public:
		/**
        * From MLocPsyKeyProcessor
        */
		TBool ProcessKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
			
    public:
		/**
        * From MAknServerAppExitObserver
        */
		virtual void HandleServerAppExit(TInt aReason);
        
    private:
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: 
    	/**
        * From CAknView
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                                TUid        aCustomMessageId,
                          const TDesC8&     aCustomMessage );
            
        /**
        * From CAknView
        */
        void DoDeactivate();
        
        /**
        * Launch the popup.
        *
        * @param aResourceId Resource ID.
        */
        void DoLaunchPopupL( TInt aResourceId );
        
        /**
        * Uses LocAppUI::ActivateLocalViewL.
        * @param aViewId
        * @param aCustomMessageId
        * @param aCustomMessage
        */
        void ActivateLocalViewL(       TUid     aViewId, 
                                       TUid     aCustomMessageId,
                                 const TDesC8&  aCustomMessage );

        /**
        * Uses LocAppUI::ActivateLocalViewL.
        * @param aViewId
        * @param aCustomMessageId
        * @param aCustomMessage
        */
        void ActivateLocalViewL( TUid aViewId );

        /**
        * Shows or dims help option, depending wheather help feature
        * is supported or not.
        * @param aMenuPane
        */
        void HandleHelpFeature( CEikMenuPane& aMenuPane ) const;

    private:
        /**
         * Owns
         */
    	CLocPsyEng*		   				iLocationEngine;
    	
    	MLocPsyCommandHandler& 		    iCommandHandler; 
        CLocPsySettingsContainer* 	    iContainer;  
        CEikMenuBar* 					iPopup;   
      
		/**
		 * Handle to the RPostionServer
		 */
		RPositionServer 				iPositionServer;    		
		
		/**
		 * Reference to the confirmation query for disabling the
		 * PSYs
		 */
		 CAknQueryDialog*				iConfirmationQuery;      
    };

#endif // CLOCPSYSETTINGSVIEW_H

// End of File
