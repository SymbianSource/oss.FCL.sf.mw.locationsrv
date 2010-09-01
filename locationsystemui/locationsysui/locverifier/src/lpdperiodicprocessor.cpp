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
* Description:   Helps retrieving request related information
*
*/


// INCLUDE FILES
//#include <eikenv.h>
#include <StringLoader.h>
#include <locverifierdlg.rsg>
#include <aknmessagequerydialog.h>
#include <epos_csuplsettingsconstants.h>
#include <e32math.h>
#include <badesca.h>
#include <aknconsts.h>
#include "lpdperiodicprocessor.h"
#include "locverifierdlgdebug.h"
#include "lpdverifiersettinglauncher.h"
#include "lpdsuplsettingsadapter.h"


// CONSTANTS
const TInt KRequestorNameSize   = 256;
const TInt KMinToSecs = 60;
const TInt KHrToSecs  = 3600;
const TInt KdayToSecs = 86400;
const TInt KRealWidth = 50;
_LIT( KNewLineEntry, "\n" );


// Unnamed namespace for local definitions

#ifdef _DEBUG
_LIT( KPanicText, "CLpdPeriodicProcessor" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif

// LinkClickedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
static TInt LinkClickedL(TAny* aPtr)
    {
    CLpdPeriodicProcessor* self = static_cast<CLpdPeriodicProcessor*>(aPtr);
    self->LaunchSettingsL();
    return ETrue;
    }


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::CLpdPeriodicProcessor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdPeriodicProcessor::CLpdPeriodicProcessor( MLpdPeriodicRequestObserver& aObserver )
: iObserver(aObserver), iMessageText( NULL ), iRequestorName( NULL ), iSessionId(-1), iRequestorId(-1) 
    {
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdPeriodicProcessor* CLpdPeriodicProcessor::NewL( MLpdPeriodicRequestObserver& aObserver )
    {
    CLpdPeriodicProcessor* self = CLpdPeriodicProcessor::NewLC( aObserver );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdPeriodicProcessor* CLpdPeriodicProcessor::NewLC( MLpdPeriodicRequestObserver& aObserver)
    {
    CLpdPeriodicProcessor* self = new( ELeave ) CLpdPeriodicProcessor ( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CLpdPeriodicProcessor::~CLpdPeriodicProcessor()
    {
    delete iRequestor;    
    delete iSUPLSettingsAdapter;
    delete iVerifierSettingsLauncher;
    delete iMessageText;
    delete iRequestorName;
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::ConstructL()
    {
    iSUPLSettingsAdapter = CLpdSUPLSettingsAdapter::NewL(*this);
    iVerifierSettingsLauncher = CLpdVerifierSettingsLauncher::NewL();
    iRequestorName = HBufC::NewL( KRequestorNameSize );
    iEikEnv = CEikonEnv::Static();

    }
    
// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::LaunchSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::LaunchSettingsL( )
    {
    if( iVerifierSettingsLauncher )
		{
		iVerifierSettingsLauncher->LaunchL( iSessionId );
		}
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::GetSessionId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt64 CLpdPeriodicProcessor::GetSessionId( )
    {
    return iSessionId;
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicRequestProcessor::SetRequestorId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::SetRequestorIdL( TInt32 aRequestorId )
    {
    __ASSERT_DEBUG( aRequestorId != KPosNullQNRequestId,
                    HandleAssertErrorL() );
    
    iRequestorId = aRequestorId;
    }
        
// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::SetSessionId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::SetSessionIdL( TInt64 aSessionId )
    {
    __ASSERT_DEBUG( iRequestorId != KPosNullQNRequestId,
                    HandleAssertErrorL() );
	
	  iSessionId = aSessionId;
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::SetRequestorIdL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::SetRequestorNameL( HBufC* aRequestorName)
   {
   __ASSERT_DEBUG( iRequestorId != KPosNullQNRequestId,
                   HandleAssertErrorL() );
   iRequestorName->Des().Copy( aRequestorName->Des() );
   }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::SetRequestTypeL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::SetRequestTypeL( 
        CPosPrivacyNotifier::TRequestType aRequestType )
    {
    	 __ASSERT_DEBUG( iRequestorId != KPosNullQNRequestId,
                    HandleAssertErrorL() );
	  iRequestType = aRequestType;
    }	

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::SetRequestorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::SetRequestorL( CPosRequestor* aRequestor )
    {
     __ASSERT_DEBUG( iRequestorId != KPosNullQNRequestId,
                    HandleAssertErrorL() );
    if( iRequestor )
        {
        delete iRequestor;
        iRequestor = NULL;
        }
    
    iRequestor = aRequestor;
    }    

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::ResetParameters
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::ResetParameters()
    {
    iRequestorId = -1;
    iSessionId = -1;
    if( iRequestor )
        {
        delete iRequestor;
        iRequestor = NULL;
        }
    
    }   

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::NotificationMessageText
// Used to prepare notification message text
// -----------------------------------------------------------------------------
//

void CLpdPeriodicProcessor::NotificationMessageTextL( HBufC*& aMessageText)
    {
    __ASSERT_DEBUG( iRequestorId != KPosNullQNRequestId,
               HandleAssertErrorL() );    
    TUint aTimePeriod;
    
    TRAPD( ret, iSUPLSettingsAdapter->GetTriggerPeriodL( iSessionId, aTimePeriod)); 
    
    if(ret != KErrNone)
    {
     User::Leave(ret);    	
    }

    if( iMessageText )
		 {
	     delete iMessageText;
	     iMessageText = NULL;
		 }   

    TInt resourceId;
    TBuf<KRealWidth> periodString;
    HBufC* msgText = NULL;

    GetResourceAndPeriodL( aTimePeriod, resourceId, periodString );
    
    GetMessageStringL(msgText, resourceId, *iRequestorName, periodString );
   
    if( msgText )
    {
    CleanupStack::PushL( msgText );
    HBufC* linkText = NULL;
    linkText = StringLoader::LoadL( R_LOC_NOTIFICATION_SERVICE_LINK, iEikEnv );
    CleanupStack::PushL( linkText );
    iMessageText = HBufC::NewL( msgText->Length() + KNewLineEntry().Length() + KOpeningLinkTag().Length() + 
                                                linkText->Length() + KClosingLinkTag().Length() );
    iMessageText->Des().Copy( *msgText );
    iMessageText->Des().Append( KNewLineEntry );
    iMessageText->Des().Append( KOpeningLinkTag );
    iMessageText->Des().Append( *linkText );
    iMessageText->Des().Append( KClosingLinkTag );
    
    CleanupStack::PopAndDestroy( ); //msgText
    CleanupStack::PopAndDestroy( ); //linkText
    //delete msgText; 
    //delete linkText; 
    }
    else
    {
      iMessageText = HBufC::NewL( KMaxTriggerSessionNameLen );
    }         
    
    aMessageText->Des().Copy(iMessageText->Des());
  }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::LinkCallBack
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCallBack CLpdPeriodicProcessor::LinkCallBack()
    {
    return TCallBack(LinkClickedL, this);
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicRequestProcessor::UpdateSessionL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::UpdateSessionL( TInt64 aSessionId )
    {
	if( iSessionId == aSessionId &&
	iRequestType == CPosPrivacyNotifier::ENotification && iRequestor
	 && CPosRequestor::ERequestPeriodic == iRequestor->RequestType())
      {
	    iObserver.UpdateCurrentNotifierL();
	    }
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::GetMessageStringL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::GetMessageStringL( HBufC*& aMessageString, 
                    TInt& aResourceId, TDesC& aNameString, TDesC& aPeriodString )
    {

    switch( aResourceId )
        {
        case R_LOC_NOTIFICATION_ONEHOUR:
            {
            aMessageString = StringLoader::LoadL( R_LOC_NOTIFICATION_ONEDAY, aNameString, iEikEnv ); 
            break;
            }
        case R_LOC_NOTIFICATION_ONEDAY:
            {
            aMessageString = StringLoader::LoadL( R_LOC_NOTIFICATION_ONEDAY, aNameString, iEikEnv ); 
            break;
            }
        case R_LOC_NOTIFICATION_MINUTES:
        case R_LOC_NOTIFICATION_HOURS:
        case R_LOC_NOTIFICATION_DAYS:
        default:
            {
            CDesCArrayFlat* strings = new CDesCArrayFlat( 2 );
            CleanupStack::PushL( strings );
                    
            strings->AppendL( aNameString ); // aNameString string
            strings->AppendL( aPeriodString ); // aPeriodString string
            
            aMessageString = StringLoader::LoadL(aResourceId, *strings, iEikEnv );
            
            CleanupStack::PopAndDestroy( ); // strings
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::GetResourceAndPeriodL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::GetResourceAndPeriodL( TUint aSeconds, 
									TInt& aResourceId, TDes& aPeriodString )
    {
    TRealFormat fmt(KDefaultRealWidth, 2);

    if( !aSeconds )
        {
        aResourceId = R_LOC_NOTIFICATION_MINUTES;
        aPeriodString.Num(aSeconds);
        return;
        }

    TReal hrs = TReal(aSeconds)/KHrToSecs;
    TReal value;
    if( hrs < 1 )
        {
        TReal mins = TReal(aSeconds)/KMinToSecs; 
        aResourceId = R_LOC_NOTIFICATION_MINUTES;
        Math::Round( value, mins, 2 );
        aPeriodString.Num(value, fmt);
        }
    else if( hrs == 1 )
        {
        aResourceId = R_LOC_NOTIFICATION_ONEHOUR;
        }
    else
        {
        TReal days = TReal(aSeconds)/KdayToSecs;
        if( days < 1 )
            {
            aResourceId = R_LOC_NOTIFICATION_HOURS;
            Math::Round( value, hrs, 2 );
            aPeriodString.Num(value, fmt);
            }
        else if( days == 1 )
            {
            aResourceId = R_LOC_NOTIFICATION_ONEDAY;
            }
        else
            {
            aResourceId = R_LOC_NOTIFICATION_DAYS;
            Math::Round( value, days, 2 );
            aPeriodString.Num(value, fmt);
            }
        }
    }
        
// -----------------------------------------------------------------------------
// CLpdPeriodicProcessor::HandleAssertErrorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdPeriodicProcessor::HandleAssertErrorL() const
    {
    #ifdef _DEBUG
        User::Panic( KPanicText, KLpdErrGeneral );
    #else
        User::Leave( KErrCorrupt );
    #endif
    }	
// end of file
