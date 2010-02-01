/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnAiwProvider class implementation
*
*/


#include <badesca.h>

#include <aknlistquerydialog.h>
#include <AknGlobalNote.h>

#include <AiwMenu.h>
#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>
#include <aknenv.h>

#include <ecom/implementationproxy.h>

#include <mnaiwproviderres.rsg>
#include "mnaiwprovideruids.hrh"

#include <lbsposition.h>

#include <mnprovider.h>
#include <mnproviderfinder.h>
#include <mnmapview.h>
#include <mnnavigator.h>
#include <mngeocoder.h>
#include <mnutils.h>
#include <mnappservices.hrh>

#include "mnaiwdebug.h"
#include "mnaiwinternal.h"

#include "mnaiwservices.h"
#include "mnaiwmenus.hrh"

#include "mnaiwprovider.h"

// =========================== LOCAL FUNCTIONS =================================

// ---------------------------------------------------------------------------
//  Cleanup item for RPointerArray<CMnProvider>
// ---------------------------------------------------------------------------
//
void CleanupProvidersArray( TAny* aArray )
    {
    ( static_cast<RPointerArray<CMnProvider>*>( aArray ) )->ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
//  Identity relation for providers. Based on UID
// ---------------------------------------------------------------------------
//
TBool ProvidersMatchByUid( const CMnProvider& aOne, const CMnProvider& aAnother )
	{
	return ( aOne.Uid().iUid == aAnother.Uid().iUid );
	}

// ========================== MEMBER FUNCTIONS =================================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwProvider* CMnAiwProvider::NewL()
    {
    LOG( "CMnAiwProvider::NewL in" );

    CMnAiwProvider* self = new (ELeave) CMnAiwProvider();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    LOG( "CMnAiwProvider::NewL out" );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwProvider::CMnAiwProvider()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwProvider::~CMnAiwProvider()
    {
    LOG( "~CMnAiwProvider in" );

    delete iEventList;

    CloseAllHandlers();
    iHandlers.Close();

    if ( iEikon )
        {
        iEikon->DeleteResourceFile( iResourceOffset );
        }
    iMnProviders.ResetAndDestroy();

    LOG( "~CMnAiwProvider out" );
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::ConstructL()
    {
    LOG( "CMnAiwProvider::ConstructL in" );
#ifdef _DEBUG
    TTime start;
    start.UniversalTime();
#endif

    iEikon = CEikonEnv::Static();

    TRAPD( err, MnUtils::FindLocalizedResourceFileL( iEikon->FsSession(), KAiwResourceFile, iResourceFilename ) );
    if ( err == KErrNotFound )
        {
        // try to find it with the name it had in 3.1
        // this is needed for the case, when upgrade is installed on 3.1-based terminals
        MnUtils::FindLocalizedResourceFileL( iEikon->FsSession(), KAiwResourceFileOld, iResourceFilename );
        }
    iResourceOffset = iEikon->AddResourceFileL( iResourceFilename );

    iEventList = CAiwGenericParamList::NewL();

#ifdef _DEBUG
    TTime finish;
    finish.UniversalTime();
    TTimeIntervalMicroSeconds elapsed = finish.MicroSecondsFrom( start );
    LOG1( "CMnAiwProvider::ConstructL out ( in %ld microseconds ) ", elapsed.Int64() );
#endif
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnAiwProvider::GetAiwServiceCommandIdL( const CAiwGenericParamList& aInParamList )
    {
    TInt index = 0;
    const TAiwGenericParam* param = aInParamList.FindFirst( index, EGenericParamServiceCommand );
    if ( index < 0 )
        {
        LOG( "CMnAiwProvider::GetServiceCommandL: param not found" );
        User::Leave( KErrArgument );
        }

    TInt32 num = 0;
    const TAiwVariant& val = param->Value();
    if ( !val.Get( num ) )
        {
        LOG( "CMnAiwProvider::GetServiceCommandL: param value not found" );
        User::Leave( KErrArgument );
        }
    return num;
    }

// -----------------------------------------------------------------------------
// From CAiwServiceIfBase
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::InitialiseL(
    MAiwNotifyCallback& /*aFrameworkCallback*/,
    const RCriteriaArray& /*aInterest*/ )
    {
    LOG("CMnAiwProvider::InitialiseL");
    }

// -----------------------------------------------------------------------------
// From CAiwServiceIfBase
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::HandleServiceCmdL(
    const TInt& aCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MAiwNotifyCallback* aCallback)
    {
    LOG1("CMnAiwProvider::HandleServiceCmdL cmdId: %x", aCmdId);

    if ( aCmdOptions & KAiwOptCancel )
        {
        // this is call to cancel async request,
        // not supported yet - ignoring, but not starting new one.
        LOG("CMnAiwProvider::HandleServiceCmdL attempt to cancel, exit");
        return;
        }

    CMnAiwCommandHandlerBase::TAiwParameters
        aiwParams( aInParamList, aOutParamList, aCallback );

    switch ( aCmdId )
        {
        case KAiwCmdMnShowMap:
        case KAiwCmdMnNavigateTo:
            {
            TInt err = KErrNone;
            TBool stop = EFalse;
            while ( !stop )
                {
                TRAP( err, ProcessSyncAiwCommandL( aCmdId, aiwParams ) );
                ProcessAiwCommandResultL( aCmdId, aiwParams, err, stop );
                }
            User::LeaveIfError( err );
            }
            break;

        case KAiwCmdMnSelectFromMap:
        case KAiwCmdMnCoordByAddr:
        case KAiwCmdMnAddrByCoord:
            if ( !aCallback )
                {
                User::Leave( KErrArgument );
                }
            StartAsyncAiwCommandL( aCmdId, aiwParams );
            break;

        default:
            User::Leave( KErrNotSupported );
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::ProcessSyncAiwCommandL(
    TInt aCommandId,
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    LOG("CMnAiwProvider::ProcessSyncAiwCommandL in");

	CMnAiwCommandHandlerBase* handler = HandlerL( aCommandId, aAiwParams );
    handler->StartL();

    LOG("CMnAiwProvider::ProcessSyncAiwCommandL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::StartAsyncAiwCommandL(
    TInt aCommandId,
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    LOG("CMnAiwProvider::StartAsyncAiwCommandL in");

    TBool stop = EFalse;
    while ( !stop )
        {
        TInt err = KErrNone;
        CMnAiwCommandHandlerBase* handler = NULL;
        TRAP( err, handler = HandlerL( aCommandId, aAiwParams ) );

        if ( !err )
            {
            handler->Start( *this );
            break;
            }
        else
            {
            if ( handler )
                {
                CloseHandler( handler );
                }

            if ( err == KErrCancel )
                {
                // must stop if provider selection fails
                stop = ETrue;
                }
            else
                {
                // this is error from command start
                ProcessAiwCommandResultL( aCommandId, aAiwParams, err, stop );
                }

            if ( stop )
                {
                // couldn't start command
                NotifyAiwObserver( aCommandId, aAiwParams, err );
                }
            }
        }

    LOG("CMnAiwProvider::StartAsyncAiwCommandL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase* CMnAiwProvider::HandlerL(
    TInt aCommandId,
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    LOG("CMnAiwProvider::HandlerL in");

    CMnProvider& provider = SelectProviderL( aCommandId, aAiwParams );

    CMnAiwCommandHandlerBase::TRunMode runMode =
        CMnAiwCommandHandlerBase::RequestedRunModeL( aAiwParams );

    if ( runMode == CMnAiwCommandHandlerBase::ERunModeDefault )
        {
        runMode = CMnAiwCommandHandlerBase::DefaultRunMode( aCommandId );
        }

    if ( runMode == CMnAiwCommandHandlerBase::ERunModeDefault )
        {
        User::Leave( KErrNotSupported );
        }

    CMnAiwCommandHandlerBase* handler = FindWorkingHandler( aCommandId, provider, runMode );

    if ( !handler )
        {
        handler = CMnAiwCommandHandlerBase::CreateHandlerL( aCommandId, provider, runMode );
        CleanupStack::PushL( handler );
        handler->SetExitObserverL( *this );
        iHandlers.AppendL( handler );
        CleanupStack::Pop( handler );
        }
    else
        {
        LOG("CMnAiwProvider::HandlerL, working handler found, reusing");
        handler->ResetParametersL();
        }

    handler->SetParametersL( aAiwParams );

    LOG("CMnAiwProvider::HandlerL out");
    return handler;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::HandleAsyncAiwCmdCompletedL(
    CMnAiwCommandHandlerBase* aHandler,
    TInt aResult )
    {
    LOG1("CMnAiwProvider::HandleAsyncAiwCmdCompletedL in, result %d", aResult );

    TInt command = aHandler->CommandId();
    CMnAiwCommandHandlerBase::TAiwParameters aiwParams = aHandler->AiwParameters();

    CloseHandler( aHandler );

    TBool stop = EFalse;
    ProcessAiwCommandResultL( command, aiwParams, aResult, stop );

    if ( stop )
        {
        NotifyAiwObserver( command, aiwParams, aResult );
        }
    else
        {
        StartAsyncAiwCommandL( command, aiwParams );
        }

    LOG("CMnAiwProvider::HandleAsyncAiwCmdCompletedL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::HandleProviderExit(
    CMnAiwCommandHandlerBase* aHandler )
    {
    LOG("CMnAiwProvider::HandleProviderExit in" );

	CloseHandler( aHandler );

    LOG("CMnAiwProvider::HandleProviderExit out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::NotifyAiwObserver(
    TInt aCommandId,
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
    TInt aResult )
    {
    LOG2("CMnAiwProvider::NotifyAiwObserver in, command 0x%X, result %d", aCommandId, aResult);
    __ASSERT_DEBUG( aAiwParams.Callback(), Panic( KErrGeneral) );

    MAiwNotifyCallback* callback = const_cast<MAiwNotifyCallback*>( aAiwParams.Callback() );

    TInt err = KErrNone;
    iEventList->Reset();
    if ( aResult )
        {
        TAiwGenericParam error( EGenericParamError, TAiwVariant( aResult ) );
        TRAP_IGNORE( iEventList->AppendL( error ) );

        TRAP( err, callback->HandleNotifyL(
            aCommandId, KAiwEventError, *iEventList, aAiwParams.InList() ) );
        }
    else
        {
        TRAP( err, callback->HandleNotifyL(
            aCommandId, KAiwEventCompleted, *iEventList, aAiwParams.InList() ) );
        }

    LOG("CMnAiwProvider::NotifyAiwObserver out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::CloseHandler( CMnAiwCommandHandlerBase* aHandler )
    {
    LOG("CMnAiwProvider::CloseHandler in");

    if ( aHandler->IsActive() )
    	{
    	// make sure we are not panicked if async handler is closed while active.
    	// at first, command is only cancelled. Cancel will call this method again,
    	// then this object will be deleted.
    	aHandler->Cancel();
    	}

	TInt index = iHandlers.Find( aHandler );
	if ( index != KErrNotFound )
	    {
    	delete aHandler;
    	iHandlers.Remove( index );
	    }
    LOG1("CMnAiwProvider::CloseHandler out, deleted handler %d", index);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::ProcessAiwCommandResultL( 
    TInt aAiwCommandId, 
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams,
    TInt aResult, 
    TBool& aStop )
    {
    LOG1("CMnAiwProvider::ProcessAiwCommandResultL in, result %d", aResult);

    TBool banMessages = EFalse;
    if ( CMnAiwCommandHandlerBase::IsErrorMessageDisabledL( aAiwParams ) )
        {
        banMessages = ETrue;
        }
    
    TInt unusedProviderId( 0 );
    TInt messageId( 0 );
    
    if ( aResult == KErrNone || aResult == KErrCancel )
        {
        aStop = ETrue;
        }
    else if ( CMnAiwCommandHandlerBase::IsProviderSpecifiedL( aAiwParams, unusedProviderId ) ) 
        {
        aStop = ETrue;
        messageId = R_MN_ERROR_SERVICE_UNAVAILABLE;
        }
    else if ( NumSuitableProviders( aAiwCommandId ) < 2 )
        {
        aStop = ETrue;
        messageId = R_MN_ERROR_NO_PROVIDER;
        }
    else
        {
        aStop = EFalse;
        messageId = R_MN_ERROR_SERVICE_UNAVAILABLE;
        }
    
    if ( !banMessages && messageId )
        {
        ErrorNote( messageId );
        }

    LOG("CMnAiwProvider::ProcessAiwCommandResultL out");
    }

// -----------------------------------------------------------------------------
// From CAiwServiceIfMenu
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::InitializeMenuPaneL(
    CAiwMenuPane& aMenuPane,
    TInt aIndex,
    TInt /*aCascadeId*/,
    const CAiwGenericParamList& aInParamList)
    {
    LOG( "CMnAiwProvider::InitializeMenuPaneL in" );

    TInt svcCmdId = GetAiwServiceCommandIdL( aInParamList );
    if ( !IsServiceAvailableL( svcCmdId ) )
        {
        LOG1( "CMnAiwProvider::InitializeMenuPaneL svc 0x%X is not available, out", svcCmdId );
        return;
        }

    switch ( svcCmdId )
        {
        case KAiwCmdMnShowMap:
            aMenuPane.AddMenuItemsL(
                iResourceFilename,
                R_MN_AIWMENU_SHOW_ON_MAP,
                KAiwCmdMnShowMap,
                aIndex);
            break;

        case KAiwCmdMnSelectFromMap:
            aMenuPane.AddMenuItemsL(
                iResourceFilename,
                R_MN_AIWMENU_SELECT_FROM_MAP,
                KAiwCmdMnSelectFromMap,
                aIndex);
            break;

        case KAiwCmdMnNavigateTo:
            aMenuPane.AddMenuItemsL(
                iResourceFilename,
                R_MN_AIWMENU_NAVIGATE_TO,
                KAiwCmdMnNavigateTo,
                aIndex);
            break;

        case KAiwCmdMnCoordByAddr:
            aMenuPane.AddMenuItemsL(
                iResourceFilename,
                R_MN_AIWMENU_FETCH_COORDINATES,
                KAiwCmdMnCoordByAddr,
                aIndex);
            break;

        case KAiwCmdMnAddrByCoord:
            aMenuPane.AddMenuItemsL(
                iResourceFilename,
                R_MN_AIWMENU_FETCH_ADDRESS,
                KAiwCmdMnAddrByCoord,
                aIndex);
            break;
        }
    LOG( "CMnAiwProvider::InitializeMenuPaneL out" );
    }


// -----------------------------------------------------------------------------
// From CAiwServiceIfMenu
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::HandleMenuCmdL(
    TInt aMenuCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MAiwNotifyCallback* aCallback)
    {
    LOG( "CMnAiwProvider::HandleMenuCmdL in" );

    TInt svcCmdId = KAiwCmdNone;

    switch ( aMenuCmdId )
        {
        case EMnAiwMenuCmdShowOnMap:
            svcCmdId = KAiwCmdMnShowMap;
            break;

        case EMnAiwMenuCmdSelectFromMap:
            svcCmdId = KAiwCmdMnSelectFromMap;
            break;

        case EMnAiwMenuCmdNavigateTo:
            svcCmdId = KAiwCmdMnNavigateTo;
            break;

        case EMnAiwMenuCmdFetchAddress:
            svcCmdId = KAiwCmdMnAddrByCoord;
            break;

        case EMnAiwMenuCmdFetchCoordinates:
            svcCmdId = KAiwCmdMnCoordByAddr;
            break;
        }

    HandleServiceCmdL( svcCmdId, aInParamList, aOutParamList, aCmdOptions, aCallback );

    LOG( "CMnAiwProvider::HandleMenuCmdL out" );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnAiwProvider::IsServiceAvailableL( TInt aAiwCommandId )
    {
    UpdateProvidersListL();
    return NumSuitableProviders( aAiwCommandId ) > 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::UpdateProvidersListL()
    {
    // get new list of providers
    RPointerArray<CMnProvider> providers;
    CleanupStack::PushL( TCleanupItem( CleanupProvidersArray, &providers ) );
    MnProviderFinder::FindProvidersL( providers );
    
    // merge new list with current list of providers
    TIdentityRelation<CMnProvider> byUid( ProvidersMatchByUid );

    // detect removed providers
    for ( TInt i = iMnProviders.Count() - 1; i >= 0 ; i-- )
        {
        if ( providers.Find( iMnProviders[i], byUid ) == KErrNotFound )
            {
            LOG1("CMnAiwProvider::UpdateProvidersListL, provider 0x%X removed", 
                iMnProviders[i]->Uid().iUid );
            CloseHandlers( iMnProviders[i] );
            delete iMnProviders[i];
            iMnProviders.Remove( i );
            }
        }

    // detect new providers
    for ( TInt i = providers.Count() - 1; i >=0 ; i-- )
		{
		if ( iMnProviders.Find( providers[i], byUid ) == KErrNotFound )
			{
            LOG1("CMnAiwProvider::UpdateProvidersListL, new provider 0x%X added", 
                providers[i]->Uid().iUid );
			iMnProviders.AppendL( providers[i] );
			providers.Remove( i ); // ownership is moved to iMnProviders
			}
		}

	CleanupStack::PopAndDestroy( &providers );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnAiwCommandHandlerBase* CMnAiwProvider::FindWorkingHandler(
    TInt aAiwCommandId,
    const CMnProvider& aProvider,
    CMnAiwCommandHandlerBase::TRunMode aRunMode )
    {
    for ( TInt i = 0; i < iHandlers.Count(); i++ )
        {
        CMnAiwCommandHandlerBase* handler = iHandlers[i];
        if ( handler->Provider().Uid() == aProvider.Uid() &&
             handler->CommandId() == aAiwCommandId &&
             handler->RunMode() == aRunMode &&
             !handler->IsActive() ) // active async handler cannot be reused
            {
            return iHandlers[i];
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::CloseHandlers( const CMnProvider* aProvider )
	{
	for ( TInt i = iHandlers.Count() - 1; i >= 0 ; i-- )
		{
		if ( ProvidersMatchByUid( iHandlers[i]->Provider(), *aProvider ) )
			{
			CloseHandler( iHandlers[i] ); // this will modify the handlers list
			}
		}
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::CloseAllHandlers()
	{
	for ( TInt i = iHandlers.Count() - 1; i >= 0 ; i-- )
		{
		CloseHandler( iHandlers[i] ); // this will modify the handlers list
		}
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::GetSuitableProvidersL(
    TInt aAiwCommandId,
    RPointerArray<CMnProvider>& aProviders )
    {
    LOG1("CMnAiwProvider::GetSuitableProvidersL in, Cmd %x", aAiwCommandId);

    CMnProvider::TService serviceNeeded = CMnProvider::EServiceNone;
    TInt featuresNeeded = 0;

    CMnAiwCommandHandlerBase::AppServiceAndFeatureNeeded(
        aAiwCommandId, serviceNeeded, featuresNeeded );

    for ( TInt i = 0; i < iMnProviders.Count(); i++ )
        {
        CMnProvider* provider = iMnProviders[i];

        if ( ( provider->SupportedServices() & serviceNeeded ) &&
             ( provider->SupportedFeatures( serviceNeeded ) & featuresNeeded ) )
            {
            LOG1("CMnAiwProvider::GetSuitableProvidersL using 0x%X", provider->Uid().iUid);
            aProviders.AppendL( provider );
            }
        }
    LOG("CMnAiwProvider::GetSuitableProvidersL out");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnAiwProvider::NumSuitableProviders( TInt aAiwCommandId )
    {
    CMnProvider::TService serviceNeeded = CMnProvider::EServiceNone;
    TInt featuresNeeded = 0;

    CMnAiwCommandHandlerBase::AppServiceAndFeatureNeeded(
        aAiwCommandId, serviceNeeded, featuresNeeded );

    TInt count = 0;
    for ( TInt i = 0; i < iMnProviders.Count(); i++ )
        {
        CMnProvider* provider = iMnProviders[i];

        if ( ( provider->SupportedServices() & serviceNeeded ) &&
             ( provider->SupportedFeatures( serviceNeeded ) & featuresNeeded ) )
            {
            count++;
            }
        }
    return count;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnProvider& CMnAiwProvider::SelectProviderL( 
    TInt aAiwCommandId,
    CMnAiwCommandHandlerBase::TAiwParameters& aAiwParams )
    {
    CMnProvider* provider = NULL;

    RPointerArray<CMnProvider> mnProviders; // array does _not_ own items
    CleanupClosePushL( mnProviders );

    UpdateProvidersListL();
    GetSuitableProvidersL( aAiwCommandId, mnProviders );
    
    TInt requestedProviderId( 0 );
    if ( CMnAiwCommandHandlerBase::IsProviderSpecifiedL( aAiwParams, requestedProviderId ) )
        {
        for ( int i = 0; i < mnProviders.Count(); i++ )
            {
            if ( mnProviders[i]->Uid().iUid == requestedProviderId )
                {
                provider = mnProviders[i];
                break;
                }
            }
        
        if ( !provider )
            {
            User::Leave( KErrArgument );
            }
        }
    else if ( mnProviders.Count() > 1 )
        {
        TInt index = KErrNotFound;
        ExecuteSelectionDialogL( index, mnProviders );
            provider = mnProviders[index];
        }
    else if ( mnProviders.Count() == 1 )
        {
        provider = mnProviders[0];
        }
    else
        {
        User::Leave( KErrNotSupported );
        }

    CleanupStack::PopAndDestroy( &mnProviders );
    LOG1("CMnAiwProvider::SelectProviderL, selected 0x%X", provider->Uid().iUid);
    return *provider;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::ExecuteSelectionDialogL(
    TInt& aIndex,
    RPointerArray<CMnProvider>& aProviders )
    {
    CDesCArraySeg* textArray = new (ELeave) CDesCArraySeg( aProviders.Count() );
    CleanupStack::PushL( textArray );

    for ( TInt i = 0; i < aProviders.Count(); i++ )
        {
        TPtrC shortName;
        aProviders[i]->GetShortName( shortName );
        textArray->AppendL( shortName );
        }

    CAknListQueryDialog* dlg = new (ELeave) CAknListQueryDialog( &aIndex );
    dlg->PrepareLC( R_MN_PROVIDER_LIST_QUERY );

    dlg->SetItemTextArray( textArray );
    dlg->SetOwnershipType( ELbmOwnsItemArray );

    TInt result = dlg->RunLD();
    CleanupStack::Pop( textArray );

    if ( !result )
        {
        User::Leave( KErrCancel );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::ErrorNote( TInt aResourceId )
    {
    TRAP_IGNORE( DoErrorNoteL( aResourceId ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnAiwProvider::DoErrorNoteL( TInt aResourceId )
    {
    HBufC* noteText = iEikon->AllocReadResourceLC( aResourceId );

    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    note->ShowNoteL( EAknGlobalErrorNote, *noteText );
    CleanupStack::PopAndDestroy( note );

    CleanupStack::PopAndDestroy( noteText );
    }

// ======== GLOBAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// Map the interface UIDs to implementation factory functions
// -----------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KMnAiwProviderBaseImplUid, CMnAiwProvider::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KMnAiwProviderMenuImplUid, CMnAiwProvider::NewL )
    };

// -----------------------------------------------------------------------------
// Exported proxy for instantiation method resolution
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = ( sizeof ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }
