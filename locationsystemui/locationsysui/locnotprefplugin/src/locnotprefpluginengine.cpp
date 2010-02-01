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
* Description:  Location Notation Preferences Plugin Engine
*
*/


// System Include
#include <e32keys.h>        // Tab Keys
#include <StringLoader.h>   // String Loader
#include <locnotprefpluginview.rsg>
#include <aknradiobuttonsettingpage.h>

// User Include
#include "locnotprefpluginengine.h"
#include "locnotprefengobserver.h"
#include "locnotprefcorddisadapter.h"
#include "locnotprefsomadapter.h"

// Constant Defintions
const TInt KNumberofSettingsItems = 2;
const TInt KMaxConversionBufferLength = 0x200;
const TInt KNoofSysofMeas = 2;
const TInt KNoofCordDisplayFormats = 3;

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CLocNotPrefPluginEngine::CLocNotPrefPluginEngine
// Overloaded Constructor
//
// ---------------------------------------------------------------------------
//  
CLocNotPrefPluginEngine::CLocNotPrefPluginEngine(  
                                MLocNotPrefEngObserver& aEngineObserver )
    :iObserver(aEngineObserver),
    iSettingsDialogOutstanding( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPluginEngine::~CLocNotPrefPluginEngine
// Destructor
//
// ---------------------------------------------------------------------------
//
CLocNotPrefPluginEngine::~CLocNotPrefPluginEngine()
    {
    // Delete the list box strings
    delete iSysofMeasTitle;
    
    delete iCoordDispTitle;
    
    delete iImperialSettings;
    
    delete iMetricSettings;
    
    delete iDegreesSettings;
    
    delete iMinutesSettings;
    
    delete iSecondsSettings;
    
    delete iConversionBuffer;
        
    // Delete the Co-ordinate display settings handle
    delete iCordDisplaySettings;

    // Delete the System of Measurements settings handle    
    delete iSysofMeasSettings;
    }
    
// ---------------------------------------------------------------------------
// CLocNotPrefPluginEngine* CLocNotPrefPluginEngine::NewL
// Creates new Location Notation Preferences plug-in engine instance.
//
// @param  aEngineObserver           Observer to the Notation preferences engine
// @return CLocNotPrefPluginEngine* Reference to the application view
// ---------------------------------------------------------------------------
//    
CLocNotPrefPluginEngine* CLocNotPrefPluginEngine::NewL( 
                            MLocNotPrefEngObserver& aEngineObserver )
    {   
    CLocNotPrefPluginEngine* self = 
                        CLocNotPrefPluginEngine::NewLC( aEngineObserver );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocNotPrefPluginEngine* CLocNotPrefPluginEngine::NewLC
// Creates new Location Notation Preferences plug-in engine instance. Leaves
// the object on the Cleanup stack
//
// @param  aEngineObserver           Observer to the Notation preferences engine
// @return CLocNotPrefPluginEngine* Reference to the application view
// ---------------------------------------------------------------------------
// 
CLocNotPrefPluginEngine* CLocNotPrefPluginEngine::NewLC( 
                            MLocNotPrefEngObserver& aEngineObserver )
    {   
    CLocNotPrefPluginEngine* self = 
                        new(ELeave) CLocNotPrefPluginEngine( aEngineObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::ConstructL
// Second Phase Constructor
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginEngine::ConstructL()
    {
    // Create Co-ordinate display settings handle
    iCordDisplaySettings = CLocNotPrefCoordinateDisplayAdapter::NewL( *this );
    
    iSysofMeasSettings = CLocNotPrefSystemofMeasAdapter::NewL( *this );
    
    // Load the list box strings
    // Allocate and set the System of measurement heading
    iSysofMeasTitle = StringLoader::LoadL( R_LOC_NOTATION_MEASURES );
    
    // Allocate and set the server ip address title field
    iCoordDispTitle = StringLoader::LoadL( R_LOC_NOTATION_COORDINATE_FORMAT );

    // Allocate and set the Imperial settings text
    iImperialSettings = StringLoader::LoadL( R_LOC_IMPERIAL_SYSTEM );
    
    // Allocate and set the Metric settings text
    iMetricSettings = StringLoader::LoadL( R_LOC_METRIC_SYSTEM );
    
    // Allocate and set the Degrees resolution settings text
    iDegreesSettings = StringLoader::LoadL( R_LOC_FORMAT_DECIMAL_DEGREES );
    
    // Allocate and set the Minutes resolution settings text
    iMinutesSettings = StringLoader::LoadL( R_LOC_FORMAT_DECIMAL_MINUTES );
    
    // Allocate and set the Seconds resolution settings text
    iSecondsSettings = StringLoader::LoadL( R_LOC_FORMAT_DECIMAL_SECONDS );
    
    // Allocate the Conversion Buffer
    iConversionBuffer = HBufC16::NewL( KMaxConversionBufferLength );        
    }
    
// ---------------------------------------------------------------------------
// TInt CLocNotPrefPluginEngine::MdcaCount
// Inherited from MDesCArray
//
// ---------------------------------------------------------------------------
//
TInt CLocNotPrefPluginEngine::MdcaCount() const
    {
    return KNumberofSettingsItems;
    }

// ---------------------------------------------------------------------------
// TInt CLocNotPrefPluginEngine::MdcaPoint
// Inherited from MDesCArray
//
// ---------------------------------------------------------------------------
//
TPtrC16 CLocNotPrefPluginEngine::MdcaPoint( TInt aIndex ) const
    {
    // Zero the internal buffer
    TPtr16 ptr( iConversionBuffer->Des());
    ptr.Zero();
    
    // Format "\t%S\t\t%S"
    
    // Append the first tab
    ptr.Append( EKeyTab );
            
    switch( aIndex )
        {
        case ESystemofMeasurement:
            {
            // Append the Title text
            ptr.Append( iSysofMeasTitle->Des());
            
            // Append the second tab
            ptr.Append( EKeyTab ); 
                            
            // Append the third tab
            ptr.Append( EKeyTab );
            
            // Obtain the current value of system of measurement and append
            // to the buffer
            TLocSystemofMeasurementValues currentSettings = ELocMetric;
            TRAPD( error, currentSettings = iSysofMeasSettings->GetCurrentSysofMeasL());
            if( KErrNone == error )
                {
                if( ELocMetric == currentSettings )
                    {
                    ptr.Append( iMetricSettings->Des());
                    }
                else if ( ELocImperial == currentSettings )
                    {
                    ptr.Append( iImperialSettings->Des());
                    }
                }
            
            break;    
            }
        case ECoordinateDisplayFormat:
            {
            // Append the Title text
            ptr.Append( iCoordDispTitle->Des());
            
            // Append the second tab
            ptr.Append( EKeyTab ); 
            
            // Append the third tab
            ptr.Append( EKeyTab );
            
            // Obtain the current value of co-ordinate display format and append
            // to the buffer
            CLocNotPrefCoordinateDisplayAdapter::TCordDisSetListPosition currentSettings = 
                    CLocNotPrefCoordinateDisplayAdapter::EPosDMSS;
            TRAPD( error, currentSettings = iCordDisplaySettings->GetCurrentDisplayFormatL());
            if( KErrNone == error )
                {
                if( CLocNotPrefCoordinateDisplayAdapter::EPosDD == currentSettings )
                    {
                    ptr.Append( iDegreesSettings->Des());
                    }
                else if ( CLocNotPrefCoordinateDisplayAdapter::EPosDMM == currentSettings )
                    {
                    ptr.Append( iMinutesSettings->Des());
                    }
                else if ( CLocNotPrefCoordinateDisplayAdapter::EPosDMSS == currentSettings )
                    {
                    ptr.Append( iSecondsSettings->Des());
                    }
                }           
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
    return iConversionBuffer->Des();
    }
 
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::HandleSettingsChangeL
// Inherited from MLocNotPrefAdapterObserver
//
// ---------------------------------------------------------------------------
//   
void CLocNotPrefPluginEngine::HandleSettingsChangeL()
    {
    iObserver.HandleSettingsChangeEventL();
    }
 
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::Close
// Closes the running Settings UI prematurely. This would result in dismissing
// any active dialogs currently running
//
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginEngine::Close()
    {
    // If any of the dialogs are active then issue a cancel event on the 
    // dialogs
    if( iSettingsDialogOutstanding )
        {
        TKeyEvent   keyEvent;
        keyEvent.iCode          = EKeyCBA2;
        keyEvent.iScanCode      = EStdKeyDevice1;         
        keyEvent.iModifiers     = EAllModifiers;
        keyEvent.iRepeats       = 0;
        CCoeEnv* cCoeEnv = CCoeEnv::Static();
        
        // Simulate a Cancel event on the running dialogs. The Simulated event
        // is for the Left Soft Key.
        // Ignore the error incase of a leave
        
        TRAP_IGNORE( cCoeEnv->SimulateKeyEventL( keyEvent, EEventKey ));
        }
    iSettingsDialogOutstanding = EFalse;                
    }
            
// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::ChangeSysofMeasSettingsL
// Change System of Measurement settings
// 
// ---------------------------------------------------------------------------
// 
void CLocNotPrefPluginEngine::ChangeSysofMeasSettingsL()
    {
    if( iSettingsDialogOutstanding )
        {
        User::Leave( KErrInUse );
        }
    // Allocate the descriptor array for Text settings for System
    // of measurement
    CDesCArrayFlat* items = new( ELeave ) CDesCArrayFlat( KNoofSysofMeas );
    CleanupStack::PushL( items );

    // Append the radio-button list items
    items->AppendL( iMetricSettings->Des());
    items->AppendL( iImperialSettings->Des());
     
    // Obtain the current value for current system of measurement
    // This would be used for setting the default value for 
    // the text settings page               

    // The settings value start from 1 whereas the list box items
    // start from 0. Hence, subtract 1 from the current settings
    // value
    TInt currentSettings = iSysofMeasSettings->GetCurrentSysofMeasL() - 1;
        
    CAknRadioButtonSettingPage* dlg = 
                        new ( ELeave )CAknRadioButtonSettingPage( R_LOC_SYSOFMEAS_SETTINGS,
                                                                  currentSettings, 
                                                                  items );
    // Settings Outstanding flag is marked True to enable dismissal incase
    // of a Cancel event                                                                  
    iSettingsDialogOutstanding = ETrue;                                                                  
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ))
        {
        // The settings value start from 1 whereas the list box items
        // start from 0. Hence, add 1 from the current settings
        // value
        TLocSystemofMeasurementValues newValue = 
                    static_cast<TLocSystemofMeasurementValues>( currentSettings + 1 );
        iSysofMeasSettings->SetCurrentSysofMeasL( newValue );
        } 
    iSettingsDialogOutstanding = EFalse;           
    // Free the items resource
    CleanupStack::PopAndDestroy( items );
     
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::ToggleSysofMeasSettingsL
// Toggle System of Measurement settings
// 
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginEngine::ToggleSysofMeasSettingsL()
    {      
    if( iSettingsDialogOutstanding )
        {
        User::Leave( KErrInUse );
        }    
    iSysofMeasSettings->ToggleSysofMeasSettingsL();
    }

// ---------------------------------------------------------------------------
// void CLocNotPrefPluginEngine::ToggleSysofMeasSettingsL
// Change Co-ordinate display format settings
// 
// ---------------------------------------------------------------------------
//
void CLocNotPrefPluginEngine::ChangeCoordinateDisplaySettingsL()
    {
    if( iSettingsDialogOutstanding )
        {
        User::Leave( KErrInUse );
        }
            
    // Allocate the descriptor array for Text settings for Co-ordinate display
    // formats
    CDesCArrayFlat* items = new( ELeave ) CDesCArrayFlat( KNoofCordDisplayFormats );
    CleanupStack::PushL( items );

    // Append the radio-button list items
    items->AppendL( iSecondsSettings->Des());    
    items->AppendL( iMinutesSettings->Des());
    items->AppendL( iDegreesSettings->Des());
     
    // Obtain the current value for current system of measurement
    // This would be used for setting the default value for 
    // the text settings page               

    TInt currentSettings = iCordDisplaySettings->GetCurrentDisplayFormatL();
        
    CAknRadioButtonSettingPage* dlg = 
                        new ( ELeave )CAknRadioButtonSettingPage( R_LOC_COORDDISPLAY_SETTINGS,
                                                                  currentSettings, 
                                                                  items );
                                                                           
    // Settings Outstanding flag is marked True to enable dismissal incase
    // of a Cancel event                                                                  
    iSettingsDialogOutstanding = ETrue;

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenAccepted ))
        {
        // The settings value start from 1 whereas the list box items
        // start from 0. Hence, add 1 from the current settings
        // value
        CLocNotPrefCoordinateDisplayAdapter::
            TCordDisSetListPosition newValue = static_cast<
                CLocNotPrefCoordinateDisplayAdapter::TCordDisSetListPosition>( 
                                                                    currentSettings );
                                                                    
        iCordDisplaySettings->SetCurrentDisplayFormatL( newValue );
        }

    iSettingsDialogOutstanding = EFalse;
              
    // Free the items resource
    CleanupStack::PopAndDestroy( items );    
    }
