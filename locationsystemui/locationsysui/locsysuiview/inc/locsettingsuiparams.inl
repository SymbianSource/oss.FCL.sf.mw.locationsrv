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
* Description:  Inline functions for CLocSettingsUiParams
*
*/


// System Include 
#include <aknViewAppUi.h>

// User Includes
#include "locsettingsuiobserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocSettingsUiParams::CLocSettingsUiParams
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUiParams::CLocSettingsUiParams( MLocSettingsUiObserver&	aSettingsUiObserver,
					 			 			       CAknViewAppUi&			aAppUi )
    :iSettingsUiObserver( aSettingsUiObserver ),
     iAppUi( aAppUi )
    {
    }

// -----------------------------------------------------------------------------
// CLocSettingsUiParams::~CLocSettingsUiParams
// Destructor.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUiParams::~CLocSettingsUiParams()
    {
    }

// -----------------------------------------------------------------------------
// CLocSettingsUiParams* CLocSettingsUiParams::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUiParams* CLocSettingsUiParams::NewL( MLocSettingsUiObserver& aObserver,
									 	 		         CAknViewAppUi&		     aAppUi )
    {
   	CLocSettingsUiParams* self = new(ELeave) CLocSettingsUiParams( aObserver,
    													 	       aAppUi );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
    }
  
// -----------------------------------------------------------------------------
// CLocSettingsUiParams* CLocSettingsUiParams::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUiParams* CLocSettingsUiParams::NewLC( MLocSettingsUiObserver&	aObserver,
									 	 			      CAknViewAppUi&			aAppUi )
    {
   	CLocSettingsUiParams* self = new(ELeave) CLocSettingsUiParams( aObserver,
    													 	       aAppUi );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }

// -----------------------------------------------------------------------------
// void CLocSettingsUiParams::ConstructL
// Second phase of Construction
// -----------------------------------------------------------------------------
//
inline void CLocSettingsUiParams::ConstructL()
	{
	}
		       
// -----------------------------------------------------------------------------
// SettingsUIObserver Accessor function
//
// @return MSettingsUIObserver&	Reference to the Settings UI Observer
// -----------------------------------------------------------------------------
//
 inline MLocSettingsUiObserver& CLocSettingsUiParams::SettingsUiObserver()
 	{
 	return iSettingsUiObserver;
 	}

// -----------------------------------------------------------------------------
// CAknViewAppUi Accessor function
//
// @return CAknViewAppUi&	Reference to the AppUi Object
// -----------------------------------------------------------------------------
//
 inline CAknViewAppUi&	CLocSettingsUiParams::AppUi()
 	{
 	return iAppUi;
 	}
		 
// End of File

