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
* Description:  Provides Location request view listbox model.
*
*/



// INCLUDE FILES
#include <locpsysettingsuiview.rsg>
#include <avkon.rsg>
#include <eikenv.h>

#include "locpsylbmodel.h"
#include "locpsyappdebug.h"
#include "locpsysettingsuiconsts.h"
#include "locpsysettings.h"
#include "locpsyinfo.h"

enum
	{
	EPsyDisabledIconIndex = '1',
	EPsyEnabledIconIndex = '0'
	};

		
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLocPsyLBModel::CLocPsyLBModel
// Constructor
// ---------------------------------------------------------
CLocPsyLBModel::CLocPsyLBModel( MLocPsySettings*    aPsySettingsModel )
    :iPsySettingsModel( aPsySettingsModel )
    {
    }

// ---------------------------------------------------------
// CLocPsyLBModel::~CLocPsyLBModel
// Destructor
// ---------------------------------------------------------
CLocPsyLBModel::~CLocPsyLBModel()
    {
    delete iBuffer;
    }
    
// ---------------------------------------------------------
// CLocPsyLBModel* CLocPsyLBModel::NewL
// Static two phase Constructor
// ---------------------------------------------------------
CLocPsyLBModel* CLocPsyLBModel::NewL( MLocPsySettings*      aPsySettingsModel )
    {
    CLocPsyLBModel* lbmodel = CLocPsyLBModel::NewLC( aPsySettingsModel );
    CleanupStack::Pop( lbmodel );
    return lbmodel;
    }

// ---------------------------------------------------------
// CLocPsyLBModel* CLocPsyLBModel::NewLC
// Static two phase Constructor
// ---------------------------------------------------------
CLocPsyLBModel* CLocPsyLBModel::NewLC( MLocPsySettings*     aPsySettingsModel )
    {
    CLocPsyLBModel* lbmodel = new (ELeave) CLocPsyLBModel( aPsySettingsModel );
    CleanupStack::PushL( lbmodel );
    lbmodel->ConstructL();
    return lbmodel;
    }

// ---------------------------------------------------------
// CLocPsyLBModel::ConstructL
// Symbian two phased constructor
// ---------------------------------------------------------
void CLocPsyLBModel::ConstructL()
    {
    iBuffer = HBufC::NewL( KLocPsyMaxLen + KTabLength + KTabLength ); // Icon Id + Tab Key
    }

// ---------------------------------------------------------
// TInt CLocPsyLBModel::MdcaCount
// ---------------------------------------------------------
TInt CLocPsyLBModel::MdcaCount() const
    {
    LOCAPPDEBUG1("CLocSettingsLBModel::MdcaCount number of PSYs=%d",
                 iPsySettingsModel->PSYCount());
    return iPsySettingsModel->PSYCount();
    }

// ---------------------------------------------------------
// TPtrC16 CLocPsyLBModel::MdcaPoint
// ---------------------------------------------------------
TPtrC16 CLocPsyLBModel::MdcaPoint( TInt aIndex ) const
    {
    TPtr ptr( iBuffer->Des() );
    ptr.Zero();
    if( iPsySettingsModel->IsPsyEnabled( aIndex ) )
    	{
    	ptr.Append( EPsyEnabledIconIndex );
    	}
    else
    	{
    	ptr.Append( EPsyDisabledIconIndex );
    	}
    	
    ptr.Append( EKeyTab );
    // Add the icon index here
    if ( iPsySettingsModel->PSYCount() > 0 )
        {
        // volatile --> stack instead of register to be leave-safe
        CLocPsyInfo* psyInfo = NULL; 
        TRAPD(err, psyInfo = iPsySettingsModel->GetPsyL( aIndex ));
        if ( err != KErrNone)
            {
            LOCAPPDEBUG2("CLocSettingsLBModel::MdcaPoint index=%d err=%d",
                aIndex, err );
            ptr.Zero();

            CEikonEnv::Static()->NotifyIdleErrorWhileRedrawing(err);

            return NULL;
            }
        else
            {
            ptr.Append( psyInfo->PsyName() );
            }
        }
    return *iBuffer;
    }

// End of File  
