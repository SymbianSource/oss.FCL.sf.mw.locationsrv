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
* Description: Interface class for defining the engine.
*
*/



#ifndef __MSATELLITEENG_H__
#define __MSATELLITEENG_H__

// INCLUDES
#include <lbspositioninfo.h>
#include <lbs.h>
#include <locnotprefplugindomaincrkeys.h>

// FORWARD DECLARATIONS
class MEngObserver;
class TSatelliteData;
// CLASS DECLARATION

/**
* Interface class to implement all the functionality required 
* by the satellite engine
*/
class MSatelliteEng
    {
    public:  // Constructors and destructor
        /**
         * Destructor.
         */
        virtual ~MSatelliteEng(){};

    public: // New functions
        /**
         * Sets satellite observer
		 *
         * @param aObserver A reference to observer
         */
        virtual void SetObserver(MEngObserver& aObserver) = 0;

        /**
         * Request satelite data
         * @param aSrvName
         */
        virtual void RequestSatelliteDataL( const TDesC& aSrvName ) = 0;

        /**
         * Get satellite data 
         * @param aIndex
         * @param aSatelliteData
         * @return error code
         */
        virtual TInt GetSatelliteData( TUint aIndex, 
                      TSatelliteData& aSatelliteData ) const = 0;

        /**
         * Get Position Info
         * @param aPosition
         */
        virtual void GetPosition(TPosition& aPosition) = 0;
        
        /**
         * Get Course Info
         * @param aCourse
         */
        virtual void GetCourse(TCourse& aCourse) = 0;
        
        /**
         * Get Course Info
         * @param aCourse
         */
        virtual TLocSystemofMeasurementValues SysOfMeasurementL() = 0;

        /**
         * Get number of satellites
         * @return number of satellites
         */
        virtual TInt NumberOfSatellites() = 0;

        /**
         * Get number of satellites used
         * @return number of satellites used
         */
        virtual TInt NumberOfSatellitesUsed() = 0;

        /**
         * Stop requesting
         */
        virtual void StopRequesting() = 0;

        /**
         * Start requesting
         */
        virtual void StartRequestingL() = 0;
        
        /**
         * Clears the satellite data
         *
         */
        virtual void ClearSatelliteData() = 0;
        
        /**
         * Check if any satelllite data is available
         *
         */
        virtual TBool IsSatelliteDataAvailable() = 0;        
        
        /**
         * Function to obtain the Azimuth and elevation of sun
         */
        virtual void GetSunAzimuthElevation(TReal& aAzimuth, TReal& aElevation ) = 0;
        
        /**
         * Function to obtain the Azimuth and elevation of moon
         */
        virtual void GetMoonAzimuthElevation(TReal& aAzimuth, TReal& aElevation ) = 0;
    };

#endif      // __MSATELLITEENG_H__

// End of File
