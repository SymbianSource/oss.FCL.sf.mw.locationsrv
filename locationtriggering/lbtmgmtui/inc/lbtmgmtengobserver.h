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
* Description:  Location Triggering Management Plugin Engine Observer
*
*/


#ifndef MLBTMGMTENGOBSERVER_H_
#define MLBTMGMTENGOBSERVER_H_

// Class Declaration
/**
 * Observer to the Location Triggering Management plug-in.
 */
 
class MLbtMgmtEngObserver
    {
    public:
        /**
         * Notifies a change in the Location Triggering Management settings values
         */
        virtual void HandleSettingsChangeEventL() = 0;
           
    };
    
#endif // MLBTMGMTENGOBSERVER_H_

