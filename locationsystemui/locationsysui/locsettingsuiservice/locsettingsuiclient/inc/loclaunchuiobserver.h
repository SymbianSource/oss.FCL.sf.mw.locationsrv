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
* Description:  Observer to notify the completion of launching a UI.
*
*/


#ifndef M_LOCLAUNCHUIOBSERVER_H
#define M_LOCLAUNCHUIOBSERVER_H

// System Includes
#include <e32base.h>

/**
 *  Observer to the Launch UI Active Object
 *
 *  The observer is used to notify the completion of a Lauch call. The 
 *  call back from the observer in addition to notifying the completion of
 *  the launch call also returns back an error code which denotes the success
 *  of the launch operation
 *
 *  @lib locsettingsuiclient.lib
 *  @since S60 v3.1
 */
class MLocLaunchUiObserver
    {

public:
    /**
     * Call back function to notify the success state of the Launch function
     * function call
     *
     * @since S60 v3.1
     * @param aError Error code to denote the success/failure of the Launch 
     *               operation
     */
    virtual void LaunchUiComplete( TInt    aError ) = 0;

    };


#endif // M_LOCLAUNCHUIOBSERVER_H
