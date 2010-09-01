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
* Description:  Defining local variation for SUPL Settings UI
*
*
*/


#ifndef LOCSUPLSETTINGSPRIVATECRKEYS_H
#define LOCSUPLSETTINGSPRIVATECRKEYS_H

// UID value for the Central Repository Key used for local variationing in 
// SUPL Settings UI
const TInt KCRUidLocSuplSettingsUi = 0x1028190A;

// Central repository Key identifer value
const TInt KLocSuplSettingsLVFlags = 0x00000001;

// Enumeration values that the Central repository key KLocSuplSettingsLVFlags 
// can take
enum TSuplSettingsLVValue
	{
	ESuplHslpAddressAbsent 	= 0x00000000,
	ESuplHslpAddressPresent = 0x00000001
	};

#endif      // LOCSUPLSETTINGSPRIVATECRKEYS_H