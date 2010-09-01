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
* Description:  List box for the SUPL Settings UI
*
*/


// System Includes
#include <e32keys.h>
#include <locsuplsettingsui.rsg>
#include <avkon.rsg>
#include <StringLoader.h>
#include <centralrepository.h>
#include <epos_csuplsettings.h>


// User Include
#include "locsuplsettingslbmodel.h"
#include "locsuplsettingsuiengine.h"
#include "locsuplsettingsprivatecrkeys.h"
#include "epos_omasuplconfigurationkeys.h"
#include "locsupldebug.h"
		  					   	  
// Constants

const TInt KMaxConversionBufferLength = 0x200;

_LIT( KDefaultServerIPAddress, " " );

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CLocSUPLSettingsLBModel::CLocSUPLSettingsLBModel
// Default Constructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsLBModel::CLocSUPLSettingsLBModel( 
                            CLocSUPLSettingsUiEngine& aEngine )
    :iEngine( aEngine )
	{		
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsLBModel::~CLocSUPLSettingsLBModel
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsLBModel::~CLocSUPLSettingsLBModel()
	{	
	DEBUG( + CLocSUPLSettingsLBModel::~CLocSUPLSettingsLBModel );	
	// Delete the Buffers
	delete iBuffer;
	iBuffer = NULL;
	
	delete iServerDetailTitle;
	iServerDetailTitle = NULL;
	
	delete iSuplUsageTitle;
	iSuplUsageTitle = NULL;
	
	delete iSuplServerDetail;
	iSuplServerDetail = NULL;

	delete iDefaultSuplUsage;
	iDefaultSuplUsage = NULL;
	DEBUG( - CLocSUPLSettingsLBModel::~CLocSUPLSettingsLBModel );	
	}
	
// ---------------------------------------------------------------------------
// CLocSUPLSettingsLBModel* CLocSUPLSettingsLBModel::NewL
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLSettingsLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsLBModel* CLocSUPLSettingsLBModel::NewL( 
                                    CLocSUPLSettingsUiEngine& aEngine )
	{
	DEBUG( + CLocSUPLSettingsLBModel::NewL );	
	CLocSUPLSettingsLBModel* self = CLocSUPLSettingsLBModel::NewLC( aEngine );
	CleanupStack::Pop( self );
	DEBUG( - CLocSUPLSettingsLBModel::NewL );	
	return self;
	}

// ---------------------------------------------------------------------------
// CLocSUPLSettingsLBModel* CLocSUPLSettingsLBModel::NewLC
// Two-phased constructor
// 
// @param  aEngine                  Reference the SUPL Settings Engine 
// @return CLocSUPLSettingsLBModel*	Reference to the object created
//
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsLBModel* CLocSUPLSettingsLBModel::NewLC( 
                                    CLocSUPLSettingsUiEngine& aEngine )
	{
	CLocSUPLSettingsLBModel* self = 
	                new( ELeave )CLocSUPLSettingsLBModel( aEngine );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
		
// ---------------------------------------------------------------------------
// void CLocSUPLSettingsLBModel::ConstructL
// Second Phase of the two phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CLocSUPLSettingsLBModel::ConstructL()
	{
	DEBUG( + CLocSUPLSettingsLBModel::ConstructL );	
    // Allocate the Conversion Buffer
	iBuffer = HBufC16::NewL( KMaxConversionBufferLength ); 
	 
	// Read the Local Variation Key and set the Item count
	TInt flagValue = 0;
	CRepository* repository = CRepository::NewLC( TUid::Uid( KCRUidLocSuplSettingsUi ) );
	repository->Get( KLocSuplSettingsLVFlags, flagValue );
	CleanupStack::PopAndDestroy( repository );
	repository = NULL;	
	if( flagValue )
	    {
	    iServerAddressPresent = ETrue;
	       
     	// Allocate and set the server ip address title field
    	iServerDetailTitle = StringLoader::LoadL( R_LOC_SUPL_SERVER_DETAIL );
    	 
    	// Allocate the Default IP address
    	iSuplServerDetail = HBufC16::NewL( KDefaultServerIPAddress().Length() );
    	TPtr16 ptr( iSuplServerDetail->Des() );
    	ptr.Copy( KDefaultServerIPAddress );		       
	    }
    
    // Reinitializing the flag value
    flagValue = 0;
    TRAPD( error, repository = CRepository::NewL( KCRUidOMASuplConfiguration ) );
    if ( !error )
        {
        error = repository->Get( KOMASuplConfigurationSuplPermissionQueryUsage, flagValue );
        if ( error || flagValue < 0 )
            {
            flagValue = 0;
            }
    	delete repository;
    	repository = NULL;
        }	
	    	    
	// Check for SUPL Setting usage variationing.
	if( flagValue )
	    {
	    iSuplUsagePresent = ETrue;
	       
     	// Allocate and set the server ip address title field
    	iSuplUsageTitle = StringLoader::LoadL( R_LOC_SUPL_USE );
    	
    	iDefaultSuplUsage =  StringLoader::LoadL( R_AVKON_SELEC_SETT_VAL_FIELD_NONE );
    	// Default value can be used for this field as well
    	// So no need to have another variable.		       
	    }
	    	
	DEBUG( - CLocSUPLSettingsLBModel::ConstructL );	
	}

// ---------------------------------------------------------------------------
// TSUPLSettingsItemId CLocSUPLSettingsLBModel::ResolveSettingsItem
// Packs the server address to the end of the buffer passed.
//
// @param  aPtr Buffer to which the server address needs to be appended.
// ---------------------------------------------------------------------------
//
CLocSUPLSettingsLBModel::TSUPLSettingsItemId 
    CLocSUPLSettingsLBModel::ResolveSettingsItem( TInt aListboxIndex ) const
    {
 	DEBUG( + CLocSUPLSettingsLBModel::ResolveSettingsItem );	
   	TSUPLSettingsItemId selectedItem = ESUPLSettingsUsage;
    
    switch ( aListboxIndex )
        {
        case 0:
            {
            selectedItem = ESUPLSettingsUsage;
            break;
            }        
        case 1:
            {
            selectedItem = ESUPLSettingsServersDetail;
            break;
            }
        case 2:
            {
            selectedItem = ESUPLSettingsActiveSessions;
            break;   
            }
        default:
            {
            break;            
            }
        }
	DEBUG( - CLocSUPLSettingsLBModel::ResolveSettingsItem );	
    return selectedItem;
    }
        
// ---------------------------------------------------------------------------
// TInt CLocSUPLSettingsLBModel::MdcaCount() const
// Inherited from MDesCArray. Returns the total number of items in the 
// Settings description array
//
// @return TInt The total number of elements in the Settings Description array
// ---------------------------------------------------------------------------
//
TInt CLocSUPLSettingsLBModel::MdcaCount() const
	{
	DEBUG( + CLocSUPLSettingsLBModel::MdcaCount );	
	// Initialize the count to 0 
	TInt itemCount = 0;
	
	// Check for SUPL usage.
	if ( iSuplUsagePresent )
	    {
	    itemCount++;
	    // Check if the usage value is disabled
	    if ( iEngine.GetSuplUsage() == CSuplSettings::ESuplUsageDisabled )
	        {
	        return itemCount;
	        }
		// Check for Server address
		iServerAddressPresent ? itemCount++ : itemCount;	
	    }
	itemCount++; // For the third entry "Active sessions"
	// Check if the SUPL Usage value is Disabled
	// If that is the case then only SUPL usage will be displayed
	DEBUG( - CLocSUPLSettingsLBModel::MdcaCount );	
	return itemCount;
	}

// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLSettingsLBModel::MdcaPoint() const
// Inherited from MDesCArray. Returns the list box entry corresponding to the 
// index value
//
// @param  aIndex The index whose list box entry needs to be retrieved
// @return TPtr16 The list box description 
// ---------------------------------------------------------------------------
//
TPtrC16 CLocSUPLSettingsLBModel::MdcaPoint( TInt aIndex ) const
	{
	DEBUG( + CLocSUPLSettingsLBModel::MdcaPoint );	
	// Zero the internal buffer
	TPtr16 ptr( iBuffer->Des() );
	ptr.Zero();
	
	// CAknSettingStyleListBox
	// Format "\t%S\t\t%S"
	
	// Append the first tab
	ptr.Append( EKeyTab );
	
	TSUPLSettingsItemId index = ResolveSettingsItem( aIndex );		
	switch( index )
	    {
	    case ESUPLSettingsUsage:
	        {
	        AppendSuplUsage( ptr );
	        break;
	        }
	    case ESUPLSettingsServersDetail:
	        {
	        AppendSuplServerDetail( ptr );
	        break;    
	        }
	    case ESUPLSettingsActiveSessions:
	        {
	        TRAP_IGNORE( AppendSuplActiveSessionsDetailL( ptr ) );
	        break;
            }
	    default:
	        {
        	// Append the second tab
        	ptr.Append( EKeyTab );        	
        	// Append the third tab
        	ptr.Append( EKeyTab );
	        break;    
	        }
	    }  
	DEBUG( - CLocSUPLSettingsLBModel::MdcaPoint );	
	return iBuffer->Des();
	}

// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLSettingsLBModel::AppendSuplServerDetail()
// Packs the server address to the end of the buffer passed.
//
// @param  aPtr Buffer to which the server address needs to be appended.
// ---------------------------------------------------------------------------
//	
void CLocSUPLSettingsLBModel::AppendSuplUsage( TDes& aPtr ) const
    {
	DEBUG( + CLocSUPLSettingsLBModel::AppendSuplUsage );	
    // Append the Title text
    aPtr.Append( iSuplUsageTitle->Des() );
    
    // Append the second tab
	aPtr.Append( EKeyTab ); 
	
	// Append the third tab
	aPtr.Append( EKeyTab );
	
	// Obtain the SUPL usage from the SUPL Settings Engine. If
	// the settings is not set then a Not defined value would be 
	// displayed to the user
    TPtr16 suplUsagePtr( NULL, 0 );
    TRAPD( error, suplUsagePtr.Set( iEngine.GetSuplUsageL() ) );         
    if( error || !suplUsagePtr.Length() )
        {
        // Using the same string as SUPL settings AP
        aPtr.Append( iDefaultSuplUsage->Des() );
        }
    else
        {
        aPtr.Append( suplUsagePtr );              
        }    
	DEBUG( - CLocSUPLSettingsLBModel::AppendSuplUsage );	
    }    

// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLSettingsLBModel::AppendSuplServerDetail()
// Packs the server address to the end of the buffer passed.
//
// @param  aPtr Buffer to which the server address needs to be appended.
// ---------------------------------------------------------------------------
//	
void CLocSUPLSettingsLBModel::AppendSuplServerDetail( TDes& aPtr ) const
    {
	DEBUG( + CLocSUPLSettingsLBModel::AppendSuplServerDetail );	
    // Append the Title text
    aPtr.Append( iServerDetailTitle->Des() );
    
    // Append the second tab
	aPtr.Append( EKeyTab ); 
	
	// Append the third tab
	aPtr.Append( EKeyTab );

	// Append the forth tab
	aPtr.Append( EKeyTab );	
	DEBUG( - CLocSUPLSettingsLBModel::AppendSuplServerDetail );	
    }
    
// ---------------------------------------------------------------------------
// TPtrC16 CLocSUPLSettingsLBModel::AppendSuplActiveSessionsDetailL()
// Packs the server address to the end of the buffer passed.
//
// @param  aPtr Buffer to which the server address needs to be appended.
// ---------------------------------------------------------------------------
//	
void CLocSUPLSettingsLBModel::AppendSuplActiveSessionsDetailL( TDes& aPtr ) const
    {
	DEBUG( + CLocSUPLSettingsLBModel::AppendSuplActiveSessionsDetailL );	
	
    HBufC16* suplActiveSessionsTitle;
    
    TRAPD( error, iEngine.GetActiveSessionsCountL( iSessionCount ) );
    
	if( !error )
		{
		switch( iSessionCount )
			{
			case 0:
				suplActiveSessionsTitle = StringLoader::LoadL( R_LOC_SUPL_NO_SERVICE );
				break;
			case 1:
				suplActiveSessionsTitle = StringLoader::LoadL( R_LOC_SUPL_SINGLE_SERVICE );
				break;
			default:
				suplActiveSessionsTitle = StringLoader::LoadL( R_LOC_SUPL_MULTIPLE_SERVICES, iSessionCount );
				break;
			}

		if( suplActiveSessionsTitle )
			{
    	    // Append the Title text
	        aPtr.Append( suplActiveSessionsTitle->Des() );
            delete suplActiveSessionsTitle;
	        suplActiveSessionsTitle = NULL;
			}
		}
    
    // Append the second tab
	aPtr.Append( EKeyTab ); 
	
	// Append the third tab
	aPtr.Append( EKeyTab );

	// Append the forth tab
	aPtr.Append( EKeyTab );	
	
	DEBUG( - CLocSUPLSettingsLBModel::AppendSuplActiveSessionsDetailL );	
}
    
// ---------------------------------------------------------------------------
// TInt CLocSUPLSettingsLBModel::ActiveSessionCount()
// Returns the number of Active sessions.
//
// @return  TInt Returns the number of Active sessions count.
// ---------------------------------------------------------------------------
//	
TInt CLocSUPLSettingsLBModel::ActiveSessionCount( ) const
    {
	return iSessionCount;
	}


// End of file
