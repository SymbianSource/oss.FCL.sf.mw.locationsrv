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
* Description:  Implementes settings extension API
*
*/


// System Include
#include <lbs/epos_cposmodules.h>
#include <lbs/epos_cposmoduleidlist.h>
#include <lbs/epos_cposmoduleupdate.h> // CPosModuleUpdate
#include <lbs/epos_mposmodulesobserver.h> 

// User Include
#include "locpsysettings.h"
#include "locpsyengdebug.h"
#include "locpsyengobserver.h"
#include "locpsyinfo.h" 
#include "locpsysettingsengconsts.h"
#include "locpsyconfigclientimpl.h"
#include "locpsyconfigclientwrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// Factory function
CLocPsySettings* CLocPsySettings::NewL()
    {
    CLocPsySettings* self = new( ELeave ) CLocPsySettings();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::CLocPsySettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLocPsySettings::CLocPsySettings()
    {
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLocPsySettings::ConstructL()
    {
    iModuleDb = CPosModules::OpenL();
    iPsyList = iModuleDb->ModuleIdListL();
    iPsy = new (ELeave) CLocPsyInfo();

    iModuleDb->SetObserverL( *this );
    }

// Destructor

CLocPsySettings::~CLocPsySettings()
    {
    if ( iModuleDb )
    	{
    	iModuleDb->RemoveObserver();
    	delete iModuleDb;
    	}

    delete iPsyList;
    delete iPsy;
    delete iServClient;
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::GetPsyL
// -----------------------------------------------------------------------------
//
CLocPsyInfo* CLocPsySettings::GetPsyL( TInt aIndex )
    {
    TPositionModuleInfo moduleInfo;
    // Sanity check of input parameter
    if ( aIndex < 0 || aIndex >= iPsyList->Count())
    	{
    	// Invalid index values so Leave indicating bad argument
    	User::Leave(KErrArgument);
    	}
    TPositionModuleId moduleId = iPsyList->At( aIndex );
    iModuleDb->GetModuleInfoL( moduleId, moduleInfo);
    iPsy->SetPsyId( moduleId );
    TBuf<KLocPsyMaxLen>* buf=new (ELeave) TBuf<KLocPsyMaxLen>;
    moduleInfo.GetModuleName( *buf );
    CleanupStack::PushL( buf );
    iPsy->SetPsyNameL( *buf );
    CleanupStack::PopAndDestroy(buf);
    return iPsy;
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::HandleSettingsChangeL
// -----------------------------------------------------------------------------
//
void CLocPsySettings::HandleSettingsChangeL(TPosModulesEvent aEvent)
    {
	LOCENGDEBUG("CLocPsySettings::HandleSettingsChangeL called");
    CPosModuleIdList* temp = iModuleDb->ModuleIdListL();
    delete iPsyList;

    iPsyList = temp;
	LOCENGDEBUG3("CLocPsySettings::HandleSettingsChangeL event=%d psy=%d no of PSYs=%d",
				aEvent.iType, aEvent.iModuleId, iPsyList->Count());
				
    if( aEvent.iType == EPosModulesEventModuleRemoved && iServClient &&
    	aEvent.iModuleId == TUid::Uid(iServClient->PsyId()) )
    	{
		LOCENGDEBUG("Observer: PSY that is being configured is uninstalled");
    	iServClient->CloseServer(KErrAbort);
    	}
    if(iObserver)
    	{
    	iObserver->NotifyL();
    	}
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::EnablePsyL
// -----------------------------------------------------------------------------
//
void CLocPsySettings::EnablePsyL( TInt aIndex )
    {
    CLocPsyInfo* psyInfo = GetPsyL(aIndex);
    CPosModuleUpdate* posModuleUpdate = CPosModuleUpdate::NewLC();   
    posModuleUpdate->SetUpdateAvailability( ETrue );
    iModuleDb->UpdateModuleL( psyInfo->PsyId(), *posModuleUpdate );
	LOCENGDEBUG2("CLocPsySettings::EnablePsyL: PSY %d index %d",
				psyInfo->PsyId(), aIndex);
    CleanupStack::PopAndDestroy(posModuleUpdate);
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::DisablePsyL
// -----------------------------------------------------------------------------
//
void CLocPsySettings::DisablePsyL( TInt aIndex )
    {
    CLocPsyInfo* psyInfo = GetPsyL(aIndex);
    CPosModuleUpdate* posModuleUpdate = CPosModuleUpdate::NewLC();   
    posModuleUpdate->SetUpdateAvailability( EFalse );
    iModuleDb->UpdateModuleL( psyInfo->PsyId(), *posModuleUpdate );
	LOCENGDEBUG2("CLocPsySettings::DisablePsyL: PSY %d index %d",
				psyInfo->PsyId(), aIndex);
    CleanupStack::PopAndDestroy(posModuleUpdate);
    }

// -----------------------------------------------------------------------------
// CLocPsySettings::ConfigurePsyL
// -----------------------------------------------------------------------------
//
void CLocPsySettings::ConfigurePsyL(TInt aIndex, 
									MAknServerAppExitObserver& aObserver)
	{
	CLocPsyConfigUiReg* reg = NULL;
	TRAPD(error, GetPsyL( aIndex );
    		reg = CLocPsyConfigUiServiceClientImpl::DiscoverLC(
    							iPsy->PsyId().iUid);
    		CleanupStack::Pop(reg););
	// If GetModuleInfoL is not supported it returns KErrNotSupported
	// If DiscoverLC fails it returns KErrNotFound or KErrNoMemory
	if ( error == KErrNone && reg )
		{
		CleanupStack::PushL( reg );
		LOCENGDEBUG2("CLocPsySettings::ConfigurePsyL: PSY %d index %d",
					iPsy->PsyId(), aIndex);
		iServClient = static_cast<CLocPsyConfigUiServiceClientImpl*>
		 		(CLocPsyConfigUiServiceClientImpl::NewLC(*reg, aObserver));
		CleanupStack::Pop(iServClient);		 		
		iServClient->StartServerL();
		CleanupStack::PopAndDestroy( reg );
		}
	}

// -----------------------------------------------------------------------------
// CLocPsySettings::ClosePsyConfigurationApp
// -----------------------------------------------------------------------------
//
void CLocPsySettings::ClosePsyConfigurationApp()
	{
	delete iServClient;
	iServClient = NULL;
	}
	
// -----------------------------------------------------------------------------
// CLocSettings::IsPsyEnabled
// -----------------------------------------------------------------------------
//
TBool CLocPsySettings::IsPsyEnabled( TInt aIndex )
	{
    TPositionModuleInfo moduleInfo;
    // Sanity check of input parameter
    if (aIndex < 0 || aIndex >= iPsyList->Count() )
    	{
    	// Invalid index values so just return False
    	return EFalse;
    	}
    TPositionModuleId moduleId = iPsyList->At( aIndex );
    TRAPD(error, iModuleDb->GetModuleInfoL( moduleId, moduleInfo););
    // If GetModuleInfoL returns KErrNotSupported we return EFalse
    if(error == KErrNone ) 
    	{
    	return moduleInfo.IsAvailable();
    	}
    else
    	{
    	return EFalse;	
    	}
	}


// -----------------------------------------------------------------------------
// CLocSettings::IsPsyConfigurable
// -----------------------------------------------------------------------------
//
TBool CLocPsySettings::IsPsyConfigurable( TInt aIndex )
	{
	CLocPsyConfigUiReg* reg = NULL;
    TRAPD(error, GetPsyL( aIndex );
    		reg = CLocPsyConfigUiServiceClientImpl::DiscoverLC(
    							iPsy->PsyId().iUid);
    		CleanupStack::Pop(reg););	
	// If GetModuleInfoL is not supported it returns KErrNotSupported
	// If DiscoverLC fails it returns KErrNotFound or KErrNoMemory
	if (error == KErrNone && reg )
		{
		delete reg;
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

// -----------------------------------------------------------------------------
// CLocSettings::SetObserver
// -----------------------------------------------------------------------------
//
void CLocPsySettings::SetObserver( MLocPsyEngObserver& aObserver )
    {
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CLocSettings::RemoveModelObserver
// -----------------------------------------------------------------------------
//
void CLocPsySettings::RemoveModelObserver()
    {
    iObserver = NULL;
    }

//  End of File  
