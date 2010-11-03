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
* Description:  CMnAiwCommandHandlerBase class implementation
*
*/


#include <AiwCommon.h>
#include <AiwCommon.hrh>
#include <AiwGenericParam.h>
#include <AiwGenericParam.hrh>

#include <EPos_Landmarks.h>
#include <EPos_CPosLandmark.h>
#include <epos_poslandmarkserialization.h>

#include <aknlistquerydialog.h>

#include <mnprovider.h>
#include <mnproviderfinder.h>
#include <mnappservices.hrh>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"
#include "mnaiwservices.h"

#include "mnshowonmaphandler.h"
#include "mnselectfrommaphandler.h"
#include "mnnavigatetohandler.h"
#include "mnfetchaddresshandler.h"
#include "mnfetchcoordinateshandler.h"

#include "mnaiwcommandhandlerbase.h"

// ================= LOCAL FUNCTIONS =======================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::CMnAiwCommandHandlerBase( CMnProvider& aProvider )
:   CActive( CActive::EPriorityStandard ),
    iProvider( aProvider )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::~CMnAiwCommandHandlerBase()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::AppServiceAndFeatureNeeded(
    TInt aAiwServiceCommand, 
    CMnProvider::TService& aServiceNeeded,
    TInt& aFeaturesNeeded )
    {
    switch ( aAiwServiceCommand )
        {
        case KAiwCmdMnShowMap:
            aServiceNeeded = CMnProvider::EServiceMapView;
            aFeaturesNeeded = KMnSvcFeatureShowMap;
            break;
    
        case KAiwCmdMnSelectFromMap:
            aServiceNeeded = CMnProvider::EServiceMapView;
            aFeaturesNeeded = KMnSvcFeatureSelectFromMap;
            break;
    
        case KAiwCmdMnNavigateTo: 
            aServiceNeeded = CMnProvider::EServiceNavigation;
            aFeaturesNeeded = KMnSvcFeatureNavigateTo;
            break;
    
        case KAiwCmdMnCoordByAddr: 
            aServiceNeeded = CMnProvider::EServiceGeocoding;
            aFeaturesNeeded = KMnSvcFeatureAddrToCoord;
            break;
    
        case KAiwCmdMnAddrByCoord:         
            aServiceNeeded = CMnProvider::EServiceGeocoding;
            aFeaturesNeeded = KMnSvcFeatureCoordToAddr;
            break;
            
        default:            
            aServiceNeeded = CMnProvider::EServiceNone;
            aFeaturesNeeded = 0;
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TRunMode CMnAiwCommandHandlerBase::DefaultRunMode( 
    TInt aAiwServiceCommand )
    {
    switch ( aAiwServiceCommand )
        {
        case KAiwCmdMnShowMap:
            return KShowOnMapChainedByDefault ? ERunModeChained : ERunModeStandalone;
    
        case KAiwCmdMnSelectFromMap:
            return KSelectFromMapChainedByDefault ? ERunModeChained : ERunModeStandalone;
    
        case KAiwCmdMnNavigateTo: 
            return KNavigateToChainedByDefault ? ERunModeChained : ERunModeStandalone;
    
        case KAiwCmdMnCoordByAddr: 
            return KFetchCoordinatesChainedByDefault ? ERunModeChained : ERunModeStandalone;
    
        case KAiwCmdMnAddrByCoord:         
            return KFetchAddressChainedByDefault ? ERunModeChained : ERunModeStandalone;
            
        default:            
            return ERunModeDefault;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase* CMnAiwCommandHandlerBase::CreateHandlerL( 
    TInt aAiwServiceCommand, 
    CMnProvider& aProvider,
    TRunMode aRunMode )
    {
    CMnAiwCommandHandlerBase* handler = NULL;
    switch ( aAiwServiceCommand )
        {
        case KAiwCmdMnShowMap:
            handler = CMnShowOnMapHandler::NewL( aProvider, aRunMode );
            break;

        case KAiwCmdMnNavigateTo:
            handler = CMnNavigateToHandler::NewL( aProvider, aRunMode );
            break;

        case KAiwCmdMnSelectFromMap:
            handler = CMnSelectFromMapHandler::NewL( aProvider, aRunMode );
            break;

        case KAiwCmdMnAddrByCoord:
            handler = CMnFetchAddressHandler::NewL( aProvider, aRunMode );
            break;

        case KAiwCmdMnCoordByAddr:
            handler = CMnFetchCoordinatesHandler::NewL( aProvider, aRunMode );
            break;

        default:
            User::Leave( KErrNotSupported );
            break;
        }
        
    return handler;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::SetParametersL( const TAiwParameters& aAiwParameters )
    {
    __ASSERT_DEBUG( !IsActive(), Panic( KErrGeneral ) );
    iAiwParameters = aAiwParameters;
    ProcessAiwParametersL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ResetParametersL()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::StartL()
    {
    DoStartL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::Start( MMnAsyncAiwCmdHandlerCallback& aCallback )
    {
    LOG("CMnAiwCommandHandlerBase::Start in");
    
    iCallback = &aCallback;
    __ASSERT_DEBUG( iCallback, Panic( KErrGeneral ) );
    
    TInt err = KErrNone;
    TRAP( err, DoStartL() );
    if ( err )
        {
        Done( err );
        }
    else
        {
        SetActive();
        }
    
    LOG("CMnAiwCommandHandlerBase::Start out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TAiwParameters& CMnAiwCommandHandlerBase::AiwParameters()
    {
    return iAiwParameters;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TRunMode CMnAiwCommandHandlerBase::RunMode()
	{
	return iRunMode;
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::SetExitObserverL( 
	MMnProviderExitCallback& aExitCallback )
	{
	iExitCallback = &aExitCallback;
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnProvider& CMnAiwCommandHandlerBase::Provider()
    {
    return iProvider;
    }

// ================= INTERNAL FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::Done( TInt aResult )
    {
    TRAP_IGNORE( iCallback->HandleAsyncAiwCmdCompletedL( this, aResult ) );
    } 

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ProcessAiwParametersL()
    {
    LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL in");

    RArray<TPtrC8> lmBuffers;
    CleanupClosePushL( lmBuffers );

    TPtrC dbUri;
    RArray<TPosLmItemId> lmIds;
    CleanupClosePushL( lmIds );
    
    for ( TInt i = 0; i < AiwParameters().InList().Count(); i++ )
        {
        const TAiwGenericParam& param = AiwParameters().InList()[i];
        switch ( param.SemanticId() )
            {
            case EGenericParamMnCommonOptions:
                //LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL, CommonOptions");
                // already handled
                break;
                
            case EGenericParamLandmark:
                //LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL, Landmark");
                {
                TPtrC8 lmData;
                ReadLandmarkDataL( param, lmData );
                lmBuffers.AppendL( lmData );
                break;
                }
            case EGenericParamLandmarkDatabase:
                //LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL, LandmarkDatabase");
                {
                if ( dbUri.Length() && lmIds.Count() )
                    {
                    HandleAiwParamLandmarksListL( dbUri, lmIds );
                    dbUri.Set( NULL, 0 );
                    lmIds.Reset();
                    }
                ReadDatabaseUriL( param, dbUri );
                break;
                }
            case EGenericParamLandmarkId:
                //LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL, LandmarkId");
                {
                if ( dbUri.Length() )
                    {
                    TPosLmItemId lmId = ReadLandmarkId( param );
                    lmIds.AppendL( lmId );
                    }
                else
                    {
                    User::Leave( KErrArgument );
                    }
                break;
                }
            case EGenericParamLandmarkIdList:
                //LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL, LandmarkIdList");
                {
                if ( dbUri.Length() )
                    {
                    TInt numLmIds = 0;
                    const TPosLmItemId* lmIdList = NULL;
                    ReadLandmarkIdListL( param, lmIdList, numLmIds );
                    for ( TInt i = 0; i < numLmIds; i++ )
                        {
                        lmIds.AppendL( *(lmIdList + i) );
                        }
                    }
                else
                    {
                    User::Leave( KErrArgument );
                    }
                break;
                }
            default:
                HandleAiwParamL( param );                
            }
        }

    if ( lmBuffers.Count() )
        {
        HandleAiwParamLandmarksL( lmBuffers );
        }
        
    if ( dbUri.Length() && lmIds.Count() )
        {
        HandleAiwParamLandmarksListL( dbUri, lmIds );
        }
        
    CleanupStack::PopAndDestroy( &lmIds );
    CleanupStack::PopAndDestroy( &lmBuffers );
    LOG("CMnAiwCommandHandlerBase::ProcessAiwParametersL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TRunMode CMnAiwCommandHandlerBase::RequestedRunModeL( 
    const CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    TMnAiwCommonOptionsParamExtended commonOptionsExtended;

    if ( FindCommonOptionsL( aAiwParams, commonOptionsExtended ) )
        {
        if ( commonOptionsExtended.iFieldsSet & EMnAiwCommonOptionsFieldRunChained )
            {
            return commonOptionsExtended.iRunChained ? ERunModeChained : ERunModeStandalone;
            }
        }
    return ERunModeDefault;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnAiwCommandHandlerBase::IsProviderSpecifiedL( 
    const CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
    TInt& aProviderId )
    {
    TMnAiwCommonOptionsParamExtended commonOptionsExtended;
    if ( FindCommonOptionsL( aAiwParams, commonOptionsExtended ) )
        {
        if ( commonOptionsExtended.iFieldsSet & EMnAiwCommonOptionsFieldProviderId )
            {
            aProviderId = commonOptionsExtended.iProviderId;
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnAiwCommandHandlerBase::IsErrorMessageDisabledL( 
    const CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    TMnAiwCommonOptionsParamExtended commonOptionsExtended;
    if ( FindCommonOptionsL( aAiwParams, commonOptionsExtended ) )
        {
        if ( commonOptionsExtended.iFieldsSet & EMnAiwCommonOptionsFieldMessagesDisabled )
            {
            return commonOptionsExtended.iMessagesDisabled;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnAiwCommandHandlerBase::FindCommonOptionsL( 
    const TAiwParameters& aAiwParams,
    TMnAiwCommonOptionsParamExtended& aCommonOptions )
    {
    TInt indexOptions = 0;
    aAiwParams.InList().FindFirst( indexOptions, EGenericParamMnCommonOptions, EVariantTypeDesC8 );
    if ( indexOptions != KErrNotFound )
        {
        const TAiwVariant& value = aAiwParams.InList()[indexOptions].Value();
        TPtrC8 data( value.AsData() ); 
        if ( data.Size() == sizeof( TMnAiwCommonOptionsParamExtended ) )
            {
            TPckg<TMnAiwCommonOptionsParamExtended> optionsPack( aCommonOptions );
            optionsPack.Copy( data );
            return ETrue;
            }
        else if ( data.Size() == sizeof( TMnAiwCommonOptionsParam ) )
            {
            TMnAiwCommonOptionsParam commonOptions;
            TPckg<TMnAiwCommonOptionsParam> optionsPack( commonOptions );
            optionsPack.Copy( data );
            aCommonOptions.iFieldsSet = EMnAiwCommonOptionsFieldRunChained;
            aCommonOptions.iRunChained = commonOptions.iRunChained;
            return ETrue;
            }
        }
    return EFalse;        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ReadLandmarkDataL( const TAiwGenericParam& aParam, TPtrC8& aLandmarkPtr )
    {
    aLandmarkPtr.Set( aParam.Value().AsData() );
    // verify it
    CPosLandmark* lm = PosLandmarkSerialization::UnpackL( aLandmarkPtr );
    delete lm;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnAiwCommandHandlerBase::FindLandmarkData( TInt aIndex, TAiwGenericParam& aParam )
    {
    TInt found = -1;
    for ( TInt i = 0; i < AiwParameters().InList().Count(); i++ )
        {
        const TAiwGenericParam& param = AiwParameters().InList()[i];
        if ( param.SemanticId() == EGenericParamLandmark )
            {
            found++;
            }
        if ( found == aIndex )
            {
            aParam = param;
            return ETrue;
            }
        }
        
    return EFalse;        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ReadDatabaseUriL( const TAiwGenericParam& aParam, TPtrC& aDbUri )
    {
    aDbUri.Set( aParam.Value().AsDes() );
    if ( !aDbUri.Length() )
        {
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CMnAiwCommandHandlerBase::ReadLandmarkId( const TAiwGenericParam& aParam )
    {
    return aParam.Value().AsTInt32();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::ReadLandmarkIdListL( 
    const TAiwGenericParam& aParam,
    const TPosLmItemId*& aListPtr,
    TInt& aListSize )
    {
    TPtrC8 data( aParam.Value().AsData() );
    
    if ( data.Size() % sizeof( TPosLmItemId ) )
        {
        User::Leave( KErrArgument );
        }
    
    aListPtr = reinterpret_cast<const TPosLmItemId*>( data.Ptr() );
    aListSize = data.Size() / sizeof( TPosLmItemId );
    }        

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::WriteLandmarkL( const CPosLandmark& aLandmark )
    {
    HBufC8* lmBuf = PosLandmarkSerialization::PackL( aLandmark );
    CleanupStack::PushL( lmBuf );

    TAiwGenericParam param( EGenericParamLandmark, TAiwVariant( *lmBuf ) );
    iAiwParameters.OutList().AppendL( param );

    CleanupStack::PopAndDestroy( lmBuf );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::WriteLandmarkL( TPosLmItemId aLandmarkId, const TDesC& aDatabaseUri )
    {
    TAiwGenericParam lmId( EGenericParamLandmarkId, TAiwVariant( aLandmarkId ) );
    TAiwGenericParam dbUri( EGenericParamLandmarkDatabase, TAiwVariant( aDatabaseUri ) );

    iAiwParameters.OutList().AppendL( dbUri );
    iAiwParameters.OutList().AppendL( lmId );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::HandleAiwParamL( const TAiwGenericParam& /*aParam*/ )
    {
    User::Leave( KErrArgument );
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::HandleAiwParamLandmarksL( RArray<TPtrC8>& /*aLandmarkDataList*/ )
    {
    User::Leave( KErrArgument );
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::HandleAiwParamLandmarksListL( 
    const TDesC& /*aDbUri*/, 
    RArray<TPosLmItemId>& /*aLmIds*/ )
    {
    User::Leave( KErrArgument );
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::RunL()
    {
#ifdef _DEBUG    
    LOG("CMnAiwCommandHandlerBase::RunL in, panicking");
    Panic( KErrGeneral );
#endif    
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
TInt CMnAiwCommandHandlerBase::RunError( TInt aError )
    {
    Done( aError );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//  From class CActive
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::DoCancel()
    {
    Done( KErrCancel );
    }

// -----------------------------------------------------------------------------
// From MAknServerAppExitObserver
// -----------------------------------------------------------------------------
//
void CMnAiwCommandHandlerBase::HandleServerAppExit( TInt aReason )
	{
    LOG1("CMnAiwCommandHandlerBase::HandleServerAppExit in, reason %d", aReason );
	
	if ( iRunMode == ERunModeChained )
	    {
	    // for standlone mode this event is ignored,
	    // otherwise client will be shutdown whenever provider exits
	    MAknServerAppExitObserver::HandleServerAppExit( aReason );
	    }

	if ( iExitCallback )
		{
		iExitCallback->HandleProviderExit( this );
		}
#ifdef _DEBUG
	else
		{
		Panic( KErrGeneral );
		}
#endif		
	    
    LOG("CMnAiwCommandHandlerBase::HandleServerAppExit out");
	}

//===================== TAiwParameters ====================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TAiwParameters::TAiwParameters()
:   iInParamList( NULL ),    
    iOutParamList( NULL ),
    iCallback( NULL )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase::TAiwParameters::TAiwParameters(
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    const MAiwNotifyCallback* aCallback )
:   iInParamList( &aInParamList ),    
    iOutParamList( &aOutParamList ),
    iCallback( aCallback )
    {
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const CAiwGenericParamList& CMnAiwCommandHandlerBase::TAiwParameters::InList() const
    {
    return *iInParamList;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CAiwGenericParamList& CMnAiwCommandHandlerBase::TAiwParameters::OutList()
    {
    return *iOutParamList;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const MAiwNotifyCallback* CMnAiwCommandHandlerBase::TAiwParameters::Callback() const
    {
    return iCallback;
    }
