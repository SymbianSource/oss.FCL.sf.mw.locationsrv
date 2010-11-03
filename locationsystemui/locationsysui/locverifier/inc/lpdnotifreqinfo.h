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
* Description:  This class stores information regarding notification 
*                requests.
*
*/


#ifndef CLPDNOTIFREQINFO_H
#define CLPDNOTIFREQINFO_H

#include <lbs/epos_privacy.h>

// Forward class declarations

class RPosRequestorStack;

/**
 *  Class to store information about Notification dialogs
 *
 *  If a notification query is already running, further requests for
 *  notification query will be ignored and the information will be lost.
 *  Instead the logic is to query these requests and show them to the
 *  user when the notification query is dismissed. This class is used to store
 *  information about each notification request.
 *
 *  @since S60 v3.0
 */
NONSHARABLE_CLASS(CLpdNotifReqInfo) : public CBase
    {

public:

    /**
     * Symbian 2 Phase Constructor
     * Ownership of the RequestorStack array is transferred to this class.
     *
     * @since S60 v3.0
     * @param aSource     The Source of the Notification Request.
     * @param aReason     The reason for cancellation request.
     * @param aDecision   The decision by the network.
     * @param aRequestors The array of requestors for this notification request.
     * @return new instance of class CLpdNotifReqInfo
     */
    static CLpdNotifReqInfo* NewL(
    	TPosRequestSource aSource,
    	TPosVerifyCancelReason aReason,
    	TPosRequestDecision aDecision,
    	RPosRequestorStack* aRequestors);

    /**
     * Symbian 2 Phase Constructor
     * Ownership of the RequestorStack array is transferred to this class.
     *
     * @since S60 v3.0
     * @param aSource     The Source of the Notification Request.
     * @param aReason     The reason for cancellation request.
     * @param aDecision   The decision by the network.
     * @param aRequestors The array of requestors for this notification request.
     * @return new instance of class CLpdNotifReqInfo
     */
    static CLpdNotifReqInfo* NewLC(
    	TPosRequestSource aSource,
    	TPosVerifyCancelReason aReason,
    	TPosRequestDecision aDecision,
    	RPosRequestorStack* aRequestors);

public:
    /**
     * Symbian virtual destructor
     */
    virtual ~CLpdNotifReqInfo();

    /**
     * Accessor Method
     *
     * @since S60 v3.0
     * @return The Source of the Notification Request.
     */
    const TPosRequestSource& Source();

    /**
     * Accessor Method
     *
     * @since S60 v3.0
     * @return The reason for cancellation request.
     */
    const TPosVerifyCancelReason& Reason();

    /**
     * Accessor Method
     *
     * @since S60 v3.0
     * @return The decision by the network.
     */
    const TPosRequestDecision& Decision();

    /**
     * Accessor Method
     *
     * @since S60 v3.0
     * @return The Array of Requestors.
     */
    const RPosRequestorStack& Requestors();

private:

    CLpdNotifReqInfo(
    	TPosRequestSource aSource,
    	TPosVerifyCancelReason aReason,
    	TPosRequestDecision aDecision,
    	RPosRequestorStack* aRequestors);

    void ConstructL();

private: // data

    /**
     * The Source of the Notification Request.
     */
    TPosRequestSource iSource;
    /**
     * The reason for cancellation request.
     */
    TPosVerifyCancelReason iReason;
    /**
     * The decision by the network.
     */
    TPosRequestDecision iDecision;
    /**
     * The array of requestors for this notification request.
     * Own.
     */
	RPosRequestorStack* iRequestors;
    };

#endif // CLPDNOTIFREQINFO_H
