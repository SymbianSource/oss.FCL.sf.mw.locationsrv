/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   RunSession parameters merging
*
*/




#ifndef EPOS_SUPLPARAMETERS_H_
#define EPOS_SUPLPARAMETERS_H_

#include <e32std.h>
#include <epos_suplterminalconstants.h>
#include <epos_suplterminaltrigger.h>

class TSuplTerminalQop;

class TSuplParameters
    {
public:
    TSuplTerminalQop qop;
    TBuf<KMaxHslpAddressLength> HslpAddress;
    TBool FallBack;
	TSuplTerminalPeriodicTrigger PeriodicTrigger;    
    };

#endif /*EPOS_SUPLPARAMETERS_H_*/
