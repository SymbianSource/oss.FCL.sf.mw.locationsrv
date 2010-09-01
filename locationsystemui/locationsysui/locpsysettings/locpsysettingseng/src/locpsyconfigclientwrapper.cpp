/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  PSY Configuration UI launching helper class implementations.
 *
*/

 
// System Include
#include <apgcli.h>
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <apaid.h>
#else
#include <apaid.h>
#include <apaidpartner.h>
#endif
#include <barsread2.h>
#include <LocPsyConfigUiUID.hrh>

// User Include
#include "locpsyconfigclientwrapper.h"

// -----------------------------------------------------------------------------
// CLocPsyConfigUiReg::CLocPsyConfigUiReg
// -----------------------------------------------------------------------------
//
CLocPsyConfigUiReg::CLocPsyConfigUiReg()
{
}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiReg::NewL
// -----------------------------------------------------------------------------
//
CLocPsyConfigUiReg* CLocPsyConfigUiReg::NewL( TUid aApp, const TDesC8& aResource )
	{
	CLocPsyConfigUiReg* reg = new(ELeave) CLocPsyConfigUiReg;
	CleanupStack::PushL(reg);
	reg->ConstructL( aApp, aResource );
	CleanupStack::Pop(reg);
	return reg;
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiReg::ConstructL
// -----------------------------------------------------------------------------
//	
void CLocPsyConfigUiReg::ConstructL( TUid aApp, const TDesC8& aResource )
	{
	iAppUid = aApp;
	RResourceReader r;
	r.OpenLC(aResource);
	iRegId = r.ReadInt32L();					// read psy_uid
	CleanupStack::PopAndDestroy(&r);
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiReg::AppUid
// -----------------------------------------------------------------------------
//
TUid CLocPsyConfigUiReg::AppUid()
	{
	return iAppUid;
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiReg::RegId
// -----------------------------------------------------------------------------
//	
TInt CLocPsyConfigUiReg::RegId()
	{
	return iRegId;
	}

// -----------------------------------------------------------------------------
// CLocPsyConfigUiServiceClient::DiscoverLC
// -----------------------------------------------------------------------------
//	
CLocPsyConfigUiReg* CLocPsyConfigUiServiceClient::DiscoverLC( const TInt PsyUid )
	{
	// Get all of the implementations of the transform service
	CLocPsyConfigUiReg* regInfo = NULL;
	TInt error;
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);
	CApaAppServiceInfoArray* services = NULL;
	do
		{
		// If Error is EAppListInvalid then try again till you get the list.
		// The services structure
		TRAP(error, 
				services = ls.GetServiceImplementationsLC(
									TUid::Uid(KLocPsyConfigServiceUid));
				CleanupStack::Pop(services););
		} while (error == RApaLsSession::EAppListInvalid);
	CleanupStack::PushL(services);
	
	// If there was an error then leave
	User::LeaveIfError(error);

	TArray< TApaAppServiceInfo > ServInfoArray = services->Array();
	for (TInt i=0; i<ServInfoArray.Count() ; i++)
		{
		// This is under TRAP so that corrupt resources don't crash the 
		// discovery procedure.
		// The TRAP's return code is ignored intentionally
		const TApaAppServiceInfo& appInfo = ServInfoArray[i];
		TRAP(error, regInfo = 
			CLocPsyConfigUiReg::NewL(appInfo.Uid(), appInfo.OpaqueData()););
		if(	error == KErrNone && regInfo->RegId() == PsyUid )
			{
			// Found the correct PSY module so just break out of the loop
			// and return the regInfo from outside the loop.
			break;
			}
		else
			{
			delete regInfo;
			regInfo = NULL;
			}
		}
	
	CleanupStack::PopAndDestroy(services);
	CleanupStack::PopAndDestroy() ;// ls
	CleanupStack::PushL(regInfo);
	return regInfo;
	}
