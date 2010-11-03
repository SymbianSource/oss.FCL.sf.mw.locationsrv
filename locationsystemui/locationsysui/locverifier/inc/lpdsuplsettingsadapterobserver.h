/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer for the SUPL Settings adapter
*
*/


#ifndef MLPDSUPLSETTINGSADAPTEROBSERVER_H_
#define MLPDSUPLSETTINGSADAPTEROBSERVER_H_

// Class Declaration

/**
 * Observer for the SUPL Session. The observer provides callbacks for
 * any changes in the SUPL session
 *
 * @lib locverifierdlg.lib
 * @since S60 v9.2tb 
 */
class MLpdSUPLSettingsAdapterObserver
    {
    public:
        	
        /**
         * Called when a change has been detected in the SUPL Session 
         * identified by the session id parameter.
         * @param aSessionId Updated Session Id.
         *
         */
        virtual void UpdateSessionL( TInt64 aSessionId ) = 0;

    };

#endif      // MLPDSUPLSETTINGSADAPTEROBSERVER_H_  
            
// End of File
