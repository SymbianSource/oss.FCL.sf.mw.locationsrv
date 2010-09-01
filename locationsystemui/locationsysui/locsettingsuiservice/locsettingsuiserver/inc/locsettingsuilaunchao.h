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
* Description:  The declaration for Settings Launch AO class of Location
*                Settings UI Server
*
*/


#ifndef C_LOCSETTINGSUILAUNCHAO_H
#define C_LOCSETTINGSUILAUNCHAO_H

#include <e32base.h>
#include <e32std.h> //For TSglQue

#include "locsettingsuiinfo.h"
#include "locationui.h"

//Forward Declaration
class CLocSysUiEngine;
class CEikonEnv;
class CActiveSchedulerWait;
/**
 *  The Settings UI Launch class.
 *
 *  This class interacts with the LocationSysUi engine interface
 *  to execute the requested settings UI.
 *  It maintains a queue of all the requests and schedules them
 *  to execute in a serialized manner.
 *
 *  @lib
 *  @since S60 v3.1
 */
class CLocSettingsUISrvLaunchAO : public CActive,
                                  public MLocationUIObserver
    {
public:
    /**
     * Symbian 2 Phase Constructor
     *
     * @since S60 v3.1
     * @return A pointer to the created CLocSettingsUISrvLaunchAO object
     */
    static CLocSettingsUISrvLaunchAO* NewL();

    /**
     * Symbian 2 Phase Constructor
     *
     * @since S60 v3.1
     * @return A pointer to the created CLocSettingsUISrvLaunchAO object
     */
    static CLocSettingsUISrvLaunchAO* NewLC();

    /**
     * Destructor
     */
    virtual ~CLocSettingsUISrvLaunchAO();

public:
    /**
     * Enqueue the Settings UI launch request.
     *
     * @since S60 v3.1
     * @param aReqInfo Pointer to the Settings UI Request Info class which
     * contains all the information needed to launch the settings UI.
     * @return None
     */
    void EnqueueRequest( CLocSettingsUIInfo* aReqInfo );

    /**
     * Cancel the launched Settings UI.
     * Checks if the UI to be cancelled is in the queue. If so the item
     * is removed from the queue. Otherwise if the setitngs UI is currently
     * running then it is cancelled and then removed.
     *
     * @since S60 v3.1
     * @param aSession The pointer to the session from which the settings UI
     * launch was requested.
     * @return None
     */
    void CancelRequest( const CSession2* aSession );

    /**
     * Returns whether the server is running in embedded application server
     * mode or not.
     *
     * @since S60 v3.1
     * @return ETrue if running in embedded application server mode. EFalse
     * otherwise.
     */
    TBool IsSrvRunningAsEmbeddedApp() const;

    /**
     * From Base class MLocationUIObserver
     * Notifies the termination of the Settings UI
     *
     * @param aErrorCode The termination reason. KErrNone for normal 
     *					 terminations. In case of error or pre-mature
     *					 aborting System wide Error codes.
     */
    virtual void LocationUIDismissed( TInt aErrorCode );

protected: // Functions from base classes
    /**
     * From Base class CActive 
     * Handles asynchronous request completion.
     */
    virtual void RunL();

    /**
     * From Base class CActive 
     * Cancels any outstanding request.
     */
    virtual void DoCancel();

    /**
     * From Base class CActive 
     * Handles a leave in RunL.
     *
     * @param aError The RunL leave code.
     * @return Always KErrNone, because all errors are handled
     */
    virtual TInt RunError(TInt aError);

private:
    /**
     * Constructor
     */
    CLocSettingsUISrvLaunchAO();

    /**
     * Symbian 2nd Phase Constructor
     * Leaves in case of Error.
     *
     * @since S60 v3.1
     * @return None
     */
    void ConstructL();

private: // Helper methods
    /**
     * Complete the specified request.
     * Deques the request from the queue and also updates the request count.
     *
     * @since S60 v3.1
     * @param aReq pointer to the request to be completed and dequeued.
     * @param aErrorCode the error code to be returned to the client that
     * originated the request.
     * @return None
     */
    void CompleteRequest( CLocSettingsUIInfo* aReq, TInt aErrorCode );
    
    /**
     * Sends the Applicaiton Server UI to Background and hides it from
     * the FSW as well.
     *
     * @since S60 v3.1
     * @return None
     */
    void SendAppToBackground();

    /**
     * Brings the Application Server UI to Foreground and enables it in
     * the FSW as well.
     *
     * @since S60 v3.1
     * @return None
     */
    void BringAppToForeground();
    
    /**
     * Since this Active Object is a dummy object we have to reschedule
     * it to run when ever needed. This method does that functionality.
     * mode or not.
     *
     * @since S60 v3.1
     * @return None
     */
    void ScheduleAORun();

private: // data

    /**
     * This data member maintains a list of all the Launch Requests.
     */
    TSglQue<CLocSettingsUIInfo> iRequestQue;

    /**
     * The Engine Interface for launching Settings UI.
     * Does not own. Just for reference.
     * Will be created in this class and then ownership will be transferred
     * to the Document class.
     */
    CLocSysUiEngine* iEngine;

    /**
     * Reference to the Eikon Environment.
     * It is accessed multiple times. Hence Stored here to prevent
     * unnecessary access each time to the TLS.
     */
    CEikonEnv* iEikEnv;
    
    /**
     * Determines whether this server is running as an embedded application
     * server.
     */
    TBool iIsSrvRunningAsEmbeddedApp;
    
    /**
     * Reference to the Location UI that is currently outstanding.
     * At any instant of time there can be only one request outstanding.
     * Incase, there are no requests outstanding then the value is set to
     * NULL.
     */
    MLocationUI*        iLocationUI;
    };

#endif // C_LOCSETTINGSUILAUNCHAO_H
