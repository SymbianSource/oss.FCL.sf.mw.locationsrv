/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  List box for the SUPL server UI
*
*/


// System Includes
#include <e32keys.h>
#include <locsuplsettingsui.rsg>
#include <StringLoader.h>
#include <epos_csuplsettings.h>
#include <epos_csuplsettingparams.h>
#include <epos_csuplsettingsconstants.h>
#include <bldvariant.hrh>
#include <featmgr.h>

// User Include
#include "locsuplserverlbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"
		  					   	  
// Constants

const TInt KMaxConversionBufferLength 	= 0x200;
const TInt KServerEntryGranularity		= 10;
const TInt KMaxServerAddressLength = 255;


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel::CLocSUPLServerLBModel
// Default Constructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLServerLBModel::CLocSUPLServerLBModel( 
                            CLocSUPLSettingsUiEngine&     aEngine )
    	:	iEngine( aEngine ), 
    		iSlpIdList( KServerEntryGranularity )
	{		
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel::~CLocSUPLServerLBModel
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLServerLBModel::~CLocSUPLServerLBModel()
	{
	DEBUG( + CLocSUPLServerLBModel::~CLocSUPLServerLBModel );
	
	iEnableFlagList.Close();
	iSlpAddressList.ResetAndDestroy();
	iSlpAddressList.Close();
	iSlpIdList.Close();
			
	// Delete the Buffers
	delete iBuffer;
	iBuffer = NULL;		
	DEBUG( - CLocSUPLServerLBModel::~CLocSUPLServerLBModel );
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel* CLocSUPLServerLBModel::NewL
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLServerLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLServerLBModel* CLocSUPLServerLBModel::NewL( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLServerLBModel::NewL );
	CLocSUPLServerLBModel* self = CLocSUPLServerLBModel::NewLC( aEngine );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLServerLBModel::NewL );
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel* CLocSUPLServerLBModel::NewLC
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLServerLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLServerLBModel* CLocSUPLServerLBModel::NewLC( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	CLocSUPLServerLBModel* self = 
	                new ( ELeave ) CLocSUPLServerLBModel( aEngine );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
		
// ---------------------------------------------------------------------------
// void CLocSUPLServerLBModel::ConstructL
// Second Phase of the two phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLServerLBModel::ConstructL()
	{
	DEBUG( + CLocSUPLServerLBModel::ConstructL );
    // Allocate the Conversion Buffer
	iBuffer = HBufC16::NewL( KMaxConversionBufferLength ); 	   
	
	UpdateSlpIdsL();
	DEBUG( - CLocSUPLServerLBModel::ConstructL );
	}

// ---------------------------------------------------------------------------
// TInt CLocSUPLServerLBModel::MdcaCount() const
// Inherited from MDesCArray. Returns the total number of items in the 
// Settings description array
//
// @return TInt The total number of elements in the Settings Description array
// ---------------------------------------------------------------------------
//
TInt CLocSUPLServerLBModel::MdcaCount() const
	{
	DEBUG1( CLocSUPLServerLBModel::MdcaCount= %d, iSlpIdList.Count() );
	return iSlpIdList.Count();
	}

// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLServerLBModel::MdcaPoint() const
// Inherited from MDesCArray. Returns the list box entry corresponding to the 
// index value
//
// @param  aIndex The index whose list box entry needs to be retrieved
// @return TPtr16 The list box description 
// ---------------------------------------------------------------------------
//
TPtrC16 CLocSUPLServerLBModel::MdcaPoint( TInt aIndex ) const
	{	
	DEBUG1( + CLocSUPLServerLBModel::MdcaPoint =%d, aIndex );
	
	// Zero the internal buffer
	TPtr16 ptr( iBuffer->Des() );
	ptr.Zero();
	
	AppendSuplServerDetail( ptr, aIndex );

	DEBUG( - CLocSUPLServerLBModel::MdcaPoint );
	return iBuffer->Des();
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel::AppendSuplServerDetail()
// Packs the server address to the end of the buffer passed.
//
// @param  aPtr Buffer to which the server address needs to be appended.
// ---------------------------------------------------------------------------
//	
void CLocSUPLServerLBModel::AppendSuplServerDetail( TDes& aPtr, TInt aIndex ) const
    {
	DEBUG( + CLocSUPLServerLBModel::AppendSuplServerDetail );    

	// CAknSingleNumberStyleListBox 	"2\tTextLabel\t0\t1"
	
	_LIT(KItemString, "%d\t%S\t\t" );
	_LIT(KItemString2, "%d\t%S\t%d\t" );

   	RBuf hslpAddr;
   	TRAP_IGNORE( hslpAddr.CreateL( *iSlpAddressList[ aIndex ] ) );

	if( iEnableFlagList[ aIndex ] )
		{
		// Format the string
		aPtr.Format( KItemString, aIndex+1, &hslpAddr );			
		} 
	else
		{
		// Format the string
		aPtr.Format( KItemString2, aIndex+1, &hslpAddr, ETrue );					
		}	
	hslpAddr.Close();   		
	
	DEBUG( - CLocSUPLServerLBModel::AppendSuplServerDetail );    
    }

// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel::UpdateSlpIdsL()
// Packs the server ids to id array
//
// ---------------------------------------------------------------------------
//	    
void CLocSUPLServerLBModel::UpdateSlpIdsL()
	{
	DEBUG( + CLocSUPLServerLBModel::UpdateSlpIdsL );

	iSlpIdList.Reset();
	iSlpAddressList.ResetAndDestroy();
	iEnableFlagList.Reset();

	TBool displayImsiAddress = FeatureManager::FeatureSupported(KFeatureIdFfSuplImsiGeneratedAddressDisplay);


	if( iEngine.SlpCount() > 0 )
		{
		RPointerArray< CServerParams > 	serverList;
		
		// Get all server entries
		iEngine.GetAllSlpL( serverList );		
		
		TBuf<KMaxServerAddressLength> imsiAddress;
		iEngine.GenerateHslpAddressFromImsi(imsiAddress);		
		
		TInt serverCount = 	serverList.Count();
		
		for( TInt i = 0; i < serverCount ; i++ )
			{
		    TInt64 slpId;
		    HBufC* hslpAddr = HBufC::NewLC( KMaxHSLPAddrLen );    
		    HBufC* iapName 	= HBufC::NewLC( KMaxIAPLen );
		    TBool enabledFlag, simChangeFlag, usageInHomeNwFlag, editFlag;    
		    		    
		    TInt errParams = serverList[i]->Get(
		    							slpId, 
		    							hslpAddr->Des(), 
		    							iapName->Des(), 
		    							enabledFlag, 
		    							simChangeFlag, 
		    							usageInHomeNwFlag, 
		    							editFlag
		    						);
		    						
		    						
		   			
		    if ( errParams != KErrNone )
		        {
		         	User::Leave(errParams);
		        }
		        	       
			if ( displayImsiAddress || imsiAddress.Compare(hslpAddr->Des()) ) // Flag is true->Show IMSI server too, not otherwise!
			{
			iSlpIdList.Append( slpId );
			iSlpAddressList.Append( hslpAddr->AllocL() );
			iEnableFlagList.Append( enabledFlag );
			}
			
			CleanupStack::PopAndDestroy(2);
			}
		serverList.ResetAndDestroy();
		serverList.Close();		
		}	
	DEBUG( - CLocSUPLServerLBModel::UpdateSlpIdsL );
	}        
    
// ---------------------------------------------------------------------------
// CLocSUPLServerLBModel::SlpIds()
// returns server id array
//
// ---------------------------------------------------------------------------
//  
RArray<TInt64>& CLocSUPLServerLBModel::SlpIds()
	{
	DEBUG( CLocSUPLServerLBModel::SlpIds );
	return iSlpIdList;	
	}

// End of file
