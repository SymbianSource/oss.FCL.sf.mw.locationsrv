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
* Description:  Observer for the change in System of Measurement
*
*/



#ifndef __MSATELLITEMEASUREMENTSETTINGSOBSERVER_H__
#define __MSATELLITEMEASUREMENTSETTINGSOBSERVER_H__

// CLASS DECLARATION

/**
*  Observer for the change in System of Measurement.
*
*/
class MSatelliteMeasurementSettingObserver
    {
    public: // New functions

        /**
         * Notification function for change in System of Measurement
         *
         * @return None 
         */
        virtual void HandleSysMeasureValueL( ) = 0;
    };

#endif      // __MSATELLITEMEASUREMENTSETTINGSOBSERVER_H__

// End of File
