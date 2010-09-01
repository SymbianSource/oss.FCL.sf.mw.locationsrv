/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implements the CTestProviderAppUi class
*
*/


#include <aknnavi.h>
#include <aknnavide.h>
#include <w32std.h>
#include <EikServerApp.h>

#include "debug.h"

#include "testproviderappview.h"
#include "testprovidernaviview.h"
#include "testprovidermapview.h"

#include "testproviderappui.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderAppUi::CTestProviderAppUi()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::ConstructL()
    {
    BaseConstructL(EAknEnableSkin);

    if ( !iEikonEnv->StartedAsServerApp() )
        {
        CAknAppUi::Exit();	
        }
/*
    CEikAppServer* server = iEikonEnv->AppServer();
    if ( server )
        {
        iShutter = new ( ELeave ) CShutter( *server );
        iShutter->ConstructL();
        
        const TTimeIntervalMicroSeconds32 KShutdownDelay = 2 * 1000 * 1000;
        iShutter->Go();After( KShutdownDelay );
        }
*/        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderAppUi::~CTestProviderAppUi()
    {
    delete iNaviLabel;
    
    delete iAppView;
    if ( iNaviView )
        {
        RemoveFromStack( iNaviView );
        }
    delete iNaviView;
    if ( iMapView )
        {
        RemoveFromStack( iMapView );
        }
    delete iMapView;
/*
    if ( iShutter )
        {
        iShutter->Cancel();
        delete iShutter;
        }
*/        
    LOG("application destroyed");
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::HandleCommandL( TInt aCommand )
    {
    switch(aCommand)
        {
        case EAknSoftkeyBack:
            {
    	    Exit( EEikCmdExit ); 
        	break;
            }
            
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            Exit( EAknCmdExit ); // this will close client also
        	break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::Exit( TInt aReason )
    {
    CEikAppServer* server = iEikonEnv->AppServer();
    if ( server )
    	{
        server->NotifyServerExit( aReason );
    	}
    CAknAppUi::Exit();	
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CTestProviderAppUi::ProcessCommandParametersL( CApaCommandLine &aCommandLine )
    {
    iParentWg = aCommandLine.ParentWindowGroupID();
    LOG1("parent window group id %d", iParentWg );
    
    return CAknAppUi::ProcessCommandParametersL( aCommandLine );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::DynInitMenuPaneL(TInt /*aResourceId*/, CEikMenuPane* /*aMenuPane*/)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::PostConstructL()
    {
/*
    // Create TestProvider view
    CTestProviderAppView* TestProviderAppView = CTestProviderAppView::NewLC();
    AddViewL(TestProviderView); // Transfer ownership to base class
    CleanupStack::Pop(TestProviderView);

    // Activate TestProvider view
    TestProviderView->ActivateViewL(TestProviderView->ViewId());
*/
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderNaviView& CTestProviderAppUi::ShowNaviViewL()
    {
    if ( !iNaviView )
        {
        _LIT( KNavigation, "Navigation" );
        SetNaviPaneTitleL( KNavigation );
        
        iNaviView = CTestProviderNaviView::NewL( ClientRect() );
        AddToStackL( iNaviView );
        }
    return *iNaviView;    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderMapView& CTestProviderAppUi::ShowMapViewL()
    {
    if ( !iMapView )
        {
        _LIT( KMapView, "MapView" );
        SetNaviPaneTitleL( KMapView );
        
        iMapView = CTestProviderMapView::NewL( ClientRect() );
        AddToStackL( iMapView );
        }
    return *iMapView;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CTestProviderAppUi::IsChainedMode()
    {
    return ( iParentWg != 0 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CTestProviderAppUi::IsAnyViewOpen()
    {
    return iMapView || iNaviView;
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
CEikStatusPane* CTestProviderAppUi::StatusPane()
    {
    return CEikonEnv::Static()->AppUiFactory()->StatusPane();
    }
    
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CTestProviderAppUi::SetNaviPaneTitleL( const TDesC& aPaneTitle )
    {
    if ( !iNaviPane )
        {
        iNaviPane = static_cast<CAknNavigationControlContainer*>(
                StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        }

    if ( iNaviLabel )
        {
        CAknNavigationDecorator* naviLabel = iNaviPane->CreateNavigationLabelL( aPaneTitle );
        CleanupStack::PushL( naviLabel );

        iNaviPane->ReplaceL( *iNaviLabel, *naviLabel );

        delete iNaviLabel;
        iNaviLabel = naviLabel;

        CleanupStack::Pop( naviLabel );
        }
    else
        {
        iNaviLabel = iNaviPane->CreateNavigationLabelL( aPaneTitle );
        iNaviPane->PushL( *iNaviLabel );
        }
    }    
/*
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CTestProviderAppUi::CShutter::CShutter( CTestProviderAppServer& aServer ) 
: CTimer( CActive::EPriorityIdle ),
  iServer( aServer )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::CShutter::ConstructL()
    {
    CTimer::ConstructL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::CShutter::RunL()
    {
    if ( iServer.IsFree() )
        {
        iAppUi.Exit( EAknCmdExit );
        }
    else
        {
        Go();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CTestProviderAppUi::CShutter::Go()
    {
    const TTimeIntervalMicroSeconds32 KShutdownDelay = 2 * 1000 * 1000;
    iShutter->After( KShutdownDelay );
    }
*/    