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
* Description:  CMnMapView class implementation
*
*/


#include <e32base.h>
#include <f32file.h>
#include <e32math.h>
#include <barsc2.h>
#include <barsread2.h>
#include <bautils.h>
#include <eikenv.h>
#include <AknServerApp.h>
#include <apaserverapp.h>

#include <lbsposition.h>

#include <mnclientlib.rsg>

#include "mnerrors.h"
#include "mninternal.h"
#include "mndebug.h"
#include "mnutils.h"

#include "mnprovider.h"
#include "mnmapviewserviceclient.h"
#include "mnmapview.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnMapView::CMnMapView()
    {
    ResetMapArea();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView* CMnMapView::NewL( CMnProvider& aProvider )
    {
    CMnMapView* self = new (ELeave) CMnMapView();
    CleanupStack::PushL( self );
    self->ConstructNewL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView* CMnMapView::NewChainedL( CMnProvider& aProvider )
    {
    CMnMapView* self = new (ELeave) CMnMapView();
    CleanupStack::PushL( self );
    self->ConstructChainedL( aProvider );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapView::ConstructCommonL( CMnProvider& aProvider )
    {
    if ( !( aProvider.SupportedServices() & CMnProvider::EServiceMapView ) )
        {
        User::Leave( KErrNotSupported );
        }
    
    // default request text
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    iDefaultRequestText = LoadResourceTextL( fs, KLibResourceFile, R_MN_LIBRARY_REQUEST_TEXT );

    CleanupStack::PopAndDestroy( &fs );
    
    // session with provider
    iSession = new (ELeave) RMnMapViewServiceClient;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapView::ConstructNewL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectL( aProvider.Uid() );
    PostConstructL();            
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapView::ConstructChainedL( CMnProvider& aProvider )
    {
    ConstructCommonL( aProvider );
    iSession->ConnectChainedL( aProvider.Uid() );
LOG("CMnMapView::ConstructChainedL, after connect chained");
    PostConstructL();            
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnMapView::PostConstructL()
    {
    iSession->InitSelectionRequestTextL( iDefaultRequestText );            
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::~CMnMapView()
    {
    delete iDefaultRequestText;
    delete iExitMonitor;
    
    if ( iSession )
        {
        iSession->Close();
        delete iSession;
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::ShowMapL()
    {
    TMnMapOptions options;
    options.iOptions = iOptions;
    options.iCurrentLocationOption = iCurrentLocationOption;
    options.iCentralPoint = iCentralPoint;
    options.iRadius = iRadius;

    iSession->ShowMapL( options );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::ShowCurrentLocationL()
    {
    TMnMapOptions options;
    options.iOptions = iOptions;
    options.iCurrentLocationOption = iCurrentLocationOption;
    options.iCentralPoint = iCentralPoint;
    options.iRadius = iRadius;

    iSession->ShowCurrentLocationL( options );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SelectFromMapL( TRequestStatus& aStatus )
    {
    TMnMapOptions options;
    options.iOptions = iOptions;
    options.iCurrentLocationOption = iCurrentLocationOption;
    options.iCentralPoint = iCentralPoint;
    options.iRadius = iRadius;

    iSession->SelectFromMap( options, aStatus );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::AddLandmarksToShowL(
    const TDesC& aDatabaseUri,
    RArray<TPosLmItemId>& aLandmarkIds)
    {
    iSession->AddLandmarksToShowL( aDatabaseUri, aLandmarkIds );
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::AddLandmarksToShowL( RPointerArray<CPosLandmark>& aLandmarks )
    {
    iSession->AddLandmarksToShowL( aLandmarks );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::AddLandmarksToShowL( RArray<TPtrC8>& aPackedLandmarks )
    {
    iSession->AddLandmarksToShowL( aPackedLandmarks );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::ResetLandmarksToShow()
    {
    iSession->ResetLandmarksToShow();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetMapAreaL(const TCoordinate& aCentralPoint, TReal aRadius)
    {
    iCentralPoint = aCentralPoint;
    iRadius = aRadius;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::ResetMapArea()
    {
    TCoordinate empty;
    iCentralPoint = empty;
    
    TRealX nan;
    nan.SetNaN();
    iRadius = nan;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetCustomRequestTextL(const TDesC& aCustomText)
    {
    iSession->SetSelectionRequestTextL( aCustomText );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetUseDefaultRequestText()
    {
    iSession->SetSelectionRequestText( iDefaultRequestText );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetCurrentLocationOption(TCurrentLocationOption aCurrentLocationOption)
    {
    iCurrentLocationOption = aCurrentLocationOption;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::TCurrentLocationOption CMnMapView::CurrentLocationOption() const
    {
    return iCurrentLocationOption;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetOptions(TOptions aOptions)
    {
    iOptions = aOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::TOptions CMnMapView::Options() const
    {
    return iOptions;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CMnMapView::TSelectionResultType CMnMapView::SelectionResultType() const
    {
    return iSession->SelectionResultType();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C CPosLandmark* CMnMapView::RetrieveSelectionResultL()
    {
    return iSession->GetSelectedLandmarkL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::RetrieveSelectionResultL( TInt& aLandmarkIndex )
    {
    iSession->GetSelectedLandmarkL( aLandmarkIndex );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::RetrieveSelectionResultL( 
    TPosLmItemId& aLandmarkId,
    HBufC*& aDatabaseUri )
    {
    iSession->GetSelectedLandmarkL( aLandmarkId, aDatabaseUri );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::Cancel()
    {
    iSession->Cancel();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::SetExitObserverL( MAknServerAppExitObserver& aObserver )
	{
	LOG("CMnMapView::SetExitObserverL in");	
	if ( iExitMonitor )
		{
		User::Leave( KErrAlreadyExists );
		}
	else
		{
		iExitMonitor = CApaServerAppExitMonitor::NewL( 
			*iSession, aObserver, CActive::EPriorityStandard );
		}
	LOG("CMnMapView::SetExitObserverL out");	
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C void CMnMapView::RemoveExitObserver()
	{
	delete iExitMonitor;
	iExitMonitor = NULL;
	}

// ======== INTERNAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
HBufC* CMnMapView::LoadResourceTextL( 
    RFs& aFsSession, 
    const TDesC& aRscFile, 
    TInt aResourceId )
    {
    TFileName fileName;
    MnUtils::FindLocalizedResourceFileL( aFsSession, aRscFile, fileName );

    CResourceFile* resFile = CResourceFile::NewL( aFsSession, fileName, 0, 0 );
    CleanupStack::PushL( resFile );
    resFile->ConfirmSignatureL( 0 );
    
    RResourceReader reader;
    reader.OpenL( resFile, aResourceId );
    CleanupClosePushL( reader );
    
    HBufC* text = reader.ReadHBufC16L();
    
    CleanupStack::PopAndDestroy( &reader );
    CleanupStack::PopAndDestroy( resFile );
        
    return text;        
    }
