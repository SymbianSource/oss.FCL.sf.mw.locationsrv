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
* Description:  Dll entry point
*
*/


#ifndef __SATELLITE_DLLMAIN_H__
#define __SATELLITE_DLLMAIN_H__

//  INCLUDES
#include <e32std.h>

// DATA TYPES
/// Satellite panic codes.
enum TSatellitePanicCode
    {
    EModelTypeNotSupported= 0
    };

// FUNCTION PROTOTYPES
/**
 * Stop the program execution with module specific text and error code.
 * Call if an unrecoverable error occurs in this module's code.
 *
 * @param aCode     error code
 *
 * @see TSatellitePanicCode
 */
void SatellitePanic(TInt aCode);

#endif // __SATELLITE_DLLMAIN_H__

// End of File
