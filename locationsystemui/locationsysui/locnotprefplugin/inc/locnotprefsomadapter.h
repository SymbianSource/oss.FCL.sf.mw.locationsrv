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
* Description:  Location Notation Preferences Plugin System of measurement
*                display adapter
*
*/

#ifndef C_LOCNOTPREFSYSTEMOFMEASADAPTER_H_
#define C_LOCNOTPREFSYSTEMOFMEASADAPTER_H_

//  System Includes
#include <e32base.h>

// User Includes
#include "locnotprefplugindomaincrkeys.h"

// Forward Declarations
class MLocNotPrefAdapterObserver;
class CRepository;

// Constant Declarations

// Class Definition

/**
 * Adapter to the System of Measurement settings storage
 */
class CLocNotPrefSystemofMeasAdapter : public CActive
    {
    public:
        /**
         * Creates new System of Measurement adapter
         *
         * @param  aAdapterObsrv                        Observer to the Settings adapter
         * @return CLocNotPrefSystemofMeasAdapter&  Reference to the application view
         */                                                 
        static CLocNotPrefSystemofMeasAdapter* NewL( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv );


        /**
         * Creates new System of Measurement adapter
         * Leaves the object on the Clean up stack         
         *
         * @param  aAdapterObsrv                        Observer to the Settings adapter
         * @return CLocNotPrefSystemofMeasAdapter&  Reference to the application view
         */                                                 
        static CLocNotPrefSystemofMeasAdapter* NewLC( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv );    

        
        /**
         * Destructor
         */
        ~CLocNotPrefSystemofMeasAdapter ();

        /**
         * Returns the current value of the System of measurements settings
         *
         * @return TLocSystemofMeasurementValues  Current value of the System of measurements settings
         */
        TLocSystemofMeasurementValues GetCurrentSysofMeasL();
        
        /**
         * Sets the value of the System of measurements settings
         *
         * @param aSettingsValue  New value of the System of measurements settings
         */
        void SetCurrentSysofMeasL( TLocSystemofMeasurementValues aSettingsValue );
        
        /**
         * Toggles the settings value
         */
        void ToggleSysofMeasSettingsL();
        
    protected:
        /**
         * Inherited from CActive
         */
        void RunL();
        
        /**
         * Inherited from CActive
         */
        void DoCancel();
                        
    private:
        /**
         * Constructor
         */
        CLocNotPrefSystemofMeasAdapter( MLocNotPrefAdapterObserver& aAdapterObsrv );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
    
        /**
         * Start notification from the Central repository server for any changes in the
         * System of measurements key value
         */
        void StartNotification();
                
    private:
        /**
         * Reference to the Adapter observer
         */
        MLocNotPrefAdapterObserver&     iObserver;

        /**
         * Reference to the Central repository object
         *
         * Owns
         */
        CRepository*                    iRepository;        
        
    };          

#endif // C_LOCNOTPREFSYSTEMOFMEASADAPTER_H_