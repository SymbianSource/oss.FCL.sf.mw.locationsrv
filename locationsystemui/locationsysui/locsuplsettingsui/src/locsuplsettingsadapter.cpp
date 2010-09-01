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
* Description:  Adapter to the SUPL Settings API
*
*/


// System Includes
#include <epos_csuplsettings.h>
#include <locsuplsettingsui.rsg>
#include <StringLoader.h>

// User Includes
#include "locsuplsettingsadapter.h"
#include "locsuplsettingsadapterobserver.h"
#include "locsuplsettingssessionobserver.h"
#include "locsupldebug.h"



// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsAdapter::CLocSUPLSettingsAdapter( 
                            MLocSUPLSettingsAdapterObserver&    aObserver )
		:CActive( EPriorityStandard ),
		iObserver( aObserver )
    {
    }
 
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
CLocSUPLSettingsAdapter::~CLocSUPLSettingsAdapter()
    {
	DEBUG( + CLocSUPLSettingsAdapter::~CLocSUPLSettingsAdapter );	
    Cancel();
    
    if( iSUPLSettings )
        {
        iSUPLSettings->RemoveObserver();
        iSUPLSettings->RemoveSessionObserver();
        }
        
    delete iSUPLSettings;
    iSUPLSettings = NULL;
    
    delete iAutomatic;
    iAutomatic = NULL;
    
    delete iAutomaticatHome;
    iAutomaticatHome = NULL;
    
    delete iAsk;
    iAsk = NULL;
    
    delete iDisable;
    iDisable = NULL;
	DEBUG( - CLocSUPLSettingsAdapter::~CLocSUPLSettingsAdapter );	
    } 

// ---------------------------------------------------------------------------
// CLocSUPLSettingsAdapter* CLocSUPLSettingsAdapter::NewL
// Static Two phase contructor that instantiates the CLocSUPLSettingsAdapter
// 
// @param aObserver				    Observer to the SUPL settings adapter
// @return CLocSUPLSettingsAdapter*	Reference to the object created
// ---------------------------------------------------------------------------
//    
CLocSUPLSettingsAdapter* CLocSUPLSettingsAdapter::NewL(
                            MLocSUPLSettingsAdapterObserver&    aObserver )
    {
	DEBUG( + CLocSUPLSettingsAdapter::NewL );	
    CLocSUPLSettingsAdapter* self = 
                new ( ELeave ) CLocSUPLSettingsAdapter( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettingsAdapter::NewL );	
    return self;
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::ConstructL
// Second phase of the two phase constructor
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::ConstructL()
    {
	DEBUG( + CLocSUPLSettingsAdapter::ConstructL );	
    // Create the SUPL Settings
    iSUPLSettings = CSuplSettings::NewL();
    
    // Set the Observer for SUPL Settings
    iSUPLSettings->SetObserverL( *this );
    
    // Set the Observer for SUPL Sessions
    iSUPLSettings->SetSessionObserverL( *this );
    
    // Load the SUPL settings usage strings
    iAutomatic 			= StringLoader::LoadL( R_LOC_SUPL_AUTOMATIC );
    iAutomaticatHome 	= StringLoader::LoadL( R_LOC_SUPL_HOME_AUTOMATIC );
    iAsk 				= StringLoader::LoadL( R_LOC_SUPL_ASK );
    iDisable 			= StringLoader::LoadL( R_LOC_SUPL_DISABLED );
    
    CActiveScheduler::Add( this );
	DEBUG( - CLocSUPLSettingsAdapter::ConstructL );	
    }           

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::Initialize
// Initializes the SUPL Settings API. This is an asynchronus call, the call
// back for which is given through the observer
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::Initialize()
    {
	DEBUG( + CLocSUPLSettingsAdapter::Initialize );	
    if( !IsActive())
        {
        iSUPLSettings->Initialize( iStatus );
        SetActive();
        }
	DEBUG( - CLocSUPLSettingsAdapter::Initialize );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::CancelInitialize
// Cancels the Initialization
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::CancelInitialize()
    {
    Cancel();
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetSuplUsageL
// ---------------------------------------------------------------------------
//       
void CLocSUPLSettingsAdapter::SetSuplUsageL( 
            const CLocSUPLSettingsAdapter::TLocSuplUsage    aIndex )
    {
	DEBUG( + CLocSUPLSettingsAdapter::SetSuplUsageL );	
    CSuplSettings::TSuplSettingsUsage value = CSuplSettings::ESuplUsageAlwaysAsk;
    switch ( aIndex )
        {
        case ELocSuplAutomatic:
            {
            value = CSuplSettings::ESuplUsageAutomatic;
            break;
            }
        case ELocSuplAutomaticatHome:
            {
            value = CSuplSettings::ESuplUsageHomeAutomatic;
            break;
            }
        case ELocSuplAlwaysAsk:
            {
            value = CSuplSettings::ESuplUsageAlwaysAsk;
            break;
            }
        case ELocSuplDisable:
            {
            value = CSuplSettings::ESuplUsageDisabled;
            break;
            }
        default:
            {
            User::Leave( KErrNotFound );
            break;    
            }
        }

    // Write back to the SUPL settings API
    TInt ret = iSUPLSettings->SetSuplUsage( value );            
            
    // If the Set failed, then Leave with the corresponding Error code
    if( ret )
        {
        User::Leave ( ret );
        }
	DEBUG( - CLocSUPLSettingsAdapter::SetSuplUsageL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetSuplUsageL
// ---------------------------------------------------------------------------
//  
void CLocSUPLSettingsAdapter::GetSuplUsageL( TDes&    aSuplUsage )
    {
	DEBUG( + CLocSUPLSettingsAdapter::GetSuplUsageL );	
    CSuplSettings::TSuplSettingsUsage value = CSuplSettings::ESuplUsageAlwaysAsk;
    User::LeaveIfError( iSUPLSettings->GetSuplUsage( value ));

    switch ( value )
        {
        case CSuplSettings::ESuplUsageAutomatic:
            {
            if ( aSuplUsage.MaxLength() < iAutomatic->Des().Length())
                {
                User::Leave( KErrNoMemory );
                }
            aSuplUsage.Copy( *iAutomatic );
            break;
            }
        case CSuplSettings::ESuplUsageHomeAutomatic:
            {
            if ( aSuplUsage.MaxLength() < iAutomaticatHome->Des().Length())
                {
                User::Leave( KErrNoMemory );
                }
            aSuplUsage.Copy( *iAutomaticatHome );          
            break;
            }
        case CSuplSettings::ESuplUsageAlwaysAsk:
            {
            if ( aSuplUsage.MaxLength() < iAsk->Des().Length())
                {
                User::Leave( KErrNoMemory );
                }            
            aSuplUsage.Copy( *iAsk );
            break;
            }
        case CSuplSettings::ESuplUsageDisabled:
            {
            if ( aSuplUsage.MaxLength() < iDisable->Des().Length())
                {
                User::Leave( KErrNoMemory );
                }
            aSuplUsage.Copy( *iDisable );
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
	DEBUG( - CLocSUPLSettingsAdapter::GetSuplUsageL );	
    }

// ---------------------------------------------------------------------------
// TLocSuplUsage CLocSUPLSettingsAdapter::GetSuplUsage
// ---------------------------------------------------------------------------
// 
TInt CLocSUPLSettingsAdapter::GetSuplUsage()
    {
	DEBUG( + CLocSUPLSettingsAdapter::GetSuplUsage TInt );	
    CSuplSettings::TSuplSettingsUsage value = CSuplSettings::ESuplUsageAlwaysAsk;
    TInt error = iSUPLSettings->GetSuplUsage( value );
    if ( error )
        {
        // If there is an error then return the default value
        return CSuplSettings::ESuplUsageAlwaysAsk;
        }
	DEBUG( - CLocSUPLSettingsAdapter::GetSuplUsage TInt );	
    return value;
    }

// ---------------------------------------------------------------------------
// TLocSuplUsage CLocSUPLSettingsAdapter::GetSuplUsageIndex
// ---------------------------------------------------------------------------
// 
CLocSUPLSettingsAdapter::TLocSuplUsage CLocSUPLSettingsAdapter::GetSuplUsageIndex()
    {    
	DEBUG( + CLocSUPLSettingsAdapter::GetSuplUsageIndex );	
    TLocSuplUsage index = ELocSuplAutomatic;
    CSuplSettings::TSuplSettingsUsage value = CSuplSettings::ESuplUsageAlwaysAsk;
    if ( iSUPLSettings->GetSuplUsage( value ))
        {
        // If there is an error then return the default value.
        return index;
        }
        
    switch ( value )
        {
        case CSuplSettings::ESuplUsageAutomatic:
            {
            index = ELocSuplAutomatic;
            break;
            }
        case CSuplSettings::ESuplUsageHomeAutomatic:
            {
            index = ELocSuplAutomaticatHome;         
            break;
            }
        case CSuplSettings::ESuplUsageAlwaysAsk:
            {
            index = ELocSuplAlwaysAsk;
            break;
            }
        case CSuplSettings::ESuplUsageDisabled:
            {
            index = ELocSuplDisable;
            break;
            }
        default:
            {
            break;
            }
        }
    return index;   
    }
    
// ---------------------------------------------------------------------------
// const TDesC& CLocSUPLSettingsAdapter::Automatic
// ---------------------------------------------------------------------------
// 
const TDesC& CLocSUPLSettingsAdapter::Automatic()
    {
    return *iAutomatic;
    }

// ---------------------------------------------------------------------------
// const TDesC& CLocSUPLSettingsAdapter::AutomaticAtHome
// ---------------------------------------------------------------------------
//
const TDesC& CLocSUPLSettingsAdapter::AutomaticAtHome()
    {
    return *iAutomaticatHome;  
    }

// ---------------------------------------------------------------------------
// const TDesC& CLocSUPLSettingsAdapter::AlwaysAsk
// ---------------------------------------------------------------------------
//
const TDesC& CLocSUPLSettingsAdapter::AlwaysAsk()
    {
    return *iAsk;   
    }

// ---------------------------------------------------------------------------
// const TDesC& CLocSUPLSettingsAdapter::Disable
// ---------------------------------------------------------------------------
//
const TDesC& CLocSUPLSettingsAdapter::Disable()
    {
    return *iDisable; 
    }
                   
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::HandleSuplSettingsChangeL
// Handle the Settings UI changes
// ---------------------------------------------------------------------------
//     
void CLocSUPLSettingsAdapter::HandleSuplSettingsChangeL( 
                TSuplSettingsEventType /*aEvent*/,
                TInt /*aSlpId*/                         
        		)
    {
	DEBUG( + CLocSUPLSettingsAdapter::HandleSuplSettingsChangeL );	
    iObserver.HandleSuplSettingsChangeL( 
                    MLocSUPLSettingsAdapterObserver::ESUPLSettingsChanged );
	DEBUG( - CLocSUPLSettingsAdapter::HandleSuplSettingsChangeL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::HandleSuplSessionChangeL
// Handle the Supl Session changes
// ---------------------------------------------------------------------------
//     
void CLocSUPLSettingsAdapter::HandleSuplSessionChangeL( 
                TSuplSessionEventType aEvent,
                TInt64 aSessionId                        
        		)
    {
	DEBUG( + CLocSUPLSettingsAdapter::HandleSuplSessionChangeL );	
	if( iSessionObserver && iSessionObserver->SessionId() == aSessionId )
		{
		MLocSUPLSettingsSessionObserver::TUpdateType updateType = 
                            MLocSUPLSettingsSessionObserver::ESessionModified;
							
		if( MSuplSessionObserver::ESuplSessionDBDeleteEvent == aEvent )
		    updateType = MLocSUPLSettingsSessionObserver::ESessionDeleted;

		iSessionObserver->UpdateSessionL( updateType );
		}
		
    iObserver.HandleSuplSettingsChangeL( 
                    MLocSUPLSettingsAdapterObserver::ESUPLSettingsChanged );
	DEBUG( - CLocSUPLSettingsAdapter::HandleSuplSessionChangeL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::RunL
// Inherited from CActive
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::RunL()
    {
	DEBUG( + CLocSUPLSettingsAdapter::RunL );	
    // If the SUPL settings API has been successfull initalized then a success
    // call back needs to be issued. In all other cases, other than a KErrCancel
    // it is assumed that the initailization has failed.
    if( iStatus.Int() == KErrNone )
        {        
        iObserver.HandleSuplSettingsChangeL( 
                    MLocSUPLSettingsAdapterObserver::ESUPLSettingsIntialized );
        }
    else if( iStatus.Int() != KErrCancel )
        {
        iObserver.HandleSuplSettingsChangeL( 
                    MLocSUPLSettingsAdapterObserver::ESUPLSettingsInitalizeError );    
        }
	DEBUG( - CLocSUPLSettingsAdapter::RunL );	
    }

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::DoCancel
// Inherited from CActive
// ---------------------------------------------------------------------------
//     
void CLocSUPLSettingsAdapter::DoCancel()
    {
    iSUPLSettings->CancelInitialize();
    }


//***************************************************************************
//  
//  @since S60 9.1TB 
// 
// ***************************************************************************

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::AddNewServerL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::AddNewServerL( 
        const TDesC& aServerAddress,
        const TDesC& aIapName,
        const TBool aUsageInHomeNw
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::AddNewServerL );	
	CServerParams* serverParam = CServerParams::NewL();
	CleanupStack::PushL( serverParam );
	User::LeaveIfError(serverParam->Set( 
						aServerAddress, 
						aIapName, 
						ETrue, 
						EFalse, // default value
						aUsageInHomeNw, 
						ETrue ));
	
	TInt64 slpId;	
	User::LeaveIfError( iSUPLSettings->AddNewServer( serverParam, slpId ) );							
	CleanupStack::PopAndDestroy();
	DEBUG( - CLocSUPLSettingsAdapter::AddNewServerL );	
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::RemoveServerL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::RemoveServerL(
		const TInt64 aSlpId
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::RemoveServerL );	
	User::LeaveIfError( iSUPLSettings->RemoveServer( aSlpId ) );
	DEBUG( - CLocSUPLSettingsAdapter::RemoveServerL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetAllSlpL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetAllSlpL(
        RPointerArray<CServerParams>& aParamValues
		) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetAllSlpL );	
	User::LeaveIfError( iSUPLSettings->GetAllSlp( aParamValues ) );
	DEBUG( - CLocSUPLSettingsAdapter::GetAllSlpL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetSlpInfoFromIdL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetSlpInfoFromIdL(
	            const TInt64 aSlpId, 
	            CServerParams *aParamValues
	    ) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetSlpInfoFromIdL );	
	User::LeaveIfError( iSUPLSettings->GetSlpInfoFromId( aSlpId, aParamValues ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetSlpInfoFromIdL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::ChangePriorityL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::ChangePriorityL(
        TInt64 aSlpId, 
        TInt aPriority,
        TBool aDirection
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::ChangePriorityL );	
	User::LeaveIfError( 
		iSUPLSettings->ChangePriority( aSlpId, aPriority, aDirection ) );		
	DEBUG( - CLocSUPLSettingsAdapter::ChangePriorityL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetServerAddressL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetServerAddressL( 
        const TInt64 aSlpId, 
        const TDesC& aServerAddress
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetServerAddressL );	
	User::LeaveIfError( iSUPLSettings->SetServerAddress( aSlpId, aServerAddress ) );	
	DEBUG( - CLocSUPLSettingsAdapter::SetServerAddressL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetServerAddressL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetServerAddressL( 
        TInt64 aSlpId, 
        TDes& aServerAddress
		) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetServerAddressL );	
	User::LeaveIfError( iSUPLSettings->GetServerAddress( aSlpId, aServerAddress ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetServerAddressL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetIapNameL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetIapNameL( 
        const TInt64 aSlpId, 
        const TDesC& aIapName
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetIapNameL );	
	User::LeaveIfError( iSUPLSettings->SetIapName( aSlpId, aIapName ) );	
	DEBUG( - CLocSUPLSettingsAdapter::SetIapNameL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetIapNameL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetIapNameL( 
        const TInt64 aSlpId, 
        TDes& aIapName
		) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetIapNameL );	
	User::LeaveIfError( iSUPLSettings->GetIapName( aSlpId, aIapName ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetIapNameL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetServerEnabledFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetServerEnabledFlagL(
        const TInt64 aSlpId, 
        const TBool aEnable 
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetServerEnabledFlagL );	
	User::LeaveIfError( iSUPLSettings->SetServerEnabledFlag( aSlpId, aEnable ) );	
	DEBUG( - CLocSUPLSettingsAdapter::SetServerEnabledFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetServerEnabledFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetServerEnabledFlagL(
        const TInt64 aSlpId, 
        TBool& aEnable 
		) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetServerEnabledFlagL );	
	User::LeaveIfError( iSUPLSettings->GetServerEnabledFlag( aSlpId, aEnable ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetServerEnabledFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetUsageInHomwNwFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetUsageInHomwNwFlagL(
        const TInt64 aSlpId, 
        const TBool aHomeNwFlag 
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetUsageInHomwNwFlagL );	
	User::LeaveIfError( iSUPLSettings->SetUsageInHomwNwFlag( aSlpId, aHomeNwFlag ) );	
	DEBUG( - CLocSUPLSettingsAdapter::SetUsageInHomwNwFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetUsageInHomwNwFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetUsageInHomwNwFlagL(
        const TInt64 aSlpId, 
        TBool& aHomeNwFlag 
		) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetUsageInHomwNwFlagL );	
	User::LeaveIfError( iSUPLSettings->GetUsageInHomwNwFlag( aSlpId, aHomeNwFlag ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetServerEnabledFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetEditableFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetEditableFlagL( 
	            const TInt64 aSlpId, 
	            const TBool aEditFlag 
	    ) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::SetEditableFlagL );	
	User::LeaveIfError( iSUPLSettings->SetEditableFlag( aSlpId, aEditFlag ) );
	DEBUG( - CLocSUPLSettingsAdapter::SetEditableFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetEditableFlagL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::GetEditableFlagL(
	            const TInt64 aSlpId, 
	            TBool& aEditFlag 
	    ) const
	{
	DEBUG( + CLocSUPLSettingsAdapter::GetEditableFlagL );	
	User::LeaveIfError( iSUPLSettings->GetEditableFlag( aSlpId, aEditFlag ) );	
	DEBUG( - CLocSUPLSettingsAdapter::GetEditableFlagL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SlpCountL
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SlpCountL(
        TInt& aCount 
		)
	{
	DEBUG( + CLocSUPLSettingsAdapter::SlpCountL );	
	User::LeaveIfError( iSUPLSettings->SlpCount( aCount ) );	
	DEBUG( - CLocSUPLSettingsAdapter::SlpCountL );	
	}

// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetActiveSessionsCountL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::GetActiveSessionsCountL( TInt& aActiveSessionsCount )
    {
    DEBUG( + CLocSUPLSettingsAdapter::GetActiveSessionsCountL );
    RPointerArray< CTriggerParams > aParamValues;
    TInt ret = iSUPLSettings->GetTriggerParams( aParamValues );
    if( ret == KErrNotFound )
        {
        aParamValues.ResetAndDestroy();
        aParamValues.Close(); 
        aActiveSessionsCount = 0;
        }
    else if ( ret == KErrNone )
        {
        TInt activeSessionsCount = aParamValues.Count(); 
        aParamValues.ResetAndDestroy();
        aParamValues.Close();   
        aActiveSessionsCount = activeSessionsCount;
        }
    else
        {
        aParamValues.ResetAndDestroy();
        aParamValues.Close();
        //Leave with error
        User::Leave( ret );
        }
        DEBUG( - CLocSUPLSettingsAdapter::GetActiveSessionsCountL );   
    }
 
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetTriggerParamsL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::GetTriggerParamsL( RPointerArray<CTriggerParams>& aParamValues ) const
    {
    DEBUG( + CLocSUPLSettingsAdapter::GetTriggerParamsL );
    TInt ret = iSUPLSettings->GetTriggerParams( aParamValues );
    if(( ret != KErrNotFound ) && ( ret != KErrNone ))
        {
         User::Leave( ret );   
        }
    DEBUG( - CLocSUPLSettingsAdapter::GetTriggerParamsL );  
    }
 
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::GetTriggerParamsL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::GetTriggerParamsL( TInt64 aSessionId, CTriggerParams*& aTrigger ) const
    {
    DEBUG( + CLocSUPLSettingsAdapter::GetTriggerParamsL );
    TInt ret = iSUPLSettings->GetTriggerParams( aSessionId, aTrigger );
    if( !ret )
        {
         User::Leave( ret );   
        }
    DEBUG( - CLocSUPLSettingsAdapter::GetTriggerParamsL );  
    }
 
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::ChangeNotificationStatusL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::ChangeNotificationStatusL( TInt64 aSessionId, TBool aTriggerNotificationStatus ) const
    {
    DEBUG( + CLocSUPLSettingsAdapter::ChangeNotificationStatusL );
    TInt ret = iSUPLSettings->SetNotificationStatus( aSessionId, aTriggerNotificationStatus );
    if(( ret != KErrNotFound ) && ( ret != KErrNone ))
        {
         User::Leave( ret );   
        }
    DEBUG( - CLocSUPLSettingsAdapter::ChangeNotificationStatusL );  
    }
    
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::RemoveTriggerSessionL
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsAdapter::RemoveTriggerSessionL( TInt64 aSessionId ) const
    {
    DEBUG( + CLocSUPLSettingsAdapter::RemoveTriggerSessionL );
    iSUPLSettings->CancelTriggerSession( aSessionId );
    DEBUG( - CLocSUPLSettingsAdapter::RemoveTriggerSessionL );  
    }
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::SetSessionObserver
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::SetSessionObserver( MLocSUPLSettingsSessionObserver* aObserver )
	{
	iSessionObserver = aObserver;
	}
	
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsAdapter::RemoveSessionObserver
// ---------------------------------------------------------------------------
// 
void CLocSUPLSettingsAdapter::RemoveSessionObserver( )
	{
	iSessionObserver = NULL;
	}
    
// End of file
