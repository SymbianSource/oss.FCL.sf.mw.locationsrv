/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Adapter for System of Measurement.
*
*/


#ifndef CSATELLITEMEASUREMENTSETTING_H
#define CSATELLITEMEASUREMENTSETTING_H

//  System Includes
#include <e32base.h>
#include <locnotprefplugindomaincrkeys.h>

// Forward Declarations
class MSatelliteMeasurementSettingObserver;
class CRepository;

// Constant Declarations

// Class Definition
/**
 * Adapter for System of Measurement.
 */
NONSHARABLE_CLASS( CSatelliteMeasurementSetting ) : public CActive
	{
	public:
		/**
     * Creates new System of Measurement Adapter
     *
     * @param  aAdapterObsrv             			Observer to the System of Measurement Adapter
 		 * @return CSatelliteMeasurementSetting*	Pointer to the CSatelliteMeasurementSetting
     */                                                 
    static CSatelliteMeasurementSetting* NewL( 
                        MSatelliteMeasurementSettingObserver&	aAdapterObsrv );


		/**
     * Creates new System of Measurement Adapter
     * Leaves the object on the Clean up stack         
     *
     * @param  aAdapterObsrv			            Observer to the System of Measurement Adapter
 		 * @return CSatelliteMeasurementSetting*	Pointer to the CSatelliteMeasurementSetting
     */                                                 
    static CSatelliteMeasurementSetting* NewLC( 
                        MSatelliteMeasurementSettingObserver&	aAdapterObsrv );	

 
		/**
		 * Destructor
		 */
		~CSatelliteMeasurementSetting ();

		/**
		 * Returns the current value of the System of measurements settings
		 *
		 * @return TLocSystemofMeasurementValues  Current value of the System of measurements settings
		 */
		TLocSystemofMeasurementValues SysofMeasurementL();
        
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
		CSatelliteMeasurementSetting( MSatelliteMeasurementSettingObserver&	aAdapterObsrv );

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
	     * Reference to the Settings observer
	     */
		MSatelliteMeasurementSettingObserver&     iObserver;
	
		/**
		 * Reference to the Central repository object
		 *
		 * Owns
		 */
		CRepository*                    iRepository; 		
	};			


#endif //CSATELLITEMEASUREMENTSETTING_H