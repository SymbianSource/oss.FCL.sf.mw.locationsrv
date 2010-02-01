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
* Description:  The declaration for Settings UI Info class of Location
*                Settings UI Server
*
*/


#ifndef C_LOCSETTINGSUIINFO_H
#define C_LOCSETTINGSUIINFO_H

#include <e32cmn.h>
#include <e32std.h>

//Forward declarations
class CSession2;

/**
 *  The Info class which contains all information pertaining to
 *  the settings UI launch request.
 *
 *  @lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUIInfo) : public CBase
    {
public:
    /**
     * Symbian 2 Phase Constructor
     *
     * @since S60 v3.1
     * @param The request IPC Message sent by the client.
     * @param The Launch parameters with which the UI should be launched
     * @return A pointer to the created CLocSettingsUIInfo object
     */
    static CLocSettingsUIInfo* NewL( const RMessage2&   aMessage );

    /**
     * Symbian 2 Phase Constructor
     *
     * @since S60 v3.1
     * @param The request IPC Message sent by the client.
     * @param The Launch parameters with which the UI should be launched
     * @return A pointer to the created CLocSettingsUIInfo object
     */
    static CLocSettingsUIInfo* NewLC( const RMessage2&  aMessage );

    /**
     * Destructor
     */
    virtual ~CLocSettingsUIInfo();

public:
    /**
     * Complete the Launch request with the Error Code
     *
     * @since S60 v3.1
     * @param aErrorCode The error code returned from the completion of the
     * launch request.
     * @return Pointer to the session object.
     */
    void Complete(TInt aErrorCode);
     
public: //Accessor methods

    /**
     * Retrieve the IPC message
     * @return RMessage2&    The IPC message contained in the Info structure
     *
     */
    RMessage2&   IPCMessage();
    
    /**
     * Retrieve the offset of the TSglQueLink member.
     * This offset value is needed by the TSglQue to maintain and
     * manipulate the Linked List.
     *
     * @since S60 v3.1
     * @return Offset value of iNext member in this class.
     */
    static TInt QueLinkOffset();
    
    /**
     * Retrieve the session object through which this Request was sent.
     *
     * @since S60 v3.1
     * @return Pointer to the session object.
     */
    CSession2* Session() const;

    /**
     * Retrieve the Settings UI UID for which launch 
     * is requested.
     *
     * @since S60 v3.1
     * @return The UID value for the requested Settings UI.
     */
    TUid SettingsUID() const;

    /**
     * Retrieve the Settings UI Launch Parameters.
     *
     * @since S60 v9.2
     * @return The Int Launch parameters for the Settings UI
     */
    TInt SettingsUIIntParams();

    /**
     * Retrieve the Settings UI Launch String Parameters.
     *
     * @since S60 v9.2
     * @return The String Launch parameters for the Settings UI
     */
    TPtrC SettingsUIStringParamsL();

private:
    /**
     * Constructor
     */
    CLocSettingsUIInfo(const RMessage2 &aMessage );

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
     * The data member contains the RMessage2 IPC message that was received
     * from the Client
     */
    RMessage2   iReqMsg;  

    /**
     * The Settings UI launch string parameters.
     */
    HBufC* iParamsString;

    /**
     * The Single Queue Link Object.
     */
    TSglQueLink iNext;
    };

#endif // C_LOCSETTINGSUIINFO_H
