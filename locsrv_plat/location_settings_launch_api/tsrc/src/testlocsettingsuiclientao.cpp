/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Test Module DLL to Location Settings Launch API
*
*/



// System Includes
#include <locsettingsuiclient.h>
#include <w32std.h>
#include <coemain.h>
#include <eikon.hrh>

// User Includes
#include "testlocsettingsuiclientao.h"


// Constant Declarations

    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestLocSettingsUiClientAO* CTestLocSettingsUiClientAO::NewL(CStifLogger* aLog)
    {
    CTestLocSettingsUiClientAO* self = new(ELeave) CTestLocSettingsUiClientAO(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
void CTestLocSettingsUiClientAO::ConstructL()
    {
    iLocSettingsUiClient = CLocSettingsUiClient::NewL();
    }
    
// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestLocSettingsUiClientAO::CTestLocSettingsUiClientAO(CStifLogger* aLog)
    :CActive(0),iLog(aLog),iErr(0)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CTestLocSettingsUiClientAO::~CTestLocSettingsUiClientAO()
    {
    if( IsActive() )
	    {
	    Cancel(); 	
	    }
	    
    if(iLocSettingsUiClient)
    	{
    	delete iLocSettingsUiClient;	
    	iLocSettingsUiClient = NULL;
    	}
    }

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::RunL 
//
// RunL to Handle Completed Request
// ---------------------------------------------------------
//	
void CTestLocSettingsUiClientAO::RunL()
	{
	switch(iState)
		{		
		case EStart:
			switch(iFunction)
				{
				case ELaunchPosSettingsL:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchPosSettingsL(iStatus);
					SetActive();
				break;				
				case ELaunchPosSettingsAsEmbeddedAppL:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchPosSettingsAsEmbeddedAppL(iStatus);
					SetActive();
				case ELaunchSettingsUiL:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchSettingsUiL(iTUid,iParam,iStatus);
					SetActive();
				break;				
				case ELaunchSettingsUiAsEmbeddedAppL:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchSettingsUiAsEmbeddedAppL(iTUid,iParam,iStatus);
					SetActive();
					break;
					
				case ELaunchSettingsUiLOverLoaded:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchSettingsUiL(iTUid,iParamString,iStatus);
					SetActive();
					break;
					
				case ELaunchSettingsUiAsEmbeddedAppLOverLoaded:
					iState = ECompleted;
					iLocSettingsUiClient->LaunchSettingsUiAsEmbeddedAppL(iTUid,iParamString,iStatus);
					SetActive();
					break;					
				}
		break;	
		case ECompleted:
			{

			iLog->Log(_L("RunL %d"),iStatus.Int());		
			CActiveScheduler::Stop();			
			}
		break;
		}
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::DoCancel 
//
// To Handle Cancel Request
// ---------------------------------------------------------
//	
void CTestLocSettingsUiClientAO::DoCancel()
	{
//	TInt err = iPosServer.CancelRequest(EPositionServerNotifyModuleStatusEvent);
//	iLog->Log(_L("CancelL %d"),err);

	}

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::RunError 
//
// To Handle Error in RunL
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::RunError(TInt aError)
	{
	iErr = aError;
	iLog->Log(_L("Leave in RunError %d"),aError);
	CActiveScheduler::Stop();
	return KErrNone;
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestNewL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestNewL()
	{
	CLocSettingsUiClient* aLocSettingsUiClient = CLocSettingsUiClient::NewL();
    if(aLocSettingsUiClient)
    	{
    	delete aLocSettingsUiClient;	
    	aLocSettingsUiClient = NULL;
    	}
	return KErrNone;
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestNewLC
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestNewLC()
	{
	CLocSettingsUiClient* aLocSettingsUiClient = CLocSettingsUiClient::NewLC();
	CleanupStack::PopAndDestroy(aLocSettingsUiClient);
	return KErrNone;
	}
		
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestLaunchSettingsL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestLaunchSettingsL(CStifItemParser& aItem)
	{
	TInt choice;
	User::LeaveIfError(aItem.GetNextInt(choice));
	switch(choice)
		{
		case 0:
			IssueRequestL(ELaunchPosSettingsL);
		break;
		case 1:
			IssueRequestL(ELaunchPosSettingsAsEmbeddedAppL);
		break;
		case 2:
			TUint UidInInt;
			TInt param;
			if(aItem.GetNextInt(UidInInt, EHex) != KErrNotFound && 
				aItem.GetNextInt(param) != KErrNotFound )
				{
				iTUid.iUid = UidInInt;
				iParam = param;
				IssueRequestL(ELaunchSettingsUiL);
				}
			else
				{
				return KErrGeneral;
				}
		break;
		case 3:
			TUint UidInInt1;
			TInt param1;
			if(aItem.GetNextInt(UidInInt1, EHex) != KErrNotFound && 
				aItem.GetNextInt(param1) != KErrNotFound )
				{
				iTUid.iUid = UidInInt1;
				iParam = param1;
				IssueRequestL(ELaunchSettingsUiAsEmbeddedAppL);
				}
			else
				{
				return KErrGeneral;
				}
			break;

		case 4:
			TUint UidInInt2;
			TPtrC param2;
			if(aItem.GetNextInt(UidInInt2, EHex) != KErrNotFound && 
				aItem.GetNextString(param2) != KErrNotFound )
				{
				iTUid.iUid = UidInInt2;
				iParamString.Copy(param2);
				IssueRequestL(ELaunchSettingsUiLOverLoaded);
				}
			else
				{
				return KErrGeneral;
				}
			break;
			
		case 5:
			TUint UidInInt3;
			TPtrC param3;
			if(aItem.GetNextInt(UidInInt3, EHex) != KErrNotFound && 
				aItem.GetNextString(param3) != KErrNotFound )
				{
				iTUid.iUid = UidInInt3;
				iParamString.Copy(param3);
				IssueRequestL(ELaunchSettingsUiAsEmbeddedAppLOverLoaded);
				}
			else
				{
				return KErrGeneral;
				}
			break;
			
		default:
			return KErrGeneral;
		}
	CActiveScheduler::Start();

	if(iErr != KErrNone)
		{
		iLog->Log(_L("TestLaunchPosSettingsAsEmbeddedAppL - %d"),iErr);	
		}
	
	return iErr;
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::IssueRequestL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
void CTestLocSettingsUiClientAO::IssueRequestL(TFunction aFunction)
	{
	SetActive();
	iFunction = aFunction;
	iState = EStart;
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);	
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUi
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUi(CStifItemParser& aItem)
	{
	TUint UidInInt1;
	TInt param1;
	User::LeaveIfError(aItem.GetNextInt(UidInInt1, EHex));
	User::LeaveIfError(aItem.GetNextInt(param1));
	iTUid.iUid = UidInInt1;
	iParam = param1;
	
	iLocSettingsUiClient->LaunchSettingsUiAsEmbeddedAppL(iTUid,iParam,iStatus);
	TInt err = iLocSettingsUiClient->CancelLaunchedSettingsUi();
	return err;	
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUi
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestCancelLaunchedSettingsUiErr()
	{
	TInt err = iLocSettingsUiClient->CancelLaunchedSettingsUi();
	return err;	
	}

// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestClosePosSettings
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestClosePosSettings()
	{
	iLocSettingsUiClient->LaunchPosSettingsL(iStatus);
	SetActive();
	TInt err = iLocSettingsUiClient->ClosePosSettings();
	return err;	
	}
	
// ---------------------------------------------------------
// CTestLocSettingsUiClientAO::TestClosePosSettingsErr
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestLocSettingsUiClientAO::TestClosePosSettingsErr()
	{
	TInt err = iLocSettingsUiClient->ClosePosSettings();
	return err;	
	}
	
