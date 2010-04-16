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
* Description:  Requestor utilities, supports privacy rules.
*
*/


// INCLUDE FILES

#include <e32base.h>
#include <barsread.h>


#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <lbs/epos_cposcontactrequestor.h>


#include "locrequestorutilsresolverimpl2.h"
#include "locutilsdebug.h"
#include "locfileutils.h"

// ============= CLocRequestorUtilsResolver MEMBER FUNCTIONS =================

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::NewL
// 2 Phase Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLocRequestorUtilsResolver* CLocRequestorUtilsResolver::NewL()
	{
	CLocRequestorUtilsResolver* self = 
							new(ELeave) CLocRequestorUtilsResolver();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
    return self;
	}

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::~CLocRequestorUtilsResolver()
	{
	}

// ---------------------------------------------------------------------------
// The method resolves requestors with the contact database
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocRequestorUtilsResolver::ProcessRequestorsL( 
	RPointerArray<CPosRequestor>& /*aRequestors*/ )
	{
    // Removed this functionality to avoid App layer dependency
	}
	
// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::PhoneNumberAsName
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CLocRequestorUtilsResolver::PhoneNumberAsName(
    const CPosRequestor& aRequestor )
    {
    if ( aRequestor.RequestorIdFormat() != CPosRequestor::EIdFormatPhoneNumber )
        { // id is of some other format
        return EFalse;
        }
    else
    	{
    	return ETrue;
    	}
    }

// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RequestorIdValid
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CLocRequestorUtilsResolver::RequestorIdValid(
    const CPosRequestor& aRequestor )
    {
    TBool rc(ETrue);
    if ( aRequestor.RequestorIdFormat() == CPosRequestor::EIdFormatUnknown )
        {
        rc = EFalse;
        }
    return rc;
    }


// -----------------------------------------------------------------------------
// CLocRequestorUtilsResolver::RequestorNameL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* CLocRequestorUtilsResolver::RequestorNameL(
    const CPosRequestor& aRequestor )
    {
	return aRequestor.RequestorIdString().AllocL();
    }

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::CLocRequestorUtilsResolver
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::CLocRequestorUtilsResolver()
	{
	// Nothing to do.
	}

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ConstructL
// Symbian Second Phase COnstructor
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ConstructL()
	{
	// Nothing to do.
	}

//  End of File
