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
* Description:  Observer for the SUPL Session
*
*/


#ifndef MLOCSUPLSETTINGSSESSIONOBSERVER_H_
#define MLOCSUPLSETTINGSSESSIONOBSERVER_H_

// Class Declaration

/**
 * Observer for the SUPL Session. The observer provides callbacks for
 * any changes in the SUPL session
 *
 * @lib locsuplsettings.lib
 * @since S60 v3.1 
 */
class MLocSUPLSettingsSessionObserver
    {
    public:
        /**
         * Update type for the Session
         */
        enum TUpdateType
        {
        // Enum for Session modification.
        ESessionModified = 0,
        // Enum for Session deletion.
        ESessionDeleted
        };
    
    public:
        	
        /**
         * Called when a change has been detected in the SUPL Session 
         * identified by the session id provided by SessionIdL.
         * @param aType Type of Updation
         *
         */
        virtual void UpdateSessionL( TUpdateType aType ) = 0;
        
        /**
         * Called when a change has been detected in the SUPL Session.
         *
         */
        virtual TInt64 SessionId( ) = 0;

    };

#endif      // MLOCSUPLSETTINGSSESSIONOBSERVER_H_  
            
// End of File
