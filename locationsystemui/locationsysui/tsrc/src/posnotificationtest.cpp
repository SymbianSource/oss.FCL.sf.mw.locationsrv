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
 * Description: Implementation of the notification related test cases.
 *
 */
#include "posnotificationtest.h"
#include <lbs/epos_cposnetworkprivacy.h>
#include <EPos_CPosGSMPrivacyRequestInfo.h> 
#include <EPos_CPosSUPLPrivacyRequestInfo.h>
#include <EPos_CPosNetworkPrivacyRequestInfo.h> 
#include <QDebug>
// constants
_LIT( KTxtRequestorName, "TestNotificationRequestor" );
_LIT( KTxtLongRequestorName, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
_LIT( KTxtRequestorURL, "www.nokia.com" );
_LIT( KTxtRequestorPhone, "0501231231" );
_LIT( KTxtRequestorEmail, "abcd" );
_LIT( KTxtRequestorSIPURL, "James@Hill.se" );
_LIT(KTxtRequestorIMSPublicIdentity, "abcde123");
_LIT( KTxtRequestorIMS, "5665656656" );
_LIT( KTxtRequestorMIN, "9999999999" );
_LIT( KTxtRequestorMDN, "5665656656" );

const TInt KWaitPeriod1 = 20000000;
const TInt KWaitPeriod2 = 40000000;

//---------------------------------------------------------------------
// PosNotificationTest::HandleVerifyComplete()
// 
//---------------------------------------------------------------------

void PosNotificationTest::HandleVerifyComplete(TInt /*aRequestId*/,
        TInt /*aCompletionCode*/)
    {
    mEventLoop.exit();
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest1()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest1()
    {
    TRAPD(error,IssueNotificationRequest1L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest2()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest2()
    {
    TRAPD(error,IssueNotificationRequest2L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest3()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest3()
    {
    TRAPD(error,IssueNotificationRequest3L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest4()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest4()
    {
    TRAPD(error,IssueNotificationRequest4L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest5()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest5()
    {
    TRAPD(error,IssueNotificationRequest5L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest6()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest6()
    {
    TRAPD(error,IssueNotificationRequest6L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest7()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest7()
    {
    TRAPD(error,IssueNotificationRequest7L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest8()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest8()
    {
    TRAPD(error,IssueNotificationRequest8L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest9()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest9()
    {
    TRAPD(error,IssueNotificationRequest9L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest10()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest10()
    {
    TRAPD(error,IssueNotificationRequest10L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest11()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest11()
    {
    TRAPD(error,IssueNotificationRequest11L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest12()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest12()
    {
    TRAPD(error,IssueNotificationRequest12L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest13()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest13()
    {
    TRAPD(error,IssueNotificationRequest13L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest14()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest14()
    {
    TRAPD(error,IssueNotificationRequest14L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest15()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest15()
    {
    TRAPD(error,IssueNotificationRequest15L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest16()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest16()
    {
    TRAPD(error,IssueNotificationRequest16L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest17()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest17()
    {
    TRAPD(error,IssueNotificationRequest17L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest18()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest18()
    {
    TRAPD(error,IssueNotificationRequest18L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut1()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut1()
    {
    TRAPD(error,IssueNotifyVerificationTimeOut1L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut2()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut2()
    {
    TRAPD(error,IssueNotifyVerificationTimeOut2L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut3()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut3()
    {
    TRAPD(error,IssueNotifyVerificationTimeOut3L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut4()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut4()
    {
    TRAPD(error,IssueNotifyVerificationTimeOut4L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueMultipleRequests1()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueMultipleRequests1()
    {
    TRAPD(error,IssueMultipleRequests1L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueMultipleRequests2()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueMultipleRequests2()
    {
    TRAPD(error,IssueMultipleRequests2L());
    QVERIFY( error == KErrNone );
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest1L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest1L()
    {
    qDebug() << "IssueNotificationRequest1L - In";
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    qDebug() << "After call to NotifyLocationRequestL,error =" << error;
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest2L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest2L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be periodic
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestPeriodic);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest3L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest3L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by name
    info->SetLCSClientL(KTxtRequestorName,
            CPosGSMPrivacyRequestInfo::EIdTypeLogicalName);
    // set the requestor identified by name
    info->SetRequestorL(KTxtRequestorName,
            CPosGSMPrivacyRequestInfo::EIdTypeLogicalName);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest4L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest4L()
    {
    	
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by email
    info->SetLCSClientL(KTxtRequestorEmail,
            CPosGSMPrivacyRequestInfo::EIdTypeEmailAddress);
    // set the requestor identified by email
    info->SetRequestorL(KTxtRequestorEmail,
            CPosGSMPrivacyRequestInfo::EIdTypeEmailAddress);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    qDebug()<<"error code  is  "<<error << " in testcase4";
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest5L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest5L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by url
    info->SetLCSClientL(KTxtRequestorURL,
            CPosGSMPrivacyRequestInfo::EIdTypeURL);
    // set the requestor identified by url
    info->SetRequestorL(KTxtRequestorURL,
            CPosGSMPrivacyRequestInfo::EIdTypeURL);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest6L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest6L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by URL of user in a SIP session
    info->SetLCSClientL(KTxtRequestorSIPURL,
            CPosGSMPrivacyRequestInfo::EIdTypeSIPURL);
    // set the requestor identified by URL of user in a SIP session
    info->SetRequestorL(KTxtRequestorSIPURL,
            CPosGSMPrivacyRequestInfo::EIdTypeSIPURL);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest7L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest7L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by user Identifier in an IP Multimedia Service session.
    info->SetLCSClientL(KTxtRequestorIMSPublicIdentity,
            CPosGSMPrivacyRequestInfo::EIdTypeIMSPublicIdentity);
    // set the requestor identified by Mobile Identification Number
    info->SetRequestorL(KTxtRequestorIMSPublicIdentity,
            CPosGSMPrivacyRequestInfo::EIdTypeIMSPublicIdentity);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }
//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest8L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest8L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest9L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest9L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be periodic
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestPeriodic);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }

    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest10L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest10L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by Logical name
    info->SetLCSClientL(KTxtRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set the requestor identified by Logical name
    info->SetRequestorL(KTxtRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest11L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest11L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by email
    info->SetLCSClientL(KTxtRequestorEmail,
            CPosSUPLPrivacyRequestInfo::EIdTypeEmailAddress);
    // set the requestor identified by email
    info->SetRequestorL(KTxtRequestorEmail,
            CPosSUPLPrivacyRequestInfo::EIdTypeEmailAddress);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }

    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest12L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest12L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by url
    info->SetLCSClientL(KTxtRequestorURL,
            CPosSUPLPrivacyRequestInfo::EIdTypeURL);
    // set the requestor identified by url
    info->SetRequestorL(KTxtRequestorURL,
            CPosSUPLPrivacyRequestInfo::EIdTypeURL);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest13L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest13L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by URL of user in a SIP session
    info->SetLCSClientL(KTxtRequestorSIPURL,
            CPosSUPLPrivacyRequestInfo::EIdTypeSIPURL);
    // set the requestor identified by URL of user in a SIP session
    info->SetRequestorL(KTxtRequestorSIPURL,
            CPosSUPLPrivacyRequestInfo::EIdTypeSIPURL);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest14L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest14L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by Mobile Identification Number
    info->SetLCSClientL(KTxtRequestorMIN,
            CPosSUPLPrivacyRequestInfo::EIdTypeMIN);
    // set the requestor identified by Mobile Identification Number
    info->SetRequestorL(KTxtRequestorMIN,
            CPosSUPLPrivacyRequestInfo::EIdTypeMIN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest15L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest15L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by Mobile Directory Number
    info->SetLCSClientL(KTxtRequestorMDN,
            CPosSUPLPrivacyRequestInfo::EIdTypeMDN);
    // set the requestor identified by Mobile Directory Number
    info->SetRequestorL(KTxtRequestorMDN,
            CPosSUPLPrivacyRequestInfo::EIdTypeMDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest16L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest16L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo* info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by user Identifier in an IP Multimedia Service session.
    info->SetLCSClientL(KTxtRequestorIMSPublicIdentity,
            CPosSUPLPrivacyRequestInfo::EIdTypeIMSPublicIdentity);
    // set the requestor identified by Mobile Identification Number
    info->SetRequestorL(KTxtRequestorIMSPublicIdentity,
            CPosSUPLPrivacyRequestInfo::EIdTypeIMSPublicIdentity);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest17L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest17L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotificationRequest18L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotificationRequest18L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtLongRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtLongRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut1L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut1L()
    {
    qDebug() << "IssueNotifyVerificationTimeOut1L - IN";
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = 123;
    // create an instance of CPosNetworkPrivacy used to issue the notify verifiaiton timeout
    // with default decision as accepted
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    // issue a verify location request
  //  TRAPD( err,privacy->VerifyLocationRequestL(*info,requestId,*this,CPosNetworkPrivacy::EDecisionAccepted));
    
    TRAPD(error,privacy->NotifyVerificationTimeoutL(*info, requestId,CPosNetworkPrivacy::EDecisionAccepted));

 //   if( err == KErrNone )
 //       {
 //       mEventLoop.exec();
 //       }
    qDebug() << " After NotifyVerificationTimeoutL error = " << error;
    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut2L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut2L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = 123;
    // create an instance of CPosNetworkPrivacy used to issue the notify verifiaiton timeout
    // with default decision as rejected
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    // issue a verify location request
  //  TRAPD( err,privacy->VerifyLocationRequestL(*info,requestId,*this,CPosNetworkPrivacy::EDecisionRejected));

    TRAPD(error,privacy->NotifyVerificationTimeoutL(*info, requestId,CPosNetworkPrivacy::EDecisionRejected));

  //  if( err == KErrNone )
  //      {
  //      mEventLoop.exec();
  //      }

    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut3L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut3L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo* info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = 123 ;
    // create an instance of CPosNetworkPrivacy used to issue the notify verifiaiton timeout
    // with default decision as accepted
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    // issue a verify location request
 //   TRAPD( err,privacy->VerifyLocationRequestL(*info,requestId,*this,CPosNetworkPrivacy::EDecisionAccepted));

    TRAPD(error,privacy->NotifyVerificationTimeoutL(*info, requestId,CPosNetworkPrivacy::EDecisionAccepted));
  //  if( err == KErrNone )
   //     {
   //     mEventLoop.exec();
   //     }

    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueNotifyVerificationTimeOut4L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueNotifyVerificationTimeOut4L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo* info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = 123;
    // create an instance of CPosNetworkPrivacy used to issue the notify verifiaiton timeout
    // with default decision as rejected
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    // issue a verify location request
  //  TRAPD( err,privacy->VerifyLocationRequestL(*info,requestId,*this,CPosNetworkPrivacy::EDecisionRejected));

    TRAPD(error,privacy->NotifyVerificationTimeoutL(*info, requestId,CPosNetworkPrivacy::EDecisionRejected));
 //   if( err == KErrNone )
 //       {
 //       mEventLoop.exec();
 //       }

    if (error == KErrNone)
        {
        if (requestId < 0)
            {
            User::Leave(KErrGeneral);
            }
        }
    else
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueMultipleRequests1L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueMultipleRequests1L()
    {
    TInt result = KErrNone;
    // Create a new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosSUPLPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    // Create another new CPosSUPLPrivacyRequestInfo object    
    CPosSUPLPrivacyRequestInfo * info1 = CPosSUPLPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info1->SetLCSClientL(KTxtRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set the requestor identified by phone number
    info1->SetRequestorL(KTxtRequestorName,
            CPosSUPLPrivacyRequestInfo::EIdTypeLogicalName);
    // set request type to be single shot
    info1->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    if (error == KErrNone)
        {
        if (requestId > 0)
            {
            // issue the same request again
            TRAPD(error1,privacy->NotifyLocationRequestL(*info1, requestId));
            if (error1 == KErrNone)
                {
                if (requestId > 0)
                    {
                    result = KErrNone;
                    }
                else
                    {
                    result = KErrGeneral;
                    }
                }
            else
                {
                result = error1;
                }
            }
        else
            {
            result = KErrGeneral;
            }
        }
    else
        {
        result = error;
        }
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info1);
    CleanupStack::PopAndDestroy(info);
    User::Leave(result);
    }

//---------------------------------------------------------------------
// PosNotificationTest::IssueMultipleRequests2L()
// 
//---------------------------------------------------------------------
void PosNotificationTest::IssueMultipleRequests2L()
    {
    TInt result = KErrNone;
    // Create a new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info->SetLCSClientL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set the requestor identified by phone number
    info->SetRequestorL(KTxtRequestorPhone,
            CPosGSMPrivacyRequestInfo::EIdTypeMSISDN);
    // set request type to be single shot
    info->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    // Create another new CPosGSMPrivacyRequestInfo object    
    CPosGSMPrivacyRequestInfo* info1 = CPosGSMPrivacyRequestInfo::NewLC();
    // Set Lcs client identified by phone number
    info1->SetLCSClientL(KTxtRequestorName,
            CPosGSMPrivacyRequestInfo::EIdTypeLogicalName);
    // set the requestor identified by phone number
    info1->SetRequestorL(KTxtRequestorName,
            CPosGSMPrivacyRequestInfo::EIdTypeLogicalName);
    // set request type to be single shot
    info1->SetRequestType(CPosNetworkPrivacyRequestInfo::ERequestSingleShot);

    TInt requestId = -1;
    TInt requestId1 = -1;
    // create an instance of CPosNetworkPrivacy used to issue the notification request
    CPosNetworkPrivacy* privacy = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy);
    // create another instance of CPosNetworkPrivacy used to issue the second notification request
    CPosNetworkPrivacy* privacy1 = CPosNetworkPrivacy::NewL();
    CleanupStack::PushL(privacy1);
    TRAPD(error,privacy->NotifyLocationRequestL(*info, requestId));
    TRAPD(error1,privacy1->NotifyLocationRequestL(*info1, requestId1));

    if (error == KErrNone && error1 == KErrNone)
        {
        if (requestId > 0 && requestId1 > 0)
            {
            result = KErrNone;
            }
        else
            {
            result = KErrGeneral;
            }
        }
    else
        {
        result = KErrGeneral;
        }
    CleanupStack::PopAndDestroy(privacy1);
    CleanupStack::PopAndDestroy(privacy);
    CleanupStack::PopAndDestroy(info1);
    CleanupStack::PopAndDestroy(info);
    User::Leave(result);
    }

QTEST_MAIN_S60(PosNotificationTest)

