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
* Description: This is a header file for engine class of Location satellite info
*
*/


#ifndef __SATELLITEENG_H__
#define __SATELLITEENG_H__

//  INCLUDES
#include <e32base.h>
#include <s32std.h>
#include <lbs.h>
#include <lbssatellite.h>
#include "MSatelliteEng.h"
#include "msatellitemeasurementsettingobserver.h"


// FORWARD DECLARATION
class CEikonEnv;
class CSatelliteMeasurementSetting;
// CLASS DECLARATION
/**
 *
 */
NONSHARABLE_CLASS( CSatelliteEng ) : public CActive, public MSatelliteEng,
									public MSatelliteMeasurementSettingObserver
    {
    public:
        /**
         * Two-phased constructor.
         *
         * @return CSatelliteEng object.
         */
        static CSatelliteEng* NewL();

        /**
         * Destructor.
         */
        ~CSatelliteEng();

    private:
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

        /**
         * C++ default constructor.
         */
        CSatelliteEng();

    public: // from CActive
        void RunL();
        void DoCancel();
        TInt RunError(TInt aError);        

    private: //from MSatelliteEng
        void SetObserver(MEngObserver& aObserver);
        void RequestSatelliteDataL( const TDesC& aNameOfRule );
        TInt GetSatelliteData(TUint aIndex, 
                              TSatelliteData& aSatelliteData) const;
		
		void GetPosition(TPosition& aPosition);
        void GetCourse(TCourse& aCourse);
        TLocSystemofMeasurementValues SysOfMeasurementL();
        TInt NumberOfSatellitesUsed();
        TInt NumberOfSatellites();
        void StopRequesting();
        void StartRequestingL();
        void SortVisibleSatellitesDataL();
        void UpdateIntervalL();
        void ClearSatelliteData();
        TBool IsSatelliteDataAvailable();
        void ObjectElevationAzimuth(TInt aIndex, TReal& aAzimuth, TReal& aElevation );
        void SortSatelliteDataL();
        void GetSunAzimuthElevation(TReal& aAzimuth, TReal& aElevation );
        void GetMoonAzimuthElevation(TReal& aAzimuth, TReal& aElevation );
        void Julian(TReal& aJulianDay,
        			TReal& aJulianCentury, 
        			TInt aDay,
        			TInt aMonth,
        			TInt aYear,
        			TReal universalTime);
        void TrueLongToRaDeltadec(TReal& aRa,TReal& aDd,TReal aTrueLongitude);
        TReal Kepler(TReal aE, TReal aM);
        void LocationCoordinates(TReal& aAzimuth, 
								 TReal& aElevation, 
								 TReal aTau,
								 TReal aDd, 
								 TReal aLatitude);
		TReal SideRealTime(TReal aJulianDay, 
						   TReal aJulianCentury,
						   TReal aLongitude,
						   TReal aRa);		
        
    private: //from MSatelliteMeasurementSettingObserver
		void HandleSysMeasureValueL();

    private:    // data
        /// Own: iServer
        RPositionServer iServer;

        /// Own: RPositioner
        RPositioner iPositioner;

        /// Own: TPositionSatelliteInfo
        TPositionSatelliteInfo iSatelliteInfo;
        
        /// Own: CSatelliteMeasurementSetting
        CSatelliteMeasurementSetting* iMeasurementSetting;
        
        /// Ref: Refrence to the observer for position notification
        MEngObserver* iObserver;

        /// Own: Start and stop requesting
        TBool iRequesting;

        /// Number of modules
        TUint iNumberOfModules;

        /// used modules index
        TInt iModuleIndex;

        /// used as an sorted index array to the satellite info data
        TFixedArray<TUint8, KPositionMaxSatellitesInView> iSortIndices;
        
        // Own: Array to sort the satellite data in increasing order of 
        // the signal strengths
        RArray<TSatelliteData> iSortedSatData;
        
        // Keplers constant for sun and moon position calculations
        struct TKeplers
        	{
        	struct params
        		{
        		TReal iA0;
        		TReal iA1;
        		};
        	params iM;
        	params iW;
        	params iE;
        	};
        TKeplers iKeplers[2];
    };

#endif // __SATELLITEENG_H__

// End of File
