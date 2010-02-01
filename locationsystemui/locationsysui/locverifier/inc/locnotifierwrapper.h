/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the declarations for the Notifier Proxy.
*
*/


#include <AknNotifierWrapper.h> // link against aknnotifierwrapper.lib

#ifndef LOCNOTIFIERWRAPPER_H 
#define LOCNOTIFIERWRAPPER_H


/**
 *  The class to provide custom security check.
 *
 *  This class provides a custom security check that will be called whenever
 *  a Location notifier message is received by the UIKON Server.
 *
 *  @lib locnotifierwrapper.lib
 *  @since S60 v3.1
 */
 class CLocNotifierSecurityCheck : public CBase, 
                                   public MAknNotifierCustomSecurityCheck
    {
    public: // from base class MAknNotifierCustomSecurityCheck
    /**
     * From MAknNotifierCustomSecurityCheck.
     * This method will be called by the framework classes to free up all 
     * resources that have been allocated while doing the security check.
     *
     * @since S60 v3.1
     */
    void Release();
    
    /**
     * From MAknNotifierCustomSecurityCheck.
     * This is the method that performs the Security check.
     * It is called from the framework classes.
     * This method Leaves with KErrPermissionDenied to indicate a check fail
     *
     * @since S60 v3.1
     * @param aMessage Pointer the Notification request message.
     */
    void CustomSecurityCheckL(const RMessagePtr2& aMessage);
    };

 
#endif //LOCNOTIFIERWRAPPER

