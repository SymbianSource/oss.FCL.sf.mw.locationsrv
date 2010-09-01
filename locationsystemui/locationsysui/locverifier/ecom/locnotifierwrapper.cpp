/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Methods for Location notifier Ecom Plugins entry point
*
*/


#include <ecom/implementationproxy.h>
#include <AknNotifierWrapper.h> // link against aknnotifierwrapper.lib
#include <lbs/epos_privacynotifier.hrh>

#include "locnotifierwrapper.h"


const MEikSrvNotifierBase2::TNotifierPriority KNotifierPriority =
        MEikSrvNotifierBase2::ENotifierPriorityHigh;
const TUid KNotifierChannel = { 0x100065ac };
const TInt KMaxSynchReplyBufLength = 256;
const TInt KPrivSrvSecureId = 0x10281D45;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLocNotifierSecurityCheck::Release
// This method will release all resources that have been allocated for
// performing the custom security check.
// ---------------------------------------------------------------------------
//
void CLocNotifierSecurityCheck::Release()
    {
    delete this;
    }


// ---------------------------------------------------------------------------
// CLocNotifierSecurityCheck::CustomSecurityCheckL
// This method performs the security check
// ---------------------------------------------------------------------------
//
void CLocNotifierSecurityCheck::CustomSecurityCheckL(const RMessagePtr2& aMessage)
    {
    if (!aMessage.IsNull())
        {
		static _LIT_SECURITY_POLICY_S0(SidPolicy, KPrivSrvSecureId);
		if (!SidPolicy().CheckPolicy(aMessage))
			{
			User::Leave(KErrPermissionDenied);
			}
        }
    }
    
		
// ======== LOCAL FUNCTIONS ========

// ECom registration related methods

// ---------------------------------------------------------------------------
// Helper method to perform cleanup of the CArrayPtrFlat array object.
// ---------------------------------------------------------------------------
//
void CleanupArray(TAny* aArray)
    {
    CArrayPtrFlat<MEikSrvNotifierBase2>*     
        subjects=static_cast<CArrayPtrFlat<MEikSrvNotifierBase2>*>(aArray);
    TInt lastInd = subjects->Count()-1;
    for (TInt i=lastInd; i >= 0; i--)
        subjects->At(i)->Release();	    	    
    delete subjects;
    }


// ---------------------------------------------------------------------------
// Creates an array of CAknCommonNotifierWrapper objects
// ---------------------------------------------------------------------------
//
CArrayPtr<MEikSrvNotifierBase2>* DoCreateNotifierArrayL()
    {
    CArrayPtrFlat<MEikSrvNotifierBase2>* subjects=
        new (ELeave)CArrayPtrFlat<MEikSrvNotifierBase2>(1);
    
    CleanupStack::PushL(TCleanupItem(CleanupArray, subjects));

    CLocNotifierSecurityCheck* customSecurityImpl = 
        new(ELeave) CLocNotifierSecurityCheck;
    CleanupStack::PushL(customSecurityImpl);
    
    // Session owning notifier(if default implementation is enough)
    CAknCommonNotifierWrapper* master = 
        CAknCommonNotifierWrapper::NewL( TUid::Uid(KPosPrivacyNotifierImplUid),
                                   KNotifierChannel,
                                   KNotifierPriority,
                                   _L("LocVerifierDlg.dll"),
                                   KMaxSynchReplyBufLength);
    // set the custom security handler.
	
    CleanupStack::Pop(customSecurityImpl);
    // Control of customSecurityImpl transferred to CAknCommonNotifierWrapper
    // object
	master->SetCustomSecurityHandler(customSecurityImpl); 		   
    subjects->AppendL( master );
    CleanupStack::Pop(subjects);
    return(subjects);
    }


// ---------------------------------------------------------------------------
// Called to instantiate this ECOM Plugin implementation.
// ---------------------------------------------------------------------------
//
CArrayPtr<MEikSrvNotifierBase2>* NotifierArray()
    // old Lib main entry point
    {
    CArrayPtr<MEikSrvNotifierBase2>* array = NULL;
    TRAPD(error, array = DoCreateNotifierArrayL());
    if (error == KErrNone)
    	{
    	return array;
    	}
    else
    	{
    	return NULL;
    	}
    }

const TImplementationProxy ImplementationTable[] =
	{
#ifdef __EABI__
	{{0x10207243},(TFuncPtr)NotifierArray}
#else
	{{0x10207243},NotifierArray}
#endif
	};

// ======== GLOBAL FUNCTIONS ========

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy) ;
	return ImplementationTable;
	}

