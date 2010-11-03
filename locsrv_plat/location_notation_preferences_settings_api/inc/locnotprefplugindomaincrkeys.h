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
* Description:  Central Repository keys for Location Notation Preferences
*                plug-in. Defines the various Systems of measurements that 
*                can be used by Location application.
*                
*
*/


#ifndef LOCNOTPREFPLUGINDOMAINCRKEYS_H
#define LOCNOTPREFPLUGINDOMAINCRKEYS_H

/**
 * UID value for the Central Repository Key used for Location Notation 
 * preferences
 */
const TInt KCRUidLocNotationPref = 0x10206910;

/**
 * Central repository Key identifer value for Location Notation Preferences / System of Measurements
 */
const TInt KLocSystemofMeasurements = 0x00000001;

/**
 * Enumeration list of values for KLocSystemofMeasurements
 * 
 * Defines the System of measurements available for Location related values.
 */
enum TLocSystemofMeasurementValues
    {
    /**
     * Metric System of measurement ( mt, kgs, etc. ) would be used for
     * Location related values.
     */
    ELocMetric      = 0x00000001,
            
    /**
     * Imperial System of measurement ( ft, lbs, etc. ) would be used for 
     * Location related values.
     */  
    ELocImperial    = 0x00000002
    };

#endif      // LOCNOTPREFPLUGINDOMAINCRKEYS_H