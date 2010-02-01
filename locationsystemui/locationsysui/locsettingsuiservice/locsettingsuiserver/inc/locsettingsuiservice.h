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
* Description:  The declaration for Service class of Location Settings UI 
*                Server
*
*/


#ifndef C_LOCSETTINGSUISERVICE_H
#define C_LOCSETTINGSUISERVICE_H

#include <AknServerApp.h>

/**
 *  The service class for Location Settings UI Application Server.
 *
 *  The Service class receives requests from the clients. It
 *  processes these requests to provide the Settings UI Launch
 *  service.
 *
 *  @lib 
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS(CLocSettingsUIService) : public CAknAppServiceBase
    {
public:
    /**
     * Constructor
     */
    CLocSettingsUIService();

    /**
     * Destructor
     */
    virtual ~CLocSettingsUIService();

public:
    /**
     * Override of CAknAppServiceBase
     * This function must base-call CAknAppServiceBase::CreateL()
     *
     * @param None.
     * @return None.
     */
    void CreateL();
    
    /**
     * Override of CAknAppServiceBase
     * This function must base-call CAknAppServiceBase::ServiceL()
     *
     * @param aMessage The client message
     * @return None.
     */
    void ServiceL(const RMessage2& aMessage);

    /**
     * Override of CAknAppServiceBase
     * This function must base-call CAknAppServiceBase::ServiceError()
     *
     * @param aMessage The client message.
     * @param aError The error code to which occured during message servicing
     * @return None.
     */
    void ServiceError(const RMessage2& aMessage,TInt aError);

    /**
     * Override of CApaAppServiceBase
     * This method is called for every request.
     * This method checks whether the client has the neccessary
     * permission to execute this request.
     *
     * @param aMessage The client message.
     * @param aAction The Action to be taken if the security check fails.
     * This is typically a value from TFailureAction.
     * @param aMissing A reference to the list of security attributes missing
     * from the checked process.  The policy server initialises this
     * object to zero (that is a sid of 0, a vid of 0, and no capabilities).
     * If derived implementations wish to take advantage of a list of
     * missing attributes in their implementation of CustomFailureActionL(),
     * then they should set those missing attributes here in
     * CustomSecurityCheckL().
     * @return A value from CPolicyServer::TCustomResult.
     */
	
	CPolicyServer::TCustomResult SecurityCheckL(const RMessage2& aMessage,
	                                            TInt& aAction,
	                                            TSecurityInfo& aMissing);
    
private: // Helper methods
    /**
     * This is a helper method to determine whether the passed parameters are
     * valid or not.
     *
     * @param aSettingsUid The UID value for the settings UI.
     * @param aParams The opaque paramters that are passed from the client
     * to the Settings UI.
     * @return Etrue if the parameters valid. EFalse otherwise.
     */
    TBool IsLaunchParamsValid(      TInt aSettingsUid,
                              const TInt aParams);

private: // data
    /**
     * Boolean to indicate whether a Settings UI has been launched from this
     * session object or not.
     */
    TBool iSettingsUILaunched;
    };

#endif // C_LOCSETTINGSUISERVICE_H
