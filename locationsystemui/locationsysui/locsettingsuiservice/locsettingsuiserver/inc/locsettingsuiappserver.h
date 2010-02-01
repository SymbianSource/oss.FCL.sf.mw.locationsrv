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
* Description:  The declaration for AppServer class of Location Settings UI 
*                Server
*
*/


#ifndef C_LOCSETTINGSUIAPPSERVER_H
#define C_LOCSETTINGSUIAPPSERVER_H

#include <AknServerApp.h>

//Forward declarations
class CApaAppServiceBase;
class CLocSettingsUISrvShutdown;
class CLocSettingsUILaunchMgr;
class CEikonEnv;

/**
 *  The server class is responsible for creating session objects.
 *  
 *  This server class overrides the CreateServiceL method to create
 *  instances of the CLocSettingsUIService object. The base class invokes 
 *  the CreateServiceL method on receiving a request for session creation.
 *
 *  @lib 
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUIAppServer) : public CAknAppServer
    {
public:
    /**
     * Constructor
     */
    CLocSettingsUIAppServer();

    /**
     * Destructor
     */
    virtual ~CLocSettingsUIAppServer();

    /**
     * Symbian 2nd Phase Constructor.
     * Overriding the base class call to pass the Server Name.
     */
    virtual void ConstructL(const TDesC& aFixedServerName);

public:
    /**
     * Cancels the Shutdown Timer.
     */
    void CancelShutdownTimer();
    
protected: 
    /**
     * From Base class CApaAppServer
     * Creates CLocSettingsUIService session object.The base class invokes
     * the CreateServiceL method on receiving a request for session creation.
     *
     * @param aServiceType The Service UID for this service
     * @return A pointer to the CLocSettingsUIService session object.
     */
	virtual CApaAppServiceBase* CreateServiceL(TUid aServiceType) const;

    /**
     * From Base class CApaAppServer
     * Allows the server to handle the case when all client sessions
     * have closed.The default behaviour is to close the server immediately.
     * The Location Settings UI Server behaviour is to wait for new
     * connections for 10 seconds and then shutdown in case no new
     * client connection is established. Hence this method is overriden
     * to provide for this behaviour of the server.
     *
     * @return A pointer to the CLocSettingsUIService session object.
     */
	virtual void HandleAllClientsClosed();

private:
    /**
     * The Shutdown Timer Object.
     */
    CLocSettingsUISrvShutdown* iShutdownTimer;
    };

#endif // C_LOCSETTINGSUISERVER_H
