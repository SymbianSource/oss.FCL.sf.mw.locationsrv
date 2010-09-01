/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  PSY configuration UI application launching implementation.
*
*/


// System Include
#include <eikserverapp.h>

// User Include
#include "locpsyconfigclientwrapper.h"
#include "locpsyconfigclientimpl.h"

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClientImpl::CLocPsyConfigUiServiceClientImpl
// -----------------------------------------------------------------------------
//
CLocPsyConfigUiServiceClientImpl::CLocPsyConfigUiServiceClientImpl(
                                                          CLocPsyConfigUiReg&        aRegInfo, 
														  MAknServerAppExitObserver& aObserver )
	:iObserver(&aObserver),iAppRegInfo(aRegInfo)
	{
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClientImpl::~CLocPsyConfigUiServiceClientImpl
// -----------------------------------------------------------------------------
//
CLocPsyConfigUiServiceClientImpl::~CLocPsyConfigUiServiceClientImpl()
	{
	if(iLaunchAppService)
		{
		delete iLaunchAppService;
		}
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClient* CLocPsyConfigUiServiceClient::NewLC
// -----------------------------------------------------------------------------
//
CLocPsyConfigUiServiceClient* CLocPsyConfigUiServiceClient::NewLC(
	                                                      CLocPsyConfigUiReg&        aReg, 
	                                                      MAknServerAppExitObserver& aObserver )
	{
	CLocPsyConfigUiServiceClientImpl* self = 
						new(ELeave) CLocPsyConfigUiServiceClientImpl( aReg, aObserver );
	CleanupStack::PushL(self);
	// No second phase construction
	return self;
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClientImpl::StartServerL
// -----------------------------------------------------------------------------
//
void CLocPsyConfigUiServiceClientImpl::StartServerL()
	{
	iLaunchAppService = CAknLaunchAppService::NewL( iAppRegInfo.AppUid(), iObserver, NULL );
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClientImpl::AppUid
// -----------------------------------------------------------------------------
//
TInt CLocPsyConfigUiServiceClientImpl::PsyId()
	{
	return iAppRegInfo.RegId();
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClient::AppUid
// -----------------------------------------------------------------------------
//	
TUid CLocPsyConfigUiServiceClientImpl::AppUid()
	{
	return iAppRegInfo.AppUid();
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClientImpl::CloseServe
// -----------------------------------------------------------------------------
//	
void CLocPsyConfigUiServiceClientImpl::CloseServer( TInt reason )
	{
	iObserver->HandleServerAppExit(reason);
	}
