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
* Description:  The implementation of Service class of Location Settings UI 
*                Server
*
*/


#include <e32capability.h>

#include "locsettingsuiservice.h"
#include "locsettingsuiappserver.h"
#include "locsettingsuiserverinterface.h"
#include "locsettingsuilaunchmgr.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUIService::CLocSettingsUIService()
// ---------------------------------------------------------------------------
//
CLocSettingsUIService::CLocSettingsUIService()
    {
    iSettingsUILaunched = EFalse;
    }
    
    
// ---------------------------------------------------------------------------
// CLocSettingsUIService::~CLocSettingsUIService()
// ---------------------------------------------------------------------------
//
CLocSettingsUIService::~CLocSettingsUIService()
    {
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIService::CreateL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIService::CreateL()
    {
    // This method is called when a new session object is created.
    // Base-call first.
    CAknAppServiceBase::CreateL();
    // There are 3 scenarios to consider here
    //
    // 1. If the server is running as embedded application:
    //      In this case there is no shutdown timer running. So the call
    //      to the parent class to cancel shutdown timer will be just 
    //      ignored.
    // 2. If the server is running as a UI Server and numsession=0:
    //      In this case a shutdown timer is running. Hence cancel needs to 
    //      be called. 
    // 3. If the server is running as a UI Server and numsession > 0:
    //      In this case there is no shutdown timer running but calling
    //      timer cancel doesn't do any harm.
    // Ideally in case 1 and 3 we should not call shutdown timer cancel but
    // the num session is maintained by the base class (private member) and
    // we see that calling cancel in all situation works. Hence the 
    // solution is to just call shutdown timer cancel here.
    static_cast<CLocSettingsUIAppServer*>
        (const_cast<CServer2*>(Server()))->CancelShutdownTimer();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIService::ServiceL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIService::ServiceL(const RMessage2& aMessage)
    {
    CLocSettingsUILaunchMgr* settingsMgr = CLocSettingsUILaunchMgr::InstanceL();
    switch(aMessage.Function())
        {
        case ELaunchSettingsWithString:
            {
            TInt implUid = aMessage.Int0();
            
            // The ownership of the aMessage is transferred to the
            // Launch Manager.    
            settingsMgr->EnqueueRequestL(aMessage);
            iSettingsUILaunched = ETrue;
            break;
            }
        case ELaunchSettings:
            {
            TInt implUid = aMessage.Int0();
            TInt settingsUIParam = aMessage.Int1();
            if (!IsLaunchParamsValid(implUid, settingsUIParam))
                {
                aMessage.Panic(KLocSettingsUIPanicTxt,
                    ELocSettingsUiSrvInvalidArguments);
                break;
                }
            
            // The ownership of the aMessage is transferred to the
            // Launch Manager.    
            settingsMgr->EnqueueRequestL(aMessage);
            iSettingsUILaunched = ETrue;
            break;
            }
        case ELaunchPosSettings:          
            {
            // The request needs to the queued to the Settings Manager.
            // The corresponding UI would be launched immediately incase there
            // is no outstanding UI. If not, the request would get queued.
            settingsMgr->EnqueueRequestL(aMessage);
            iSettingsUILaunched = ETrue;
            break;
            }            
        case ECancelSettings:
        case ECancelPosSettings:        
            {
            if(!iSettingsUILaunched)
                {
                // Here in case of cancel without any UI Launch is
                // taken as a error case.
                aMessage.Complete(KErrNotSupported);
                return;
                }
            settingsMgr->CancelRequest(this);
            // This cancel message is responded with KErrNone. The 
            // request message is completed in the LaunchAO with KErrCancel.
            aMessage.Complete(KErrNone);
            iSettingsUILaunched = EFalse;
            break;
            }
        default:
            {
            CAknAppServiceBase::ServiceL(aMessage);
            break;         
            }
        }
    return;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIService::ServiceError
// ---------------------------------------------------------------------------
//
void CLocSettingsUIService::ServiceError(const RMessage2& aMessage,TInt aError)
    {
    // Will be called if the ServiceL leaves with any Error.
    // Based on the Error type the Message needs to be completed with error
    // or the server has to panic the client.
    // Call the base class method
    return CAknAppServiceBase::ServiceError(aMessage, aError);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIService::SecurityCheckL
// aMissing parameter is needed only when implementing custom security check 
// failure action. Since we use the default action, we don't need this param.
// ---------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CLocSettingsUIService::SecurityCheckL(
    const RMessage2& aMessage,
	TInt& aAction,
	TSecurityInfo& /*aMissing*/)
    {
    // In case the secuirty check fails we choose to fail the client
    // request with KErrPermissionDenied rather than panic the client.
    aAction = CPolicyServer::EFailClient;
    
    if ( aMessage.HasCapability(ECapabilityLocation) )
        {
        // Accept this request since the client has Location capability.
        return CPolicyServer::EPass;
        }
    // The client does not have Location capability. So fail this request.
    return CPolicyServer::EFail;
    }
    
    
// ---------------------------------------------------------------------------
// CLocSettingsUIService::ServiceError
// ---------------------------------------------------------------------------
//
TBool CLocSettingsUIService::IsLaunchParamsValid(
          TInt aSettingsUid,
    const TInt /*aParams*/ )
    {
    // Check if the SettingsUid is Zero
    if( 0 == aSettingsUid )
        {
        return EFalse;
        }
    return ETrue;
    }


// End of file
