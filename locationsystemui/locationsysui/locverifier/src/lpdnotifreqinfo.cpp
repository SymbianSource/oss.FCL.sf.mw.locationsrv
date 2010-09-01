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
* Description:  This class stores information regarding notification 
*                requests.
*
*/

#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_rposrequestorstack.h>
#include "lpdnotifreqinfo.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLpdNotifReqInfo::CLpdNotifReqInfo(
	TPosRequestSource aSource,
	TPosVerifyCancelReason aReason,
	TPosRequestDecision aDecision,
	RPosRequestorStack* aRequestors)
	: iSource(aSource),
	  iReason(aReason),
	  iDecision(aDecision),
	  iRequestors(aRequestors)
  	{
  	}


// ---------------------------------------------------------------------------
// 2nd Phase of Construction
// ---------------------------------------------------------------------------
//
void CLpdNotifReqInfo::ConstructL()
    {
    // Intentionally left empty
    }


// ---------------------------------------------------------------------------
// Symbian 2 Phase Constructor
// ---------------------------------------------------------------------------
//
CLpdNotifReqInfo* CLpdNotifReqInfo::NewL(
	TPosRequestSource aSource,
	TPosVerifyCancelReason aReason,
	TPosRequestDecision aDecision,
	RPosRequestorStack* aRequestors)
    {
    CLpdNotifReqInfo* self = CLpdNotifReqInfo::NewLC(aSource,
													 aReason,
													 aDecision,
													 aRequestors);
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian 2 Phase Constructor
// ---------------------------------------------------------------------------
//
CLpdNotifReqInfo* CLpdNotifReqInfo::NewLC(
	TPosRequestSource aSource,
	TPosVerifyCancelReason aReason,
	TPosRequestDecision aDecision,
	RPosRequestorStack* aRequestors)
    {
    CLpdNotifReqInfo* self = new( ELeave ) CLpdNotifReqInfo(aSource,
    														aReason,
    														aDecision,
    														aRequestors);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLpdNotifReqInfo::~CLpdNotifReqInfo()
	{
	if(iRequestors)
		{
		iRequestors->ResetAndDestroy();
		delete iRequestors;
		}
	}


// ---------------------------------------------------------------------------
// CLpdNotifReqInfo::Source()
// ---------------------------------------------------------------------------
//
const TPosRequestSource& CLpdNotifReqInfo::Source()
    {
    return iSource;
    }


// ---------------------------------------------------------------------------
// CLpdNotifReqInfo::Reason()
// ---------------------------------------------------------------------------
//
const TPosVerifyCancelReason& CLpdNotifReqInfo::Reason()
    {
    return iReason;
    }


// ---------------------------------------------------------------------------
// CLpdNotifReqInfo::Decision()
// ---------------------------------------------------------------------------
//
const TPosRequestDecision& CLpdNotifReqInfo::Decision()
    {
    return iDecision;
    }


// ---------------------------------------------------------------------------
// CLpdNotifReqInfo::Requestors()
// ---------------------------------------------------------------------------
//
const RPosRequestorStack& CLpdNotifReqInfo::Requestors()
    {
    return *iRequestors;
    }

//end of file
