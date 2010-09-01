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
* Description:  CMnNavigator class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <AknServerApp.h>
#include <apaserverapp.h>

#include <lbsfields.h>
#include <lbsfieldids.h>
#include <EPos_CPosLandmark.h>

#include "mnprovider.h"
#include "mnnavigationserviceclient.h"
#include "mnnavigator.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnNavigator::CMnNavigator()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnNavigator* CMnNavigator::NewL( CMnProvider& aProvider )
    {
    CMnNavigator* self = new (ELeave) CMnNavigator;
    CleanupStack::PushL( self );
    self->ConstructNewL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnNavigator* CMnNavigator::NewChainedL( CMnProvider& aProvider )
    {
    CMnNavigator* self = new (ELeave) CMnNavigator;
    CleanupStack::PushL( self );
    self->ConstructChainedL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnNavigator::ConstructCommonL( CMnProvider& aProvider )
    {
    if ( !( aProvider.SupportedServices() & CMnProvider::EServiceNavigation ) )
        {
        User::Leave( KErrNotSupported );
        }

    iSession = new (ELeave) RMnNavigationServiceClient;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnNavigator::ConstructNewL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectL( aProvider.Uid() );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnNavigator::ConstructChainedL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectChainedL( aProvider.Uid() );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnNavigator::~CMnNavigator()
    {
    delete iExitMonitor;
    if ( iSession )
        {
        iSession->Close();
        delete iSession;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnNavigator::NavigateToL(const CPosLandmark& aDestination)
    {
    // verify that destination landmark has coordinates
    TLocality loc;
    TBool valid = (KErrNone == aDestination.GetPosition( loc ) );
    valid &= !Math::IsNaN( loc.Latitude() ) && !Math::IsNaN( loc.Longitude() );
    
    if ( !valid )
        {
        // verify that destination landmark has address info
        TPositionFieldId fieldId = aDestination.FirstPositionFieldId();
        while ( fieldId != EPositionFieldNone )
            {
            if ( fieldId > EPositionFieldAddressCapabilitiesBegin &&
                 fieldId < EPositionFieldNMEACapabilitiesBegin)
                {
                TPtrC field;
                aDestination.GetPositionField( fieldId, field );
                if ( field.Length() )
                    {
                    valid = ETrue;
                    break;
                    }
                }
            fieldId = aDestination.NextPositionFieldId( fieldId );            
            }
        }
        
    if ( valid )
        {
        iSession->NavigateToL( aDestination );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }
        
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnNavigator::SetExitObserverL( MAknServerAppExitObserver& aObserver )
	{
	if ( iExitMonitor )
		{
		User::Leave( KErrAlreadyExists );
		}
	else
		{
		iExitMonitor = CApaServerAppExitMonitor::NewL( 
			*iSession, aObserver, CActive::EPriorityStandard );
		}
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnNavigator::RemoveExitObserver()
	{
	delete iExitMonitor;
	iExitMonitor = NULL;
	}
