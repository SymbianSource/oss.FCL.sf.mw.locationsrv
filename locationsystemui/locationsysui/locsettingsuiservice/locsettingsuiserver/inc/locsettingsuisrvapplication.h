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
* Description:  The declaration for application class of Location Settings UI
*                Server
*
*/


#ifndef C_LOCSETTINGSUISRVAPP_H
#define C_LOCSETTINGSUISRVAPP_H

#include <aknapp.h>

//Forward declarations
class CApaAppServer;
class CApaDocument;

/**
 *  The Application class for the Location Settings UI Application Server.
 *  It is responsible for creating an instance of the Server class.
 *
 *  @lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUISrvApp) : public CAknApplication
    {
public:
    /**
     * From Base class CApaApplication
     * Creates an instance of the server class CLocSettingsUIAppServer
     * @param aAppServer The server pointer to be set to the allocated
     * server object.
     * @return None
     */
    virtual void NewAppServerL(CApaAppServer*& aAppServer);
    
    /**
     * Destructor
     */
    virtual ~CLocSettingsUISrvApp();

protected:
    /**
     * From Base class CApaApplication
     * Creates CLocSettingsUISrvDocument document object.
     * @return A pointer to the created document object.
     */
    virtual CApaDocument* CreateDocumentL();
    
    /**
     * From Base class CApaApplication
     * returns application's UID (KUidlocation).
     * @return The value of KUidlocation.
     */
    virtual TUid AppDllUid() const;
    
private: // member data
    /** Own:
     * When the Server is not running in embedded mode,
     * the framework doesn't instantiate the server class.
     */
    CApaAppServer* iAppServer;
    };

#endif // C_LOCSETTINGSUISRVAPPLICATION_H
