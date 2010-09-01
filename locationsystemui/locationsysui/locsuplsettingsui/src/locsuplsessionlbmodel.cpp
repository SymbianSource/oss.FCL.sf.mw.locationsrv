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
* Description:  List box for the SUPL session UI
*
*/


// System Includes
#include <e32keys.h>
#include <locsuplsettingsui.rsg>
#include <StringLoader.h>
#include <epos_csuplsettings.h>
#include <epos_csuplsettingparams.h>
#include <epos_csuplsettingsconstants.h>

// User Include
#include "locsuplsessionlbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsupldebug.h"
		  					   	  
// Constants

const TInt KMaxConversionBufferLength 	= 0x200;
const TInt KSessionEntryGranularity		= 10;


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::CLocSUPLSessionLBModel
// Default Constructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSessionLBModel::CLocSUPLSessionLBModel( 
                            CLocSUPLSettingsUiEngine&     aEngine )
    	:	iEngine( aEngine ), 
    		iSessionIdList( KSessionEntryGranularity ), 
    		iSessionTypeList( KSessionEntryGranularity )
	{		
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::~CLocSUPLSessionLBModel
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSessionLBModel::~CLocSUPLSessionLBModel()
	{
	DEBUG( + CLocSUPLSessionLBModel::~CLocSUPLSessionLBModel );
	
	iSessionNameList.ResetAndDestroy();
	iSessionNameList.Close();
	iSessionIdList.Close();
	iSessionTypeList.Close();
	
	delete iUnnamedSuplActiveSessions;
        iUnnamedSuplActiveSessions = NULL;
			
	// Delete the Buffers
	delete iBuffer;
	iBuffer = NULL;		
	DEBUG( - CLocSUPLSessionLBModel::~CLocSUPLSessionLBModel );
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel* CLocSUPLSessionLBModel::NewL
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLSessionLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLSessionLBModel* CLocSUPLSessionLBModel::NewL( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	DEBUG( + CLocSUPLSessionLBModel::NewL );
	CLocSUPLSessionLBModel* self = CLocSUPLSessionLBModel::NewLC( aEngine );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSessionLBModel::NewL );
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel* CLocSUPLSessionLBModel::NewLC
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLSessionLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLSessionLBModel* CLocSUPLSessionLBModel::NewLC( 
                                    CLocSUPLSettingsUiEngine&     aEngine )
	{
	CLocSUPLSessionLBModel* self = 
	                new ( ELeave ) CLocSUPLSessionLBModel( aEngine );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
		
// ---------------------------------------------------------------------------
// void CLocSUPLSessionLBModel::ConstructL
// Second Phase of the two phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLSessionLBModel::ConstructL()
	{
	DEBUG( + CLocSUPLSessionLBModel::ConstructL );
    // Allocate the Conversion Buffer
	iBuffer = HBufC16::NewL( KMaxConversionBufferLength ); 	   
	
	iUnnamedSuplActiveSessions = StringLoader::LoadL( R_LOC_SUPL_UNKNOWN_SERVICE );
	
	UpdateSessionIdsL();
	DEBUG( - CLocSUPLSessionLBModel::ConstructL );
	}

// ---------------------------------------------------------------------------
// TInt CLocSUPLSessionLBModel::MdcaCount() const
// Inherited from MDesCArray. Returns the total number of items in the 
// Settings description array
//
// @return TInt The total number of elements in the Settings Description array
// ---------------------------------------------------------------------------
//
TInt CLocSUPLSessionLBModel::MdcaCount() const
	{
	DEBUG1( CLocSUPLSessionLBModel::MdcaCount= %d, iSessionIdList.Count() );
	return iSessionIdList.Count();
	}

// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLSessionLBModel::MdcaPoint() const
// Inherited from MDesCArray. Returns the list box entry corresponding to the 
// index value
//
// @param  aIndex The index whose list box entry needs to be retrieved
// @return TPtr16 The list box description 
// ---------------------------------------------------------------------------
//
TPtrC16 CLocSUPLSessionLBModel::MdcaPoint( TInt aIndex ) const
	{	
	DEBUG1( + CLocSUPLSessionLBModel::MdcaPoint =%d, aIndex );
	
	// Zero the internal buffer
	TPtr16 ptr( iBuffer->Des() );
	ptr.Zero();
	
	AppendSuplSessionDetail( ptr, aIndex );

	DEBUG( - CLocSUPLSessionLBModel::MdcaPoint );
	return iBuffer->Des();
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::AppendSuplSessionDetail()
// Packs the session name and type to the buffer passed.
//
// @param  aPtr Buffer to which session name and type will be appended.
// ---------------------------------------------------------------------------
//	
void CLocSUPLSessionLBModel::AppendSuplSessionDetail( TDes& aPtr, TInt aIndex ) const
    {
	DEBUG( + CLocSUPLSessionLBModel::AppendSuplSessionDetail );    
	
	_LIT(KItemString, "%d\t%S\t\t" );

  RBuf sessionName;
	if( iSessionNameList[aIndex]->Length() )
		{
   		TRAP_IGNORE( sessionName.CreateL( *iSessionNameList[ aIndex ] ) );
		}
	else
		{
   		TRAP_IGNORE( sessionName.CreateL( *iUnnamedSuplActiveSessions ) );
		}

	if( iSessionTypeList[ aIndex ] )
		{
		// Format the string
		aPtr.Format( KItemString, 1, &sessionName );			
		} 
	else
		{
		// Format the string
		aPtr.Format( KItemString, 2, &sessionName );					
		}	
	sessionName.Close();   		

	DEBUG( - CLocSUPLSessionLBModel::AppendSuplSessionDetail );    
    }

// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::UpdateSessionIdsL()
// Packs the server ids to id array
//
// ---------------------------------------------------------------------------
//	    
void CLocSUPLSessionLBModel::UpdateSessionIdsL()
	{
	DEBUG( + CLocSUPLSessionLBModel::UpdateSessionIdsL );

	iSessionIdList.Reset();
	iSessionNameList.ResetAndDestroy();
	iSessionTypeList.Reset();

	RPointerArray< CTriggerParams > sessionList;
		
	// Get all session entries
	iEngine.GetTriggerParamsL( sessionList );		
		
	for( TInt i = 0; i < sessionList.Count() ; i++ )
		{
	    TInt64 sessionId;
	    TUint64 outstandingTrigger;
	    TUint64 interval;
	    HBufC* sessionName = HBufC::NewL( KMaxTriggerSessionNameLen ); 
	    CleanupStack::PushL( sessionName );
	    TBool notificationPresent;
	    TBool triggerNotificationStatus;  
	    CTriggerParams::TTriggerType triggerType; 
	    CTriggerParams::TRequestType requestType;
 
		    		    
	    TInt errParams = sessionList[i]->Get(
	    							sessionId, 
	    							sessionName->Des(), 
	    							notificationPresent, 
	    							triggerNotificationStatus, 
	    							triggerType,
	    							requestType, 
	    							outstandingTrigger, 
	    							interval);
		    		
	    if ( errParams != KErrNone )
	        {
	        // Error has occured 
	        }  
			
		iSessionIdList.Append( sessionId );
		iSessionNameList.Append( sessionName->AllocL() );
		switch( triggerType )
			{
			case CTriggerParams::EPeriodic:
				iSessionTypeList.Append( ESUPLSessionTIType );
				break;
			case CTriggerParams::EAreaEvent:
				iSessionTypeList.Append( ESUPLSessionNIType );
				break;
			case CTriggerParams::EUnknown:
			default:
				iSessionTypeList.Append( ESUPLSessionTIType );
				break;
			}
			
		  CleanupStack::PopAndDestroy( sessionName );
		}
	sessionList.ResetAndDestroy();
	sessionList.Close();		
	
	DEBUG( - CLocSUPLSessionLBModel::UpdateSessionIdsL );
	}        
    
// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::SlpIds()
// returns server id array
//
// ---------------------------------------------------------------------------
//  
RArray<TInt64>& CLocSUPLSessionLBModel::SessionIds()
	{
	DEBUG( CLocSUPLSessionLBModel::SessionIds );
	return iSessionIdList;	
	}

// ---------------------------------------------------------------------------
// CLocSUPLSessionLBModel::CurrentSessionName()
// Get the currently focused session name
//
// ---------------------------------------------------------------------------
//  
TPtrC CLocSUPLSessionLBModel::SessionName(TInt aIndex)
	{
	if( iSessionNameList[aIndex]->Length() )
		{
		return iSessionNameList[aIndex]->Des();
		}
	else
		{
		return *iUnnamedSuplActiveSessions;
		}
	}

// End of file
