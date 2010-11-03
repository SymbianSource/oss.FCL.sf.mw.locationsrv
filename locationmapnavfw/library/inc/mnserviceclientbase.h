/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RMnServiceClientBase class
*
*/


#ifndef MN_SERVICECLIENTBASE_H
#define MN_SERVICECLIENTBASE_H

#include <AknServerApp.h>

#include "mninternal.h"
#include "mnappserviceuids.hrh"

/** \internal
 *  Base class for service implementations
 */
NONSHARABLE_CLASS( RMnServiceClientBase ) : public RAknAppServiceBase
    {
    public:

        /** Connects to new application server */
        void ConnectL( TUid aProviderUid );

        /** Connects to new chained application server */
        void ConnectChainedL( TUid aProviderUid );
        
        /** Cancels last async request */
        void Cancel();

        /** Closes parent session */
        void Close();
        
    protected:
        RMnServiceClientBase();
                
        TMnIpcCode      iLastRequest;
    };
  
#endif // MN_SERVICECLIENTBASE_H

