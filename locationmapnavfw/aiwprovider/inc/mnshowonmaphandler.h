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
* Description:  CMnShowOnMapHandler class declaration
*
*/


#ifndef MN_SHOWMAPHANDLER_H
#define MN_SHOWMAPHANDLER_H

#include "mnmapviewcmdhandlerbase.h"

/** \internal
 *  Handler for KAiwCmdMnShowMap command 
 */
class CMnShowOnMapHandler : public CMnMapViewCmdHandlerBase
    {
    public:
        static CMnShowOnMapHandler* NewL(
            CMnProvider& aProvider,
            TRunMode aRunMode );
        
    public: // from CMnAiwCommandHandlerBase
        TInt CommandId() { return KAiwCmdMnShowMap; };
        TBool IsAsyncCommand() { return EFalse; };

    protected:
        CMnShowOnMapHandler( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );            

    protected: // from CMnAiwCommandHandlerBase
        void DoStartL();
    };
    
#endif // MN_SHOWMAPHANDLER_H    