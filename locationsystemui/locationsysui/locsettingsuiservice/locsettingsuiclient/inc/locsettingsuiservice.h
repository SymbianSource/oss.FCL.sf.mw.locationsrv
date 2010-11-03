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
* Description:  Client side resource handle to the UI Server
*
*/


#ifndef R_LOCSETTINGSUISERVICE_H
#define R_LOCSETTINGSUISERVICE_H

// System Includes
#include <e32base.h>
#include <AknServerApp.h>

// User Includes
#include "loclaunchuiobserver.h"

// Forward Declaration
class CLocLaunchUiAO;

/**
 *  Client Side handle to the Settings UI server
 *
 *  @lib locsettingsuiclient.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( RLocSettingsUiService ): public RAknAppServiceBase,
                                            public MLocLaunchUiObserver,
                                            public MAknServerAppExitObserver
    {
public:
    /**
     * C++ Default Constructor
     */
    RLocSettingsUiService();
    
    /**
     * C++ Destructor
     * 
     * @since S60 v3.1
     */
    virtual ~RLocSettingsUiService();

    /**
     * Launch a Settings UI in an ui app server. 
     * The API is used to launch a settings UI. There can be only one 
     * outstanding Launch request at any instant of time. This is an 
     * asynchronous function. If a session is already running then the
     * function leaves with KErrInUse.
     *
     * @since S60 v3.1
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParams            Opaque parameters that is handed over to
     *                           the Settings UI.
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    void LaunchSettingsUiL( TUid            aImplementationUid, 
                            TInt            aParams,
                            TRequestStatus& aStatus );

    /**
     * Launch a Settings UI as an embedded application.
     * The API is used to launch a settings UI as an embedded application.
     *
     * @since S60 v3.1
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParams            Opaque parameters that is handed over to
     *                           the Settings UI. 
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    void LaunchSettingsUiAsEmbeddedAppL( 
                                TUid            aImplementationUid, 
                                TInt            aParams,
                                TRequestStatus& aStatus );    

    /**
     * Launch a Settings UI in an ui app server. 
     * The API is used to launch a settings UI. There can be only one 
     * outstanding Launch request at any instant of time. This is an 
     * asynchronous function. If a session is already running then the
     * function leaves with KErrInUse.
     *
     * @since S60 v9.2
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aStringParams      Opaque parameters that is handed over to
     *                           the Settings UI.
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    void LaunchSettingsUiL( TUid            aImplementationUid, 
                            const TDesC&            aStringParams,
                            TRequestStatus& aStatus );

    /**
     * Launch a Settings UI as an embedded application.
     * The API is used to launch a settings UI as an embedded application.
     *
     * @since S60 v9.2
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aStringParams      Opaque parameters that is handed over to
     *                           the Settings UI.
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    void LaunchSettingsUiAsEmbeddedAppL( 
                                TUid            aImplementationUid, 
                                const TDesC&            aStringParams,
                                TRequestStatus& aStatus );    
    
    /**
     * Cancels an already launched settings UI.
     *
     * @since S60 v3.1
     * @return a Symbian OS Error Code in case a settings UI was not launched.
     * Returns KErrNone if successful. 
     */
    TInt CancelLaunchedSettingsUi();
    
    /**
     * Launch Positioning Settings UI in an ui app server.
     * The API is used to launch Positioning Settings UI. There can be only 
     * one outstanding Launch request at any instant of time. This is an 
     * asynchronous function. If a session is already running then the 
     * function leaves with KErrInUse.
     *
     * @since S60 v3.2
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.     
     *
     */
    void LaunchPosSettingsL( TRequestStatus& aStatus );
    
    /**
     * Launch Positioning Settings UI as an embedded application.
     * There can be only one outstanding Launch request at any instant of
     * time. This is an asynchronous function. If a session is already 
     * running then the function leaves with KErrInUse.
     *
     * @since S60 v3.2
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.     
     *
     */    
    void LaunchPosSettingsAsEmbeddedAppL(  TRequestStatus& aStatus );
 
    /**
     * Closes an already launched Positioning Settings UI.
     *
     * @since S60 v3.2
     * @return Symbian OS Error Code in case Positioning settings UI
     *         was not launched.
     * Returns KErrNone if successful. 
     */   
    TInt ClosePosSettings();
        
    /**
     * Sets the Launch Active object which is used to initiate Asynchronous 
     * calls to the UI server
     */
    void SetLaunchAO( CLocLaunchUiAO*&      aLaunchAO );
    
public: // From MLocLaunchUiObserver

    void LaunchUiComplete( TInt    aError );
    
public: // From RAknAppServiceBase
    TUid ServiceUid() const;
 
public:

    /**
     * From base class MAknServerAppExitObserver
     * Handles the Server Exit Event.
     * @param aReason the command Id or error code which caused the server 
     * to close.
     */
    virtual void HandleServerAppExit(TInt aReason);
    
private:
    /**
     * Connects to a non embedded server instance. The function tries to 
     * create a new server instance if the server is not already running
     */ 
    void    ConnectServerL();
    
    /**
     * Starts a new server instance
     *
     * @return  KErrNone, If the server is successfully launched
     *          System wide error codes incase the Start fails
     */
    TInt    StartServer();
       
private:
    /**
     * Enumeration type defining the type of launch request outstanding
     */
     enum   TLaunchRequestType
        {
        ELaunchNone,
        ELaunchNormal,
        ELaunchEmbedded
        };
        
private:

    /**
     * Active object used to provide asynshronous Send Receive Api calls
     * Owns
     */
    CLocLaunchUiAO*         iLaunchAO;
    
    /**
     * Reference to the Status variable of the object on which the Launch
     * request was issued
     */
     TRequestStatus*        iRequestStatus;
    
    /**
     * Current Launch status of the Serive provider
     */
     TLaunchRequestType     iLaunchType;
     
    /**
     * Monitor the life of the server application.
     * Wait for Server Exit Event.
     * Owns
     */
     CApaServerAppExitMonitor* iSrvMonitor;
    };

#endif // R_LOCSETTINGSUISERVICE_H
