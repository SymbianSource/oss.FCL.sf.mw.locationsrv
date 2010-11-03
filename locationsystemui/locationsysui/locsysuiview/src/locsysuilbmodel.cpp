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
* Description:  List box model
*
*/


// System Includes
#include <e32keys.h>

// User Include
#include "locsysuilbmodel.h"
#include "locationsettings.h"
		  					   	  
// Constants
const TInt KMaxDescriptionText = 0x100;		  					   	  

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSysUiLBModel::CLocSysUiLBModel
// Default Constructor
//
// ---------------------------------------------------------------------------
//
CLocSysUiLBModel::CLocSysUiLBModel( RPointerArray<CLocationSettings>& 
														aSettingsArray )
    :iSettingsArray( aSettingsArray )														
	{		
	}

// ---------------------------------------------------------------------------
// CLocSysUiLBModel::~CLocSysUiLBModel
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSysUiLBModel::~CLocSysUiLBModel()
	{
	delete iBuffer;
	}
	
// ---------------------------------------------------------------------------
// CLocSysUiLBModel* CLocSysUiLBModel::NewL
// Two-phased constructor
// 
// @param aSettingsArray	Reference to the Settings UI array
//@return CLocSysUiLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSysUiLBModel* CLocSysUiLBModel::NewL(
		 					RPointerArray<CLocationSettings>& aSettingsArray )
	{
	CLocSysUiLBModel* self = new(ELeave)CLocSysUiLBModel( aSettingsArray );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}
	
// ---------------------------------------------------------------------------
// void CLocSysUiLBModel::ConstructL
// Second Phase of the two phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CLocSysUiLBModel::ConstructL()
	{
	// Allocate the conversion buffer. The maximum length of the conversion buffer
	// is limited to 256 characters												  
	iBuffer = HBufC::NewL( KMaxDescriptionText ); 
	
	}

// ---------------------------------------------------------------------------
// CLocationSettings* CLocSysUiLBModel::SettingsUi
// Returns the Settings UI corresponding to a particular Index value.
// The index value starts from 0
//
// @param	aIndex			   Index of the Settings Ui which is requested
// @return	CLocationSettings& Referrence to the Settings UI corresponding
//							   to the index.
// ---------------------------------------------------------------------------
//	
CLocationSettings& CLocSysUiLBModel::SettingsUiL( TInt	aIndex )
	{
	if( aIndex < 0 || aIndex >= iSettingsArray.Count())
		{
		User::Leave( KErrNotFound );
		}
	return *iSettingsArray[ aIndex ];
	}
	
// ---------------------------------------------------------------------------
// void CLocationSettings::UpdateListBoxContentsL
// Updates the List box contents
//
// @param aSettingsArray	Reference to the Settings UI array
// ---------------------------------------------------------------------------
//
void CLocSysUiLBModel::UpdateListBoxContentsL( 
                RPointerArray<CLocationSettings>& aSettingsArray )
	{
    iSettingsArray= aSettingsArray;
	}

// ---------------------------------------------------------------------------
// TInt CLocSysUiLBModel::MdcaCount() const
// Inherited from MDesCArray. Returns the total number of items in the 
// Settings description array
//
// @return TInt The total number of elements in the Settings Description array
// ---------------------------------------------------------------------------
//
TInt CLocSysUiLBModel::MdcaCount() const
	{
	return iSettingsArray.Count();
	}

// ---------------------------------------------------------------------------
// TPtrC16 CLocSysUiLBModel::MdcaPoint() const
// Inherited from MDesCArray. Returns the list box entry corresponding to the 
// index value
//
// @param  aIndex The index whose list box entry needs to be retrieved
// @return TPtr16 The list box description 
// ---------------------------------------------------------------------------
//
TPtrC16 CLocSysUiLBModel::MdcaPoint( TInt aIndex ) const
	{
	// Zero the internal buffer
	TPtr ptr( iBuffer->Des());
    ptr.Zero();
    
    // Format "\t%S\t%d\t\t\t%S"
    
    // Append the first tab
    ptr.Append( EKeyTab );
    
    // If the index is within the range append the caption string
    if ( aIndex >= 0 && aIndex < iSettingsArray.Count())
    	{
    	CLocationSettings* settingsItem = iSettingsArray[ aIndex ];
   		TPtrC caption = settingsItem->Caption();
   		ptr.Append( caption );
    	}
    
    // Append the second tab
    ptr.Append( EKeyTab );
    
    // Append the third tab
    ptr.Append( EKeyTab );
    
    // Append the fourth tab
    ptr.Append( EKeyTab );
    
    // Append the fifth tab
    ptr.Append( EKeyTab );
    
    return ptr;	
	}

