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
* Description:   Implementation of Session Object in the OMA SUPL Protocol Handler
*
*/


 


#include <barsc2.h>
#include <barsread2.h>
#include <bautils.h>
#include <networking/dnd_err.h>

#include <e32def.h>
#include <etelmm.h> 
#include <centralrepository.h>
#include <CoreApplicationUIsSDKCRKeys.h> // for network mode (offline)


#include <lbspositioninfo.h>
#include <lbsfieldids.h>

#include "epos_suplterminal.h"
#include "epos_suplterminalconstants.h"
#include "epos_suplterminalqop.h"

#include "epos_comasuplnotification.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_csuplconnection.h"
#include "lbs/epos_comasuplpossessionbase.h"
#include "lbs/epos_comasuplposhandlerbase.h"
#include "lbs/epos_comasuplinforequestlist.h"
#include "epos_comasupllocationid.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplresponse.h"
#include "epos_comasuplpos.h"
#include "epos_comasuplstart.h"
#include "epos_comasuplposinit.h"
#include "epos_comasuplauthresponse.h"
#include "epos_comasuplauthrequest.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_comasuplvelocity.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplinit.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"

#include "epos_comasuplprotocolmanager1.h"
#include "epos_comasuplsettings.h"
#include "epos_comasupletelnotifier.h"
#include "epos_comasuplsession.h"

#include "epos_comasuplconnrequestor.h"
#include "epos_comasuplstate.h"
#include "epos_comasuplstartstate.h"
#include "epos_comasuplposinitstate.h"
#include "epos_comasuplresponsestate.h"
#include "epos_comasuplendstate.h"
#include "epos_comasuplposstate.h"
#include "epos_comasuplinitstate.h"
#include "epos_comasuplposrequestor.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"
#include "epos_resourceutils.h"
#include "epos_comasupltimeouttimer.h"
#include "epos_comasupldialogtimer.h"
#include "epos_tomasuplposmethod.h"
#include "epos_omasuplconfigurationkeys.h"
#include "epos_csuplsettingparams.h"
#include "epos_comasuplasnbase.h"
#include "epos_csuplsettingsconstants.h"


_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplSession.cpp");

//Multiplying factor for conversion of ellipsoid to circle
//formula used sqrt(-2*natural log(1-Confidence))
// actual value = 1.0107676525947896431381113653917 

//const TReal	MultiplyFactorOneSigma = 1.01076765; 

// ============================ MEMBER FUNCTIONS ===============================

//Constructor
COMASuplSession::COMASuplSession(RMobilePhone& aMobilePhone ,
																TOMASuplReqType aRequestType,
																COMASuplPosHandlerBase *aPosHandler,
																MOMASuplSessionObserver& aSessionObserver,
																COMASuplSettings*& aSettings,
																COMASUPLProtocolManager1 &aProtoMgr,
																TInt aIpcSessionId,
																COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase):
																
																iMobilePhone(aMobilePhone),
																iRequestType(aRequestType),
																iPosHandler(aPosHandler),	
																iHSLPAddress(aSettings->SLPAddress()),
																iEncodedSuplInit(NULL),
																iSessionObserver(aSessionObserver),
																iSuplSettings(aSettings),
																iProtocolManager(aProtoMgr),
																iUIFlag(EFalse),
																iSessionIDFlag(EFalse),
																iRoaming(EFalse),
																iChanged(EFalse),
																iIpcSessionId(aIpcSessionId),
																iNwInitError(EFalse),
																iPersistFail(EFalse),
																iCapsFail(EFalse),
																iUsageDialog(EFalse),
																iUsageHomeNW(EFalse),
																iEtelNotify(EFalse),
																iEtelRoamingCheck(EFalse),
																iIapDialogShown(EFalse),
																iIapDlgTimerExpired(EFalse),
																iOMASuplAsnHandlerBaseImpl(aOMASuplAsnHandlerBase),iWlanOnly(EFalse)
																
    { 
    }
 
//2 nd Phase construction
 void COMASuplSession::ConstructL(CSuplCommunicationManager &aCommManager,
			                      CSuplSettingsInternal* aSettingsStorage,
 								  TDes& aIMSI)
    {
    	iTrace = COMASuplTrace::NewL();	
    	
    	iTrace->Trace(_L("COMASuplSession::ConstructL"), KTraceFileName, __LINE__); 
  
    		
			iConnRequestor = COMASuplConnRequestor::NewL(aCommManager,iProtocolManager,KOMASuplHSLPPort,*this);
			TBuf<64> msg;
			msg.Copy(_L("Port Number is :"));
			msg.AppendNum(KOMASuplHSLPPort);
			iTrace->Trace(msg, KTraceFileName, __LINE__); 
  
    	
    	iSuplStorageSettings = aSettingsStorage;
		
    	iSuplSessionId = COMASuplSessionID::NewL();
    	iCompleteSelfRequestor = COMASuplCompleteSelfRequestor::NewL(*this);
    	
    	iSETCapabilities = COMASuplSETCapabilities::NewL();
    	
    	//All caps are on...for NET initiated case
    	iAllowedCapabilities = KGpsSETAssisted | KGpsSETBased | KAutonomousGps| KAFLT | KECID | KEOTD | KOTDOA | KCID; 

    	//Ownership transfer to iSuplSessionId
		COMASuplSETSessionID* SETSessionId  = COMASuplSETSessionID::NewL();
		
		iSuplSessionId->SetSETSessionID(SETSessionId);
		
		//Ownership transfer to iSuplSessionId
		iSuplSessionId->SetSLPSessionID(NULL); 	
		
		iSuplVersion.SetSuplVersion(KSuplMajorVersion,KSuplMinorVersion,KSuplRevision);
		
		if( aIMSI.Length() )
		    {
		    iIMSI.Create( aIMSI );    
		    }
		
		if(iPosHandler)
			{
				iTrace->Trace(_L("Creating POSSession..."), KTraceFileName, __LINE__); 
				iPOSSession = iPosHandler->CreateNewSessionL(this);
				//Don't move this statement else where
				iOMASuplPOSRequestor = COMASuplPOSRequestor::NewL(this,iPOSSession);
			}
		else
			{
				iTrace->Trace(_L("No POS plugin exists..."), KTraceFileName, __LINE__); 				
			}
			
			
		iTimer = COMASuplTimeoutTimer::NewL(*this);


    	iIapNotifier = COMASuplIapNotifier::NewL(*this);

		
		iIsQoPPresent = EFalse;	
		iIsFirstPOSMessage = ETrue;
		isTimeoutDialogTimerStarted = EFalse;

        iDiffTime = 0;
        iSessionCompletionCode = KErrCompletion;
        
        iNetworkPrivacy 	= CPosNetworkPrivacy::NewL();
        
		iTrace->Trace(_L("End of COMASuplSession::ConstructL"), KTraceFileName, __LINE__); 
    }
    
//
// -----------------------------------------------------------------------------
// COMASuplSession::NewL
// Creates new instance of COMASuplSession 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 COMASuplSession* COMASuplSession::NewL(CSuplCommunicationManager& aCommManager,
																				RMobilePhone& aMobilePhone,
																				COMASuplSettings*& aSettings,
																				CSuplSettingsInternal* aSettingsStorage,
																				TOMASuplReqType  aRequestType,
																				MOMASuplSessionObserver& aSessionObserver,
																				COMASuplPosHandlerBase *aPosHandler,
																				TDes& aIMSI,
																				COMASUPLProtocolManager1 &aProtoMgr,
																				TInt aIpcSessionId,
																				COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase)
    { 	
	    COMASuplSession* self = new( ELeave ) 
	                COMASuplSession( aMobilePhone,aRequestType, aPosHandler, 
	                                 aSessionObserver, aSettings, aProtoMgr, aIpcSessionId,aOMASuplAsnHandlerBase);
	    CleanupStack::PushL( self );
	    self->ConstructL( aCommManager, aSettingsStorage, aIMSI );
	    CleanupStack::Pop(self);
	    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSession::DestroySession
// This method will delete this session..since only protocol manager will delete session object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::DestroySession()
	{
		iTrace->Trace(_L("COMASuplSession::DestroySession"), KTraceFileName, __LINE__); 
		
    	//This is for sending SUPL_END if session is destroed while RunSession is going on.
		if(iRequestType == ESUPL_TERMINAL && iSuplSessionState!= ESUPL_TERMINATED)
		{
			CancelRunSession();
			SessionEnd();
			CompleteSession(KErrCancel);
		}
		delete this;
	}

// -----------------------------------------------------------------------------
// COMASuplSession::PosPlugInUnloaded
// Handles Pos handler unload
// -----------------------------------------------------------------------------
void COMASuplSession::PosPlugInUnloaded() 
	{
	iTrace->Trace(_L("COMASuplSession::PosPlugInUnloaded"), KTraceFileName, __LINE__); 															
	
	// Cancel the Session
	CancelSession();

	// Delete the POS Requestor
	delete iOMASuplPOSRequestor;
	iOMASuplPOSRequestor = NULL;
	
	// Delete the POS Session
	delete iPOSSession;
	iPOSSession = NULL;
	
	if(iSuplSessionState == ESUPL_SEND)
	    {
		CompleteSession(KErrCompletion);
	    }
	else
	    {
		//Close Connection : bug Fix.
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		iSessionObserver.TerminateSession(this, KErrCompletion);	
	    }
	}

	
//Desrtuctor    
 COMASuplSession::~COMASuplSession()
    {
    	if(iTrace)
    		iTrace->Trace(_L("Start ~COMASuplSession::COMASuplSession"), KTraceFileName, __LINE__); 
    	
		CleanUp();
		
    	delete iEncodedBuffer;
    	delete iSuplState;
		delete iOMASuplPOSRequestor;
    	delete iConnRequestor;
    	delete iSuplSessionId; 
    	delete iCompleteSelfRequestor;
    	delete iDecodedAsnMessage;
    	delete iSETCapabilities;
    	delete iPosition;
    	delete iEncodedSuplInit;
    	delete iPOSSession;
    	iPOSSession = NULL;
    	delete iTimer;
        delete iIapNotifier;
    	iIMSI.Close();
        
        delete iNetworkPrivacy;

	if(iStaleLocationId)
    	  {
    	  delete iStaleLocationId;
    	  iStaleLocationId = NULL;
    	  }  
    	
    	if(iTrace)
    		iTrace->Trace(_L("End of ~COMASuplSession::COMASuplSession"), KTraceFileName, __LINE__); 
		    	
    	delete iTrace;
    }

// -----------------------------------------------------------------------------
// COMASuplSession::Initialize
// This method initializes the OMA SUPL Session Object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::InitializeL(TInt /*aRequestID*/)
    {
    	iSuplSessionState = ESUPL_INITIALIZED;

		if(iPOSSession)
		{
			iTrace->Trace(_L("COMASuplSession::InitializeL of POS"), KTraceFileName, __LINE__); 
			iOMASuplPOSRequestor->SetObserver(this);
			TInt err = iOMASuplPOSRequestor->InitilizePOSSessionL(iRequestID);
			if(err != KErrNone)
				{
					iTrace->Trace(_L("Initilization Failed ..."), KTraceFileName, __LINE__); 
					CompleteSession(KErrNotReady);
				}
		}
		else
		{
			OperationCompleteL(KErrNone);	
		}
    }

// -----------------------------------------------------------------------------
// COMASuplSession::CleanUp
// Perfomrs clean up operations on the OMA SUPL Session Object
// This releases any resource held by the object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::CleanUp()
    {
    	if(iTrace)
		iTrace->Trace(_L("COMASuplSession::CleanUp"), KTraceFileName, __LINE__); 
		
		if(iSuplState)
			{
				if(iSuplSessionState != ESUPL_SESSION_ERROR)
					{
						iSuplState->CancelOperation();
					}
			}
			 
		iConnRequestor->Cancel();
		
		delete iEncodedBuffer;
		iEncodedBuffer = NULL;
		
		delete iDecodedAsnMessage; 
		iDecodedAsnMessage = NULL;
		
		if(iTrace)
		iTrace->Trace(_L("End of COMASuplSession::CleanUp"), KTraceFileName, __LINE__); 
    }


// -----------------------------------------------------------------------------
// COMASuplSession::RequestType
// Returns the type of request associated with the session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSession::TOMASuplReqType COMASuplSession::RequestType()
    {
    	return iRequestType; //SET OR NET
    }
 
// -----------------------------------------------------------------------------
// COMASuplSession::SessionEnd
// Cleans up the session and completes
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::SessionEnd()
    {	
    	iSuplSessionState = ESUPL_TERMINATED;
    	
    	// Invoke SessionEnd on the POS Session
    	iTrace->Trace(_L("COMASuplSession::SessionEnd"), KTraceFileName, __LINE__); 
    	// CleanUp
    	CleanUp();
		if(iPOSSession)
		{
			iPOSSession->SessionEnd();
			iTrace->Trace(_L("POS Session...COMASuplSession::SessionEnd."), KTraceFileName, __LINE__); 
		}
    }   
// -----------------------------------------------------------------------------
// COMASuplSession::RunSuplSession
// Starts the SUPL Sesssion for Terminal Initiated Location Request
// The call to this method will result in the generation of SUPL START 
// and sending of the SUPL START packet to the SLP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
 void COMASuplSession::RunSuplSessionL(TRequestStatus& aStatus, TBool aFirstReq, const TDesC& aHslpAddress, 
                                       TBool aFallBack, TInt aAllowedCapabilities,
 									  TInt aSessionIdSeed,TInt aRequestID,TBool aIsStaleCellId,COMASuplLocationId* aLocationId,TBool aPrompt,TBool aWlanOnly)
    {
	    iRunRequestStatus = & aStatus;
	    *iRunRequestStatus = KRequestPending;

        iRequestID = aRequestID;

    	iTrace->Trace(_L("Start COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__); 

    	if(aIsStaleCellId)
    	    {
    	    iTrace->Trace(_L("Start COMASuplSession::RunSuplSession for Stale Cell Id"), KTraceFileName, __LINE__); 
    	    iIsStaleLocIdPresent = ETrue;

     	    
    	    if(iStaleLocationId)
    	        {
    	        delete iStaleLocationId;
    	        iStaleLocationId = NULL;
    	        }   	        

    	    iStaleLocationId = aLocationId;
    	    iStaleLocIdPrompt = aPrompt;
    	    iWlanOnly = aWlanOnly; //OCC
    	    
    	    iTrace->Trace(_L("Deleting pos requestor as session is for Stale Cell Id"), KTraceFileName, __LINE__); 
            delete iOMASuplPOSRequestor;
             iOMASuplPOSRequestor = NULL;
             
    	    // Delete the POS Session
    	     delete iPOSSession;
    	     iPOSSession = NULL;    	     

    	    }
    	// Log Session Id
    	TBuf<64> id;
    	id.Append(_L("Session  Id is "));
    	id.AppendNum(aSessionIdSeed);
    	iTrace->Trace(id,KTraceFileName, __LINE__); 
    	
    	// Log Capability from client
    	id.Copy(_L("Received Capability is "));
    	id.AppendNum(aAllowedCapabilities);
    	iTrace->Trace(id,KTraceFileName, __LINE__); 
    	
    	iSETSessionUniqueId = aSessionIdSeed;
    	
		iIhaveLaunchedUsagedialog = EFalse;
		
		TInt networkMode = 1;
		networkMode = GetNetworkModeL();
		
	 if(!iWlanOnly)	//OCC	
            {
            if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed )
                {
                    id.Copy(_L("The device is in OFFLINE mode."));
                    iTrace->Trace(id,KTraceFileName, __LINE__); 
                    iSessionObserver.TerminateSession(this, KErrGeneral);
                    return;
                }
            else
                {
                    id.Copy(_L("The device is in ON LINE mode."));
                    iTrace->Trace(id,KTraceFileName, __LINE__); 
                }
            } //OCC

    	if(aAllowedCapabilities == 0)
    		{
				iAllowedCapabilities = KGpsSETBased | KAutonomousGps| KAFLT | KECID | KEOTD | KOTDOA | KCID; 
    		}
    	else
    		{
    			iAllowedCapabilities = aAllowedCapabilities;
    		}
    		
   		//Ownership transfer to iSuplSessionId
		iSuplSessionId->SetSLPSessionID(NULL); 	
		
		iConnRequestor->SetDefaultParametersL(aHslpAddress,aFallBack);
		
		 id.Copy(_L("aFallBack value is "));
		 id.AppendNum(aFallBack);
		 iTrace->Trace(id,KTraceFileName, __LINE__);

       if(!iWlanOnly) //OCC
		    {
            iTrace->Trace(_L("iWLANOnly false COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__);
           
            id.Copy(_L("aFirstReq value is "));
            id.AppendNum(aFirstReq);
            iTrace->Trace(id,KTraceFileName, __LINE__);
            
            if (aFirstReq)
                {
                iTrace->Trace(_L("start CheckForSuplUsageL COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__);
                CheckForSuplUsageL();
                }
            else
                {
                iTrace->Trace(_L("start CheckForPreviousResultL COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__);
                CheckForPreviousResultL();  
                }
            }
		else
		    {
		    iTrace->Trace(_L("iWLANOnly true COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__); 
            InitializeL(iRequestID);
		    } //OCC     

	    
		// Clear  Position....
		delete iPosition;
		iPosition = NULL;

	    
		iTrace->Trace(_L("End of COMASuplSession::RunSuplSession"), KTraceFileName, __LINE__); 
    }

// -----------------------------------------------------------------------------
// COMASuplSession::RunSuplSession
// Starts the SUPL Sesssion for Terminal Initiated Location Request, with QoP
// The call to this method will result in the generation of SUPL START 
// and sending of the SUPL START packet to the SLP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
  void COMASuplSession::RunSuplSessionL(TRequestStatus& aStatus, TBool aFirstReq,
                                        const TDesC& aHslpAddress, TBool aFallBack,
  										TInt aAllowedCapabilities,TInt aSessionIdSeed,
  										TSuplTerminalQop& aQop, TInt aRequestID)
  	{
  	
        iIhaveLaunchedUsagedialog = EFalse;
  		
	  	iClientQop = aQop;
	  	
		TInt delay;
		if(iClientQop.GetDelay(delay) != KErrNotFound)
		{
			iSessionStartTime.HomeTime();
		}
		
  		iIsQoPPresent = ETrue;	
  		RunSuplSessionL(aStatus,aFirstReq, aHslpAddress, aFallBack, aAllowedCapabilities,aSessionIdSeed,aRequestID);
  	}

// -----------------------------------------------------------------------------
// CSuplSession::RunInvalidSessionL
// Starts the SUPL Sesssion for Invalid Session ID 
// Completes Self so that protocol Manager can process other stuff 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void COMASuplSession::RunInvalidSessionL(COMASuplAsnMessageBase* aDecodedMsg)
	{
	
			// Check if the message type is SUPL END
			COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedMsg->MessageType();

			if(messageType == COMASuplAsnMessageBase::ESUPL_END)
			{
				return;
			}

			COMASuplSessionID* retrivedSessionID = NULL;
			aDecodedMsg->SessionId(retrivedSessionID);

			// Set the SessionId.
			UpdateSLPSessionIDL(retrivedSessionID);
			UpdateSETSessionIDL(retrivedSessionID);
			
			//Set default slp to Conn Requestor...		
			iConnRequestor->UseDefaultServerL();

			// Complete Self
			iCompleteSelfRequestor->CompleteInvalidSession();
			
	}

// -----------------------------------------------------------------------------
// CSuplSession::StartInvalidSessionL
// Starts the SUPL Sesssion for Invalid Session ID 
// Generate SUPL End with error state set ti Invalid Session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void COMASuplSession::StartInvalidSessionL()
	{
				iTrace->Trace(_L("Start COMASuplSession::StartInvalidSessionL"), KTraceFileName, __LINE__); 
				iErrorStatusCode = COMASuplEnd::EInvalidSessionId;
				GenerateSuplEndL();
	}

// -----------------------------------------------------------------------------
// CSuplSession::GenerateSuplMessage
// This method generates an appropriate SUPL Message based on the state
// of the SUPL object
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplMessageL()
    {
    switch(iSuplSessionState)
        {
        //case ESUPL_INITIALIZED:
        case ESUPL_CONNECTING:
            {
            if(iRequestType == ESUPL_TERMINAL )
                {
                //SMP Changes - GenerateSuplStartL() is asynchronous and hence state must be set before calling
                iSuplSessionState = ESUPL_GENERATE;
                GenerateSuplStartL();	
                }
            if(iRequestType == ESUPL_NETWORK )
                {
                //SMP Changes - GenerateSuplPosInitL() is asynchronous and hence state must be set before calling
                iSuplSessionState = ESUPL_GENERATE;	
                GenerateSuplPosInitL();	
                }
            if(iRequestType == ESUPL_INVALID_SESSION )
                {
                //SMP Changes - iSuplSessionState = ESUPL_GENERATE is not required here as SUPL END Generation is synchronous
                //and iSuplSessionState is set inside GenerateSuplEndL itself
                GenerateSuplEndL();	
                }

            break;
            }
        case ESUPL_GENERATE:
            {
            switch(iSuplMsgType)
                {
                case ESUPL_RESPONSE:
                case ESUPL_INIT:
                    {
                    //SMP Changes - GenerateSuplPosInitL() is asynchronous and hence state must be set before calling
                    iSuplSessionState = ESUPL_ENCODE;
                    GenerateSuplPosInitL();
                    }
                    break;
                case ESUPL_END:
                    GenerateSuplEndL();
                    break;
                case ESUPL_POS:
                    {
                    //SMP Changes - GenerateSuplPosL() is asynchronous and hence state must be set before calling
                    iSuplSessionState = ESUPL_ENCODE;
                    GenerateSuplPosL();
                    }
                    break;
                default:
                    // Error
                    break;
                }
            break;		
            }
        }
    }

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplStart
// Generate SUPL START from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void COMASuplSession::GenerateSuplStartL()			
	{
		iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplStartL"), KTraceFileName, __LINE__);
		
		// Free the existing state if any
		delete iSuplState;
		iSuplState = NULL;
		
		iSuplState = COMASuplStartState::NewL(iMobilePhone, iUT1_StartTimer,iOMASuplPOSRequestor,iOMASuplAsnHandlerBaseImpl,iAllowedCapabilities);
		iSuplState->SetMsgStateObserver(this);
		if(iIsQoPPresent)
			{
				COMASuplStartState* startSuplState =  static_cast <COMASuplPosInitState *>(iSuplState);
				startSuplState->SetQop(iClientQop);
			}
		if(iIsStaleLocIdPresent)
		    {
		    iTrace->Trace(_L("GenerateSuplStartL setting stale location id in supl start"), KTraceFileName, __LINE__);
		    COMASuplStartState* startSuplState =  static_cast <COMASuplPosInitState *>(iSuplState);
		    startSuplState->SetStaleCellIdToUse(iStaleLocationId->CloneL());
		    }
        iSuplSessionState = ESUPL_GENERATE;
        iSuplMsgType = ESUPL_START;
		TInt err = iSuplState->GenerateMessageL();
	

		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
	}  
	 

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplPosInitL
// Generate SUPL POS INIT from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplPosInitL()			
	{
		iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplPosInitL"), KTraceFileName, __LINE__);
		
		// Free the existing state if any
		iTrace->Trace(_L("Deleting SUPL State"), KTraceFileName, __LINE__);
		delete iSuplState;
		iSuplState = NULL;
		
		iTrace->Trace(_L("Setting SUPL State to NULL - successful "), KTraceFileName, __LINE__);
		if(iRequestType == ESUPL_NETWORK)
		{
			iTrace->Trace(_L("iRequestType == ESUPL_NETWORK is true"), KTraceFileName, __LINE__);
			TBuf<256> serverAddress;
			HBufC8 *hslpAdress = HBufC8::NewL(256);
			iConnRequestor->GetUsedServerAddress(serverAddress);
			hslpAdress->Des().Copy(serverAddress);
			CleanupStack::PushL(hslpAdress);
			iSuplState = COMASuplPosInitState::NewL(iSETCapabilities, iMobilePhone, iUT2_PosInitTimer,
						 iOMASuplPOSRequestor,iAllowedCapabilities, iRequestType, iPosMethod, iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit, hslpAdress);

			if(hslpAdress)
			{
				CleanupStack::PopAndDestroy(hslpAdress);
			}
		}
		else
		{
			TBuf<128> msg(_L("COMASuplSession::GenerateSuplPosInitL() without last 2 params : "));
			msg.AppendNum(iPosMethod);
			iTrace->Trace(msg, KTraceFileName, __LINE__);
			iSuplState = COMASuplPosInitState::NewL(iSETCapabilities, iMobilePhone, iUT2_PosInitTimer,
 						 iOMASuplPOSRequestor,iAllowedCapabilities,iRequestType, iPosMethod,iOMASuplAsnHandlerBaseImpl);
			if(iIsStaleLocIdPresent)
			    {
			    iTrace->Trace(_L("COMASuplSession::GenerateSuplPosInitL() Setting Stale Location Id "), KTraceFileName, __LINE__);
			    COMASuplPosInitState* posInitSuplState =  static_cast <COMASuplPosInitState *>(iSuplState);
			    posInitSuplState->SetStaleCellIdToUse(iStaleLocationId->CloneL());
			    }
		
		}
		iSuplState->SetMsgStateObserver(this);
		
		//SMP Changes
		iSuplMsgType = ESUPL_POSINIT;

		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating); 
		
		iSuplState->GenerateMessageL();
		

	}   

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplPosL
// Generate SUPL POS from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplPosL()			
	{
		iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplPosL"), KTraceFileName, __LINE__);
		
		if(COMASuplState::ESUPL_POS != iSuplState->GetSuplState()) // Check if existing is of POS type.
		{
			COMASuplPosPayload* remainingPosPayload = GetRemainingPosPayloadL();
			delete iSuplState;
			iSuplState = NULL;
			iSuplState = COMASuplPosState::NewL(iPOSSession,iUT3_PosTimer,iSETCapabilities,iOMASuplAsnHandlerBaseImpl);					
			
			COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
			state->SetRemainingPosPayloadFromSuplPosInitL(remainingPosPayload);//ownership transfered to POS state class
		}
		
		iSuplState->SetMsgStateObserver(this);
		iSuplMsgType = ESUPL_POS;
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
		COMASuplPosState* posState =  static_cast <COMASuplPosState*>(iSuplState);
		if(!(posState->IsGenerationInProgress()))
			{
				iSuplSessionState = ESUPL_ENCODE;
				TInt err = iSuplState->GenerateMessageL();
				if(err != KErrNone) 
				{
					iTrace->Trace(_L("COMASuplSession::GenerateSuplPosL Failed..."), KTraceFileName, __LINE__); 
					OperationCompleteL(err);//Error handling.
				}
			}
			
	}   

// -----------------------------------------------------------------------------
// COMASuplSession::GenerateSuplEndL
// Generate SUPL END from session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GenerateSuplEndL()			
	{
		iTrace->Trace(_L("Start of COMASuplSession::GenerateSuplEnd..."), KTraceFileName, __LINE__);
		
		// Free the existing state if any
		delete iSuplState;
		iSuplState = NULL;

		if(iRequestType == ESUPL_INVALID_SESSION )
			{
			iSuplSessionState = ESUPL_INITIALIZED;
			}
		else 							
			{
			iSuplSessionState = ESUPL_ENCODE;	//Normal End
			}
		
		iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
		iSuplState->SetMsgStateObserver(this);
		iSuplMsgType = ESUPL_END;
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
		iSuplState->GenerateMessageL();
	}   
	
// -----------------------------------------------------------------------------
//COMASuplSession::CompleteSession
//Informs to client about completion of location request, thro' active object
 void COMASuplSession::CompleteSession(TInt aCompletionCode)
    {	
    	if(iRunRequestStatus)
    		{
    		
    		TTime currentTime;
    		currentTime.HomeTime();
    		iSessionCompletionCode = aCompletionCode;
    		
			TBuf<64> errorCode;
			errorCode.Append(_L("Completing Session with error code : "));
			errorCode.AppendNum(aCompletionCode);
			iTrace->Trace(errorCode, KTraceFileName, __LINE__); 
			
			iConnRequestor->UpdateNetInfo(currentTime,iCurrentCellId.iMCC,iCurrentCellId.iMNC,iCurrentCellId.iCid,iCurrentCellId.iLac,iNetworkType,aCompletionCode);
			
   			User::RequestComplete(iRunRequestStatus,aCompletionCode);
    		}
    }

// -----------------------------------------------------------------------------
// COMASuplSession::OperationComplete
// Gets control when any of the asynchronous requests are completed
// This is a very important method and drives the OMS SUPL
// State Machine
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::OperationCompleteL(TInt aErrorCode)
	{
	iTrace->Trace(_L("COMASuplSession::OperationCompleteL"), KTraceFileName, __LINE__); 
	TBuf<256> msg;
	if(KErrNone != aErrorCode)
		{
			msg.Copy(_L("Actual Error Code : "));
			msg.AppendNum(aErrorCode);
			iTrace->Trace(msg, KTraceFileName, __LINE__); 

            if (iSuplSessionState == ESUPL_CONNECTING)
                CheckForPersistentFailure(aErrorCode);                    
			HandleSuplErrorL(aErrorCode);
			return;
		}
	
	msg.Copy(_L("iSuplSessionState : "));
	msg.AppendNum(iSuplSessionState);
	iTrace->Trace(msg, KTraceFileName, __LINE__); 
		
	TInt err = KErrNone;
	
	switch(iSuplSessionState)
	{
		case ESUPL_INITIALIZED:
			{
		     if (iRequestType == ESUPL_NETWORK) 
		          {
		          iIapDialogShown = ETrue;                                                
		          TInt delay ;
		          iTrace->Trace(_L("Dialog timer not started earlier and request is NI, starting now"), KTraceFileName, __LINE__);
				          if (iDiffTime.Int() == 0)
				              {                                                
						              COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
						              TOMASuplQop qop;
						              TInt retVal = suplInit->Qop(qop);
						              
						              qop.Delay(delay);
						              //if delay is > 7, it is not valid
						              if (delay > 7)
						                  delay = 0;                                
						              if(retVal == KErrNone && delay > 0)
						               {
						                  TReal delayReal;
						                  Math::Pow(delayReal, 2, (TReal)delay);
						                  delay = (TInt) delayReal;
						                  msg.Copy(_L("Delay present in message, value is = "));
						                  msg.AppendNum(delay);
						                  iTrace->Trace(msg, KTraceFileName, __LINE__); 
						                  isTimeoutDialogTimerStarted = ETrue;
						               }
						              else
						                  {
							                  if (iSuplInitTimeOut > 0)
							                      {                                                        
								                      iTrace->Trace(_L("Delay not present in message"), KTraceFileName, __LINE__);
								                      msg.Copy(_L("Delay value in CR is"));
								                      msg.AppendNum(iSuplInitTimeOut);
								                      iTrace->Trace(msg, KTraceFileName, __LINE__);
								                      isTimeoutDialogTimerStarted = ETrue;
								                      delay = iSuplInitTimeOut * KSecond;
							                      }
								                  else
									                  {
									                      iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
									                  }    
						                  }
						           }
					          else
					              {
							              msg.Copy(_L("Time diff used, diff = "));
							              msg.AppendNum(iDiffTime.Int());
							              iTrace->Trace(msg, KTraceFileName, __LINE__);
							              isTimeoutDialogTimerStarted = ETrue;	
							              delay = iDiffTime.Int();
					              }                                                
				 TRAP( err, iConnRequestor->CreateConnectionL(delay) );
				 }
		     else //terminal initiated case
		         {
                 iTrace->Trace(_L("Connection block."), KTraceFileName, __LINE__);
            	if(iIsStaleLocIdPresent)
                     {
                     TRAP( err, iConnRequestor->CreateConnectionL(iStaleLocIdPrompt,iWlanOnly) );
                     }
                 else
                     {
                     TRAP( err, iConnRequestor->CreateConnectionL() );
                     }
		         }
			        if(KErrNone != err)
				        {
						    HandleSuplErrorL(err);
						    break;
				        }
			    iSuplSessionState = ESUPL_CONNECTING;
		    break;
			}
			
		case ESUPL_CONNECTING:
            //Retrieve the portnumber being used
            iPortNum = iConnRequestor->GetPortNumber();
		
			if(iRequestType == ESUPL_INVALID_SESSION) 
	 		{
					iTrace->Trace(_L("Handling Invalid Session..."), KTraceFileName, __LINE__); 
					  // ::POS::
        			delete iEncodedBuffer;
        			iEncodedBuffer = NULL;
					iSuplSessionState = ESUPL_ENCODE;
					iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
					if(KErrNone == err)
					{
					//SMP Changes
					iSuplSessionState = ESUPL_SEND;
					iConnRequestor->SendPacket(*iEncodedBuffer);
						
					}
			}
			else
			{				
			GenerateSuplMessageL();
			//iSuplSessionState = ESUPL_GENERATE; //SMP Changes
			}
			break;
		
		case ESUPL_SEND:
			// Clean up Encoding buffer before filling it up
			delete iEncodedBuffer;
			iEncodedBuffer = NULL;
			
			if(IsWholeMessageSentL())
				{
					SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplSent);
				} 
				
			if(  (iSuplMsgType == ESUPL_START || iSuplMsgType == ESUPL_POSINIT) && iRequestType == ESUPL_TERMINAL) // Manish : Added this to support Cell filtering...
				{
					COMASuplStartState* SuplStartState =  static_cast <COMASuplStartState *>(iSuplState);
					SuplStartState->GetCurrentCellID(iCurrentCellId,iNetworkType);
					
					#ifdef PRINT_MESSAGE
						
		               	msg.Copy(_L("After receiving from SUPL START/SUPL POS INIT"));
		               	iTrace->Trace(msg,KTraceFileName, __LINE__); 
						
		               	msg.Copy(_L("cid="));
		               	msg.AppendNum(iCurrentCellId.iCid);
		               	iTrace->Trace(msg,KTraceFileName, __LINE__); 
		               	
		               	msg.Copy(_L("Lac="));
		               	msg.AppendNum(iCurrentCellId.iLac);
		               	iTrace->Trace(msg,KTraceFileName, __LINE__);
		               	
		               	msg.Copy(_L("MCC="));
		               	msg.AppendNum(iCurrentCellId.iMCC);
		               	iTrace->Trace(msg,KTraceFileName, __LINE__);
		               	
		               	msg.Copy(_L("MNC="));
		               	msg.AppendNum(iCurrentCellId.iMNC);
		               	iTrace->Trace(msg,KTraceFileName, __LINE__); 
				#endif
					
				}

			if(iPOSSession && iSuplMsgType == ESUPL_POSINIT) //Added while doing confirmance testing...
			{
						COMASuplPosInitState* SuplPosInitState =  static_cast <COMASuplPosInitState *>(iSuplState);
						if(!(SuplPosInitState->IsPosPayloadPresent()))
						{
							iTrace->Trace(_L("StartTimer for SUPL_POSINIT..."), KTraceFileName, __LINE__); 								
							iTimer->StartTimer(iUT2_PosInitTimer * KSecond); 	
						}
						else
						{
							iTrace->Trace(_L("Timer for SUPL_POSINIT not started ..since POS payload is present..."), KTraceFileName, __LINE__); 								
						}
			}
			else
			{
				iSuplState->StartTimer();
			}
			
			if(iSuplMsgType == ESUPL_END)
				{
					iTrace->Trace(_L("COMASuplSession::OperationCompleted...SUPL_END send for Error/cancellation..."), KTraceFileName, __LINE__); 
					//Close Connection : bug Fix.
					iConnRequestor->CloseConnection();
                    iPortNum = 0;
				  	// Invoke Terminate Session
  			 		iSessionObserver.TerminateSession(this, KErrCompletion);
  			 		break;
				}
				
			if(	iPOSSession && 
			(iSuplMsgType == ESUPL_POSINIT || (iSuplMsgType == ESUPL_POS  && (!IsLastPOSMessage() || IsSegmentationDone()) ) ))
				{
					iTrace->Trace(_L("Generating POS ..."), KTraceFileName, __LINE__); 
					iSuplMsgType = ESUPL_POS;
					iSuplSessionState = ESUPL_GENERATE;
					GenerateSuplMessageL();
				}
			break;
			
		case ESUPL_GENERATE:
			// Set the appropriate state
			if(iRequestType == ESUPL_INVALID_SESSION) 
	 		{
					iTrace->Trace(_L("Handling Invalid Session..."), KTraceFileName, __LINE__); 
					  // ::POS::
        			delete iEncodedBuffer;
        			iEncodedBuffer = NULL;
					iSuplSessionState = ESUPL_ENCODE;
					iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
					if(KErrNone == err)
					{
					iSuplSessionState = ESUPL_SEND;	
					iConnRequestor->SendPacket(*iEncodedBuffer);
						
					}
			}
			else
			{
            			delete iEncodedBuffer;
            			iEncodedBuffer = NULL;
        				if(ConnectionRequired())				
        				{
        						err = SetApproriateSETSessionId();
        						iSuplSessionState = ESUPL_ENCODE;
        						CopySETCapabilities();//Copy this before and then & with Allowed Capabilities
								iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
								//SMP Changes
								iSuplSessionState = ESUPL_SEND;
								iConnRequestor->SendPacket(*iEncodedBuffer);								
								iConnRequestor->ListenToMessages();
						}
						else
							{
							//SMP Changes	
							GenerateSuplMessageL();
							//iSuplSessionState = ESUPL_ENCODE;
							}
			}
			
			if(KErrNone != err )
			{
				HandleSuplErrorL(err);
			}
			break;
		
		case ESUPL_ENCODE:
			// Clean up Encoding buffer before filling it up
			delete iEncodedBuffer;
			iEncodedBuffer = NULL;
			
			iEncodedBuffer = iSuplState->EncodeMessageL(iSuplVersion, iSuplSessionId, err);
			if(KErrNone == err)
				{
				//SMP Changes
				iSuplSessionState = ESUPL_SEND;	
				iConnRequestor->SendPacket(*iEncodedBuffer);
					
				}
			else
				{
					HandleSuplErrorL(err);
				}
			break;
			
		case ESUPL_RECEIVED: break;
		
		default:
			HandleSuplErrorL(KErrOMAUnknownState);
			break;
	}
 	}
// -----------------------------------------------------------------------------
// COMASuplSession::ConnectionRequired
// Determines of connection should be created
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplSession::ConnectionRequired()
	{
		if(iRequestType == ESUPL_INVALID_SESSION )
			{
				return ETrue;
			}
		if(iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_START)
			{
				return ETrue;
			}
		else
			{
				if(iRequestType == ESUPL_NETWORK)
					{
						if(iSuplMsgType == ESUPL_POSINIT || iSuplMsgType == ESUPL_END)
							{
								return ETrue;
							}
					}
			}
			return EFalse;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::ValidatePosition
// Validates position estimate 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplSession::ValidatePosition(COMASuplPosition *aPosition)
    {
    TOMASuplUtcTime utcTime;
    TOMASuplPositionEstimate pos;

    aPosition->GetPosition(utcTime, pos);        

    TOMASuplPositionEstimate::TOMASuplLatitudeSign  latSign;
    TInt               latitude;
    TInt               longitude;
    pos.GetPositionEstimate(latSign,latitude,longitude);	
    
    // Set Position Estimate
    if(latitude < KLatitudeMin || latitude > KLatitudeMax)
    {
        return KErrArgument;
    }
    if(longitude < KLongitudeMin || longitude > KLongitudeMax)
    {
        return KErrArgument;
    }

    // Retrieve optional parametrs of Position Estimate
	// Retrieve Uncertainity
	TOMASuplUncertainty uncertainty;
	TInt error = pos.GetUncertainty(uncertainty);
	if(!error)
		{
		TInt uncertSemiMajor;
       	TInt uncertSemiMinor;
       	TInt orientMajorAxis;
       	uncertainty.GetUncertainty(uncertSemiMajor,uncertSemiMinor,orientMajorAxis);
       	if(uncertSemiMajor < KUncertSemiMajorMin || uncertSemiMajor > KUncertSemiMajorMax)
       		{
                return KErrArgument;
       		}
       	if (uncertSemiMinor < KUncertSemiMinorMin || uncertSemiMinor > KUncertSemiMinorMax)
       		{
                return KErrArgument;
       		}
       	if (orientMajorAxis < KOrientMajorAxisMin  || orientMajorAxis > KOrientMajorAxisMax)
       		{
                return KErrArgument;
       		}
       	}

    TInt confidence;
	error = pos.GetConfidence(confidence);
	if(!error)
		{
		if(confidence < KConfidenceMin || confidence > KConfidenceMax)
			{
                return KErrArgument;
			}
		}
    // Retrieve AltitudeInfo
	TOMASuplAltitudeInfo altitudeInfo;
	error = pos.GetAltitudeInfo(altitudeInfo);
	if(!error)
		{
		TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirection;
    	TInt altitude;
       	TInt altUncert;
       	altitudeInfo.GetAltitudeInfo(altDirection,altitude,altUncert);
       	if (altitude < KAltitudeMin  || altitude > KAltitudeMax)
       		{
                return KErrArgument;
       		}
        if (altUncert < KAltitudeUncertMin || altUncert > KAltitudeUncertMax)
       		{
                return KErrArgument;
       		}
       	}

    return KErrNone;
    }        

// -----------------------------------------------------------------------------
// COMASuplSession::GetPositionComplete
// Handles any errors that may occur during protocol processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::GetPositionComplete(TInt aError)
	{
		//Close Connection : bug Fix.
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		if(aError || iPosition->Status())
			{
				delete iPosition;
				iPosition = NULL;
				// Invoke SessionTerminate
                iTrace->Trace(_L("Could not get position from POS Msg Plugin..."), KTraceFileName, __LINE__);
                iSessionObserver.TerminateSession(this, KErrNone);
			}
		else
			{
                TInt err;
                err = ValidatePosition(iPosition);                
                if (err != KErrNone)
                {
                    delete iPosition;
                    iPosition = NULL;
				    iSessionObserver.TerminateSession(this, KErrCompletion);	
                }
                else                
				    iSessionObserver.TerminateSession(this, KErrNone);	
			}	
	}

        

// -----------------------------------------------------------------------------
// COMASuplSession::HandleSuplErrorL
// Handles any errors that may occur during protocol processing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleSuplErrorL(TInt aErr)	
	{
		// All Possible errors -
		// 1. Packet generation failure
		//    SUPL START - Complete the session with aErr
		//    SUPL POS INIT - Send SUPL END with appropriate Error code
		//    SUPL END - Complete Session with aErr and terminate the session
		//    SUPL POS - Will be handled later
		// 2. Secure Socket Connection failure
		//    Complete the Session with aErr. Terminate the supl session
		// 3. Local IP Address retrieval failure
		//    Complete the session with aErr. Terminate the supl session
		// 4. Packet Encoding error
		//    SUPL START - Complete Session with aErr
		//    SUPL POS INIT - Generate SUPL END and send to SLP
		//    SUPL POS - Will be handled later
		//    SUPL END - Complete session with aErr and terminate the session
		// 5. Packet Send Error
		//    Complete the session with aErr and terminate the session
		// 6. Unknown State Error
		//    SUPL START - Complete Session with invalid state
		//    SUPL RESPONSE - Send SUPL END with appropriate error
		//    SUPL POS INIT - Send SUPL END with appropriate error
		//    SUPL POS - Will be handled later
		//    SUPL END - Complete session with invalid state and terminate the session
		//    SUPL INTI - Will be handled later
		// 7. Message Processing error
		//    SUPL RESPONSE - Send SUPL END with appropriate error code
		//    SUPL POS - Will be handled later
		//    SUPL END - Complete session with aErr and terminate the session
		// 8. Time out Handling
		//    SUPL START - Send SUPL END with appropriate error code
		//    SUPL POS INIT - Send SUPL END with appropriate error code
		//    SUPL END - There should be no timer started in the first place
		
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplError);	
		
		switch(iSuplSessionState) 
		{
		
//***************************************************************************************				 		

			case ESUPL_INITIALIZED: //Initilization error
				{
					aErr = KErrNotReady;
					iTrace->Trace(_L("HandleInitilizationError..."), KTraceFileName, __LINE__); 
					HandleInitilizationErrorL(aErr);
					break;	
				}
				
//***************************************************************************************				 

			case ESUPL_CONNECTING:  //Error while retriving Local IP address
				{
					aErr = KErrGeneral;
					iTrace->Trace(_L("HandleConnectionError..."), KTraceFileName, __LINE__); 
					HandleConnectionErrorL(aErr);
					break;					
				}
				
//***************************************************************************************				 

			case ESUPL_ENCODE:   //Encoding failure error
				{
					
					iTrace->Trace(_L("HandleEncodingError..."), KTraceFileName, __LINE__); 
					HandleEncodingErrorL(aErr);
					break;
				}
				
//***************************************************************************************				 				

			case ESUPL_GENERATE : //Generation error
				{
				    aErr = KErrGeneral;
				    iTrace->Trace(_L("HandleGenerationError..."), KTraceFileName, __LINE__); 
					HandleGenerationErrorL(aErr);
					break;
				}						
				
//***************************************************************************************				 				
 
			case ESUPL_SEND:	//Timeout & packet send error
				{
					iTrace->Trace(_L("HandleSendError..."), KTraceFileName, __LINE__); 
					HandleSendErrorL(aErr);
					break;
				}
				 
//***************************************************************************************				 								 

			case ESUPL_RECEIVED:	//Error while getting wrong message from SLP
				{
					iTrace->Trace(_L("HandlePacketsError..."), KTraceFileName, __LINE__); 					
					HandlePacketsErrorL(aErr);	
					break;		
				}
				
//***************************************************************************************				 								 				

			default:
				{
				 TBuf<64> msg(_L("Unknown ERROR occured..."));
				 msg.AppendNum(aErr);
				 iTrace->Trace(msg, KTraceFileName, __LINE__); 					
				 break;	
				} 
				
//***************************************************************************************				 								 				
		}
		
	}
	
// -----------------------------------------------------------------------------
// ReverseString
// Reverse the string
// -----------------------------------------------------------------------------
//
void ReverseString(TDes8& aLocalIPAddress,TInt aLength)
{
	TBuf8<1> temp1;
	TBuf8<1> temp2;
	for(TInt i =0 ;i < aLength / 2; ++i)
	{
		temp1.Zero();
		temp2.Zero();
		temp1.Append(aLocalIPAddress[i]); 
		temp2.Append(aLocalIPAddress[aLength - 1 - i]); 
		aLocalIPAddress.Replace(i,1,temp2);
		aLocalIPAddress.Replace(aLength - 1 - i,1,temp1);
	}
}
	
// -----------------------------------------------------------------------------
// COMASuplSession::GetLocalIPAddress
// Gets Local IP address of SET
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplSession::GetLocalIPAddress()	
	{
		iTrace->Trace(_L("COMASuplSession::GetLocalIPAddress"), KTraceFileName, __LINE__); 

        if (iSessionIDFlag)
        {
            return KErrNone;
        }                

		TInetAddr address;
		TInt err = iConnRequestor->GetLocalIPAddress(address);  
		if (err != KErrNone) 
			{
				return err;	
			}
		
		if ( address.Family() == KAfInet) //This routine will convert IPV4 address in TBuf8<4>
			{
				TUint32 ipAddress = address.Address();
				TPtr8 ptrIPAddress(reinterpret_cast<TUint8*>(&ipAddress), 
								   sizeof(ipAddress), sizeof(ipAddress));
				TBuf8<KIP4AddressLength> localIPAddress; 
				localIPAddress.Append(ptrIPAddress);
				COMASuplSETSessionID* suplSETSessionId = NULL;
				iSuplSessionId->SETSessionID(suplSETSessionId);
				ReverseString(localIPAddress,KIP4AddressLength);
				suplSETSessionId->SetSuplSETSessionIDIPv4(iSETSessionUniqueId,
														  localIPAddress);
               	//#if _DEBUG													  
               		TBuf<64> msg;
               		TBuf<16> ipaddress;
               		address.Output(ipaddress);
               		msg.Copy(_L("Its IP4 Address. Local IP Address is "));
               		msg.Append(ipaddress);
        	    	iTrace->Trace(msg,KTraceFileName, __LINE__); 
				//#endif	        
			}
			
		if ( address.Family() == KAfInet6 )  // This routine will convert IPV6 address in TBuf8<16>
			{
				TBuf8<KIP6AddressLength> localIP6Address;
				const TIp6Addr &ip6Addr = address.Ip6Address();
				for(TInt i = 0;i < 4; ++i )
					{
						TUint32 ipAddress = ip6Addr.u.iAddr32[i]; 
						TPtr8 ptrIPAddress(reinterpret_cast<TUint8*>(&ipAddress), 
										   sizeof(ipAddress), sizeof(ipAddress));
						TBuf8<KIP4AddressLength> localIPAddress;
						localIPAddress.Append(ptrIPAddress);
						localIP6Address.Append(localIPAddress);
					}
					
				COMASuplSETSessionID* suplSETSessionId = NULL;
				iSuplSessionId->SETSessionID(suplSETSessionId);
				ReverseString(localIP6Address,KIP6AddressLength);
				suplSETSessionId->SetSuplSETSessionIDIPv6(iSETSessionUniqueId,
														  localIP6Address);
               	//#if _DEBUG													  
               		TBuf<64> msg;
               		TBuf<16> ipaddress;
               		address.Output(ipaddress);
               		msg.Copy(_L("Its IP6 Address. Local IP Address is "));
               		msg.Append(ipaddress);
        	    	iTrace->Trace(msg,KTraceFileName, __LINE__); 
				//#endif	        	    																	
			}
			
			return KErrNone;
	}
	

// -----------------------------------------------------------------------------
// COMASuplSession::SetEncodedSuplInitL
// Sets the Encoded SUPL INIT. This is required
// to compute the VER in SUPL POS INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::SetEncodedSuplInitL(HBufC8 *aEncodedSuplInit)	 
	{
		TInt len = aEncodedSuplInit->Length();
		if(iEncodedSuplInit)
		{
			delete iEncodedSuplInit;
			iEncodedSuplInit = NULL;
		}
		
		iEncodedSuplInit = HBufC8::NewL(len);
		*iEncodedSuplInit = *aEncodedSuplInit;
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SuplSessionID
// Returns SessionID of Session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSessionID* COMASuplSession::SuplSessionID() const	 
	{
		return iSuplSessionId;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::GetServerAddressL
// Gets the Server Name and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetServerAddressL(TDes& ahslpAddress)
	{
		if(iSessionCompletionCode == KErrNone)
		{
			iConnRequestor->GetUsedServerAddress(ahslpAddress);
			iTrace->Trace(_L("Session has suceeded...returning address"), KTraceFileName, __LINE__); 
			iTrace->Trace(ahslpAddress, KTraceFileName, __LINE__); 
			return KErrNone;
		}
		else
		{
			iTrace->Trace(_L("Session has failed...returning Error"), KTraceFileName, __LINE__); 
			return KErrNotFound;
		}
    }
    
// -----------------------------------------------------------------------------
// COMASuplSession::GetPosition
// Gets the Position from the SUPL framework and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetPosition(TPositionInfo& aSuplPosInfo)	
	{
			TBool aPositionQualityError=TRUE;
			iTrace->Trace(_L("COMASuplSession::GetPosition(TPositionInfo)"), KTraceFileName, __LINE__); 
			if(!iPosition)	
			{
				iTrace->Trace(_L("COMASuplSession::GetPosition ... Position is Empty"), KTraceFileName, __LINE__); 
				return KErrNotFound;	
			}
			
			TPosition posInfo;
			TOMASuplUtcTime UtcTime;
			TOMASuplPositionEstimate PosEstimate;
			TDateTime TimeStamp;
			TInt ZoneCode,Zone,altitude, HorizontalAccuracy;
			TInt AltitudeUncertainty = 0;
			TOMASuplAltitudeInfo AltitudeInfo;
			TInt latitude,longitude;
			TOMASuplPositionEstimate::TOMASuplLatitudeSign LatSign;
			TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altitudeDirection;
            TOMASuplUncertainty Uncertainty;
            TInt UncertaintySemiMajor, UncertaintySemiMinor, OrientationMajorAxis;
            TReal UncertaintySemiMajorReal, UncertaintySemiMinorReal, AltitudeUncertaintyReal;
            TInt confidence;
            
			iPosition->GetPosition(UtcTime,PosEstimate);
			UtcTime.GetUtcTime(TimeStamp,ZoneCode,Zone);
			TInt altErr = PosEstimate.GetAltitudeInfo(AltitudeInfo);
			PosEstimate.GetPositionEstimate(LatSign,latitude,longitude);
            TInt err = PosEstimate.GetConfidence(confidence);
			altitude = 0;
			if (altErr == KErrNone)
			AltitudeInfo.GetAltitudeInfo(altitudeDirection,altitude,AltitudeUncertainty);
			
            TTime time(TimeStamp);
			posInfo.SetTime(time);

						if(LatSign == TOMASuplPositionEstimate::ESouth )
						{
							latitude = latitude * -1;
						}

            // Conversion of latitude and longitude
            TReal64 newLat, newLong;
            newLat = latitude;
            newLong = longitude;
            //Conversion as per GAD and OMA Spec
            newLat = (newLat/8388608)*90;
            newLong = (newLong/16777216)*360;

            if (altErr == KErrOMASuplParamNotSet)
                posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong);
            else                    
            posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong,(float)altitude);
			PosEstimate.GetUncertainty(Uncertainty);
            Uncertainty.GetUncertainty(UncertaintySemiMajor,UncertaintySemiMinor,
            							OrientationMajorAxis);
		    TBuf<64> msg;
			msg.Copy(_L("Uncertainty Semi major"));
            msg.AppendNum(UncertaintySemiMajor);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Uncertainty Semi Minor"));
            msg.AppendNum(UncertaintySemiMinor);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Orientation to Major axis "));
            msg.AppendNum(OrientationMajorAxis);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Altitude "));
            msg.AppendNum(altitude);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Altitude uncertainty "));
            msg.AppendNum(AltitudeUncertainty);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 

            // Convert uncertainty to meters
            TReal pwrMjr, pwrMnr, pwrAlt;
            Math::Pow(pwrMjr, 1.1, UncertaintySemiMajor);
            Math::Pow(pwrMnr, 1.1, UncertaintySemiMinor);
            Math::Pow(pwrAlt, 1.070000, AltitudeUncertainty); 
            UncertaintySemiMajorReal = 10 * (pwrMjr -1);
            UncertaintySemiMinorReal = 10 * (pwrMnr -1);
            AltitudeUncertaintyReal = 10 * (pwrAlt -1);

        	TReal MultiplyFactor;
            
            // Default set accuracy values to NaN
            TRealX nan;
            nan.SetNaN();

            posInfo.SetHorizontalAccuracy(nan);
            posInfo.SetVerticalAccuracy(nan);
            if (err == KErrOMASuplParamNotSet||confidence<=0 || confidence>=100)
                {
                TReal32 horizAccr;
                horizAccr = KUncertaintyConversionFactor * ( KCEP68ConversionFactor * (UncertaintySemiMajorReal + UncertaintySemiMinorReal ) );
                posInfo.SetHorizontalAccuracy(horizAccr);
                if (AltitudeUncertaintyReal > 0)
                    posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);

                }
            else
                {                    

        	        TReal lnResult,a_Sigma,b_Sigma;
           	        Math::Ln(lnResult,(1-confidence/100.0));
	                Math::Sqrt(MultiplyFactor,(-2*lnResult));	
        	
        	        a_Sigma=UncertaintySemiMajorReal/MultiplyFactor;
        	
        	        b_Sigma=UncertaintySemiMinorReal/MultiplyFactor;
        	
        	        HorizontalAccuracy=0.7542*(a_Sigma+b_Sigma);
        	
        	        posInfo.SetHorizontalAccuracy(HorizontalAccuracy);
	        	    TReal32 clientHorAcc;
	        	    if(iClientQop.GetHorizontalAccuracy(clientHorAcc)!=KErrNotFound)
	        	        {
	        			if(HorizontalAccuracy<clientHorAcc)
	        			    {
	        			    aPositionQualityError=FALSE;	
	        			    }

        	        }
            
                    /* Calculation for vertical accuracy */
                    /* If confidence is 68, then vertical accuracy = altitude uncertainty */
                    if (altitude > 0 && altErr == KErrNone)
                    {                    
                    if (confidence == 68 && AltitudeUncertaintyReal > 0)
			            posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
                    else
                        {
                        TReal32 chiValue=0, verticalAccuracy=0;                        
                        if (confidence < 6)
                            {
                            chiValue = chisquare[1];
                            }
                        else
                            {
                            chiValue = chisquare[confidence/5];
                            }                
                        verticalAccuracy = AltitudeUncertaintyReal/chiValue;
                        if (verticalAccuracy > 0)
                            {                                
			                posInfo.SetVerticalAccuracy(verticalAccuracy);
			                TReal32 clientVerAcc;
	        	            if(iClientQop.GetVerticalAccuracy(clientVerAcc)!=KErrNotFound)
	        	                {
	        			        if(verticalAccuracy<clientVerAcc)
	        			            {
	        				        aPositionQualityError=FALSE;	
	        			            }
	        			        }
	        			    }
	        	        }
                    }                    
                }                    
                    
         	    TInt aClientTime;  
                if(iClientQop.GetDelay(aClientTime)!=KErrNotFound)
                    {
              	    TTimeIntervalSeconds aDelay;
              	
              	    iSessionEndTime.SecondsFrom(iSessionStartTime,aDelay);
              	
              	    if(aClientTime< aDelay.Int())
              		    aPositionQualityError=FALSE;	
                    }
        
			/*Set the position to TPositionInfo*/
			aSuplPosInfo.SetPosition(posInfo);			
		
			if(aPositionQualityError!=TRUE)
				return KSuplPositionQualityLoss;
			return KErrNone;
	}

// -----------------------------------------------------------------------------
// COMASuplSession::GetPosition
// Gets the Position from the SUPL framework and returns it back to the client
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt COMASuplSession::GetPosition(HPositionGenericInfo& aSuplPosInfo )
	{
			TBool aPositionQualityError=TRUE;
			iTrace->Trace(_L("COMASuplSession::GetPosition(HPositionGenericInfo)"), KTraceFileName, __LINE__); 
			if(!iPosition)	
				{
					iTrace->Trace(_L("COMASuplSession::GetPosition ... Position is Empty"), KTraceFileName, __LINE__); 
					return KErrNotFound;	
				}

			TRAP_IGNORE(StoreCurrentCellIDL(aSuplPosInfo));
						
			TPosition posInfo;
			TOMASuplUtcTime UtcTime;
			TOMASuplPositionEstimate PosEstimate;
			TDateTime TimeStamp;
			TInt ZoneCode,Zone,altitude, HorizontalAccuracy;
			TInt AltitudeUncertainty = 0;
			TOMASuplAltitudeInfo AltitudeInfo;
			TInt latitude,longitude;
			TOMASuplPositionEstimate::TOMASuplLatitudeSign LatSign;
			TOMASuplAltitudeInfo::TOMASuplAltitudeDirection AltitudeDirection;
            TOMASuplUncertainty Uncertainty;
            TInt UncertaintySemiMajor, UncertaintySemiMinor, OrientationMajorAxis;
            TReal UncertaintySemiMajorReal, UncertaintySemiMinorReal, AltitudeUncertaintyReal;
            TInt confidence;
			
			iPosition->GetPosition(UtcTime,PosEstimate);
			UtcTime.GetUtcTime(TimeStamp,ZoneCode,Zone);
			TInt altErr = PosEstimate.GetAltitudeInfo(AltitudeInfo);
			PosEstimate.GetPositionEstimate(LatSign,latitude,longitude);
			TInt err = PosEstimate.GetConfidence(confidence);
			altitude = 0;
			if (altErr == KErrNone)
			AltitudeInfo.GetAltitudeInfo(AltitudeDirection,altitude,AltitudeUncertainty);
			PosEstimate.GetUncertainty(Uncertainty);
            Uncertainty.GetUncertainty(UncertaintySemiMajor,UncertaintySemiMinor,
            							OrientationMajorAxis);
            // Convert uncertainty to meters
            TReal pwrMjr, pwrMnr, pwrAlt;
            Math::Pow(pwrMjr, 1.1, UncertaintySemiMajor);
            Math::Pow(pwrMnr, 1.1, UncertaintySemiMinor);
            Math::Pow(pwrAlt, 1.070000, AltitudeUncertainty); 
            UncertaintySemiMajorReal = 10 * (pwrMjr -1);
            UncertaintySemiMinorReal = 10 * (pwrMnr -1);
            AltitudeUncertaintyReal = 10 * (pwrAlt -1);

			TReal MultiplyFactor;

		    TBuf<64> msg;
			msg.Copy(_L("Uncertainty Semi major"));
            msg.AppendNum(UncertaintySemiMajor);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Uncertainty Semi Minor"));
            msg.AppendNum(UncertaintySemiMinor);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Orientation to Major axis "));
            msg.AppendNum(OrientationMajorAxis);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Altitude "));
            msg.AppendNum(altitude);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			msg.Copy(_L("Altitude uncertainty "));
            msg.AppendNum(AltitudeUncertainty);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 

            // Default set accuracy values to NaN
            TRealX nan;
            nan.SetNaN();

            posInfo.SetHorizontalAccuracy(nan);
            posInfo.SetVerticalAccuracy(nan);

            if (err == KErrOMASuplParamNotSet||confidence<=0 || confidence>=100)
                {
                iTrace->Trace(_L("Confidance Not Set"), KTraceFileName, __LINE__); 

                TReal32 horizAccr;
                horizAccr = KUncertaintyConversionFactor * ( KCEP68ConversionFactor * (UncertaintySemiMajorReal + UncertaintySemiMinorReal ) );
                posInfo.SetHorizontalAccuracy(horizAccr);
                if (AltitudeUncertaintyReal > 0)
                    posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
                }
            else
                {
                  iTrace->Trace(_L("Confidance is Set"), KTraceFileName, __LINE__); 

	        	    TReal lnResult,a_Sigma,b_Sigma;
            	    Math::Ln(lnResult,(1-confidence/100.0));
	        	    Math::Sqrt(MultiplyFactor,(-2*lnResult));	
	        	
	        	    a_Sigma=UncertaintySemiMajorReal/MultiplyFactor;
	        	    b_Sigma=UncertaintySemiMinorReal/MultiplyFactor;
	        	
	        	    HorizontalAccuracy=0.7542*(a_Sigma+b_Sigma);
	        	
	        	    posInfo.SetHorizontalAccuracy(HorizontalAccuracy);
	        	    TReal32 clientHorAcc;
	        	    if(iClientQop.GetHorizontalAccuracy(clientHorAcc)!=KErrNotFound)
	        	        {
	        					if(HorizontalAccuracy<clientHorAcc)
	        			    	{
	        							aPositionQualityError=FALSE;	
	        			    	}
	        	        }
        	       // }
			
            /* To calculate vertical accuracy */
			//posInfo.SetVerticalAccuracy(AltitudeUncertainty);
            
                /* If confidence is 68, then vertical accuracy = altitude uncertainty */
                if (altitude > 0 && altErr == KErrNone)
                {                    
                if (confidence == 68 && AltitudeUncertaintyReal > 0)
			        posInfo.SetVerticalAccuracy(AltitudeUncertaintyReal);
                else
                    {
                    TReal32 chiValue=0, verticalAccuracy=0;                        
                    if (confidence < 6)
                        {
                        chiValue = chisquare[1];
                        }
                    else
                        {
                        chiValue = chisquare[confidence/5];
                        }                
                    verticalAccuracy = AltitudeUncertaintyReal/chiValue;
                    if (verticalAccuracy > 0)
                        {                            
			            posInfo.SetVerticalAccuracy(verticalAccuracy);
			            TReal32 clientVerAcc;
	        	        if(iClientQop.GetVerticalAccuracy(clientVerAcc)!=KErrNotFound)
	        	            {
	        			    if(verticalAccuracy<clientVerAcc)
	        			        {
	        				    aPositionQualityError=FALSE;	
	        			        }
	        		        else
	        			        aPositionQualityError=FALSE;	
	        	            }
	        	        }
                    }                    
                }                    
                }
           
            TTime time(TimeStamp);
						posInfo.SetTime(time);
						
						if(LatSign == TOMASuplPositionEstimate::ESouth )
						{
							latitude = latitude * -1;
						}

            // Conversion of latitude and longitude
            TReal64 newLat, newLong;
            newLat = latitude;
            newLong = longitude;
            newLat = (newLat/8388608)*90;
            newLong = (newLong/16777216)*360;
            if (altErr == KErrOMASuplParamNotSet)
                posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong);
            else                    
            posInfo.SetCoordinate((TReal64)newLat,(TReal64)newLong,(float)altitude);

			/*Set the position to TPositionInfo*/
			aSuplPosInfo.SetPosition(posInfo);			
		
            /* Set GenInfo related fields */
            TOMASuplVelocityType    VelType;
            COMASuplHorizVelocity*  HorizVel;
            COMASuplVelocity*       SuplVel;
            TUint16 HorSpeed;
            TUint16 Bearing;
            TUint8 VertDirection;
            TUint8 VertSpeed;
            TUint8 UncertSpeed;
            TUint8 HorUncertSpeed,VerUncertSpeed;
            
			iTrace->Trace(_L("Before iPosition->GetVelocity"), KTraceFileName, __LINE__); 
            TInt ret = iPosition->GetVelocity(SuplVel);
			iTrace->Trace(_L("After iPosition->GetVelocity"), KTraceFileName, __LINE__); 
            if (ret == KErrNone)
                {
            
            VelType = SuplVel->VelType();

            HorizVel = SuplVel->Velocity();

            switch(VelType)
                {
                case EHorizVelocity:
                	{
                	iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizVelocity"), KTraceFileName, __LINE__); 
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                    HorizVel->GetHorizVel(Bearing, HorSpeed);
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                    iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizVelocity"), KTraceFileName, __LINE__); 
                    break;                        
                	}
                case EHorizAndVertVelocity:
                	{
                	iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertVelocity"), KTraceFileName, __LINE__); 
                    COMASuplHorizAndVertVelocity* horizVertVel = (COMASuplHorizAndVertVelocity*)HorizVel;
                    horizVertVel->GetHorizAndVertVel(Bearing, HorSpeed, VertDirection,
                    								 VertSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeed);
                    aSuplPosInfo.SetValue(EPositionFieldVerticalSpeed, (TReal32)VertSpeed);
                   	iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertVelocity"), KTraceFileName, __LINE__); 
                    break;
                	}
                case EHorizUncertVelocity:
                	{
                	iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizUncertVelocity"), KTraceFileName, __LINE__); 
                    COMASuplHorizUncertVelocity* horizUncertVel = (COMASuplHorizUncertVelocity*)HorizVel;
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                    horizUncertVel->GetHorizUncertVel(Bearing, HorSpeed, UncertSpeed); 
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeedError);
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeedError, (TReal32)UncertSpeed);
                   	iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizUncertVelocity"), KTraceFileName, __LINE__); 
                    break;
                	}
                case EHorizAndVertUncertVelocity:
                	{
                   	iTrace->Trace(_L("Start of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertUncertVelocity"), KTraceFileName, __LINE__); 
                    COMASuplHorizAndVertUncertVelocity* horizVertUncertVel = (COMASuplHorizAndVertUncertVelocity*)HorizVel;
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeed);
                    horizVertUncertVel->GetHorizVertUncertVel(Bearing, HorSpeed,
                    										  VertDirection,
                    										  VertSpeed,HorUncertSpeed,VerUncertSpeed); 
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeed, (TReal32)HorSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldHorizontalSpeedError);
                    aSuplPosInfo.SetValue(EPositionFieldHorizontalSpeedError, (TReal32)HorUncertSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeed);
                    aSuplPosInfo.SetValue(EPositionFieldVerticalSpeed, (TReal32)VertSpeed);
                    aSuplPosInfo.SetRequestedField(EPositionFieldVerticalSpeedError);
                    aSuplPosInfo.SetValue(EPositionFieldVerticalSpeedError, (TReal32)VerUncertSpeed);
                   	iTrace->Trace(_L("End of COMASuplSession::GetPosition(HPositionGenericInfo) velType = EHorizAndVertUncertVelocity"), KTraceFileName, __LINE__); 
                    break;
                }
                }
                }
              TInt aClientTime;  
              if(iClientQop.GetDelay(aClientTime)!=KErrNotFound)
              {
              	TTimeIntervalSeconds aDelay;
              	iSessionEndTime.SecondsFrom(iSessionStartTime,aDelay);
              	
              	if(aClientTime< aDelay.Int())
              		aPositionQualityError=FALSE;	
              }
                
             if(aPositionQualityError==FALSE)
             	return KSuplPositionQualityLoss;   
           	return KErrNone;
	}

	
// -----------------------------------------------------------------------------
// COMASuplSession::HandleOMASuplMessage
// Handles OMA Supl messages & starts processing according to state of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage,TRequestStatus& aStatus,TInt aSessionIdSeed,TInt aErrorCode)
	{
	delete iSuplState;
	iSuplState = NULL;
		
	iSETSessionUniqueId = aSessionIdSeed;
	iRunRequestStatus = & aStatus;
	*iRunRequestStatus = KRequestPending;
	    
	TInt networkMode = 1;
	networkMode = GetNetworkModeL();
		
	TBuf<64> msg;
	if ( networkMode == ECoreAppUIsNetworkConnectionNotAllowed )
		{
		msg.Copy(_L("The device is in OFFLINE mode."));
		iTrace->Trace(msg,KTraceFileName, __LINE__); 
		iSessionObserver.TerminateSession(this, KErrGeneral);
		return;
		}
	else
		{
		msg.Copy(_L("The device is in ON LINE mode."));
		iTrace->Trace(msg,KTraceFileName, __LINE__); 
		}

   
	    
	COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();
	    
	if(messageType == COMASuplAsnMessageBase::ESUPL_INIT && aErrorCode == KErrNone)
		{
	    HandleOMASuplMessageL(aDecodedAsnMessage);
	    }
	else //Other messages than SUPL_INIT.... Send End
	    {
	    CreateCloneMessageL(aDecodedAsnMessage);		
	    UpdateSuplSessionIDL();

	    if (iRequestType == ESUPL_NETWORK && messageType == COMASuplAsnMessageBase::ESUPL_INIT)
	    	{
			ServerAddressCheckForSuplInitL();
	    	}
 
		iSuplSessionState = ESUPL_INITIALIZED;
		iSuplMsgType = ESUPL_END;

	    if((aErrorCode == KErrCompletion && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
	    	(aErrorCode == KErrOMASuplOutOfRangeParameter && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
	    	(aErrorCode == KErrOMASuplMessageLengthMismatch && messageType == COMASuplAsnMessageBase::ESUPL_INIT) ||
			(aErrorCode == KErrOMASuplShortFrame && messageType == COMASuplAsnMessageBase::ESUPL_INIT))	    			
	   		{
	    	iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;
	    	if(aErrorCode == KErrOMASuplMessageLengthMismatch)
	    		{
	    		iErrorStatusCode = COMASuplEnd::EProtocolError;
	    		}
      		if(aErrorCode == KErrOMASuplShortFrame)
	    		{
	    		iErrorStatusCode = COMASuplEnd::EDataMissing;
	    		}
			TInt len = iHSLPAddress.Length();
			HBufC8 *hslpAdress = NULL;
			if(len > 0)
				{
				hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
				hslpAdress->Des().Copy(iHSLPAddress);
				}						
			else if(len ==0)
				{
				CServerParams* serverParams = CServerParams::NewL();
				CleanupStack::PushL(serverParams);
					
				if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
					{
					iTrace->Trace(_L("HSLP generated frm IMSI"), KTraceFileName, __LINE__);
					hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
					hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());
					}		
				else
					{
					iTrace->Trace(_L("Sending End with ver for Default HSLP"), KTraceFileName, __LINE__);
					TInt64 slpId;
            		TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
					HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
					HBufC* iapName = HBufC::NewL(KMaxIAPLen);
					CleanupStack::PushL(serverAddr);
					CleanupStack::PushL(iapName);
					serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
					hslpAdress = HBufC8::NewL(serverAddr->Length());
					hslpAdress->Des().Copy(*serverAddr);
						
					CleanupStack::PopAndDestroy(iapName);
					CleanupStack::PopAndDestroy(serverAddr);
							
					}
				CleanupStack::PopAndDestroy(serverParams);
				}
			delete iSuplState;
			iSuplState = NULL;
			CleanupStack::PushL(hslpAdress);
			iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);
			CleanupStack::PopAndDestroy(hslpAdress);
	    	}
	    else
	    	{
	    		iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
	    		iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
	    	}	
	    			
	    SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
		iSuplState->SetMsgStateObserver(this);
		//SMP Changes
		iSuplState->GenerateMessageL(); //Has to be here because initialisation has to be done for NI case in decode failed scenario.
		iSuplSessionState = ESUPL_GENERATE;				 
		}	
	}

// -----------------------------------------------------------------------------
// COMASuplSession::HandleOMASuplMessage
// Handles OMA Supl messages & starts processing according to state of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleOMASuplMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)
	{
    CreateCloneMessageL(aDecodedAsnMessage);
		if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version
		{
			COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();

			if(messageType != COMASuplAsnMessageBase::ESUPL_POS)
				{
					iCompleteSelfRequestor->CompleteSelf(); //This will result in call of RequestCompleted()
				}
			else
				{
					RequestCompletedL();	
				}
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::RequestCompleted
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::RequestCompletedL()
	{
		//Start Next Processing......	
		iTrace->Trace(_L("COMASuplSession::RequestCompleted...Retriving message type"), KTraceFileName, __LINE__); 									
		
		COMASuplAsnMessageBase::TSuplMessageType messageType = iDecodedAsnMessage->MessageType();
		     
		switch(messageType)
			{
			case COMASuplAsnMessageBase::ESUPL_RESPONSE:
				{
				 iSuplSessionState = ESUPL_RECEIVED;
				 iTrace->Trace(_L("Received Message is of SUPL_RESPONSE type..."), KTraceFileName, __LINE__); 									
				 CancelAllOperations();
				 if(iSuplMsgType == ESUPL_START)
				 	{
						iSuplMsgType = ESUPL_RESPONSE; 
						ProcessSuplResponseL();
				 	}
				 else
				 	{
				 		iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
				 		// Send SUPL End with Protocol Error
						HandleSuplErrorL(KErrOMASuplProtocolError);				 		
				 	}
				 break;
				 
				}
 			case COMASuplAsnMessageBase::ESUPL_POS: 
 				{
				 iTrace->Trace(_L("Received Message is of SUPL_POS type..."), KTraceFileName, __LINE__); 									
				 if(iOMASuplPOSRequestor) //
				 	{
						 //if(((iSuplMsgType == ESUPL_POS || iSuplMsgType == ESUPL_POSINIT)) && !IsLastPOSMessage())
						 if(((iSuplMsgType == ESUPL_POS || iSuplMsgType == ESUPL_POSINIT))) //Changed for SUPL_POS after UT3
						 	{
						 		iSuplMsgType = ESUPL_POS; 
						 		iTimer->StopTimer();	 // Added by Manish
						 		iSuplState->StopTimer();
								ProcessSuplPosL();
						 	}
						 else
						 	{
						 		iSuplSessionState = ESUPL_RECEIVED; 
								CancelAllOperations();
						 		iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
								// Send SUPL End with Protocol Error
								HandleSuplErrorL(KErrOMASuplProtocolError);				 		
						 	}
				 	}
				 else // POS is not there
				 	{
							iSuplSessionState = ESUPL_RECEIVED; 
							CancelAllOperations();
					 		iTrace->Trace(_L("COMASuplSession::RequestCompletedL...Protocol Error..."), KTraceFileName, __LINE__); 									
							// Send SUPL End with Protocol Error
							HandleSuplErrorL(KErrOMASuplProtocolError);				 		
				 	}	
 				 break;
 				}
 			case COMASuplAsnMessageBase::ESUPL_END:
					{
						iSuplSessionState = ESUPL_RECEIVED;
						iTrace->Trace(_L("Received Message is of SUPL_END type..."), KTraceFileName, __LINE__); 									
						CancelAllOperations();
						//Close Connection--- Bug fix
						iConnRequestor->CloseConnection();
                        iPortNum = 0;
						if(IsGenerationInProgress()) //Check is generation going on
							{
								// Invoke SessionTerminate
								iSessionObserver.TerminateSession(this, KErrCompletion);
								break;
							}
						
						if(iSuplMsgType != ESUPL_END)
							{
								iSuplMsgType = ESUPL_END; //::POS::
								ProcessSuplEndL();		
							}						
						break;
					}

			case COMASuplAsnMessageBase::ESUPL_INIT:	 //::POS::
					{
						iTrace->Trace(_L("Received Message is of ESUPL_INIT type..."), KTraceFileName, __LINE__); 									
						iSuplSessionState = ESUPL_RECEIVED;
						CancelAllOperations();
						iSuplMsgType = ESUPL_INIT; //::POS::
						ProcessSuplInitL();
						break;
					}
					
			case COMASuplAsnMessageBase::ESUPL_AUTH_REQ:
			case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
					{
						iTrace->Trace(_L("Received Message is of ESUPL_AUTH_RESPONCE/ESUPL_AUTH_REQ type..."), KTraceFileName, __LINE__); 									
						iSuplSessionState = ESUPL_RECEIVED;
						CancelAllOperations();
						iSuplMsgType = ESUPL_AUTH_RESP; 
						ProcessSuplAuthResponceL();
						break;
					}
					
			default:
				{
				 iSuplSessionState = ESUPL_RECEIVED;	
 			 	 iTrace->Trace(_L("Received Message is of UNKNOWN type..."), KTraceFileName, __LINE__); 									
				 CancelAllOperations();
				 HandleSuplErrorL(KErrOMASuplProtocolError);
				 break;	

				}
			}
	}
	

// -----------------------------------------------------------------------------
// COMASuplSession::CancelAllOperations
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::CancelAllOperations()
	{
	if(iSuplState)
		{
			iSuplState->CancelOperation();
		}
			
	iConnRequestor->Cancel();
	iCompleteSelfRequestor->Cancel();
		
	if(iOMASuplPOSRequestor)
		iOMASuplPOSRequestor->Cancel(); 
	
	iTimer->StopTimer();	 
	}

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplResponse
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplResponseL()
	{
		iTrace->Trace(_L("Processing SUPL Response..."), KTraceFileName, __LINE__); 							
		
		// Recreate SUPL STATE
		delete iSuplState;
		iSuplState = NULL;
		
		iSuplState = COMASuplResponseState::NewL();
		iSuplState->SetVersion(iSuplVersion);
		TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
		iSuplMsgType = ESUPL_RESPONSE;
		
		iPosMethod = static_cast<COMASuplResponseState *>(iSuplState)->GetPosMethod();
		
		TBuf<128> msg(_L("ProcessSuplResponseL() got iPosMethod: "));
		msg.AppendNum(iPosMethod);
		iTrace->Trace(msg, KTraceFileName, __LINE__);
		if(KErrNone == err)
			{
				iSuplSessionState = ESUPL_GENERATE;
				UpdateSuplSessionIDL();
				OperationCompleteL(KErrNone);	
			}
		else
			{
				HandleSuplErrorL(err);
			}

	}

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplPosL
// Process SUPL POS
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplPosL()
	{
		iTrace->Trace(_L("Processing SUPL Pos..."), KTraceFileName, __LINE__); 							
		TInt error = KErrNone;
		
		// Recreate SUPL STATE if the previous state was not ESUPL_POS
		if(iSuplState)
			{
				if(COMASuplState::ESUPL_POS != iSuplState->GetSuplState())
					{
					delete iSuplState;
					iSuplState = NULL;
					
					iSuplState = COMASuplPosState::NewL(iPOSSession,iUT3_PosTimer,iSETCapabilities,iOMASuplAsnHandlerBaseImpl);					
					}
					
			iSuplState->SetVersion(iSuplVersion);
			error = iSuplState->ProcessMessageL(iDecodedAsnMessage);
			
			//This is for generating SUPL_POS, after UT3 timer started & received SUPL_POS.

			//!iIsFirstPOSMessage this is for putting new request if we
			 //gets SUPL_POS even if first POS message from SET failed to generate data 
			if( KErrNone == error && ( IsLastPOSMessage() || !iIsFirstPOSMessage )) 
				{
					iTrace->Trace(_L("Generating POS ... After starting UT3 OR After failing of Generation of first POS message..."), KTraceFileName, __LINE__); 
					iSuplMsgType = ESUPL_POS;
					iSuplSessionState = ESUPL_GENERATE;
					GenerateSuplMessageL();
					iSuplSessionState = ESUPL_ENCODE;
				}
			}
		
		if(KErrNone != error)
			{				
				TBuf<64> msg(_L("Error while Processing SUPL Pos is : "));
				msg.AppendNum(error);
				iTrace->Trace(msg, KTraceFileName, __LINE__); 
				HandleSuplErrorL(error);
			}

	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplEndL
// Process SUPL RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplEndL()
	{
		iTrace->Trace(_L("COMASuplSession::ProcessSuplEndL..."), KTraceFileName, __LINE__); 							
		
		// Recreate SUPL STATE
		delete iSuplState;
		iSuplState = NULL;
		
		iSuplState = COMASuplEndState::NewL(COMASuplEnd::EUnspecified,iOMASuplAsnHandlerBaseImpl);
		iSuplState->SetVersion(iSuplVersion);
		TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
		
		LogEndTime(); //Log end time if QoP & delay in Qop is present.
		
		if(KErrOMASuplVersionNotMatching != err) //KErrNone == err)..changed for new requirement...
												 // Even if there is status code in SUPL_END ... still process SUPL_END 	
			{
				if(iRequestType == COMASuplSession::ESUPL_TERMINAL)
					{
						// Delete Position
						delete iPosition;
						iPosition = NULL;
						
						COMASuplEndState *currState = static_cast <COMASuplEndState *>(iSuplState);
                        COMASuplEnd::TOMASuplStatusCode statusCode = currState->GetStatusCode();

                        if (statusCode == COMASuplEnd::EPosMethodMismatch)
                            {                                
                            iFailedCaps = iAllowedCapabilities;
                            iCapsFail = ETrue;
                            }                                    
						
						// Retrieve Position
						COMASuplPosition *currPosition = currState->GetPosition();
						if(currPosition)  //Position is in SUPL_END
							{
							
							iPosition =static_cast <COMASuplPosition *>(currPosition->CloneL());	

								if(KErrNone == err)
								{
									iTrace->Trace(_L("Received SUPL_END Without Status Code"), KTraceFileName, __LINE__); 							
									// Invoke SessionTerminate
									iSessionObserver.TerminateSession(this,KErrNone);
								}
								else  // Position & status code is present...thats why completing with KErrCompletion.
								{
									iTrace->Trace(_L("Received SUPL_END With Status Code"), KTraceFileName, __LINE__); 							
									// Invoke SessionTerminate
									// Changed error code as location Fw uses position if it completes with +ve value. //KErrCompletion
									iSessionObserver.TerminateSession(this,KSuplPositionPresent);
								}
							}
						else
							{
							if(iOMASuplPOSRequestor)
								{
								 iPosition = COMASuplPosition::NewL();
								 iOMASuplPOSRequestor->SetObserver(this);
								 err = iOMASuplPOSRequestor->GetPositionL(iPosition);
								 if(err != KErrNone)
								 	{
								 		delete iPosition;
								 		iPosition = NULL;
										// Invoke SessionTerminate
										iSessionObserver.TerminateSession(this, err);	
								 	}
								}
								
							else //Position && iOMASuplPOSRequestor are not there
								{
									// Invoke SessionTerminate
									iSessionObserver.TerminateSession(this, KErrNone);
								}	
								
							}
				  	 } 
				else
					{
						// Invoke SessionTerminate //Close session
						iSessionObserver.TerminateSession(this, KErrNone);	
					}				  	 
			}
		else
			{
				//  Handle Error
				HandleSuplErrorL(err);
			}
	}
	

// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplInitL
// Process SUPL INIT
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplInitL()
	{
		iTrace->Trace(_L("COMASuplSession::ProcessSuplInitL..."), KTraceFileName, __LINE__); 							
		
		TInt ret = ServerAddressCheckForSuplInitL();
		if(ret != KErrNone)
		{
				HandleSuplErrorL(KErrGeneral);
				return;
		}
		
		// Recreate SUPL STATE
		delete iSuplState;
		iSuplState = NULL;
		UpdateSuplSessionIDL();
		iSuplState = COMASuplInitState::NewL(iPrivacyTimer,iHSLPAddress,*this,iSETMode,iNetworkPrivacy);
		iSuplState->SetVersion(iSuplVersion);
		TInt err = iSuplState->ProcessMessageL(iDecodedAsnMessage);//will result in PrivacyCheckCompleteL
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	
		
		iPosMethod  = static_cast<COMASuplInitState*>(iSuplState)->GetPosMethod();
		
		TBuf<128> msg(_L("COMASuplSession::ProcessSuplInitL() got iPosMethod : "));
		msg.AppendNum(iPosMethod);
		iTrace->Trace(msg, KTraceFileName, __LINE__);
		if(KErrOMASuplNoPosition == err)
			{
				HandleSuplInitErrorL(KErrOMASuplNoPosition);
				return;
			}
			
		if(KErrNone != err)
			{
				HandleSuplErrorL(err);
			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::ProcessSuplAuthResponceL
// Process SUPL AUTH  RESPONSE
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::ProcessSuplAuthResponceL()
	{
		// No need to create state class object for now....
		// if needed in future ... we can write it...since it will add more code which is 
		//equivalent to current implementation.
		
		delete iSuplState;
		iSuplState = NULL;
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplReceived);	// inform to POS
		HandleSuplErrorL(KErrOMASuplProtocolError);
	}
// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSuplSessionIDL
//  Update SLP Sesssion ID with rececived slp id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSuplSessionIDL()	
{
	iTrace->Trace(_L("COMASuplSession::UpdateSuplSessionIDL"), KTraceFileName, __LINE__); 		
	COMASuplSessionID* retrivedSessionID = NULL;
	
	if(!iDecodedAsnMessage)
		return;

	iDecodedAsnMessage->SessionId(retrivedSessionID);
	UpdateSLPSessionIDL(retrivedSessionID);
	
}

// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSessionID
// Update SLP Sesssion ID with received session ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSLPSessionIDL(COMASuplSessionID *aSessionID)
	{
	iTrace->Trace(_L("Start of COMASuplSession::UpdateSLPSessionIDL"), KTraceFileName, __LINE__); 		
	if(aSessionID)
	{
		COMASuplSLPSessionID* suplSLPID = NULL;
		aSessionID->SLPSessionID(suplSLPID);
		
		if(suplSLPID)
			{
				TBuf8<KSLPSessionID> sessionId;
				COMASuplSLPSessionID* slpNewSessionId  = COMASuplSLPSessionID::NewL();		
				CleanupStack::PushL(slpNewSessionId);
				COMASuplSLPAddress* slpNewIPAddress = COMASuplSLPAddress::NewL();
				CleanupStack::Pop(slpNewSessionId);
				
				COMASuplSLPAddress* slpIPAddress = NULL;
				suplSLPID->SuplSLPSessionID(sessionId,slpIPAddress);
				slpNewSessionId->SetSuplSLPSessionID(sessionId,slpNewIPAddress);
				
				if(slpIPAddress)
					{	
						COMASuplSLPAddress::TSuplSLPAddressType addressType = slpIPAddress->SLPAddressType();
						if(addressType == COMASuplSLPAddress::EFqdn)
						{
							HBufC8* fqdn = NULL;
							TInt err = slpIPAddress->Fqdn(fqdn);
							if(err == KErrNone)
								{
									//Ownership transfer towords COMASuplSLPAddress
									HBufC8* newFqdn = fqdn->AllocL();
									slpNewIPAddress->SetFqdn(newFqdn);
								}
						}
						
						if(  addressType == COMASuplSLPAddress::EIPv4Address )	
						{
							TBuf8<KIPv4Address> ipAddress;
							TInt err = slpIPAddress->IPvAddress(ipAddress);
							if(err == KErrNone)
								slpNewIPAddress->SetIPv4Address(ipAddress);
						}
						
						if(addressType == COMASuplSLPAddress::EIPv6Address )	
						{
							TBuf8<KIPv6Address> ipAddress;
							TInt err = slpIPAddress->IPvAddress(ipAddress);
							if(err == KErrNone)
								slpNewIPAddress->SetIPv6Address(ipAddress);
						}
					}
					
				iSuplSessionId->SetSLPSessionID(slpNewSessionId);	
			}
			
		///////////log //////////////////
		
		#if defined(_DEBUG)  //This is only for logging purpose. 
			iTrace->Trace(_L("After UpdateSuplSessionIDL"), KTraceFileName, __LINE__); 		
			TBuf<128> msg; 
			COMASuplSETSessionID* suplSETID = NULL;
			iSuplSessionId->SETSessionID(suplSETID);
			
			COMASuplSLPSessionID* suplSlpID = NULL;
			iSuplSessionId->SLPSessionID(suplSlpID);
			
			//SET Part			
			if(suplSETID)
				{
					msg.Copy(_L("SET Session ID : "));
					TBuf<4> id;	
					TInt      SETSessionID;
					TBuf8<32> IPvAddress8;
					TBuf<32>  ipAddress;
					suplSETID->SuplSETSessionID(SETSessionID,IPvAddress8);	
					
					TLex8 lex(IPvAddress8);
					TChar chr;	
					
					chr = lex.Get();
					TUint i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));
					
					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					
					id.AppendNum(SETSessionID);
					msg.Append(id);
					msg.Append(_L("  IPAddress/IMSI :"));
					msg.Append(ipAddress);
				}
			else
				{
					msg.Copy(_L("SET Session Id is not set."));
				}
				
			iTrace->Trace(msg,KTraceFileName, __LINE__); 							
			
			//SLP Part			
			if(suplSlpID)
				{
					msg.Copy(_L("SLP Session ID : "));
					TBuf8<4> sessionId8;
					TBuf<32> sessionId;
					TBuf8<64> ipAddress8(_L8("No Address field in SLP Session ID"));				
					TBuf<64> ipAddress;

					COMASuplSLPAddress* slpIPAddress = NULL;
					suplSlpID->SuplSLPSessionID(sessionId8,slpIPAddress);
					
					if(slpIPAddress)
						{
							TInt err = slpIPAddress->IPvAddress(ipAddress8);	
							if(err != KErrNone)
								{
									msg.Copy(_L("SLP Address is not set."));
									iTrace->Trace(msg,KTraceFileName, __LINE__); 								
									return;
								}
						}
					
					TLex8 lex(ipAddress8);
					TChar chr;	
					
					chr = lex.Get();
					TUint i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));
					
					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					ipAddress.Append(_L("."));

					chr = lex.Get();
					i = chr;
					ipAddress.AppendNum(i);
					
					TLex8 lex1(sessionId8);
					for(i = 0; i < 4; ++i)	
						{
							chr = lex1.Get();
							TUint num = chr;
							sessionId.Append(chr);
						}
						
					msg.Append(sessionId);
					msg.Append(_L("  IPAddress : "));
					msg.Append(ipAddress);
					iTrace->Trace(msg,KTraceFileName, __LINE__); 								
					
				}
			else
				{
					msg.Copy(_L("SLP Session Id is not set."));
					iTrace->Trace(msg,KTraceFileName, __LINE__); 								
				}
				
		#endif
		////////////////////////////////			
	}
}

// -----------------------------------------------------------------------------
// COMASuplSession::UpdateSETSessionID
// Update SET Sesssion ID with received session ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::UpdateSETSessionIDL(COMASuplSessionID *aSessionID)
	{
	
	iTrace->Trace(_L("Start of COMASuplSession::UpdateSETSessionIDL"), KTraceFileName, __LINE__); 		
	if(aSessionID)
	{
		COMASuplSETSessionID* retrivedSETSessionId = NULL;
		aSessionID->SETSessionID(retrivedSETSessionId);
		
		if(retrivedSETSessionId)
			{
				COMASuplSETSessionID::TSuplSETIDType addressType = retrivedSETSessionId->SuplSETIDType();
				
				if(addressType == COMASuplSETSessionID::EIPv4Address)
					{
						TInt sessionUniqueNo;
						TBuf8<KIP4AddressLength>  IPvAddress;
						retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo,IPvAddress);
						
						COMASuplSETSessionID* suplSETSessionId = NULL;
						iSuplSessionId->SETSessionID(suplSETSessionId);
						suplSETSessionId->SetSuplSETSessionIDIPv4(sessionUniqueNo,IPvAddress);
                        iSessionIDFlag = ETrue;
					}
				else if(addressType == COMASuplSETSessionID::EIPv6Address)
					{
						TInt sessionUniqueNo;
						TBuf8<KIP6AddressLength>  IPvAddress;
						retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo,IPvAddress);

						COMASuplSETSessionID* suplSETSessionId = NULL;
						iSuplSessionId->SETSessionID(suplSETSessionId);
						suplSETSessionId->SetSuplSETSessionIDIPv6(sessionUniqueNo,IPvAddress);
                        iSessionIDFlag = ETrue;
					}
				else    // IMSI
				    {
						TInt sessionUniqueNo;
						TBuf8<KIMSILength>  IMSI;
						retrivedSETSessionId->SuplSETSessionID(sessionUniqueNo, IMSI);

						COMASuplSETSessionID* suplSETSessionId = NULL;
						iSuplSessionId->SETSessionID(suplSETSessionId);
						suplSETSessionId->SetSuplSETSessionIDIMSI(sessionUniqueNo, IMSI);
                        iSessionIDFlag = ETrue;
				    }
			}
	} 
	iTrace->Trace(_L("End of COMASuplSession::UpdateSETSessionIDL"), KTraceFileName, __LINE__); 			
}

// -----------------------------------------------------------------------------
// COMASuplSession::CreateCloneMessageL
// Creates Clone of COMASuplAsnMessageBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::CreateCloneMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)	
	{		
		iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL"), KTraceFileName, __LINE__); 		
		delete iDecodedAsnMessage; 
		iDecodedAsnMessage = NULL;

		COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();
		TInt error;
		switch(messageType)
			{
			case COMASuplAsnMessageBase::ESUPL_START:
					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_START"), KTraceFileName, __LINE__); 		
					iDecodedAsnMessage = COMASuplStart::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
	                break;
			case COMASuplAsnMessageBase::ESUPL_RESPONSE:
					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_RESPONSE"), KTraceFileName, __LINE__); 		
					iDecodedAsnMessage = COMASuplResponse::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
	                break;
	        case COMASuplAsnMessageBase::ESUPL_POSINIT:
					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_POSINIT"), KTraceFileName, __LINE__); 		
					iDecodedAsnMessage = COMASuplPosInit::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
	                break;	                
	        case COMASuplAsnMessageBase::ESUPL_POS:
	        		iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : SUPL_POS"), KTraceFileName, __LINE__); 		
					iDecodedAsnMessage = COMASuplPos::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
	                break;
	                
 			case COMASuplAsnMessageBase::ESUPL_END:
 					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_END"), KTraceFileName, __LINE__); 		
 					iDecodedAsnMessage = COMASuplEnd::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
 				 	break;
 			case COMASuplAsnMessageBase::ESUPL_INIT:
 					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_INIT"), KTraceFileName, __LINE__); 		
 					iDecodedAsnMessage = COMASuplInit::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
 				 	break;
 			case COMASuplAsnMessageBase::ESUPL_AUTH_RESP:
 					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_AUTH_RESP"), KTraceFileName, __LINE__); 		
 					iDecodedAsnMessage = COMASuplAuthResponse::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
 				 	break;
 			case COMASuplAsnMessageBase::ESUPL_AUTH_REQ:
 					iTrace->Trace(_L("COMASuplSession::CreateCloneMessageL : ESUPL_AUTH_RESP"), KTraceFileName, __LINE__); 		
 					iDecodedAsnMessage = COMASuplAuthRequest::NewL(); 
					iDecodedAsnMessage->Clone(aDecodedAsnMessage,error);
 				 	break;
 				 	
			default:
				 break;	
			}
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::CancelSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::CancelSession()
{
		iTrace->Trace(_L("COMASuplSession::CancelRunSession"), KTraceFileName, __LINE__); 		
		CancelAllOperations();
		iConnRequestor->CancelReadOperation();
		
		switch(iSuplSessionState)
			{
				case ESUPL_INITIALIZED:
				case ESUPL_CONNECTING:
				case ESUPL_SESSION_ERROR:
					{
						iSuplSessionState = ESUPL_SESSION_ERROR;
					  	break;
					}
					
				case ESUPL_RECEIVED:
					{	
						iSuplSessionState = ESUPL_SESSION_ERROR;
						if(iSuplMsgType == ESUPL_END)
							{
								if(iOMASuplPOSRequestor)
									iOMASuplPOSRequestor->Cancel();
							}

						break;
					}
					
				case ESUPL_GENERATE:
				case ESUPL_ENCODE:
					{
						if(iSuplMsgType == ESUPL_START)	//No need to send SUPL_END to SLP
						{
							iSuplSessionState = ESUPL_SESSION_ERROR;
							break;
						}
					}
				case ESUPL_SEND:
					{
						iErrorStatusCode = COMASuplEnd::EUnspecified;
						iSuplSessionState = ESUPL_GENERATE;
						iSuplMsgType = ESUPL_END;
						if(iOMASuplPOSRequestor) //Reset observer
							{
								iOMASuplPOSRequestor->SetObserver(NULL);	
							}
						TRAP_IGNORE(GenerateSuplMessageL());				
						//iSuplSessionState = ESUPL_SESSION_ERROR; Cancel Bug
						break;
					}
				default: break;
			}


}

// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::CancelRunSession()
{
	CancelSession();
	if(iSuplSessionState == ESUPL_SEND)
	{
		CompleteSession(KErrCancel);
	}
	else
	{
			//Close Connection : bug Fix.
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		iSessionObserver.TerminateSession(this, KErrCancel);	
	}
	
}
// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleGenerationErrorL(TInt aErr)
	{
		TBool cancelSendRequestor  = ETrue;
		if(iSuplState)
		{
			switch(iSuplState->GetSuplState())
			{
				case COMASuplState::ESUPL_START:
				{
					break;
				}
				case COMASuplState::ESUPL_POS:
				case COMASuplState::ESUPL_POS_INIT:
				{
					iErrorStatusCode = COMASuplEnd::EUnspecified;
					if(aErr == KErrSuplposMethodMismatch)
						iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;//COMASuplEnd::EPosMethodMismatch;
					cancelSendRequestor = EFalse;
					iSuplSessionState = ESUPL_GENERATE;
					iSuplMsgType = ESUPL_END;
					GenerateSuplMessageL();
					break;		
				}
				case COMASuplState::ESUPL_END:
				{
					//Close Connection : bug Fix.
					iConnRequestor->CloseConnection();
                    iPortNum = 0;
					iSessionObserver.TerminateSession(this, aErr);
					cancelSendRequestor = EFalse;
					break;
				}
				default: break;
			}
		}
		
		if(cancelSendRequestor)
		{
			CancelAllOperations();
			CleanUp();
			CompleteSession(aErr);
		}
		
	}
// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandleInitilizationErrorL(TInt aErr)
	{
		CancelAllOperations();
		CleanUp();
		CompleteSession(aErr);
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleConnectionErrorL(TInt aErr)
	{
		CancelAllOperations();
		//Close Connection
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		iSessionObserver.TerminateSession(this, aErr);
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::HandleEncodingErrorL(TInt aErr)
{
		TBool cancelSendRequestor  = ETrue;
		if(iSuplState)
		{
			iErrorStatusCode = COMASuplEnd::EUnspecified;					
			
			if(aErr == KErrOMASETCapMisMatch )
				{
					iErrorStatusCode = COMASuplEnd::EPosMethodMismatch;
                    iFailedCaps = iAllowedCapabilities;
                    iCapsFail = ETrue;
				}
				
			if(aErr == KErrSuplposMethodMismatch)
				{
					iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;
				}
			if(aErr == KErrOMAManParamMissing)
				{
				iErrorStatusCode = COMASuplEnd::EDataMissing;
				}
				
			if(aErr == KErrArgument)
				{
					iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;
				}
				
			if(aErr == KErrOMASuplPosInfo || aErr == KErrOMASuplPosInActive || aErr == KErrOMASuplDataMissing)
				{
					iErrorStatusCode = COMASuplEnd::EDataMissing;
				}
				
			switch(iSuplState->GetSuplState())
			{
				case COMASuplState::ESUPL_START:break;
				case COMASuplState::ESUPL_POS: 
					{
					//This is for,even though we gets error(no pos payload data) for first POS message from plugin
					// Just ignore it ... Don't put new pending request for POS message plugin.
						if(iIsFirstPOSMessage && ( aErr == KErrOMAManParamMissing || aErr ==  KErrOMASuplParamNotSet || aErr == KErrOMASuplPosInfo))
							{
								iTrace->Trace(_L("Failed to Generate First messsage from POS...still continueing.."), KTraceFileName, __LINE__); 		
								iIsFirstPOSMessage = EFalse;
								return;
							}
					}
				case COMASuplState::ESUPL_POS_INIT:
				{
					cancelSendRequestor = EFalse;
					iSuplSessionState = ESUPL_GENERATE;
					iSuplMsgType = ESUPL_END;
					GenerateSuplMessageL();
					break;			
				}
				
				case COMASuplState::ESUPL_END:
				{
					iConnRequestor->CloseConnection();
                    iPortNum = 0;
					iSessionObserver.TerminateSession(this, KErrGeneral);
					return;
				}
				
				default: break;
			}
		}
		
		if(cancelSendRequestor)
		{
			CancelAllOperations();
			CleanUp();
			CompleteSession(KErrGeneral);
		}
		
}
// -----------------------------------------------------------------------------
// COMASuplSession::CancelRunSession
// Cancels an in-progress RunSession on the Session
// What action to be taken for cancelRunSession will be depend on State of session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandleSendErrorL(TInt aErr)
	{
		
    // if error code is KErrDisconnected, then close connection
    // and set the state accordingly            
    if (aErr == KErrDisconnected)
        {
		//Close Connection
		iConnRequestor->CloseConnection();
		iSessionObserver.TerminateSession(this, aErr);
        }
    else
        { 
		TBool cancelSendRequestor  = ETrue;
		if(aErr == KErrTimedOut)				
		{
			iErrorStatusCode = COMASuplEnd::EUnspecified;
			cancelSendRequestor = EFalse;
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
		}
		
		if(aErr == KErrSuplposMethodMismatch)				 
		{
			iErrorStatusCode = COMASuplEnd::EPosProtocolMismatch;
			cancelSendRequestor = EFalse;
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
		}
		
		aErr = KErrCompletion;
		
		if(cancelSendRequestor)
		{
			CancelAllOperations();
			CleanUp();
			CompleteSession(aErr);
		}
		}

	}
// -----------------------------------------------------------------------------
// COMASuplSession::HandlePacketsErrorL
// HandlePacketsErrorL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//		
void COMASuplSession::HandlePacketsErrorL(TInt aErr)
{
	TBool cancelSendRequestor  = ETrue;
	
	if(iSuplMsgType == ESUPL_INIT)
		{
			HandleSuplInitErrorL(aErr);
			return;
		}
	
	if(aErr == KErrSuplEndProtocolError )
	{
		aErr = KErrCompletion;
		switch(iSuplMsgType)
		{
			case ESUPL_START: 
			case ESUPL_RESPONSE:
			case ESUPL_END:
			{
				//Close Connection
				iConnRequestor->CloseConnection();
                iPortNum = 0;
				iSessionObserver.TerminateSession(this, aErr);
				break;
			}
			default: break;
		}
	}

	if(aErr == KErrOMASuplProtocolError )
	{
		aErr = KErrCompletion;
		switch(iSuplMsgType)
		{
			case ESUPL_START: 
			case ESUPL_INIT:
			case ESUPL_POS:
			case ESUPL_RESPONSE:
			case ESUPL_POSINIT:
			case ESUPL_AUTH_RESP:
			{
				iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
				cancelSendRequestor = EFalse;
				iSuplSessionState = ESUPL_GENERATE;
				iSuplMsgType = ESUPL_END;
				GenerateSuplMessageL();				
				break;			
			}
			
			case ESUPL_END:
			{
				//Close Connection
				iConnRequestor->CloseConnection();
                iPortNum = 0;
				iSessionObserver.TerminateSession(this, aErr);
				break;
			}
			
			default: break;
		}
	}

	if( aErr == KErrOMANonProxyModeNotSupported) //aErr == KErrOMASuplVersionNotMatching ||
		{
			iErrorStatusCode = COMASuplEnd::ENonProxyModeNotSupported;
			
			aErr = KErrCompletion;
			
			if(iSuplState)
			{
				switch(iSuplState->GetSuplState())
				{
					case COMASuplState::ESUPL_RESPONSE:
					{
						cancelSendRequestor = EFalse;
						iSuplSessionState = ESUPL_GENERATE;
						iSuplMsgType = ESUPL_END;
						GenerateSuplMessageL();
						break;
					}
					default: break;
				}
			}
		}
		
		if(cancelSendRequestor)
		{
			CancelAllOperations();
			CleanUp();
			CompleteSession(aErr);
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::HandleSuplInitErrorL
// Handle Init related error
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleSuplInitErrorL(TInt aErr)
	{
		iTrace->Trace(_L("Start COMASuplSession::HandleSuplInitError"), KTraceFileName, __LINE__); 
		
		if(aErr == KErrOMAInvalidHSLPAddress)
			{
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError HSLP Address is not matching..."), KTraceFileName, __LINE__);
				//Close Connection : bug Fix.
				iConnRequestor->CloseConnection();
                iPortNum = 0;
				iSessionObserver.TerminateSession(this, KErrCompletion);
				return;
			}
				
		TBool setStatusCode = ETrue;
		
	
		
		if(aErr == KErrSuplInvalidSessionID)
			{
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Received SET Session ID"), KTraceFileName, __LINE__);
				iErrorStatusCode = COMASuplEnd::EInvalidSessionId;	

			    COMASuplSessionID* retrivedSessionID = NULL;
			    iDecodedAsnMessage->SessionId(retrivedSessionID);

			    // Set the SessionId.
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Update SET Session ID"), KTraceFileName, __LINE__);
			    UpdateSETSessionIDL(retrivedSessionID);

			}
			
		if(aErr == KErrOMANonProxyModeNotSupported)
			{
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Received Nonproxymode data"), KTraceFileName, __LINE__);
				iErrorStatusCode = COMASuplEnd::ENonProxyModeNotSupported;	
			}

		if(KErrAccessDenied == aErr || KErrOMASuplDenied == aErr)
			{
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError KErrAccessDenied|| KErrOMASuplDenied"), KTraceFileName, __LINE__);
				iErrorStatusCode = COMASuplEnd::EConsentDeniedByUser;	
			}
	
		if(aErr == KErrOMASuplNoPosition || KErrOMASuplInformationOnly == aErr)
			{
				//Supl End with No Position
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError KErrOMASuplNoPosition || KErrOMASuplInformationOnly"), KTraceFileName, __LINE__);
				setStatusCode = EFalse;
			}
		
		if(KErrOMASuplAccepted == aErr)
			{
				iTrace->Trace(_L("COMASuplSession::HandleSuplInitError EConsentGrantedByUser"), KTraceFileName, __LINE__);
				iErrorStatusCode = COMASuplEnd::EConsentGrantedByUser;	
			}
		
		TInt len = iHSLPAddress.Length();
		HBufC8 *hslpAdress = NULL;
		if(len > 0)
		{
			hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
			hslpAdress->Des().Copy(iHSLPAddress);
		}
		
			else if(len ==0)
				{
					CServerParams* serverParams = CServerParams::NewL();
					CleanupStack::PushL(serverParams);
					
						if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
						{
						iTrace->Trace(_L("HSLP generated frm IMSI"), KTraceFileName, __LINE__);
						hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
						CleanupStack::PushL(hslpAdress);
						hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());
						CleanupStack::Pop(hslpAdress);	
						}
						else
						{
							iTrace->Trace(_L("Default HSLP"), KTraceFileName, __LINE__);
							TInt64 slpId;
							TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
							HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
							HBufC* iapName = HBufC::NewL(KMaxIAPLen);
							CleanupStack::PushL(serverAddr);
							CleanupStack::PushL(iapName);
							serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
							hslpAdress = HBufC8::NewL(serverAddr->Length());
							hslpAdress->Des().Copy(*serverAddr);
						
							CleanupStack::PopAndDestroy(iapName);
							CleanupStack::PopAndDestroy(serverAddr);
							
						}
					CleanupStack::PopAndDestroy(serverParams);	
					
				}
            delete iSuplState;
            iSuplState = NULL;
            CleanupStack::PushL(hslpAdress);
            if(setStatusCode)
                {
                iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);		
                }
            else
                {
                iSuplState = COMASuplEndState::NewL(iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress );
                } 		
 			
 		CleanupStack::PopAndDestroy(hslpAdress);
 		
		iTrace->Trace(_L("COMASuplSession::HandleSuplInitError Send SUPL_END for SUPL_INIT"), KTraceFileName, __LINE__);
		
		//iSuplSessionState = ESUPL_GENERATE;		
		iSuplSessionState = ESUPL_INITIALIZED;
		iSuplState->SetMsgStateObserver(this);
		iSuplMsgType = ESUPL_END;
		SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	
		//SMP Changes
		
		iSuplState->GenerateMessageL();
		iSuplSessionState = ESUPL_GENERATE;//Has to be here because initialisation has to be done for NI case in reject scenario.
	
	}

// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidParameterErrorL
// Handles error occured related invalid parameter.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::HandleInvalidParameterErrorL(COMASuplAsnMessageBase* aDecodedAsnMessage)
{
	if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version
		{
	COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsnMessage->MessageType();
	CancelAllOperations();	
	if(messageType == COMASuplAsnMessageBase::ESUPL_END)
		{
			//Close Connection : bug Fix.
			iConnRequestor->CloseConnection();
            iPortNum = 0;
			iSessionObserver.TerminateSession(this, KErrCompletion);
		}
	else
		{
		
			iErrorStatusCode = COMASuplEnd::EUnexpectedDataValue;
			
			if((messageType == COMASuplAsnMessageBase::ESUPL_POSINIT || 
				messageType == COMASuplAsnMessageBase::ESUPL_START || 
				messageType == COMASuplAsnMessageBase::ESUPL_RESPONSE) 
				&& (iRequestType == ESUPL_NETWORK))
				{
					iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
				}
			
			if(iRequestType == ESUPL_TERMINAL )					//Protocol errors
				{
					if( !	(iSuplMsgType == ESUPL_START && messageType == COMASuplAsnMessageBase::ESUPL_RESPONSE)
							|| 				
							(iSuplMsgType == ESUPL_POSINIT && messageType == COMASuplAsnMessageBase::ESUPL_POS)
					  )		
							
						{
							iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
						}
				}
			
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
				}
		}	
}
// -----------------------------------------------------------------------------
// COMASuplSession::TerminateSession
// Terminates session
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSession::TerminateSession()
{
	iTrace->Trace(_L("COMASuplSession::TerminateSession"), KTraceFileName, __LINE__);
	if(iSuplSessionState != ESUPL_SESSION_ERROR)
		{
			CancelRunSession();
		}
	else
		{
			SessionEnd();
			CompleteSession(KErrCancel);
		}
	return;
}

// -----------------------------------------------------------------------------
// COMASuplSession::SuplVersion
// returns SUPL Version
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal COMASuplSession::SuplVersion() 
{
	return KSuplPOSPluginVersion;
}

// -----------------------------------------------------------------------------
// COMASuplSession::SetPOSMsgPluginState
// Informs Protocol state to POS if present
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void COMASuplSession::SetPOSMsgPluginState(COMASuplPosSessionBase::TOMASuplMsgState aSuplMsgState)
{
	if(iPOSSession != NULL)
		{
			iTrace->Trace(_L("Setting State of POS Msg Plugin"), KTraceFileName, __LINE__); 															
			
			COMASuplPosSessionBase::TOMASuplMsg SuplMsg = COMASuplPosSessionBase::EOMASuplStart;
			
			switch(iSuplMsgType) 
			{
				case ESUPL_START:
					SuplMsg = COMASuplPosSessionBase::EOMASuplStart;	
					break;
				case ESUPL_RESPONSE:
					SuplMsg = COMASuplPosSessionBase::EOMASuplResponse;	
					break;			
				case ESUPL_INIT:
					SuplMsg = COMASuplPosSessionBase::EOMASuplInit;	
					break;			
				case ESUPL_POSINIT:
					SuplMsg = COMASuplPosSessionBase::EOMASuplPosInit;	
					break;			
				case ESUPL_POS:
					SuplMsg = COMASuplPosSessionBase::EOMASuplPos;	
					break;			
				case ESUPL_END:
					SuplMsg = COMASuplPosSessionBase::EOMASuplEnd;				
					break;			
				default:			
					 break;
			}
			
		iPOSSession->SetSuplState(SuplMsg,aSuplMsgState); 
		}
		
}
// -----------------------------------------------------------------------------
// COMASuplSession::IsLastPOSMessage
// Checks about last POS message with POS handler
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TBool COMASuplSession::IsLastPOSMessage()
{
	if( iPOSSession && iSuplState)
		{
			COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
			TBool ret = state->IsLastMessage();
			return ret;
		}
	else 
		{
			return EFalse;	
		}
}
// -----------------------------------------------------------------------------
// COMASuplSession::IsSegmentedDone()
// Checks about segmentation of pospayload
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
TBool COMASuplSession::IsSegmentationDone()
{
	if( iPOSSession && iSuplState)
		{
			COMASuplPosState* state =  static_cast <COMASuplPosState *>(iSuplState);
			TBool ret = state->IsSegmentationDone();
			return ret;
		}
	else 
		{
			return EFalse;	
		}
}


// -----------------------------------------------------------------------------
// COMASuplSession::CopySETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::CopySETCapabilities()	 
	{
		if(iSuplMsgType == ESUPL_START)
			{
				COMASuplStartState *startState = static_cast <COMASuplStartState *>(iSuplState);
				COMASuplSETCapabilities *setCaps = startState->Capabilities();
				
				TOMASuplPosTechnology posTech;
				COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
				TOMASuplPosProtocol posProtocol;
		
				setCaps->GetSETCapabilities(posTech,prefMethod,posProtocol);
				
				iSETCapabilities->SetSETCapabilities(posTech,prefMethod, posProtocol);
			}
			
		if(iSuplMsgType == ESUPL_POSINIT && iRequestType == ESUPL_NETWORK)
			{
				TOMASuplPosTechnology posTechnology;
				COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
				TOMASuplPosProtocol   posProtocol;

				TBool  GpsSETAssisted 	= EFalse;
				TBool  GpsSETBased 		= EFalse;
				TBool  AutonomousGps 	= EFalse;
				TBool  FLT 			 	= EFalse;
				TBool  eOTD 		 	= EFalse;
				TBool  oTDOA 		 	= EFalse;

				TBool TIA801 = EFalse;
				TBool RRLP = EFalse;
				TBool RRC =  EFalse;

				posTechnology.SetPosTechnology(GpsSETAssisted,GpsSETBased,AutonomousGps,FLT,
				eOTD,oTDOA);
				posProtocol.SetPosProtocol(TIA801,RRLP,RRC);
				prefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
				iSETCapabilities->SetSETCapabilities(posTechnology,prefMethod,posProtocol);	
			}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::PrivacyCheckCompleteL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::PrivacyCheckCompleteL(TInt aCompletionCode)
	{
		if(aCompletionCode == KErrNone)
			{
				iTrace->Trace(_L("COMASuplSession::PrivacyCheckCompleteL No Error"), KTraceFileName, __LINE__);
				static_cast<COMASuplInitState*>(iSuplState)->GetRequestorID(iRequestID);  //APE Centric
				iSuplSessionState = ESUPL_INITIALIZED;
				iSuplMsgType = ESUPL_POSINIT;
                if (iSuplUsage >= 2)
                    {                        
                    COMASuplInit* suplInit = static_cast <COMASuplInit *>(iDecodedAsnMessage);
                    COMASuplNotification* notif;
                    TInt error	= suplInit->Notification(notif);
                    COMASuplNotification::TOMASuplNotificationType notifType = notif->SuplNotification();
                    if (notifType == COMASuplNotification::ENotificationOnly)
                        {                            
				        InitializeL(iRequestID);
                        }
                    else
                        {                            
                        // iRequestID = 0;            //APE Centric
                        CheckForSuplUsageL();                            
                        }
                    }
                else
				    InitializeL(iRequestID); //APE Centric
			}
		else
			{
				iTrace->Trace(_L("COMASuplSession::PrivacyCheckCompleteL with Error "), KTraceFileName, __LINE__);
                if (iSuplUsage >= 2)                    
                    {
                        iNwInitError = ETrue;                            
                        iNwInitCompletionCode = aCompletionCode;
                        CheckForSuplUsageL();                            
				     }
                else
				HandleSuplInitErrorL(aCompletionCode);
			}	
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SetConfigurationParameters...sets configuration parameters
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::SetConfigurationParameters(TInt& aUT1_StartTimer,TInt& aUT2_PosInitTimer,
 	 											TInt& aUT3_PosTimer,TInt& aPrivacyTimer, 
												TInt& aSETMode,
                                                TInt& aSuplUsage,
                                                TInt& aPersistFailTimer,
                                                TInt& aSuplInitTimeOut)
	{
		iUT1_StartTimer = aUT1_StartTimer;
		iUT2_PosInitTimer = aUT2_PosInitTimer;
		iUT3_PosTimer = aUT3_PosTimer;
		iPrivacyTimer = aPrivacyTimer;
		iSETMode = aSETMode;
        iSuplUsage = aSuplUsage;
        iPersistFailTimer = aPersistFailTimer; 
        iSuplInitTimeOut = aSuplInitTimeOut;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::IsGenerationInProgress...check about status of generation of message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsGenerationInProgress()
	{
		if( iSuplMsgType == ESUPL_POSINIT && iSuplState)
		{
			COMASuplPosInitState* state =  static_cast <COMASuplPosInitState *>(iSuplState);
			TBool ret = state->IsGenerationInProgress();
			return ret;
		}
		else 
		{
			return EFalse;	
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::LogEndTime...log the end time if delay is present in QoP
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void COMASuplSession::LogEndTime()
	{
		if(!iIsQoPPresent) 
			{
				return;
			}
			
		TInt delay;
		if(iClientQop.GetDelay(delay) != KErrNotFound)
		{
			iSessionEndTime.HomeTime();
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SetIMSIL
// Updates IMSI value in SET Session ID
// -----------------------------------------------------------------------------
void COMASuplSession::SetIMSI()
    {
    
    //if( IsIMSIVariant() )
        {
	    iTrace->Trace(_L("COMASuplSession::SetIMSIL Start"), KTraceFileName, __LINE__);
	    TInt length = iIMSI.Length();
        TBuf8<KIMSILength> imsi;	    
        TUint8 val;
        TInt index = 0;
      
        while ( index <= length )
            {
            val = 0;
            if (index == length)
                    {
                        val = 15 << 4;
                        val |= 15;
                        imsi.Append(val);
                        break;
                    }
            const TUint16& t = iIMSI[index];
            TInt8 i = t - 48;
            if ( ++index != length )
                {
                const TUint16& h = iIMSI[index];
    			TInt8 j = h - 48;
                val = j << 4;
                val |= i;
                }
            else
                {
                val = 15 << 4;
                val |= i;
                }
    		
            imsi.Append(val);
            index++;
            }
       
        if (!iSessionIDFlag)
        {                
        COMASuplSETSessionID* suplSETSessionId = NULL;
		iSuplSessionId->SETSessionID( suplSETSessionId );
		suplSETSessionId->SetSuplSETSessionIDIMSI( iSETSessionUniqueId, imsi );
        }
        
        iTrace->Trace(_L("COMASuplSession::SetIMSI End"), KTraceFileName, __LINE__);
        }

    }
			
// -----------------------------------------------------------------------------
// COMASuplSession::IsIMSIVariant
// Checks to determine if IMSI needs to be included in the SUPL message
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsIMSIVariant()
    {
    if(iIMSI.Length() && 
    (
    (iRequestType == ESUPL_TERMINAL && iSuplMsgType == ESUPL_START) || 
    (iRequestType == ESUPL_NETWORK && iSuplMsgType == ESUPL_POSINIT) || 
    (iRequestType == ESUPL_NETWORK && iSuplMsgType == ESUPL_END)
    ))
        {
        return ETrue;
        }
    else
        {
        return EFalse;    
        }
    }



// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidMessageL
// Sends SUPL_END with status code with wrong message
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidMessageL(COMASuplAsnMessageBase* aDecodedAsnMessage)
	{
	
	if(CheckProtocolVersionL(aDecodedAsnMessage)) //Checks version with supported version 
		{
			iTrace->Trace(_L("COMASuplSession::HandleInvalidMessageL...Sending SUPL_END for invalid message"), KTraceFileName, __LINE__);
			iErrorStatusCode = COMASuplEnd::EUnexpectedMessage;
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidMessageLengthL
// Sends SUPL_END with status code with protocol error
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidMessageLengthL()
	{
			iTrace->Trace(_L("COMASuplSession::HandleInvalidMessageLengthL...Sending SUPL_END for invalid message length"), KTraceFileName, __LINE__);
			iErrorStatusCode = COMASuplEnd::EProtocolError;
			iSuplSessionState = ESUPL_GENERATE;
			iSuplMsgType = ESUPL_END;
			GenerateSuplMessageL();				
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::CheckProtocolVersion
// Returns true if its matching else false
// -----------------------------------------------------------------------------
TBool COMASuplSession::CheckProtocolVersionL(COMASuplAsnMessageBase* aDecodedAsn)
	{
    TOMASuplVersion version;
    COMASuplSessionID* SessionID = NULL;
    aDecodedAsn->MessageBase(version,SessionID);
    
    TInt recMajor,recMinor,recServInd;
    version.SuplVersion(recMajor,recMinor,recServInd);		
    
    if(recMajor == KSuplMajorVersion )
        {
        return  ETrue;
        } 
    else
        {
        COMASuplAsnMessageBase::TSuplMessageType messageType = aDecodedAsn->MessageType();
        if(messageType == COMASuplAsnMessageBase::ESUPL_END) 
            {
            iTrace->Trace(_L("Received message is SUPL_END...with wrong Version.Terminating session"), KTraceFileName, __LINE__); 
            iSessionObserver.TerminateSession(this, KErrCompletion);	
            }
        else
            {
            iTrace->Trace(_L("Received message with wrong version."), KTraceFileName, __LINE__); 

            delete iSuplState;
            iSuplMsgType = ESUPL_END;
            iErrorStatusCode = COMASuplEnd::EVersionNotSupported;	
            SetPOSMsgPluginState(COMASuplPosSessionBase::EOMASuplCreating);	

            if(iRequestType == ESUPL_NETWORK )
                {
                if (messageType == COMASuplAsnMessageBase::ESUPL_INIT)    
                    {                                
                    iSuplSessionState = ESUPL_INITIALIZED;
                    TInt len = iHSLPAddress.Length();
                    HBufC8 *hslpAdress = NULL;
                    if(len > 0)
                        {
                        hslpAdress = HBufC8::NewL(iHSLPAddress.Length());
                        hslpAdress->Des().Copy(iHSLPAddress);
                        }
                    else if(len ==0)
                        {
                        CServerParams* serverParams = CServerParams::NewL();
                        CleanupStack::PushL(serverParams);
        
                        if (iSuplStorageSettings->GetDefaultServer(serverParams) == KErrNotFound )
                            {
                            iTrace->Trace(_L("Length of HSLP Address is = 0, passing the HSLP generated frm IMSI"), KTraceFileName, __LINE__);
                            hslpAdress = HBufC8::NewL(iSuplSettings->SLPAddressfromImsi().Length());
                            hslpAdress->Des().Copy(iSuplSettings->SLPAddressfromImsi());	
                            }		
                        else
                            {
                            iTrace->Trace(_L("Sending End with ver for Default HSLP"), KTraceFileName, __LINE__);
                            TInt64 slpId;
                            TBool aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable;
                            HBufC* serverAddr = HBufC::NewL(KMaxHSLPAddrLen);
                            HBufC* iapName = HBufC::NewL(KMaxIAPLen);
                            CleanupStack::PushL(serverAddr);
                            CleanupStack::PushL(iapName);
                            serverParams->Get(slpId,serverAddr->Des(),iapName->Des(),aServerEnabled, aSimChangeRemove, aUsageInHomeNw, aEditable);
                            hslpAdress = HBufC8::NewL(serverAddr->Length());
                            hslpAdress->Des().Copy(*serverAddr);
            
                            CleanupStack::PopAndDestroy(iapName);
                            CleanupStack::PopAndDestroy(serverAddr);
                            }
                        CleanupStack::PopAndDestroy(serverParams);
                        }
                    if(iSuplState)
                    	{
                    delete iSuplState;
                    iSuplState = NULL;
                      }
                    CleanupStack::PushL(hslpAdress);
                    ServerAddressCheckForSuplInitL();
                    iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl,iEncodedSuplInit,hslpAdress);		
                    CleanupStack::PopAndDestroy(hslpAdress);

                    // Set the SessionId.
                    iTrace->Trace(_L("COMASuplSession::CheckProtocolVersionL Update SLP Session ID"), KTraceFileName, __LINE__);
                    UpdateSLPSessionIDL(SessionID);

                    iTrace->Trace(_L("COMASuplSession::CheckProtocolVersionL Update SET Session ID"), KTraceFileName, __LINE__);
                    UpdateSETSessionIDL(SessionID);
                    iSuplState->SetMsgStateObserver(this);
                    iSuplState->GenerateMessageL();
                    iSuplSessionState = ESUPL_GENERATE;
                    }
                else
                    {                                
                    iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);		
                    UpdateSLPSessionIDL(SessionID);
                    UpdateSETSessionIDL(SessionID);
                    iSuplState->SetMsgStateObserver(this);
                    iRequestType = ESUPL_INVALID_SESSION;
                    iSuplSessionState = ESUPL_GENERATE;
                    iSuplState->GenerateMessageL();
                    }
                } 
            else 							
                {
                iSuplState = COMASuplEndState::NewL(iErrorStatusCode,iOMASuplAsnHandlerBaseImpl);
                iSuplSessionState = ESUPL_ENCODE;
                iSuplState->SetMsgStateObserver(this);
                iSuplState->GenerateMessageL();
                }
            }	
        return EFalse;
        }	
	}

// -----------------------------------------------------------------------------
// COMASuplSession::GetRemainingPosPayloadL
// Returns remaining pos payload
// -----------------------------------------------------------------------------
COMASuplPosPayload* COMASuplSession::GetRemainingPosPayloadL()
	{
		if( iSuplState && COMASuplState::ESUPL_POS_INIT == iSuplState->GetSuplState())
		{
			COMASuplPosInitState* state =  static_cast <COMASuplPosInitState *>(iSuplState);
			return state->GetRemainingPospayLoadL(); 
		}
		else
		{
			return NULL;
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::IsWholeMessageSent
// Is whole message get sent
// -----------------------------------------------------------------------------
TBool COMASuplSession::IsWholeMessageSentL()
	{
		COMASuplState::TSuplMessageType msgType = iSuplState->GetSuplState();
		switch(msgType)
			{
				case COMASuplState::ESUPL_POS_INIT:
					{
						COMASuplPosPayload* remainingPosPayload = GetRemainingPosPayloadL();
						if(remainingPosPayload )
							return EFalse;
						else
							return ETrue;
					}
					
				case COMASuplState::ESUPL_POS:
					{
						return !IsSegmentationDone();
					}
				default : //For Start , End
					{
						return ETrue;	
					} 
			}
	}

// -----------------------------------------------------------------------------
// COMASuplSession::GetNetworkModeL()
// Checks wheather phone is online or Offline mode...
// -----------------------------------------------------------------------------

TInt COMASuplSession::GetNetworkModeL()
	{
	
	TInt networkMode;
	// get network mode
	CRepository* centralRepository = CRepository::NewLC(KCRUidCoreApplicationUIs);
	User::LeaveIfError(centralRepository->Get(KCoreAppUIsNetworkConnectionAllowed, networkMode));
    CleanupStack::PopAndDestroy(centralRepository);
	return networkMode;
	}

// -----------------------------------------------------------------------------
// COMASuplSession::SetSessionUIFlag
// Sets/Resets the Session UI flag 
// -----------------------------------------------------------------------------
void COMASuplSession::SetSessionUIFlag(TBool aValue)
    {
    iUIFlag = aValue;
    }        

// -----------------------------------------------------------------------------
// COMASuplSession::GetSessionUIFlag
// Returns Session UI flag 
// -----------------------------------------------------------------------------
TBool COMASuplSession::GetSessionUIFlag()
    {
    return iUIFlag;
    }    



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// COMASuplSession::CloseAllOperations
// Close all operations...from here we will stop all processing releated with session and will 
// not send SUPL_END 
// -----------------------------------------------------------------------------
void COMASuplSession::CloseAllOperations()
    {
    	iCompleteSelfRequestor->CompleteInvalidSLPSession();
    }    
    
// -----------------------------------------------------------------------------
// COMASuplSession::HandleInvalidSLPSessionL
// Relinquish control to Communication manager....
// 
// -----------------------------------------------------------------------------
void COMASuplSession::HandleInvalidSLPSession()
	{
    iTrace->Trace(_L("Cancelling All operations..since received invalid SLP session..."), KTraceFileName, __LINE__); 
    iConnRequestor->CancelReadOperation();
		CancelAllOperations();
		iConnRequestor->CloseConnection();
        iPortNum = 0;
		iSessionObserver.TerminateSession(this, KErrCompletion);//// Invoke Terminate Session
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::IsUIDisplayedL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
	
TBool COMASuplSession::IsUIDisplayedL()	
	{
	return EFalse;
	}


// -----------------------------------------------------------------------------
// COMASuplSession::SetSETSessionIdL
// Sets the proper SET Session id depending on the type of Varient.
// 
// -----------------------------------------------------------------------------

TInt COMASuplSession::SetApproriateSETSessionId()
	{
		TInt err = KErrNone;
		if( IsIMSIVariant() )
			{
				SetIMSI();  
			}
		else
			{
				err = GetLocalIPAddress();
			}	
			
		return err;	
	}

// -----------------------------------------------------------------------------
// COMASuplSession::TimerExpiredL
// Checks whether UI is displayed or not previously
// 
// -----------------------------------------------------------------------------
void COMASuplSession::TimerExpiredL()
{
    iTrace->Trace(_L("Timer Expired for SUPL_POSINIT"), KTraceFileName, __LINE__); 
    OperationCompleteL(KErrTimedOut);
}



// -----------------------------------------------------------------------------
// COMASuplSession::CheckForSuplUsageL
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForSuplUsageL()
    {

             
            if (iNwInitError)
                {
                iNwInitError = EFalse;                    
                HandleSuplInitErrorL(iNwInitCompletionCode);                             
                }                
            else
            	{
            		if (iRoaming)                
                    HandleRoamingCheckCompleteL(KErrNone, EFalse);                    
                else
                    {                        
                    iEtelRoamingCheck = ETrue;
                    iProtocolManager.CheckForRoaming();
                    }
	          	}
           

    }        

// -----------------------------------------------------------------------------
// COMASuplSession::CheckForTimerExpiry
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForTimerExpiry()
    {
    if (iPersistFail)
        {            
        TTime nowTime;
        nowTime.HomeTime();
        TTimeIntervalMicroSeconds diff = nowTime.MicroSecondsFrom(iFailTime);	

        if (diff > (iPersistFailTimer * KSecond) )
            iChanged = ETrue;            
        }
    }        
// -----------------------------------------------------------------------------
// COMASuplSession::CheckForPreviousResultL
// 
// -----------------------------------------------------------------------------
void COMASuplSession::CheckForPreviousResultL()
    {
    CheckForTimerExpiry();            
    if (iChanged)
        {
        iProtocolManager.CancelNotify();
        iEtelNotify = EFalse;
        iPersistFail = EFalse;
        iCapsFail = EFalse;
        iChanged = EFalse;
        CheckForSuplUsageL();
        return;
        }

    // Check for previous request result
    if (iPrevUsageResult == KErrNone && !iPersistFail && !iCapsFail)
        {            
	    InitializeL(iRequestID); 
        }
    else if (iCapsFail && iAllowedCapabilities != iFailedCaps)
        {            
	    InitializeL(iRequestID); 
        }
    else
        {            
	    //iSessionObserver.TerminateSession(this, iPrevUsageResult);	
        if (iPrevUsageResult != KErrNone)
	        iSessionObserver.TerminateSession(this, iPrevUsageResult);	
        else if (iPersistFail)
	        iSessionObserver.TerminateSession(this, iPrevReqResult);	
        else if (iCapsFail)
	        iSessionObserver.TerminateSession(this, KErrGeneral);	
        }
    }        

void COMASuplSession::HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw)
    {
    iTrace->Trace(_L("COMASuplSession::HandleCurrentNetworkChangeL"), KTraceFileName, __LINE__); 
    iEtelNotify = EFalse;
    if (aErrorCode == KErrNone)
        {            
        iChanged = ETrue;
        if (!aHomeNw)
            {            
            iRoaming = ETrue;            
            }
        else
            iRoaming = EFalse;                
        }
    }            

void COMASuplSession::HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw)
    {
    iTrace->Trace(_L("COMASuplSession::HandleRoamingCheckCompleteL"), KTraceFileName, __LINE__); 
    iEtelRoamingCheck = EFalse;            
    if (aErrorCode == KErrNone)            
        {            
            iUsageHomeNW = aHomeNw;   
            iConnRequestor->UpdateSLPListForHomeUsage(iUsageHomeNW);             
                 

                    if(iSuplMsgType == ESUPL_INIT)
                        {
                        iTrace->Trace(_L("Dialog timer started"), KTraceFileName, __LINE__);
				     
                        COMASuplInit* suplInit = static_cast <COMASuplInit*> (iDecodedAsnMessage);
                        TOMASuplQop qop;
                        TInt retVal = suplInit->Qop(qop);
                        TInt delay;
                        qop.Delay(delay);
                        //if delay is > 7, it is not valid
                        if (delay > 7)
                            delay = 0;                                
                        if(retVal == KErrNone && delay > 0)
                            {
                            TReal delayReal;
                            Math::Pow(delayReal, 2, (TReal)delay);
                            delay = (TInt) delayReal;
                            TBuf<128> msg(_L("Delay present in message, value is = "));
                            msg.AppendNum(delay);
                            iTrace->Trace(msg, KTraceFileName, __LINE__); 
                            isTimeoutDialogTimerStarted = ETrue;
                            
                            }  	
                        else
                            {
                            if (iSuplInitTimeOut > 0)
                                {                                                        
                                isTimeoutDialogTimerStarted = ETrue;
                                
                                }
                            else
                                iTrace->Trace(_L("Timeout is <= 0"), KTraceFileName, __LINE__);
                            }
                        }
                                
                if (iNwInitError)
                    {
                    iNwInitError = EFalse;                    
                    HandleSuplInitErrorL(iNwInitCompletionCode);                             
                    }                
                else
                {
                	iTrace->Trace(_L("COMASuplSession::InitializeL called after roaming check"), KTraceFileName, __LINE__);
	                InitializeL(iRequestID);  
	              }  
        }
   else
        {
      	iSessionObserver.TerminateSession(this, KErrGeneral);	
        }                
    }            
TInt COMASuplSession::SuplIpcSessionID() 
    {
    return iIpcSessionId;
    }    

void COMASuplSession::UpdateSuplUsage()
    {        
    iPrevUsageResult = KErrNone;            
    }    

void COMASuplSession::SettingsChanged()
    {
    iChanged = ETrue;
    }    

void COMASuplSession::HandleIapSettingsChangeL(TInt aErrorCode)
    {
    iTrace->Trace(_L("COMASuplSession::HandleIapSettingsChangeL"), KTraceFileName, __LINE__);
    if (aErrorCode >= 0)
        iChanged = ETrue;
    }        

void COMASuplSession::CheckForPersistentFailure(TInt aErrorCode)
    {
    if (iRequestType == ESUPL_TERMINAL)            
        {            
        if (
            (aErrorCode <= KErrDndNameNotFound && aErrorCode >= KErrDndServerUnusable) || // DNS errors
            (aErrorCode <= KErrGprsAndNonGprsServicesNotAllowed && aErrorCode >= KErrGprsFeatureNotSupported) || // GPRS errors
            (aErrorCode <= KErrSSLNoSharedCipher && aErrorCode >= KErrSSLDllLeave) || // TLS errors
            (aErrorCode == KErrCouldNotConnect)                            // Server does not exist
        )
            {
            iPersistFail = ETrue;
            iPrevReqResult = KErrGeneral;

            iFailTime.HomeTime();

            iTrace->Trace(_L("COMASuplSession::CheckForPersistentFailure, listening to network change"), KTraceFileName, __LINE__);
            iEtelNotify = ETrue;
            iProtocolManager.NotifyCurrentNetworkChange();

            iTrace->Trace(_L("COMASuplSession::CheckForPersistentFailure, listening to IAP change"), KTraceFileName, __LINE__);
            if (!iIapNotifier->IsActive())
                iIapNotifier->NotifyIapSettingsChange();
            }            
        }            
    }        
void COMASuplSession::SetSuplUsageFlag()
    {
    iUsageDialog = ETrue;            
    }        

void COMASuplSession::ReSetSuplUsageFlag()
    {
    iUsageDialog = EFalse;            
    }        

TBool COMASuplSession::GetSuplUsageFlag()
    {
    return iUsageDialog;
    }        



TBool COMASuplSession::IsEtelNotifySet()
    {
    iTrace->Trace(_L("COMASuplSession::IsEtelNotifySet"), KTraceFileName, __LINE__); 
    return iEtelNotify;            
    }

TBool COMASuplSession::IsEtelRoamingSet()
    {
    iTrace->Trace(_L("COMASuplSession::IsEtelRoamingSet"), KTraceFileName, __LINE__); 
    return iEtelRoamingCheck;
    }        
	
TBool COMASuplSession::HasMeLaunchedUsageDialog()
{
	return iIhaveLaunchedUsagedialog; 
}


	
// -----------------------------------------------------------------------------
// COMASuplSession::StoreCurrentCellIDL
// Stores current cell id in CR
// 
// -----------------------------------------------------------------------------
void COMASuplSession::StoreCurrentCellIDL(HPositionGenericInfo& /*aSuplPosInfo*/)
	{
		iTrace->Trace(_L("Calling StoreCurrentCellIDL"),KTraceFileName, __LINE__);
 		TPckg<TCellIdInfo> cellPkg(iCurrentCellId);
				
			#ifdef PRINT_MESSAGE
				TBuf<64> msg;
               	msg.Copy(_L("cid=:"));
               	msg.AppendNum(iCurrentCellId.iCid);
               	iTrace->Trace(msg,KTraceFileName, __LINE__); 
               	
               	msg.Copy(_L("Lac=:"));
               	msg.AppendNum(iCurrentCellId.iLac);
               	iTrace->Trace(msg,KTraceFileName, __LINE__);
               	
               	msg.Copy(_L("MCC=:"));
               	msg.AppendNum(iCurrentCellId.iMCC);
               	iTrace->Trace(msg,KTraceFileName, __LINE__);
               	
               	msg.Copy(_L("MNC=:"));
               	msg.AppendNum(iCurrentCellId.iMNC);
               	iTrace->Trace(msg,KTraceFileName, __LINE__); 
			#endif
				
				CRepository* ConfigurationCenRepSession = CRepository::NewL(KCRUidOMASuplConfiguration);
				CleanupStack::PushL(ConfigurationCenRepSession);
				User::LeaveIfError(ConfigurationCenRepSession->Set(KOMASuplCurrentCellid, cellPkg));
		
				CleanupStack::PopAndDestroy(ConfigurationCenRepSession);
}

// -----------------------------------------------------------------------------
// COMASuplSession::ServerAddressCheckForSuplInitL()
// Stores current cell id in CR
// 
// -----------------------------------------------------------------------------
TInt COMASuplSession::ServerAddressCheckForSuplInitL()
	{
		TBuf<256> serverName;
		TBuf<64> msg;
		
		COMASuplSLPAddress* slpAddress = NULL;
		COMASuplInit* SuplInit = static_cast <COMASuplInit *>(iDecodedAsnMessage);
		TInt err = SuplInit->SLPAddress(slpAddress);
		
		if(err == KErrOMASuplParamNotSet)
			{
  			msg.Copy(_L("SLP address is NOT PRESENT in SUPL_INIT."));
				iTrace->Trace(msg,KTraceFileName, __LINE__); 

				//Set default slp to Conn Requestor...		
				iConnRequestor->UseDefaultServerL();
				return KErrNone;
			}

		COMASuplSLPAddress::TSuplSLPAddressType addressType = slpAddress->SLPAddressType();
		if(addressType == COMASuplSLPAddress::EFqdn)
		{
			HBufC8* fqdn;
			TInt err = slpAddress->Fqdn(fqdn);
			serverName.Copy(fqdn->Des());
			msg.Copy(_L("SLP address as FQDN is PRESENT in SUPL_INIT:"));
			msg.Append(serverName);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
		}
		else if (addressType == COMASuplSLPAddress::EIPv4Address)
		{
			TBuf8<256> IPslpaddress;
			slpAddress->IPvAddress(IPslpaddress);
			serverName.Copy(IPslpaddress);
			msg.Copy(_L("SLP address as IPv4 is PRESENT in SUPL_INIT:"));
			msg.Append(serverName);
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			iTrace->Trace(_L("Server Name Length:"),KTraceFileName, __LINE__); 
			msg.Zero();
			msg.AppendNum(serverName.Length());
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			
				if (serverName.Length() == 4)  //IPv4 encoded in ASCII..have to decode it to an IP address.
					{
						iTrace->Trace(_L("SLP encoded as ASCII :"),KTraceFileName, __LINE__); 
    			
    			TInt firstOctet = serverName[0];
    			TInt secondOctet = serverName[1];
    			TInt thirdOctet = serverName[2];
    			TInt fourthOctet = serverName[3];
    			
    			serverName.Zero();
    			serverName.AppendNum(firstOctet);
    			serverName.Append(_L("."));
    			serverName.AppendNum(secondOctet);
    			serverName.Append(_L("."));
    			serverName.AppendNum(thirdOctet);
    			serverName.Append(_L("."));
    			serverName.AppendNum(fourthOctet);
    			msg.Copy(_L("SLP address as IPv4 after conversion is:"));
    			msg.Copy(serverName);
    			iTrace->Trace(msg,KTraceFileName, __LINE__);
					}

		}
		else // It is an IPv6 address
			{
				TBuf8<256> IPslpaddress;
				slpAddress->IPvAddress(IPslpaddress);
				serverName.Copy(IPslpaddress);
				msg.Copy(_L("SLP address as IPv6 is PRESENT in SUPL_INIT:"));
				msg.Append(serverName);
				iTrace->Trace(msg,KTraceFileName, __LINE__); 
				
				if (serverName.Length() == 16)
					{
						TBuf<64> tempServer;
						tempServer.Zero();
						TInt octet(0), i;
						for (i = 0;  i < (serverName.Length() - 1); ++i)
						{
							octet = serverName[i];
							tempServer.AppendNum(octet);
							tempServer.Append(_L("."));
						}
						octet = serverName[i];
						tempServer.AppendNum(octet);
					}
		}

		TBool isExist = iSuplStorageSettings->IsSlpExists(serverName);
		if(isExist)
			{
				//Set to Conn Requestor...
				iConnRequestor->UseServerL(serverName);
				return KErrNone;
			}
		else
			{
				msg.Copy(_L("Dropping Packet.SLP absent in server List:"));
				msg.Append(serverName);
				iTrace->Trace(msg,KTraceFileName, __LINE__); 
				return KErrNotFound;
			}	
		
	}

void COMASuplSession::CancelTriggerSession()
    {

    }
	
TUint COMASuplSession::GetPortNumUsed()
{
	return iPortNum;
}

