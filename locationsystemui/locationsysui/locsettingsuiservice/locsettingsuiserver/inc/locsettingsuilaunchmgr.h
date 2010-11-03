/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The declaration for Settings Launch Manager class of Location
*                Settings UI Server
*
*/


#ifndef C_LOCSETTINGSUILAUNCHMGR_H
#define C_LOCSETTINGSUILAUNCHMGR_H

#include<coemain.h>

//Forward declarations
class CLocSettingsUISrvLaunchAO;

/**
 *  The Settings UI Launch Management class.
 *  This class is a Singleton. Session objects enqueue the requests
 *  to CLocSettingsUILaunchAO object through this Singleton.
 *
 *  @lib
 *  @since S60 v3.1
 */
class CLocSettingsUILaunchMgr : public CCoeStatic
    {
public:
    /**
     * Factory Method to create the Singleton.
     *
     * @since S60 v3.1
     * @return A pointer to the created CLocSettingsUILaunchMgr object
     */
    static CLocSettingsUILaunchMgr* InstanceL();

public:
    /**
     * Enqueue the Settings UI launch request.
     *
     * @since S60 v3.1
     * @param aMessage The Request IPC message from the Client.
     */
    void EnqueueRequestL( const RMessage2& aMessage );

    /**
     * Cancel the launched Settings UI request.
     *
     * @since S60 v3.1
     * @param aSession Pointer to the session that launched the settings UI
     * to be cancelled.
     */
    void CancelRequest( const CSession2* aSession );

    /**
     * Close the currently running Settings UI.
     * Needed when the Settings UI issues a EEikCmdExit which needs to be
     * handled in the Server App UI
     *
     * @since S60 v3.1
     * @param aErrorCode The error code to send to the Client
     */
    void CloseRunningSettingsUi(TInt aErrorCode);

private:
    /**
     * Constructor
     */
    CLocSettingsUILaunchMgr();

    /**
     * Destructor
     */
    virtual ~CLocSettingsUILaunchMgr();

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
     * The Active Object which launches the Setitngs UI and monitors the
     * actions.
     */
    CLocSettingsUISrvLaunchAO* iLauncher;
    };

#endif // C_LOCSETTINGSUILAUNCHMGR_H
