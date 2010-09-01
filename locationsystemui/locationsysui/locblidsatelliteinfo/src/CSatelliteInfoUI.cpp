/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements interface for CSatelliteInfoUI
*
*/


// INCLUDE FILES
#include <AknPanic.h>
#include "CSatelliteInfoUI.h"
#include "CSatellite.h"
#include "SatInfoConsts.h"
#include "Debug.h"

#include <locationvariationdomaincrkeys.h>
#include <centralrepository.h>
// Panic for set launch view panic
_LIT(KSatellitePanic, "BLID Satellite");

// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// CSatelliteInfoUI::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CSatelliteInfoUI* CSatelliteInfoUI::NewL()
    {
    CSatelliteInfoUI* self = new( ELeave ) CSatelliteInfoUI;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::ConstructL
// ---------------------------------------------------------
//
void CSatelliteInfoUI::ConstructL()
    {    
    iSatellite = CSatellite::NewL();
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::CSatelliteInfoUI
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CSatelliteInfoUI::CSatelliteInfoUI()
    {
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::~CSatelliteInfoUI
// ---------------------------------------------------------
//
EXPORT_C CSatelliteInfoUI::~CSatelliteInfoUI()
    {
    if ( iDestroyedPtr ) 
        {
        *iDestroyedPtr = ETrue;
        }
    delete iSatellite;
    }

// ---------------------------------------------------------
// CSatelliteInfoUI::ExecuteLD
// ---------------------------------------------------------
//                       
EXPORT_C TInt CSatelliteInfoUI::ExecuteLD( const TDesC& aNameOfRule )
    {
    if(aNameOfRule.Compare(KNullDesC) == 0 || aNameOfRule.Length() > 255 || aNameOfRule.Length() == 0)
    	{
    	delete this;
    	User::Leave(KErrArgument);
    	}
    
    TBool thisDestroyed = EFalse;
    iDestroyedPtr = &thisDestroyed;
    
    CleanupStack::PushL( this );

    TInt ret ( iSatellite->ExecuteL( aNameOfRule ) );

    if ( thisDestroyed )
        {
        // this object has been deleted already
        CleanupStack::Pop( this );
        }
    else
        {
        CleanupStack::PopAndDestroy(this); //this
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::HandleForegroundEventL
// ----------------------------------------------------------------------------
//
EXPORT_C void CSatelliteInfoUI::HandleForegroundEventL(TBool aForeground)
    {
    if ( iSatellite )
        {
        iSatellite->HandleForegroundEventL( aForeground );
        }
    }

// ----------------------------------------------------------------------------
// CSatelliteInfoUI::SetLaunchView
// ----------------------------------------------------------------------------
//
EXPORT_C void CSatelliteInfoUI::SetLaunchView(TSatelliteView aLaunchView)
	{
	switch(aLaunchView)
	    {
	    case ESatelliteFirmamentView:
	        {
		    iSatellite->SetLaunchView(EGraphicalView);
	        break;
	        }	    
	    case ESatelliteSignalStrengthView:
	        {
		    iSatellite->SetLaunchView(ESignalStrengthView);
	        break;
	        }
	    case ESatelliteCompassView:
	        {
                TInt error = KErrNone;
                TInt returnCode = KErrNone;
                TInt settingsValue = 0;
		        CRepository* repository = NULL;
	            TRAP(error, repository = CRepository::NewL( TUid::Uid( KCRUidLocLocalVariation) ));
	            TRAP(error, CleanupStack::PushL( repository ); returnCode = repository->Get( KLocHideCoordinates,settingsValue );CleanupStack::PopAndDestroy( repository ) );
	            if(returnCode != KErrNone)
	                {
	                return;
	                }
         
	            if ( ELocCoordinatesHidden == settingsValue)
	            {
		             iSatellite->SetLaunchView(EGraphicalView);
		             break;	
	            }
		    iSatellite->SetLaunchView(ECompassView);
	        break;
	        }
	    default:
	        {
		    User::Panic(KSatellitePanic, EAknPanicOutOfRange);
	        break;
	        }
	    }
	}

// End of File
