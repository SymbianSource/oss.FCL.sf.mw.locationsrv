/*
 * Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:   Class for doing asynchronous service.
 *
 */



#include "epos_comasuplprotocolmanager1.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_csuplsettingparams.h"
#include "epos_omasuplconstants.h"
#include "epos_comasuplconnrequestor.h"
#include "epos_momasuplconnobserver.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplfallbackhandler.h"

#include "epos_comasupldialogtimer.h"
_LIT(KTraceFileName,"SUPL_OMA_SESSION::epos_comasuplconnrequestor.cpp");

const TInt KStringLength = 100;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::COMASuplConnRequestor
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplConnRequestor::COMASuplConnRequestor(
        CSuplCommunicationManager& aCommMgr,
        COMASUPLProtocolManager1& aProtoMgr, TUint aPort,
        MOMASuplConnObserver& aObserver) :
    CActive(EPriorityStandard), iCommMgr(aCommMgr), iProtocolManager(
            aProtoMgr),iConnection(NULL), iPort(aPort), iObserver(aObserver), iIapDialogShown(
            EFalse), iIapDlgTimerExpired(EFalse),
            iIsTimeoutDialogTimerStarted(EFalse)
    {

    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
//void COMASuplConnRequestor::ConstructL(const TDesC8& aHostAddr)
void COMASuplConnRequestor::ConstructL()
    {
    CActiveScheduler::Add( this );

    iTLSAuthenticationFailed = EFalse;
    iSuplSettings = CSuplSettingsInternal::NewL();
    iTrace = COMASuplTrace::NewL();
    iIsHSLPGenerated = EFalse;
    iHslpAddrFromImsiUsed = EFalse;

    iFallBackHandler = COMASuplFallBackHandler::NewL(*iSuplSettings);

    iLastConnectionError = KErrNone;

    iCurrentSLPId = KErrNotFound;

    iDialogTimer = COMASuplDialogTimer::NewL(*this);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//


COMASuplConnRequestor* COMASuplConnRequestor::NewL( CSuplCommunicationManager& aCommMgr,COMASUPLProtocolManager1& aProtoMgr,  
        TUint aPort,MOMASuplConnObserver& aObserver)

    {
    COMASuplConnRequestor* self =  new ( ELeave ) COMASuplConnRequestor(aCommMgr,aProtoMgr,aPort,
            aObserver);	    																	
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::COMASuplConnRequestor
// -----------------------------------------------------------------------------
//
COMASuplConnRequestor::~COMASuplConnRequestor()
    {
    Cancel();
    if(iDialogTimer)
        {
        iDialogTimer->Cancel();
        delete iDialogTimer;
        iDialogTimer = NULL;
        }
    delete iSuplSettings;
    delete iTrace;
    delete iFallBackHandler;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CreateConnectionL()
    {
	if(!iIsSettingInitilized)
        {
		iState = EInitialState;
        InitilizeSetting();
        }
    else
        {
		TBuf<100> iapName;
		TBuf<128> buffer;
		iState = EConnecting;  
		TBool isIapDialogShown = EFalse;
		TInt errorCode = iFallBackHandler->GetNextSLPAddressL(iCurrentSLPId,iHostAddress,iapName,iTls,iPskTls,iLastConnectionError,isIapDialogShown);
		
		if(errorCode == KErrNone)
			{
			TBool ret = ConvertIAPNameToIdL(iapName,iIAPId);
			if(!ret)
				{
					buffer.Copy(_L("No access point configured for "));
					buffer.Append(iHostAddress);
					iTrace->Trace(buffer,KTraceFileName, __LINE__); 				
					if( isIapDialogShown )
						{
						TInt err = iProtocolManager.LaunchSettingsUI(this,iHostAddress);
						if(err != KErrNone)
							{
							buffer.Copy(_L("Error in launching UI : "));
							buffer.AppendNum(err);
							iTrace->Trace(buffer,KTraceFileName, __LINE__);                 
							iHostAddress.Zero();
							iObserver.OperationCompleteL(err);
							}
							else
							iIapDialogShown = ETrue;
						}
					
					
				}
			else
	            {
	            iIsTimeoutDialogTimerStarted = EFalse;
	            iDialogTimer->Cancel();
	            buffer.Copy(_L("Connecting to "));
	            buffer.Append(iHostAddress);
	            iTrace->Trace(buffer,KTraceFileName, __LINE__); 				
	            iConnection = iCommMgr.CreateConnectionL(iHostAddress,iTls,iPskTls,iPort,iIAPId);
	            OpenConnection();
	            }	
			}
	    else
	        {
	        iHostAddress.Zero();
	        iObserver.OperationCompleteL(errorCode);
	        }
		}
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CreateConnectionL(TInt aDialogTimeOutDelay)
    {
    iIsTimeoutDialogTimerStarted = ETrue;

    iDialogTimer->StartTimer(aDialogTimeOutDelay); 
    CreateConnectionL();
    }
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
TInt COMASuplConnRequestor::GetLocalIPAddress(TInetAddr &aAddress)
    {
    if(iConnection)
        {
        return iConnection->GetLocalIPAddress(aAddress);	
        }

    return KErrGeneral;

    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SendPacket
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SendPacket(const TDesC8& aPacket)
    {
      if(iConnection)
    	{
        iState = ESending;
	    iConnection->Send(aPacket,iStatus);
		SetActive();
	  	}
	}
	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::OpenConnection()
    {

    if(iIsSettingInitilized)
        {
        if(iConnection)
            {
            iState = EConnecting;
            iConnection->Connect(iStatus);
            SetActive();
            }
        }
    else
        {
		iState = EInitilizeSetting;
        InitilizeSetting();
        }
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ListenToMessages
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::ListenToMessages()
    {
    if(iConnection)
        {
        iConnection->ListenToMessages();
        }
    }


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::CloseConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CloseConnection()
    {

		if(iConnection )
		    {
		    iConnection->CloseConnection();           
		iCommMgr.DestroyConnection(iConnection);
		    iConnection = NULL;
		    }
	}
		

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::RunL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::RunL()
    {
    iLastConnectionError = iStatus.Int();

    TBuf<64> buffer(_L("Error during connection : "));
    buffer.AppendNum(iLastConnectionError);
    iTrace->Trace(buffer,KTraceFileName, __LINE__); 					

    switch(iState)
        {
		
		case EInitialState:
            {
            iIsSettingInitilized = ETrue;
            iTrace->Trace(_L("Setting API Initilizing Completed..."), KTraceFileName, __LINE__);
            CreateConnectionL();
            
            break;
            }

        case  EConnecting:
            {
            if(iLastConnectionError < 0)
                {
                iHostAddress.Zero();
                CloseConnection();
                CreateConnectionL();
                }
            else
                {
                ListenToMessages();                            
                iObserver.OperationCompleteL(iLastConnectionError);
                }
            break;
            }
        case EInitilizeSetting:
            {
            if(iLastConnectionError == KErrNone)
                {
                iIsSettingInitilized = ETrue;
                iTrace->Trace(_L("Setting API Initilizing Completed..."), KTraceFileName, __LINE__); 				
                OpenConnection();
                }
            else
                {
                iTrace->Trace(_L("Setting API Initilizing Failed..."), KTraceFileName, __LINE__); 				
                iObserver.OperationCompleteL(iLastConnectionError);
                }	
            break;
            }

        case ESending:
            {
            iObserver.OperationCompleteL(iLastConnectionError);
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::RunError
// -----------------------------------------------------------------------------
//
TInt COMASuplConnRequestor::RunError(TInt /* aError */)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::DoCancel()
    {    
    switch(iState)
        {
        case EConnecting:
            if ( iConnection )
                iConnection->CancelConnect(iStatus);
            break;
        case ESending:
            if ( iConnection )	
                iConnection->CancelSend(iStatus);
            break;
        case EInitilizeSetting:
            iSuplSettings->CancelInitialize();
        default:
            break;
        } 
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::CancelReadOperation()
    {    
    if(iConnection)
        {
        iConnection->CancelListenToMessages();
        }
    }


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetIAPID
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SetIAPID(TInt aIAPID)
    {
    iIAPId = aIAPID;
    }

	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::InitilizeSetting
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::InitilizeSetting()
    {
    iTrace->Trace(_L("Intilizing Setting API..."), KTraceFileName, __LINE__); 				
    iSuplSettings->Initialize(iStatus);
    SetActive();
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::IsHslpGenerated
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::IsHslpGenerated(TBool aIsGenerated)
    {
    iIsHSLPGenerated = aIsGenerated;
    }	

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::IsHslpAddrFromImsiUsed
// -----------------------------------------------------------------------------
//
TBool COMASuplConnRequestor::IsHslpAddrFromImsiUsed()
    {
    return iHslpAddrFromImsiUsed;
    }	

TUint COMASuplConnRequestor::GetPortNumber()
    {
    if (iConnection)
        {
        return iConnection->GetPortNumberUsed();
        }
    else
        return 0;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetDefaultParametersL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SetDefaultParametersL(const TDesC& aClientProvidedHSLPAddress,TBool aFallBackAllowed)	
    {
    iFallBackHandler->SetFallBackParametersL(aClientProvidedHSLPAddress,aFallBackAllowed);	
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetDefaultParametersL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::GetUsedServerAddress(TDes& aHSLPAddressUsed)	
    {
    aHSLPAddressUsed.Copy(iHostAddress);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ConvertIAPNameToIdL
// 
// -----------------------------------------------------------------------------
TBool COMASuplConnRequestor::ConvertIAPNameToIdL(const TDesC& aIAPName, TUint32& aIAPId)
    {
    TBool result = EFalse;
    CCommsDatabase* commDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
    CleanupStack::PushL(commDb);
    CCommsDbTableView* tableView = commDb->OpenIAPTableViewMatchingBearerSetLC(ECommDbBearerCSD|ECommDbBearerGPRS,
            ECommDbConnectionDirectionOutgoing);
    TInt retval = tableView->GotoFirstRecord();
    while ((retval == KErrNone) && (!result))
        {
        HBufC * iap_name = tableView->ReadLongTextLC( TPtrC( COMMDB_NAME) );

	      iap_name->Des().LowerCase();
        if (iap_name && (iap_name->Compare(aIAPName) == 0))
            {
            tableView->ReadUintL(TPtrC(COMMDB_ID), aIAPId);
            result = ETrue;
            }
        CleanupStack::PopAndDestroy(); // iap_name
        retval = tableView->GotoNextRecord();
        }
    CleanupStack::PopAndDestroy(2);//delete tableView and commDb
    return result;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SettingsUICompleted
// 
// -----------------------------------------------------------------------------

void COMASuplConnRequestor::SettingsUICompletedL(TInt aError)
    {
		TBuf<128> buffer(_L("COMASuplConnRequestor:SettingsUICompleted Error: "));
    buffer.AppendNum(aError);
    iTrace->Trace(buffer,KTraceFileName, __LINE__); 

    if (iIsTimeoutDialogTimerStarted)
        {                
        iTrace->Trace(_L("COMASuplSession::SettingsUICompleted, stopping timer "), KTraceFileName, __LINE__);
        iIsTimeoutDialogTimerStarted = EFalse;                    
        iDialogTimer->StopTimer();
        }
    if (iIapDlgTimerExpired)
        {
        iIapDlgTimerExpired = EFalse;
        iIapDialogShown = EFalse;
        iProtocolManager.LaunchSuplDialogTimeoutUI(this);
        iObserver.OperationCompleteL(KErrNone);
        return;
        }
    if(aError == KErrNone)
        {
        TInt err = KErrGeneral;
        iTrace->Trace(
                _L("COMASuplConnRequestor::SettingsUICompletedL KErrNone"),
                KTraceFileName, __LINE__);
        TBuf<100> IapName, buffer;
        CServerParams* params = CServerParams::NewL();

        err = iSuplSettings->GetSlpInfoAddress(iHostAddress, params);
        iTrace->Trace(_L("iSuplSettings->GetSlpInfoAddress returned : "),
                KTraceFileName, __LINE__);
        buffer.AppendNum(err);
        iTrace->Trace(buffer, KTraceFileName, __LINE__);

        // Fix for ou1cimx#475026 and ou1cimx#471138
        // When there are no access points defined for any SUPL server, the user is asked to
        // select the access point only once and same access point is used for the following servers.
        if (err == KErrNone)
            {
            HBufC* aServerAddress = HBufC::NewL(KStringLength);
            HBufC* aIapName = HBufC::NewL(KStringLength);
            TInt64 aSlpId;
            TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;

            err = params->Get(aSlpId, aServerAddress->Des(), aIapName->Des(),
                    aServerEnabled, aSimChangeRemove, aUsageInHomeNw,
                    aEditable);

            iTrace->Trace(_L("params->Get() returned: "), KTraceFileName,
                    __LINE__);
            buffer.Zero();
            buffer.AppendNum(err);
            iTrace->Trace(buffer, KTraceFileName, __LINE__);
            IapName.Copy(*aIapName);
            IapName.LowerCase();

            //converting the iap name to id.
            ConvertIAPNameToIdL(IapName, iIAPId);
            //updating the list of SLP with the chosen access point
            iFallBackHandler->UpdateSLPListWithAccessPoint(IapName);

            delete aServerAddress;
            delete aIapName;
            delete params;

            // if the getting the list parameters is success opening the connection with the chosen access point
            if (err == KErrNone)
                {
                buffer.Copy(_L("Connecting to "));
                buffer.Append(iHostAddress);
                buffer.Append(_L(" using IAP "));
                buffer.Append(IapName);
                iTrace->Trace(buffer, KTraceFileName, __LINE__);
                iConnection = iCommMgr.CreateConnectionL(iHostAddress, iTls,
                        iPskTls, iPort, iIAPId);
                OpenConnection();
                }
            else
                {
                iObserver.OperationCompleteL(err);
                }
            }
        else
            {
            delete params;
            iObserver.OperationCompleteL(err);
            }
        }
    else
        {
        iObserver.OperationCompleteL(aError);
        }

    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SaveAccessPoint
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::SaveAccessPoint(const TDesC& aIapName)
    {
    TInt err = iSuplSettings->SetIapName(iCurrentSLPId,aIapName);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UpdateNetInfo
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UpdateNetInfo(const TTime& aLastTimeUsed,
        TInt& aMcc,
        TInt& aMnc,
        TInt& aCid, 
        TInt& aLac,
        TInt& aType,
        TInt& aErrorCode 
)
    {

    CSuplSettingsInternal::TSuplSettingsNetworkType type = (CSuplSettingsInternal::TSuplSettingsNetworkType)aType; 

    if(aErrorCode == KErrNone)
        {
        iSuplSettings->SetNetInfoLastSucess(iCurrentSLPId,type,aMcc,aMnc,aCid,aLac);
        iSuplSettings->SetLastUseTime(iCurrentSLPId,aLastTimeUsed);
        }
    else
        {
        iSuplSettings->SetNetInfoLastUse(iCurrentSLPId,type,aMcc,aMnc,aCid,aLac);
        iSuplSettings->SetLastTriedTime(iCurrentSLPId,aLastTimeUsed);
        }	

    }
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseDefaultServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UseDefaultServerL()
    {
    iFallBackHandler->ReadSLPListForNIL(KNullDesC);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UseServerL(const TDesC& aClientProvidedHSLPAddress)
    {
    iFallBackHandler->ReadSLPListForNIL(aClientProvidedHSLPAddress);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UpdateSLPListForHomeUsage(TBool aHomeNetwork)
    {
    iFallBackHandler->UpdateSLPListForHomeUsage(aHomeNetwork);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::DialogTimerExpiredL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::DialogTimerExpiredL()
    {
    iTrace->Trace(_L("COMASuplConnRequestor:Timer Expired for SUPL IAP Dialog"), KTraceFileName, __LINE__); 

    if (!iIapDialogShown)
        iProtocolManager.LaunchSuplDialogTimeoutUI(this);
    else
        iIapDlgTimerExpired = ETrue;  
    return; 
    }
//  End of File
