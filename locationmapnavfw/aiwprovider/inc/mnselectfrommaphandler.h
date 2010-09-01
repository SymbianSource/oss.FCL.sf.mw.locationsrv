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
* Description:  CMnSelectFromMapHandler class declaration
*
*/


#ifndef MN_SELECTFROMMAPHANDLER_H
#define MN_SELECTFROMMAPHANDLER_H

#include "mnmapviewcmdhandlerbase.h"

/** \internal 
 *  Handler for KAiwCmdMnSelectFromMap command 
 */
class CMnSelectFromMapHandler : public CMnMapViewCmdHandlerBase
    {
    public:
        static CMnSelectFromMapHandler* NewL( 
            CMnProvider& aProvider, 
            TRunMode aRunMode );
            
    public: // from CMnAiwCommandHandlerBase
        TInt CommandId() { return KAiwCmdMnSelectFromMap; };
        TBool IsAsyncCommand() { return ETrue; };

    protected:
        CMnSelectFromMapHandler( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );
        
    protected: // from CActive
        void RunL();
        void DoCancel();

    protected: // from CMnAiwCommandHandlerBase

        /** Processes EGenericParamRequestText parameter */
        void HandleAiwParamL( const TAiwGenericParam& aParam );

        void DoStartL();
    };

#endif // MN_SELECTFROMMAPHANDLER_H
