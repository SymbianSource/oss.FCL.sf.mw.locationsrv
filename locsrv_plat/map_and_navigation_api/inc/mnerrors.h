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
* Description:  Error constants for Map and Navigation FW
*
*/


#ifndef MN_ERRORS_H
#define MN_ERRORS_H

/** \file MnErrors.h Defines Map and Navigation APIs error constants 
 */

/** Client panic category
 *  Used to report client's irrecoverable error.
 */
_LIT(KMnPanicClientFault, "MnPanicClient");

/** Server panic category
 *  Used to report irrecoverable errors in service implementations.
 */
_LIT(KMnPanicServerFault, "MnPanicServer");

/** Panic codes */
enum TMnPanicCodes 
    {
    /** Raised if array index is out of bounds */
    KMnPanicArrayIndexOutOfBounds,

    /** Raised if client issues new asynchronous request 
     *  before previous one is completed or cancelled. */
    KMnPanicDuplicateRequest,

    /** Raised if CMnServiceBase-derived class attempts 
     *  to complete async request, when there is no outstanding one. */
    KMnPanicAttemptToCompleteNoRequest,
    
    /** Raised if CMnServiceBase-derived class 
     *  completes a request and provides invalid result parameters */
     KMnPanicInvalidResult
    };

#endif // MN_ERRORS_H
