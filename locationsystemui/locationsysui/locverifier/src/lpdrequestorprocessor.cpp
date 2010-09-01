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
* Description:  Processes requestor related data.
*
*/


// INCLUDE FILES
#include "lpdrequestorprocessor.h"
#include "lpdverifnotiflbmodel.h"
#include "lpdbasemodel.h"
#include "locconsts.h"
#include "locverifierdlgdebug.h"
#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_cposcontactrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <EPos_CPosPrivacyNotifier.h>

// CONSTANTS
_LIT( KUnknownRequestor, "Unknown" );

// Unnamed namespace for local definitions

const TInt KRequestorsGranularity = 5;
#ifdef _DEBUG
_LIT( KPanicText, "CLpdRequestorProcessor" );
enum TPanicCode
    {
    KLpdErrGeneral = 1
    };
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::CLpdRequestorProcessor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdRequestorProcessor::CLpdRequestorProcessor() 
: iRequestors( KRequestorsGranularity),
  iSuplRequest ( 0 ),
  iSessionId ( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdRequestorProcessor* CLpdRequestorProcessor::NewL()
    {
    CLpdRequestorProcessor* self = new( ELeave ) CLpdRequestorProcessor;
    // Nothing to do in the Second Phase Constructor
    return self;
    }
    
// Destructor
CLpdRequestorProcessor::~CLpdRequestorProcessor()
    {
    iRequestors.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::ReadRequestorsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLpdRequestorProcessor::ReadRequestorsL(
    const CPosPrivacyNotifier& aPosNotifier )
    {
    __ASSERT_DEBUG( aPosNotifier.CurrentRequest() != KPosNullQNRequestId,
                    HandleAssertErrorL() );

    this->ResetAndDestroyRequestors();
    
    TBool allRequestorIdsValid = ETrue;
	  CLocRequestorUtilsResolver* reqUtils = CLocRequestorUtilsResolver::NewL();
	  CleanupStack::PushL(reqUtils);
    // Let's read the requestors associated with the current request:
    TInt nofRequestors = aPosNotifier.RequestorCountL();

    for ( TInt i = 0; i < nofRequestors; i++ )
        {
        CPosRequestor* requestor = aPosNotifier.RequestorLC( i );
        if ( !reqUtils->RequestorIdValid( *requestor ) )
            { // mark that some requestor id is invalid
            allRequestorIdsValid = EFalse;
            }
   
        if( iSuplRequest )
           {
           TInt64 sessionId = RetrieveSessionId( requestor->RequestorIdString() );
           SetSessionId( sessionId );
           TPtrC RequestorName = RetrieveRequestorNameL(requestor->RequestorIdString());           
           requestor->SetRequestorIdL(requestor->RequestorIdFormat(), RequestorName);
           }
        User::LeaveIfError( iRequestors.Append( requestor ) );
        CleanupStack::Pop( requestor ); // iRequestors owns now the requestor
        }

    if ( nofRequestors == 0 )
        {
        allRequestorIdsValid = EFalse; // zero requestors == invalid
        }

    // Use Requestor Utils and resolve the requestor here itself
    reqUtils->ProcessRequestorsL(iRequestors);
    CleanupStack::PopAndDestroy(reqUtils);
    return allRequestorIdsValid;
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::RtorNamesForVerifNotifL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CLpdBaseModel* CLpdRequestorProcessor::RtorNamesForVerifNotifL(
    CEikFormattedCellListBox& aListBox )
    {
    return CLpdVerifNotifLBModel::NewL( *this, aListBox );
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::ResetAndDestroyRequestors
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestorProcessor::ResetAndDestroyRequestors()
    {
    iRequestors.ResetAndDestroy();
    }

void CLpdRequestorProcessor::SetRequestType( TInt aReqType )
    {
       iSuplRequest = aReqType ;
    } 
// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::Requestors
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const RPointerArray<CPosRequestor>& CLpdRequestorProcessor::Requestors() const
    {
    return iRequestors;
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::HandleAssertErrorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestorProcessor::HandleAssertErrorL() const
    {
    #ifdef _DEBUG
        User::Panic( KPanicText, KLpdErrGeneral );
    #else
        User::Leave( KErrCorrupt );
    #endif
    }

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::RetrieveRequestorsL
// The Requestors information for the current request is read and returned.
// -----------------------------------------------------------------------------
//
RPosRequestorStack* CLpdRequestorProcessor::RetrieveRequestorsL( 
	const CPosPrivacyNotifier& aPosNotifier )
	{
    __ASSERT_DEBUG( aPosNotifier.CurrentRequest() != KPosNullQNRequestId,
                    HandleAssertErrorL() );

    RPosRequestorStack* requestors = new(ELeave) RPosRequestorStack;

    // Let's read the requestors associated with the current request:
    TInt nofRequestors = aPosNotifier.RequestorCountL();

    for ( TInt i = 0; i < nofRequestors; i++ )
        {
        CPosRequestor* requestor = aPosNotifier.RequestorLC( i );
        requestors->AppendL( requestor );
        CleanupStack::Pop( requestor ); // requestors owns now the requestor
        }

    // Return the Index of the Array where the requestor Arary is appended.
    return requestors;
	}

// -----------------------------------------------------------------------------
// CLpdRequestorProcessor::ReadRequestorsL
// Read the requestors passed as parameter. Resolve them and store in
// iRequestors member variable.
// -----------------------------------------------------------------------------
//
void CLpdRequestorProcessor::ReadRequestorsL( 
	const RPosRequestorStack& aRequestors )
	{
    TInt nofRequestors = aRequestors.Count();
	  CLocRequestorUtilsResolver* reqUtils = CLocRequestorUtilsResolver::NewL();
	  CleanupStack::PushL(reqUtils);

    for ( TInt i = 0; i < nofRequestors; i++ )
        {
        CPosRequestor* requestor;
        if( iSuplRequest )
            {
                TInt64 sessionId = RetrieveSessionId( aRequestors[i]->RequestorIdString() );
                SetSessionId( sessionId );
                TPtrC RequestorName = RetrieveRequestorNameL(aRequestors[i]->RequestorIdString());
                aRequestors[i]->SetRequestorIdL(aRequestors[i]->RequestorIdFormat(), RequestorName);
            }
        if (aRequestors[i]->RequestorType() == 
        	CPosRequestor::ERequestorService)
        	{
        	    requestor = CPosServiceRequestor::NewLC(
        					    	aRequestors[i]->RequestorIdFormat(),
        					    	aRequestors[i]->RequestorIdString());
        	}
        else
        	{
        	requestor = CPosContactRequestor::NewLC(
        						aRequestors[i]->RequestorIdFormat(),
        						aRequestors[i]->RequestorIdString());
        	}
        
        iRequestors.AppendL( requestor );
        CleanupStack::Pop( requestor ); // requestors owns now the requestor
        }

    // Return the Index of the Array where the requestor Arary is appended.
    // Use Requestor Utils and resolve the requestor here itself
    reqUtils->ProcessRequestorsL(iRequestors);
    CleanupStack::PopAndDestroy(reqUtils);
	}

TPtrC CLpdRequestorProcessor::RetrieveRequestorNameL(  const TDesC& aString )
{
    //Parse string and extract characters leaving last
    // 3 characters and return as requestor name. 
    //If requestor name not there, say unknown requestor
    if( aString.Length() > 3 )
    {
       TPtrC requestorName = aString.Left( aString.Length() - 3 );
       return requestorName;
    }
    else   
    {
    HBufC* buf = HBufC::NewL( KUnknownRequestor().Length() );
    buf->Des().Append( KUnknownRequestor );
    return buf->Des();
    }
}


TInt64 CLpdRequestorProcessor::RetrieveSessionId(  const TDesC& aString )
{
    //Parse string and return session id(last 3 characters)
    //after converting to TInt
    TInt sessId;
    TInt64 sessionId;
    TPtrC sessionIdPart = aString.Right( 3 );

    TLex lex( sessionIdPart );
    lex.Val( sessId );
    sessionId = sessId;
    return sessionId;
}

void CLpdRequestorProcessor::SetSessionId(  TInt64 aSessionId )
{
    iSessionId = aSessionId;
}
	
TInt64 CLpdRequestorProcessor::GetSessionId()
{
    return iSessionId;
}

// end of file
