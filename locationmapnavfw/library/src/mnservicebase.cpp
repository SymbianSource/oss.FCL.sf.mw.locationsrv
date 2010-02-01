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
* Description:  CMnServiceBase class implementation
*
*/


#include <e32base.h>

#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include "mndebug.h"
#include "mnerrors.h"
#include "mninternal.h"
#include "mninternal.inl"

#include "mnservicebase.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnServiceBase::CMnServiceBase()
    {
    iCurrentRequest = EMnIpcNone;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnServiceBase::~CMnServiceBase()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnServiceBase::BaseConstructL()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnServiceBase::CompleteRequest( TInt aResult )
    {
    LOG1("CMnServiceBase::CompleteRequest, result %d", aResult);
    if ( iCurrentRequest == EMnIpcNone )
        {
        PanicServer( KMnPanicAttemptToCompleteNoRequest );
        }
    else
        {
        Complete( iMessage, aResult );
        iCurrentRequest = EMnIpcNone;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnServiceBase::Complete( const RMessage2& aMsg, TInt aResult )
    {
    //LOG1("CMnServiceBase::Complete, result %d", aResult);
    if ( !aMsg.IsNull() )
        {
        aMsg.Complete( aResult );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnServiceBase::HandleCancelRequestL( const RMessage2& aMessage )
    {
    LOG("CMnServiceBase::HandleCancelRequestL in");
    
    if ( IsAsyncRequestActive() && aMessage.Int0() == CurrentAsyncRequest() )
        {
        DoCancel();
        CompleteRequest( KErrCancel );
        Complete( aMessage, KErrNone );
        }
    else
        {
        LOG("CMnServiceBase::HandleCancelRequestL leave, not found");
        User::Leave( KErrNotFound );
        }
        
    LOG("CMnServiceBase::HandleCancelRequestL out");
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC8* CMnServiceBase::CopyClientBuffer8LC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam )
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = aMessage.GetDesLengthL( aClientBufferParam );
    HBufC8* buffer = HBufC8::NewLC( dataSize );

    // Populate server side buffer with client data
    TPtr8 ptrToBuf = buffer->Des();
    aMessage.ReadL( aClientBufferParam, ptrToBuf );
    return buffer;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC* CMnServiceBase::CopyClientBufferLC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = aMessage.GetDesLengthL( aClientBufferParam );
    HBufC* buffer = HBufC::NewLC( dataSize );

    // Populate server side buffer with client data
    TPtr ptrToBuf = buffer->Des();
    aMessage.ReadL( aClientBufferParam, ptrToBuf );
    return buffer;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CPosLandmark* CMnServiceBase::UnpackLandmarkLC( 
    const RMessage2& aMessage,
    TInt aParamIndex )
    {
    LOG("CMnServiceBase::UnpackLandmarkLC");
    
    HBufC8* lmBuf = CopyClientBuffer8LC( aMessage, aParamIndex );
    
    CPosLandmark* landmark = PosLandmarkSerialization::UnpackL( *lmBuf );
    CleanupStack::PopAndDestroy( lmBuf );
    CleanupStack::PushL( landmark );
    return landmark;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnServiceBase::CurrentAsyncRequest()
    {
    return iCurrentRequest;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TBool CMnServiceBase::IsAsyncRequestActive()
    {
    return ( iCurrentRequest != EMnIpcNone );
    }

// ---------------------------------------------------------------------------
//  Put new request ID to stack to guarantee that if starting of asynchronous
//  request leaves, the iCurrentRequest will be reset.
//  If starting goes well, the PostStartAsyncRequest will pop this cleanup
//  from stack and "current request ID" will be kept until request is competed
//  by service implmentation.
// ---------------------------------------------------------------------------
//
void CMnServiceBase::PrepareAsyncRequestL( const RMessage2& aMessage )
    {
    LOG("CMnServiceBase::PrepareAsyncRequestL in");

    if ( IsAsyncRequestActive() )
        {
        PanicClient( aMessage, KMnPanicDuplicateRequest );
        User::Leave( KErrInUse );
        }

    iMessage = aMessage;
    iCurrentRequest = aMessage.Function();

    LOG("CMnServiceBase::PrepareAsyncRequestL out");
    }

// ---------------------------------------------------------------------------
//  From class CApaAppServiceBase
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnServiceBase::ServiceError( const RMessage2 &aMessage, TInt aError )
    {
    LOG2("CMnServiceBase::ServiceError function %d, error %d",
        aMessage.Function(), aError );
    
    if ( IsAsyncRequestActive() && 
         aMessage.Handle() == iMessage.Handle() )
        {
        // starting this async request failed, rollback
        LOG1("CMnServiceBase::ServiceError forgetting async request %d", iCurrentRequest );
        iCurrentRequest = EMnIpcNone;
        }

    CAknAppServiceBase::ServiceError( aMessage, aError );
    LOG("CMnServiceBase::ServiceError out");
    }
