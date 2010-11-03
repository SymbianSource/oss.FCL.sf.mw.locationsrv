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
* Description:  The implementation of Info class of Location Settings UI 
*                Server
*
*/


#include <e32base.h>

#include "locsettingsuiinfo.h"
#include "locsettingsuiserverinterface.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::NewL()
// ---------------------------------------------------------------------------
//
CLocSettingsUIInfo* CLocSettingsUIInfo::NewL(
    const RMessage2&    aMessage )
    {
    CLocSettingsUIInfo* self = NewLC( aMessage );
    CleanupStack::Pop( self );
    return self;    
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::NewLC()
// ---------------------------------------------------------------------------
//
CLocSettingsUIInfo* CLocSettingsUIInfo::NewLC(
    const RMessage2&    aMessage )
    {
    CLocSettingsUIInfo* self = new( ELeave ) CLocSettingsUIInfo( aMessage );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;    
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::~CLocSettingsUIInfo()
// ---------------------------------------------------------------------------
//
CLocSettingsUIInfo::~CLocSettingsUIInfo()
    {
    delete iParamsString;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::Complete(TInt aErrorCode)
// ---------------------------------------------------------------------------
//
void CLocSettingsUIInfo::Complete( TInt aErrorCode )
    {
    if (!iReqMsg.IsNull())
        {
        iReqMsg.Complete(aErrorCode);
        }
    }

// ---------------------------------------------------------------------------
// RMessage2&   CLocSettingsUIInfo::IPCMessage()
// ---------------------------------------------------------------------------
//
RMessage2&   CLocSettingsUIInfo::IPCMessage()
    {
    return iReqMsg;
    }
    
// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::QueLinkOffset()
// ---------------------------------------------------------------------------
//
TInt CLocSettingsUIInfo::QueLinkOffset()
    {
    return _FOFF(CLocSettingsUIInfo, iNext);
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::Session()
// Accessor Method
// ---------------------------------------------------------------------------
//
CSession2* CLocSettingsUIInfo::Session() const
    {
    return iReqMsg.Session();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::SettingsUID()
// Accessor Method
// ---------------------------------------------------------------------------
//
TUid CLocSettingsUIInfo::SettingsUID() const
    {
    return TUid::Uid(iReqMsg.Int0());
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::SettingsUIIntParams()
// Accessor Method
// ---------------------------------------------------------------------------
//
TInt CLocSettingsUIInfo::SettingsUIIntParams()
    {
    if( IPCMessage().Function() == ELaunchSettings )
        return iReqMsg.Int1();
    else
        return 0;
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::SettingsUIStringParamsL()
// Accessor Method
// ---------------------------------------------------------------------------
//
TPtrC CLocSettingsUIInfo::SettingsUIStringParamsL()
    {
    if( IPCMessage().Function() == ELaunchSettingsWithString &&
            0 != iReqMsg.Ptr1(), NULL != iReqMsg.Ptr2() )
        {
        delete iParamsString;
        iParamsString = NULL;
        
        TInt length = iReqMsg.Int1();
        iParamsString = HBufC::NewL(length);
        TPtr ptr = iParamsString->Des();
        iReqMsg.ReadL(2, ptr);
        return *iParamsString;
        }
    else
        return KNullDesC();
    }


// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::CLocSettingsUIInfo(const RMessage2 &aMessage)
// ---------------------------------------------------------------------------
//
CLocSettingsUIInfo::CLocSettingsUIInfo(
    const RMessage2&    aMessage )
    {
    // From now on both of these are owned by this class.
    iReqMsg = aMessage;
    }
    

// ---------------------------------------------------------------------------
// CLocSettingsUIInfo::ConstructL()
// ---------------------------------------------------------------------------
//
void CLocSettingsUIInfo::ConstructL()
    {
    // Left empty intentionally.
    }


// End of file
    

