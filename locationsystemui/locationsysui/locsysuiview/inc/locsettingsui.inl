/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline functions for CLocSettingsUi
*
*/


// System Include 
#include <aknViewAppUi.h>

// User Includes
#include "locsettingsuiobserver.h"
#include "locsettingsuiparams.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocSettingsUi::CLocSettingsUi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUi::CLocSettingsUi( MLocSettingsUiObserver&		aSettingsUiObserver,
					 			       CAknViewAppUi&				aAppUi )
    :iSettingsUiObserver( aSettingsUiObserver ),
     iAppUi( aAppUi )
    {
    }

// -----------------------------------------------------------------------------
// CLocSettingsUi::~CLocSettingsUi
// Destructor.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUi::~CLocSettingsUi()
    {
    // Call ECom Session Destroy function to delete the object
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

// -----------------------------------------------------------------------------
// CLocSettingsUi* CLocSettingsUi::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
inline CLocSettingsUi* CLocSettingsUi::NewL( const TUid					aImplementationUid,
										 	 CLocSettingsUiParams&		aInitParams )
    {
    TAny* ptr;
    TInt32 keyOffset = _FOFF( CLocSettingsUi, iDtor_ID_Key );
    ptr = REComSession::CreateImplementationL( aImplementationUid, keyOffset, &aInitParams);
    return reinterpret_cast<CLocSettingsUi*>( ptr );
    }
    
// -----------------------------------------------------------------------------
// TInt CLocSettingsUi::CLocSettingsUi
//
// -----------------------------------------------------------------------------
//
inline TUid CLocSettingsUi::GetSettingsUiID()
    {
    return iDtor_ID_Key;
    }
    
// -----------------------------------------------------------------------------
// TInt CLocSettingsUi::LaunchL
//
// -----------------------------------------------------------------------------
//
inline void CLocSettingsUi::LaunchL( const TDesC&		/*aUiLaunchParams*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
// End of File

