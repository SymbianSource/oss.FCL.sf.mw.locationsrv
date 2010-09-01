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
* Description:  Location Triggering Management Plugin Engine
*
*/

#ifndef CLBTMGMTPLUGINENGINE_H_
#define CLBTMGMTPLUGINENGINE_H_

//  System Includes
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// User Includes
#include "lbtmgmtadapterobserver.h"

// Forward Declarations
class MLbtMgmtEngObserver;
class CLbtMgmtLbtAdapter;

// Constant Declarations

// Class Definition

/**
 * Engine for Location Triggering Management Plugin.
 *
 * The class for interacting accessing and modifying Triggering Management settings. The 
 * class interfaces with the corresponding Adapter Class for Trigger Status 
 * and Default Tone.
 * Further, its also doubles up as a model for the Location Triggering Management 
 * list-box
 */
class CLbtMgmtPluginEngine : public CBase,
                                public MDesCArray,
                                public MLbtMgmtAdapterObserver
    {
    public:
        /**
         * Enumeration for the Settings list
         */
        enum TSettingsList
            {
            /**
             * Trigger status
             */
            ETriggeringStatus = 0,
            
            /**
             * Active Triggers
             */
            EActiveTriggers    
            };    
    public:
        /**
         * Creates new Location Triggering Management plug-in engine instance.
         *
         * @param  aEngineObserver           Observer to the Triggering Management engine
         * @return CLbtMgmtPluginEngine* Reference to the application view
         */                                                 
        static CLbtMgmtPluginEngine* NewL( MLbtMgmtEngObserver& aEngineObserver );

        /**
         * Creates new Location Triggering Management plug-in engine instance.
         * Leaves the object on the Clean-up stack.
         *
         * @param  aEngineObserver           Observer to the Triggering Management engine
         * @return CLbtMgmtPluginEngine* Reference to the application view
         */                                                 
        static CLbtMgmtPluginEngine* NewLC( MLbtMgmtEngObserver& aEngineObserver );
        
        /**
         * Destructor
         */
        ~CLbtMgmtPluginEngine();
        
    public:
        /**
         * Inherited from MDesCArray
         */
        TInt MdcaCount() const;

        /**
         * Inherited from MDesCArray
         */
        TPtrC16 MdcaPoint( TInt aIndex ) const;

        /**
         * Inherited from MLbtMgmtAdapterObserver
         */     
        void HandleSettingsChangeL();
        
    public:
        /**
         * Closes the running Settings UI prematurely. This would result in dismissing
         * any active dialogs currently running
         */
        void Close();
            
        /**
         * Change Trigger settings
         */
        void ChangeTiggerStatusL();
        
        /**
         * Toggle Trigger settings
         */
        void ToggleTiggerStatusL();
        
        /**
         * Show Info dialog for active triggers
         */
        void ShowInfoDialogL();
        
        /**
         * Clears all triggers
         */
        void ClearAllTriggersL();
        
        /**
         * Returns total active triggers
         */
        TInt ActiveTriggers();
                
    private:
        /**
         * Constructor
         */
        CLbtMgmtPluginEngine(  MLbtMgmtEngObserver& aEngineObserver );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
        
    private:
        
        /**
         * Reference to the Engine observer
         */
        MLbtMgmtEngObserver&                 iObserver;
        
        /**
         * Lbt Adapter handle
         * Owns
         */ 
        CLbtMgmtLbtAdapter*    iLbtAdapter;
        
        /**
         * Triggering Settings heading
         * Owns
         */
        HBufC16*                                iTriggeringTitle;
        
        /**
         * Active Triggers Settings heading
         * Owns
         */
        HBufC16*                                iActiveTitle;
        
        /**
         * No Active Triggers Settings heading
         * Owns
         */
        HBufC16*                                iNoActiveTitle;
        
        /* Info message about active triggers
          * Owns
          */
        HBufC16*                                iInfoMessage;    
        
        /**
         * Triggering Settings Value - On
         * Owns
         */
        HBufC16*                                iOnSettings;
        
        /**
         * Triggering Settings Value - Off
         * Owns
         */
        HBufC16*                                iOffSettings; 

        /**
         * Temporary Buffer for returning values to the Accessor 
         * functions.
         * Owns
         */
       HBufC16*                                iConversionBuffer;
        
        /**
         * Boolean value to denote whether any settings dialog is currently
         * outstanding
         */
        TBool                                   iSettingsDialogOutstanding;     
       	 
        /**
         * Total Active Triggers
         *
         */
    	 TInt iTotalActiveTriggers;
                    
    };          

#endif // CLBTMGMTPLUGINENGINE_H_

