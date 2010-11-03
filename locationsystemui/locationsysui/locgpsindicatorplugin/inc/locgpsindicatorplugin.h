/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains class declaration for GPS indicator plugin.
*
*/



#ifndef C_CLOCGPSINDICATORPLUGIN_H
#define C_CLOCGPSINDICATORPLUGIN_H

//  INCLUDES
#include <AknIndicatorPlugin.h> // CAknIndicatorPlugin
#include <AknPreviewPopUpController.h> // Controlling the preview pop-up component
#include <eikcmobs.h>
#include <ConeResLoader.h>

/**
*  GPS indicator plugin implementation
*  This class handles the tap event generated by tapping on the GPS icon.
*
*  @lib locgpsindicatorplugin
*  @since 5.0
*/
class CLocGpsIndicatorPlugin : public CAknIndicatorPlugin,
							   public MAknPreviewPopUpObserver,
                               public MEikCommandObserver
                                         
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CLocGpsIndicatorPlugin* NewL();

        /**
        * Destructor.
        */
        ~CLocGpsIndicatorPlugin();

    private: // From CAknIndicatorPlugin
        
        /**
         * @see CAknIndicatorPlugin
         */
        void HandleIndicatorTapL( const TInt aUid );

    public: // From MEikCommandObserver
    
        /**
         * @see MEikCommandObserver
         */
    	void ProcessCommandL( TInt aCommandId );
    	
	public: // From MAknPreviewPopUpObserver
   
        /**
         * @see MAknPreviewPopUpObserver
         */
        void HandlePreviewPopUpEventL(
            CAknPreviewPopUpController* aController,
            TPreviewPopUpEvent aEvent );    	

    private:

        /**
        * C++ default constructor.
        */
        CLocGpsIndicatorPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    };

#endif      // C_LOCGPSINDICATORPLUGIN_H

// End of File