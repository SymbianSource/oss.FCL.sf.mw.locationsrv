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
* Description:  BtGpsPsy Central Repository keys for locbtnotifier dialog settings
*
*/


#ifndef LOCBTNOTIFIERCRKEYS_H
#define LOCBTNOTIFIERCRKEYS_H

/**
* BT GPS PSY Setting Storage CenRep UID
*/
const TUid KCRUidLocBtGpsPsy = { 0x2001FCBB };


/**
* "External antenna connected" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyExtAntennaConnectedDialog = 0x10000001;
	
/**
* "External antenna disconnected" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyExtAntennaDisconnectedDialog = 0x10000002;

/**
* "Charger connected" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyExtPowerConnectedDialog = 0x10000003;
	
/**
* "Charger disconnected" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyExtPowerDisconnectedDialog = 0x10000004;
	
/**
* "Battery low" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyBatteryLowDialog = 0x10000005;

/**
* "Battery full" dialog setting key
*
* 0 = Off
* 1 = On
*/
const TUint32 KBluetoothGpsPsyBatteryFullDialog = 0x10000006;
	

#endif      // LOCBTNOTIFIERCRKEYS_H

// End of file