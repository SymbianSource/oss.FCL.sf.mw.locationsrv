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
* Description:  RMnServiceClientBase class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mnserviceclientbase.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
RMnServiceClientBase::RMnServiceClientBase() :
    iLastRequest( EMnIpcNone )
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnServiceClientBase::ConnectL( TUid aProviderUid )
    {
    LOG1("RMnServiceClientBase::ConnectL (provider 0x%X)", aProviderUid.iUid);
    ConnectNewAppL( aProviderUid );
#ifdef _DEBUG    
    TPtrC serverName( ServerName() );
    LOG1("RMnServiceClientBase::ConnectL connected to server '%S'", &serverName);
#endif    
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnServiceClientBase::ConnectChainedL( TUid aProviderUid )
    {
    LOG1("RMnServiceClientBase::ConnectChainedL (provider 0x%X)", aProviderUid.iUid);
    ConnectChainedAppL( aProviderUid );
#ifdef _DEBUG    
    TPtrC serverName( ServerName() );
    LOG1("RMnServiceClientBase::ConnectChainedL connected to server '%S'", &serverName);
#endif    
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnServiceClientBase::Cancel()
    {
    LOG("RMnServiceClientBase::Cancel");
    if ( Handle() && iLastRequest != EMnIpcNone )
        {
        TIpcArgs args;
        args.Set( EMnIpcCancelRequestIdIndex, iLastRequest );
        SendReceive( EMnIpcCancel, args );
        }
    iLastRequest = EMnIpcNone;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void RMnServiceClientBase::Close()
    {
    LOG("RMnServiceClientBase::Close");
    Cancel();
    RAknAppServiceBase::Close();
    }
