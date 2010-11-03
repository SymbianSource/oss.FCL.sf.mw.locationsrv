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
* Description:  CMnMapViewCmdHandlerBase class declaration
*
*/


#ifndef MN_FETCHADDRESSHANDLER_H
#define MN_FETCHADDRESSHANDLER_H

#include "mngeocodingcmdhandlerbase.h"

/** \internal
 *  Handler for KAiwCmdMnAddrByCoord command
 */
class CMnFetchAddressHandler : public CMnGeocodingCmdHandlerBase
    {
    public:
        static CMnFetchAddressHandler* NewL( 
            CMnProvider& aProvider, 
            TRunMode aRunMode );
            
    public: // from CMnAiwCommandHandlerBase
        TInt CommandId() { return KAiwCmdMnAddrByCoord; };
        TBool IsAsyncCommand() { return ETrue; };

    protected:
        CMnFetchAddressHandler( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );

    protected: // from CMnAiwCommandHandlerBase
        void DoStartL();

    protected: // from CActive
        void RunL();
        void DoCancel();
    };

#endif // MN_FETCHADDRESSHANDLER_H
