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
* Description:  CMnFetchCoordinatesHandler class declaration
*
*/


#ifndef MN_FETCHCOORDINATESHANDLER_H
#define MN_FETCHCOORDINATESHANDLER_H

#include "mngeocodingcmdhandlerbase.h"

/** \internal
 *  Handler fo KAiwCmdMnCoordByAddr AIW command
 */
class CMnFetchCoordinatesHandler : public CMnGeocodingCmdHandlerBase
    {
    public:
        static CMnFetchCoordinatesHandler* NewL( 
            CMnProvider& aProvider, TRunMode aRunMode );

    public: // from CMnAiwCommandHandlerBase
        void Start();
        TInt CommandId() { return KAiwCmdMnCoordByAddr; };
        TBool IsAsyncCommand() { return ETrue; };

    protected:
        CMnFetchCoordinatesHandler( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );

    protected: // from CActive
        void RunL();
        void DoCancel();

    protected: // from CMnAiwCommandHandlerBase
        void HandleAiwParamL( const TAiwGenericParam& aParam );
        void HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList );
        void ResetParametersL();
        void DoStartL();

    protected:
        TPtrC   iPlainAddress;
    };

#endif // MN_FETCHCOORDINATESHANDLER_H
