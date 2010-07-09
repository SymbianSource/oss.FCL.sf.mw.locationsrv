/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description: Declaration of the notification related test cases.
 *
 */

#ifndef POSNOTIFICATIONTEST_H_
#define POSNOTIFICATIONTEST_H_

#include <QObject>
#include <QtTest/QtTest>
#include <EPos_MPosVerificationObserver.h>
#include "testconsole.h"


class PosNotificationTest : public QObject, public MPosVerificationObserver
    {
Q_OBJECT

public:
    /**
     * From MPosVerificationObserver
     */
    void HandleVerifyComplete(int aRequestId, TInt aCompletionCode);

private slots:
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request.
     */

    void IssueNotificationRequest1();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with periodic request.
     */
    void IssueNotificationRequest2();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Logical name"
     */
    void IssueNotificationRequest3();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Email address."
     */
  
    void IssueNotificationRequest4();
        
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "URL"
     */
    void IssueNotificationRequest5();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "URL of user in a SIP session"
     */
    void IssueNotificationRequest6();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "user Identifier in an IP Multimedia Service session"
     */
    void IssueNotificationRequest7();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request.
     */
    void IssueNotificationRequest8();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with periodic request.
     */
    void IssueNotificationRequest9();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Logical name"
     */
    void IssueNotificationRequest10();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Email address."
     */
    void IssueNotificationRequest11();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with ,single shot request & the LCS client & requestor identified by "URL"
     */
    void IssueNotificationRequest12();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "URL of user in a SIP session"
     */
    void IssueNotificationRequest13();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "Mobile Identification Number"
     */
    void IssueNotificationRequest14();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "Mobile Directory Number"
     */
    void IssueNotificationRequest15();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "user Identifier in an IP Multimedia Service session"
     */
    void IssueNotificationRequest16();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request & empty string for LCS Client & requestor name.
     */
    void IssueNotificationRequest17();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request & a long string value for LCS Client & requestor name.
     */
    void IssueNotificationRequest18();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with default accept policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut1();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with default reject policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut2();

    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with default accept policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut3();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with default reject policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut4();
    /**
     * To test issuing notificaiton request multiple times using 
     * CPosSUPLPrivacyRequestInfo with single shot with same instance of CPosNetworkPrivacy.
     */
    void IssueMultipleRequests1();
    /**
     * To test issuing notificaiton request multiple times using 
     * CPosGSMPrivacyRequestInfo with single shot using 2 different instances of CPosNetworkPrivacy.
     */
    void IssueMultipleRequests2();


private:
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request.
     */
    void IssueNotificationRequest1L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with periodic request.
     */
    void IssueNotificationRequest2L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Logical name"
     */
    void IssueNotificationRequest3L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Email address."
     */
    void IssueNotificationRequest4L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "URL"
     */
    void IssueNotificationRequest5L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "URL of user in a SIP session"
     */
    void IssueNotificationRequest6L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "user Identifier in an IP Multimedia Service session"
     */
    void IssueNotificationRequest7L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request.
     */
    void IssueNotificationRequest8L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with periodic request.
     */
    void IssueNotificationRequest9L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Logical name"
     */
    void IssueNotificationRequest10L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by "Email address."
     */
    void IssueNotificationRequest11L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with ,single shot request & the LCS client & requestor identified by "URL"
     */
    void IssueNotificationRequest12L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "URL of user in a SIP session"
     */
    void IssueNotificationRequest13L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "Mobile Identification Number"
     */
    void IssueNotificationRequest14L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "Mobile Directory Number"
     */
    void IssueNotificationRequest15L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with single shot request & the LCS client & requestor identified by 
     * "user Identifier in an IP Multimedia Service session"
     */
    void IssueNotificationRequest16L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request & empty string for LCS Client & requestor name.
     */
    void IssueNotificationRequest17L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo  
     * with single shot request & a long string value for LCS Client & requestor name.
     */
    void IssueNotificationRequest18L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with default accept policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut1L();
    /**
     * To Test issuing notification request using CPosGSMPrivacyRequestInfo 
     * with default reject policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut2L();

    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with default accept policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut3L();
    /**
     * To Test issuing notification request using CPosSUPLPrivacyRequestInfo 
     * with default reject policy,single shot request.
     */
    void IssueNotifyVerificationTimeOut4L();
    /**
     * To test issuing notificaiton request multiple times using 
     * CPosSUPLPrivacyRequestInfo with single shot with same instance of CPosNetworkPrivacy.
     */
    void IssueMultipleRequests1L();
    /**
     * To test issuing notificaiton request multiple times using 
     * CPosGSMPrivacyRequestInfo with single shot using 2 different instances of CPosNetworkPrivacy.
     */
    void IssueMultipleRequests2L();
      
private:
    /**
     * Event loop
     */
    QEventLoop mEventLoop;

    };

#endif // POSNOTIFICATIONTEST_H_
