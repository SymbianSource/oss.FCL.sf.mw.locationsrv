/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class which handles the dialog and expiration timer.
*
*/


#ifndef CLPDNOTIFIERQUERYLAUNCHER_H
#define CLPDNOTIFIERQUERYLAUNCHER_H

//  INCLUDES
#include "lpdquerylauncherbase.h"
#include <e32base.h>
#include <lbs/epos_privacy.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  Implements functionality for executing notifications.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdNotifierQueryLauncher :
public CLpdQueryLauncherBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aHandler of ther query result
        * @return created object
        */
        static CLpdNotifierQueryLauncher* NewL( MLpdResultHandler& aHandler );
        
        /**
        * Destructor. Call Cancel() first if you have launched a query.
        */
        virtual ~CLpdNotifierQueryLauncher();

    public: // New functions

        /**
        * Prepares notification resources.
        * @param aReason for notification
        * @param aDecision that was applied to the request
        * @param aCurrentRequest request id of current request
        */
        void PrepareNotificationResourcesL( TPosNotificationReason aReason,
                                             TPosRequestDecision aDecision );
        
       /**
        * Prepares notification resources for supl periodic request.
        * @param aReason for notification
        * @param aCurrentRequest request id of current request
        */       
        void PrepareSuplNotificationResourcesL( TPosNotificationReason aReason );

        /**
        * Prepares resources for error notification caused by
        * privacy query cancellation. Otherwise similar to 
        * PrepareNotificationResourcesL().
        * @param aDecision that was applied to the request
        * @param aCurrentRequest request id of current request
        */
        void PrepareCancelNotifResourcesL( TPosRequestDecision aDecision );

      /**
        * Prepares resources for error notification caused by
        * privacy query cancellation in supl periodic request.
        * Otherwise similar to PrepareNotificationResourcesL().
        * @param aDecision that was applied to the request
        * @param aCurrentRequest request id of current request
        */
        void PrepareSuplCancelNotifResourcesL( TPosRequestDecision aDecision );


    protected: // Functions from base classes

        /**
        * From CLpdQueryLauncherBase, called when dialog is dismissed.
        */
        void ExtendedHandleDlgDismissedL();

    private: // Constructors and destructors

        /**
        * C++ constructor.
        * @param aHandler of the query result
        */
        CLpdNotifierQueryLauncher( MLpdResultHandler& aHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    protected: // data

    };

#endif      // CLPDNOTIFIERQUERYLAUNCHER_H   
            
// End of File
