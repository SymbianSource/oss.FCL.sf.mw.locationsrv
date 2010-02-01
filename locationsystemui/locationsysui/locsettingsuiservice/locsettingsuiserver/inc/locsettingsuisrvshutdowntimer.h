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
* Description:  The declaration for Shutdown Timer class of Location Settings
*                UI Server
*
*/


#ifndef C_LOCSETTINGSUISRVSHUTDOWN_H
#define C_LOCSETTINGSUISRVSHUTDOWN_H


#include <e32std.h>
#include <e32base.h>

/**
 *  The Server Shutdown Timer class.
 *
 *  This class provides the timer functionality to shutdown the 
 *  Location Settings UI Server.
 *
 *  @lib 
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUISrvShutdown) : public CActive
    {
public:
    /**
     * Symbian 2 Phase Constructor
     *
     * @since S60 v3.1
     * @return A pointer to the created CLocSettingsUISrvShutdown
     * object
     */
    static CLocSettingsUISrvShutdown* NewL();

    /**
     * Destructor
     */
    virtual ~CLocSettingsUISrvShutdown();

public:
    /**
     * Starts the Shutdown Timer.
     * Since it uses an Asynchronous Timer the callback will be
     * received in the RunL.
     *
     * @since S60 v3.1
     */
    void Start();
    
protected: // Functions from base class CActive

   /**
    * From CActive Handles asynchronous request completion.
    */
    void RunL();

   /**
    * From CActive Cancels any outstanding request.
    */
    void DoCancel();

   /**
    * From CActive Handles a leave in RunL.
	*
    * @param aError The RunL leave code.
    * @return Always KErrNone, because all errors are handled
    */
    TInt RunError(TInt aError);

private:
    /**
     * Constructor
     */
    CLocSettingsUISrvShutdown();

    /**
     * Symbian 2nd Phase Constructor
     * Leaves in case of Error.
     *
     * @since S60 v3.1
     * @return None
     */
    void ConstructL();


private: // data
    /**
     * The Timer object
     */
    RTimer iTimer;
    };

#endif // C_LOCSETTINGSUISRVSHUTDOWN_H
