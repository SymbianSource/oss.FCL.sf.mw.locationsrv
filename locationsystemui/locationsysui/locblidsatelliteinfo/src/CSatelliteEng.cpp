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
* Description: This defines the engine class for Location satellite info
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <e32math.h>
#include <e32cmn.h>
#include <libc/math.h>
#include "SatInfoConsts.h"
#include "CSatelliteEng.h"
#include "MEngObserver.h"
#include "csatellitemeasurementsetting.h"
#include "Debug.h"

// ============================ FUNCTIONS DECLARATION =========================
TInt SortCriteria(const TSatelliteData& aCurrent, const TSatelliteData& aNext);


// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// CSatelliteEng::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CSatelliteEng* CSatelliteEng::NewL()
    {
    CSatelliteEng* self = new( ELeave ) CSatelliteEng;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ---------------------------------------------------------
// CSatelliteEng::ConstructL
// ---------------------------------------------------------
//
void CSatelliteEng::ConstructL()
    {
    User::LeaveIfError( iServer.Connect() );
    User::LeaveIfError(iPositioner.Open( iServer ));

	// System of Measurements
	iMeasurementSetting = CSatelliteMeasurementSetting::NewL(*this);
	
    // Keplers parameters    
    // Define parameters for the Sun
    iKeplers[0].iM.iA0=356.0470;
    iKeplers[0].iM.iA1=0.9856002585;
    iKeplers[0].iW.iA0=282.9404;
    iKeplers[0].iW.iA1=4.70935e-005;
    iKeplers[0].iE.iA0=0.016709;
    iKeplers[0].iE.iA1=-1.151e-009;

    //Define parameters for the Moon
    iKeplers[1].iM.iA0=115.3654;
    iKeplers[1].iM.iA1=13.0649929509;
    iKeplers[1].iW.iA0=318.0634;
    iKeplers[1].iW.iA1=0.1643573223;
    iKeplers[1].iE.iA0=0.0549;
    iKeplers[1].iE.iA1=0.0;    
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::CSatelliteEng
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CSatelliteEng::CSatelliteEng() : CActive(EPriorityStandard), iRequesting( EFalse )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------
// CSatelliteEng::~CSatelliteEng
// ---------------------------------------------------------
//
CSatelliteEng::~CSatelliteEng()
    {
    Cancel();
    iPositioner.Close();
    iServer.Close();
    iSortedSatData.Reset();
    delete iMeasurementSetting;
    }

// ---------------------------------------------------------
// CSatelliteEng::RunL  
// ---------------------------------------------------------
//
void CSatelliteEng::RunL()
    {
    TInt status = iStatus.Int();
    
    /// Check if we have satellite data
    if(!(status == KErrNone ||
         status == KPositionPartialUpdate) )
	    {	    
	    ClearSatelliteData();
		}	
    SortSatelliteDataL();
    
    if ( iObserver )
        {
        iObserver->NotifyL( iStatus.Int() );
        } 
    #ifdef _DEBUG
    DEBUG1( "CSatelliteEng::RunL(): Error = %d", iStatus.Int() );
    #endif
    
    if ( iRequesting && !IsActive())
        {
        iPositioner.NotifyPositionUpdate( iSatelliteInfo, iStatus);
        SetActive();        
        }
	}

// ---------------------------------------------------------
// CSatelliteEng::DoCancel
// ---------------------------------------------------------
//
void CSatelliteEng::DoCancel()
    {
    iPositioner.CancelRequest( EPositionerNotifyPositionUpdate );    
    }

// ---------------------------------------------------------
// CSatelliteEng::RunError
// ---------------------------------------------------------
//
TInt CSatelliteEng::RunError(TInt aError)
    {    
    if ( iObserver && (aError == KErrCancel) )
        {
        iObserver->NotifyError( aError );
        return KErrNone;
        }
    return aError;
    }
// ---------------------------------------------------------
// CSatelliteEng::SetObserver
// ---------------------------------------------------------
//
void CSatelliteEng::SetObserver(MEngObserver& aObserver)
    {
    iObserver = &aObserver;
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::RequestSatelliteDataL
// ----------------------------------------------------------------------------
//
void CSatelliteEng::RequestSatelliteDataL( const TDesC& aNameOfRule )
    {    
    UpdateIntervalL();
    
    User::LeaveIfError(iPositioner.SetRequestor(CRequestor::ERequestorService,
                                                CRequestor::EFormatApplication,
                                                aNameOfRule ));

    iPositioner.NotifyPositionUpdate( iSatelliteInfo, iStatus);        
    SetActive();    
    iRequesting = ETrue;    
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::GetSatelliteData
// ----------------------------------------------------------------------------
//
TInt CSatelliteEng::GetSatelliteData( TUint aIndex, 
                                      TSatelliteData& aSatelliteData) const
    {
    if( aIndex >= iSortedSatData.Count() )
    	{
    	return KErrNotFound;
    	}    
    aSatelliteData = iSortedSatData[aIndex];
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::GetPosition
// ----------------------------------------------------------------------------
//
void CSatelliteEng::GetPosition(TPosition& aPosition)
	{
	iSatelliteInfo.GetPosition(aPosition);
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::GetCourse
// ----------------------------------------------------------------------------
//
void CSatelliteEng::GetCourse(TCourse& aCourse)
	{
	iSatelliteInfo.GetCourse(aCourse);
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::SortSatelliteDataL
// ----------------------------------------------------------------------------
//
void CSatelliteEng::SortSatelliteDataL()
	{
	TSatelliteData tempSatelliteData;
	TLinearOrder<TSatelliteData> order(SortCriteria);
	iSortedSatData.Reset();
	TInt satellitesInView = iSatelliteInfo.NumSatellitesInView();
	for(TInt i = 0;i < satellitesInView ;i++ )
		{
		User::LeaveIfError(iSatelliteInfo.GetSatelliteData(i,tempSatelliteData));
		if(tempSatelliteData.SignalStrength() <= KBlidMinSatelliteSignalStrength)
        	{
        	continue;
        	}        
    	iSortedSatData.InsertInOrderAllowRepeats(tempSatelliteData, order);                
		}
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::ObjectElevationAzimuth
// ----------------------------------------------------------------------------
//
void CSatelliteEng::ObjectElevationAzimuth(TInt aIndex, TReal& aAzimuth, TReal& aElevation )
	{	
	TTime time = iSatelliteInfo.SatelliteTime();	
	TDateTime dateTime = time.DateTime();
	TReal hours = dateTime.Hour();
	TReal minutes = dateTime.Minute();
	TReal seconds = dateTime.Second();
	TInt day = dateTime.Day() + 1;
	TInt month = dateTime.Month() + 1;
	TInt year = dateTime.Year();
	TReal julianDay, julianCentury;
	
	//Universal time is the number of hours from the begining of the day	
	TReal universalTime = hours + minutes/60.0 + seconds/(60.0*60.0);
	Julian(julianDay, julianCentury, day, month, year, universalTime);
	
	//This algorithm uses a starting point for the Julian day count that is 1.5 days earlier.
	TReal JD_Mod = julianDay - 2451545.0 + 1.5;
	
	//Mean anomaly, in degrees
	TReal m=iKeplers[aIndex].iM.iA0 + iKeplers[aIndex].iM.iA1 * (JD_Mod);
	//Argument of perihelion, in degrees
    TReal w=iKeplers[aIndex].iW.iA0 + iKeplers[aIndex].iW.iA1 * (JD_Mod);
    //eccentricity
    TReal e=iKeplers[aIndex].iE.iA0 + iKeplers[aIndex].iE.iA1 * (JD_Mod);
    
    // Calculate true anomaly    
    TReal E = Kepler(e, m*KPi/180);
    
    TReal tmp, sinE, cosE;
    Math::Sqrt(tmp, 1 - e * e);
    Math::Sin(sinE, E);
    Math::Cos(cosE, E);
    
    TReal v;
    v = atan2(tmp * sinE, cosE - e); //In radians
        
    TReal truelongitude = v + (KPi/180) * w; // in radians
    TReal ra, dd;    
    TrueLongToRaDeltadec(ra, dd, truelongitude);
    ra = ra * 360/24; //In degrees
    dd = dd * 180/KPi; // In degrees
    
    TPosition position;
    TLocality locality;    
    
    iSatelliteInfo.GetPosition(position);        
    
    TReal tau = SideRealTime(julianDay, julianCentury, position.Longitude(), ra);
    
    LocationCoordinates(aAzimuth, aElevation, tau, dd, position.Latitude());
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::SideRealTime
// ----------------------------------------------------------------------------
//
TReal CSatelliteEng::SideRealTime(TReal aJulianDay, 
								  TReal aJulianCentury,
								  TReal aLongitude,
								  TReal aRa)
	{
	TReal theta0 = 280.46061837 + 
				   360.98564736629 * (aJulianDay-2451545.0) + 
				   0.000387933*aJulianCentury*aJulianCentury - 
				   aJulianCentury*aJulianCentury*aJulianCentury/38710000;
    TReal rem;
	Math::Mod(rem, theta0, 360);
	theta0 = rem;
	TReal theta = theta0 + aLongitude;
	return (theta - aRa);
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::LocationCoordinates
// ----------------------------------------------------------------------------
//
void CSatelliteEng::LocationCoordinates(TReal& aAzimuth, 
										TReal& aElevation, 
										TReal aTau,
										TReal aDd, 
										TReal aLatitude)
	{
	//Convert latitude from degrees to radians	
	TReal sinValue;
	Math::Sin(sinValue, aLatitude * KPi/180);
	TReal sinValue2;
	Math::Sin(sinValue2, aDd * KPi/180);
	
	TReal cosValue;
	Math::Cos(cosValue, aLatitude * KPi/180);
	TReal cosValue2;
	Math::Cos(cosValue2, aDd * KPi/180);
	TReal cosValue3;
	Math::Cos(cosValue3, aTau * KPi/180);
	
	TReal sinh = sinValue * sinValue2 + cosValue * cosValue2 * cosValue3;
	
	TReal asinValue;
	Math::ASin(asinValue, sinh);
	aElevation = (180/KPi) * asinValue;
	
	Math::Sin(sinValue, aTau * KPi / 180);
	TReal a1 = -sinValue;
	
	TReal tanValue;
	Math::Tan(tanValue, aDd * KPi / 180);
	TReal a2 = cosValue * tanValue - sinValue * cosValue3;
	
	TReal atanvalue;	
	atanvalue = atan2(a1, a2);
	aAzimuth = (180/KPi) * atanvalue;
	
	if(aAzimuth < 0)
		{
		aAzimuth += 360;
		}
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::Julian
// ----------------------------------------------------------------------------
//
void CSatelliteEng::Julian(TReal& aJulianDay,
						   TReal& aJulianCentury,
						   TInt aDay,
						   TInt aMonth,
						   TInt aYear,						   
						   TReal aUniversalTime)
	{
	if(aMonth <= 2)
		{
		aMonth += 12;
		--aYear;
		}
	TInt tmp;
	tmp = 365.25 * aYear;
	TReal temp = tmp;
	
	tmp = 30.6001 * (aMonth+1);
	TReal temp2 = tmp;
	
	aJulianDay = temp + temp2 - 15.0 + 1720996.5 + aDay + aUniversalTime/24;
	aJulianCentury = (aJulianDay - 	2451545.0)/36525.0;
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::Kepler
// ----------------------------------------------------------------------------
//
TReal CSatelliteEng::Kepler(TReal aE, TReal aM)
	{
	//Solving Kepler's equation by the Newton-Raphson iteration
    //Note aE, aM in radians
    TReal sinm, cosm;
    TReal E = aE;
    Math::Sin(sinm, aM);
    Math::Cos(cosm, aM);
    TReal EO = aM + aE * sinm * (1 - aE * cosm);
    
    while(1)
    	{
    	TReal sineo, coseo;
    	Math::Sin(sineo, EO);
    	Math::Cos(coseo, EO);
    	E = EO - (EO - aE * sineo - aM) / (1-aE * coseo);
    	
    	if( Abs(E - EO) < (1e-6) )
    		{
    		break;
    		}
    	EO = E;
    	}
    return E;
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::TrueLongToRaDeltadec
// ----------------------------------------------------------------------------
//
void CSatelliteEng::TrueLongToRaDeltadec(TReal& aRa,TReal& aDd,TReal aTrueLongitude)
	{
	//This function converts True Longitude (in radians) to Right Ascension (in hours) and
    //Delta declination (in radians)
    TReal X;
    Math::Cos(X, aTrueLongitude);
    
    TReal sintl;
    Math::Sin(sintl, aTrueLongitude);
    
    TReal cosvalue;
    Math::Cos(cosvalue, 23.43999 * KPi/180);
    TReal Y = cosvalue * sintl;
    
    TReal sinvalue;
    Math::Sin(sinvalue, 23.43999 * KPi/180);
    TReal Z = sinvalue * sintl;
    
    TReal R;
    Math::Sqrt(R, 1-Z*Z);
    
    Math::ATan(aDd, Z/R);
    
    TReal tanValue;
    Math::ATan(tanValue, Y/(X+R) );
    aRa = (24/KPi)*tanValue;
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::GetSunAzimuthElevation
// ----------------------------------------------------------------------------
//
void CSatelliteEng::GetSunAzimuthElevation(TReal& aAzimuth, TReal& aElevation )
	{	
	ObjectElevationAzimuth(0, aAzimuth, aElevation );	
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::GetMoonAzimuthElevation
// ----------------------------------------------------------------------------
//	
void CSatelliteEng::GetMoonAzimuthElevation(TReal& aAzimuth, TReal& aElevation )
	{
	ObjectElevationAzimuth(1, aAzimuth, aElevation );
	}
	
// ----------------------------------------------------------------------------
// CSatelliteEng::NumberOfSatellites
// ----------------------------------------------------------------------------
//
TInt CSatelliteEng::NumberOfSatellites()
    {
	return iSortedSatData.Count();	
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::NumberOfSatellites
// ----------------------------------------------------------------------------
//
TInt CSatelliteEng::NumberOfSatellitesUsed()
    {
	return iSatelliteInfo.NumSatellitesUsed();	
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::StopRequesting
// ----------------------------------------------------------------------------
//
void CSatelliteEng::StopRequesting()
    {
    iRequesting = EFalse;
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::StartRequestingL
// ----------------------------------------------------------------------------
//
void CSatelliteEng::StartRequestingL()
    {
    if ( !IsActive() )
        {
        iPositioner.NotifyPositionUpdate( iSatelliteInfo, iStatus );        
        SetActive();
        }
    iRequesting = ETrue;    
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::UpdateIntervalL
// ----------------------------------------------------------------------------
//
void CSatelliteEng::UpdateIntervalL( )
    {
    TPositionUpdateOptions updateOptions;
    updateOptions.SetUpdateInterval(TTimeIntervalMicroSeconds(KSatelliteIntervalTime));
    updateOptions.SetAcceptPartialUpdates( ETrue );
    updateOptions.SetUpdateTimeOut(TTimeIntervalMicroSeconds(KSatelliteTimeOut));
    User::LeaveIfError( iPositioner.SetUpdateOptions(updateOptions) );
    }

// ----------------------------------------------------------------------------
// CSatelliteEng::ClearSatelliteData
// ----------------------------------------------------------------------------
//
void CSatelliteEng::ClearSatelliteData()
	{
	iSatelliteInfo.ClearSatellitesInView();	
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::IsSatelliteDataAvailable
// ----------------------------------------------------------------------------
//	
TBool CSatelliteEng::IsSatelliteDataAvailable()
	{	
	TInt count = iSortedSatData.Count();
	if(count == 0)
		{
		return EFalse;
		}
    return ETrue;
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::SysOfMeasurementL
// ----------------------------------------------------------------------------
//	
TLocSystemofMeasurementValues CSatelliteEng::SysOfMeasurementL()
	{
	return iMeasurementSetting->SysofMeasurementL( );
	}

// ----------------------------------------------------------------------------
// CSatelliteEng::HandleSysMeasureValueL
// ----------------------------------------------------------------------------
//	
void CSatelliteEng::HandleSysMeasureValueL()
	{
	iObserver->NotifyL( KErrNone );
	}

// ----------------------------------------------------------------------------
// SortCriteria
// ----------------------------------------------------------------------------
//	
TInt SortCriteria(const TSatelliteData& aFirst, const TSatelliteData& aNext)
	{
	if(aFirst.SignalStrength() > aNext.SignalStrength())
		{
		return 1;
		}
	else if(aFirst.SignalStrength() < aNext.SignalStrength())
		{
		return -1;
		}
	return 0;
	}
	

// End of File
