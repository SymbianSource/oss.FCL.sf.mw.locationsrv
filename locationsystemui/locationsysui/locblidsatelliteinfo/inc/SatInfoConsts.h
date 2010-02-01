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
* Description:  Contains common constants
*
*/


#ifndef __SATELLITECONSTS_H__
#define __SATELLITECONSTS_H__


const TInt KSatInfoBufferSize( 50 );

_LIT( KSatInfoRscFileName, "SatInfo.rsc" );
//needed to get full screen dialog
const TInt KInitialSatellites(5);

const TInt KBlidMaxSatelliteSignalStrength( 40 );

const TInt KBlidMinSatelliteSignalStrength( 5 );

// Interval timeout to recive satellite data 
const TInt KSatelliteIntervalTime( 3000000 ); //3 seconds

const TInt KSatelliteTimeOut(4000000); // 4 seconds

const TInt KRightAngle(90); // 90 degree right angle

// trailing backslash has been added to it:
_LIT( KSatTmp_RESOURCE_FILES_DIR, "\\resource\\" );

const TInt KMaxSatellites(32);

enum TSatDialogView
	{
	EGraphicalView,
	ESignalStrengthView,
	ECompassView
	};

// Path to find the mbm file
_LIT( KBlidSatSystemIconPath, ":\\resource\\apps\\satellite.mif" );

//distance converters
const TReal KMetreToFoot( 3.280839895013123 );
const TReal KKilometreToMiles( 0.6213711922373338 );

//Speed Format
_LIT( KSpeedFormat,"%.1f" );
const TReal KSpeedConvertion( 3.6 ); // m/s => km/h
const TInt KDecimals3( 3 );

// Fix Type
const TInt K2dFix( 3 );

#endif // __SATELLITECONSTS_H__

// End of File
