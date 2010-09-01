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
* Description:  Location Notation Preferences Plugin Engine Observer
*
*/


#ifndef M_LOCNOTPREFENGOBSERVER_H_
#define M_LOCNOTPREFENGOBSERVER_H_

// Class Declaration
/**
 * Observer to the Location Notation Preferences plug-in.
 */
 
class MLocNotPrefEngObserver
    {
    public:
        /**
         * Notifies a change in the Location notation preferences settings values
         */
        virtual void HandleSettingsChangeEventL() = 0;
           
    };
    
#endif // M_LOCNOTPREFENGOBSERVER_H_

