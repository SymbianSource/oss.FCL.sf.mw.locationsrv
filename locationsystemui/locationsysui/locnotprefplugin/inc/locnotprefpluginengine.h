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
* Description:  Location Notation Preferences Plugin Engine
*
*/

#ifndef C_LOCNOTPREFPLUGINENGINE_H_
#define C_LOCNOTPREFPLUGINENGINE_H_

//  System Includes
#include <e32base.h>
#include <bamdesca.h>  // MDesCArray

// User Includes
#include "locnotprefadapterobserver.h"

// Forward Declarations
class MLocNotPrefEngObserver;
class CLocNotPrefCoordinateDisplayAdapter;
class CLocNotPrefSystemofMeasAdapter;

// Constant Declarations

// Class Definition

/**
 * Engine for Location Notation Preferences Plugin.
 *
 * The class for interacting accessing and modifying Notation preferences settings. The 
 * class interfaces with the corresponding Central repositories for Measurement and 
 * co-ordinate settings and also listens for changes to the Cen-rep keys.
 * Further, its also doubles up as a model for the Location notation preferences 
 * list-box
 */
class CLocNotPrefPluginEngine : public CBase,
                                public MDesCArray,
                                public MLocNotPrefAdapterObserver
    {
    public:
        /**
         * Enumeration for the Settings list
         */
        enum TSettingsList
            {
            /**
             * System of Measurement
             */
            ESystemofMeasurement,
            
            /**
             * Co-ordinate display format
             */
            ECoordinateDisplayFormat      
            };
            
    public:
        /**
         * Creates new Location Notation Preferences plug-in engine instance.
         *
         * @param  aEngineObserver           Observer to the Notation preferences engine
         * @return CLocNotPrefPluginEngine* Reference to the application view
         */                                                 
        static CLocNotPrefPluginEngine* NewL( MLocNotPrefEngObserver& aEngineObserver );

        /**
         * Creates new Location Notation Preferences plug-in engine instance.
         * Leaves the object on the Clean-up stack.
         *
         * @param  aEngineObserver           Observer to the Notation preferences engine
         * @return CLocNotPrefPluginEngine* Reference to the application view
         */                                                 
        static CLocNotPrefPluginEngine* NewLC( MLocNotPrefEngObserver& aEngineObserver );
        
        /**
         * Destructor
         */
        ~CLocNotPrefPluginEngine();
        
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
         * Inherited from MLocNotPrefAdapterObserver
         */     
        void HandleSettingsChangeL();
        
    public:
        /**
         * Closes the running Settings UI prematurely. This would result in dismissing
         * any active dialogs currently running
         */
        void Close();
            
        /**
         * Change System of Measurement settings
         */
        void ChangeSysofMeasSettingsL();
        
        /**
         * Toggle System of Measurement settings
         */
        void ToggleSysofMeasSettingsL();
        
        /**
         * Change Co-ordinate display format settings
         */
        void ChangeCoordinateDisplaySettingsL();
                
    private:
        /**
         * Constructor
         */
        CLocNotPrefPluginEngine(  MLocNotPrefEngObserver& aEngineObserver );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
        
    private:
        
        /**
         * Reference to the Engine observer
         */
        MLocNotPrefEngObserver&                 iObserver;
        
        /**
         * Co-ordinate display format settings handle
         * Owns
         */ 
        CLocNotPrefCoordinateDisplayAdapter*    iCordDisplaySettings;
        
        /**
         * System of measurements settings handle
         * Owns
         */ 
        CLocNotPrefSystemofMeasAdapter*         iSysofMeasSettings;
        
        /**
         * System of measurement heading
         * Owns
         */
        HBufC16*                                iSysofMeasTitle;
        
        /**
         * Co-ordinate display format heading
         * Owns
         */
        HBufC16*                                iCoordDispTitle;
        
        /**
         * Imperial settings text
         * Owns
         */
        HBufC16*                                iImperialSettings;
        
        /**
         * Metric settings text
         * Owns
         */
        HBufC16*                                iMetricSettings; 
        
         /**
         * Degrees resolution settings text
         * Owns
         */
        HBufC16*                                iDegreesSettings;
        
        /**
         * Minutes resolution settings text
         * Owns
         */
        HBufC16*                                iMinutesSettings;
        
        /**
         * Seconds resolution settings text
         * Owns
         */
        HBufC16*                                iSecondsSettings;
        
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
                    
    };          

#endif // C_LOCNOTPREFPLUGINENGINE_H_

