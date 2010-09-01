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
* Description:  Active Object to monitor the UI Server launch requests
*
*/


#ifndef C_LOCLAUNCHUIAO_H
#define C_LOCLAUNCHUIAO_H

// System Includes
#include <e32base.h>

// Forward Declarations
class MLocLaunchUiObserver;

/**
 *  Active Object to monitor the UI Server launch requests
 *
 *  This class is a helper class. The status variable of this active obect is 
 *  used to provide the asynchronous Send Receive server calls. The IPC message
 *  completes on this active object. 
 *
 *  @lib locsettingsuiclient.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CLocLaunchUiAO ): public CActive
    {
public:
    /**
     * Two Phase constructor 
     * 
     * @since S60 v3.1
     * @param aObserver Observer to the Launch UI Active Object
     * @return The reference of the newly allocated CLocLaunchUiAO 
     *         object
     */
    static CLocLaunchUiAO* NewL( MLocLaunchUiObserver&    aObserver );

    /**
     * Two Phase constructor. Leaves the allocated object on the Clean-up
     * stack 
     * 
     * @since S60 v3.1
     * @param aObserver Observer to the Launch UI Active Object
     * @return The reference of the newly allocated CLocLaunchUiAO 
     *         object
     */
    static CLocLaunchUiAO* NewLC( MLocLaunchUiObserver&    aObserver );

    /**
     * C++ Destructor
     * 
     * @since S60 v3.1
     */
    virtual ~CLocLaunchUiAO();

    /**
     * Informs the active object that there is an outstanding request.
     * The function leaves with error KErrInUse
     *
     * @since S60 v3.1
     */
    void    SetRequestOutStandingL();
    
    /**
     * Returns a reference to the active objects Status variable
     *
     * @since S60 v3.1
     * @return TRequestStatus&  Reference to the Status variable
     */
    TRequestStatus&     GetStatusVariable();
    
protected:
    
    /**
     * Inherited from CActive
     */
    void RunL();
    
    /**
     * Inherited from CActive
     */
    void DoCancel();

private:
    /**
     * C++ Default Constructor
     */
    CLocLaunchUiAO( MLocLaunchUiObserver&    aObserver );

private:
    /**
     * Reference to the Observer
     */ 
     MLocLaunchUiObserver&    iObserver ;
     
    };

#endif //  C_LOCLAUNCHUIAO_H
