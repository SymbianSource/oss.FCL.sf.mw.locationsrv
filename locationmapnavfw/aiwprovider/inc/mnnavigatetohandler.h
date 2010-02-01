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
* Description:  CMnNavigateToHandler class declaration
*
*/


#ifndef MN_NAVIGATETOHANDLER_H
#define MN_NAVIGATETOHANDLER_H

#include "mnaiwcommandhandlerbase.h"

class CMnNavigator;

/** \internal
 *  Handler for KAiwCmdMnNavigateTo command 
 */
class CMnNavigateToHandler : public CMnAiwCommandHandlerBase
    {
    public:
        static CMnNavigateToHandler* NewL(
            CMnProvider& aProvider,
            TRunMode aRunMode );
            
        ~CMnNavigateToHandler();            
        
    public: // from CMnAiwCommandHandlerBase
        TInt CommandId() { return KAiwCmdMnNavigateTo; };
        TBool IsAsyncCommand() { return EFalse; };
        
        void SetExitObserverL( MMnProviderExitCallback& aExitCallback );

    protected:
        CMnNavigateToHandler( CMnProvider& aProvider );
            
        void ConstructL( TRunMode aRunMode );            

    protected: // from CMnAiwCommandHandlerBase
        void HandleAiwParamLandmarksL( RArray<TPtrC8>& aLandmarkDataList );
        void ResetParametersL();
        void DoStartL();

    private:        
        CMnNavigator*   iNavigator;
        TPtrC8          iLandmarkData;
    };

#endif // MN_NAVIGATETOHANDLER_H
