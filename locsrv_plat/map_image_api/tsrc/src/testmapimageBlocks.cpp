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
* Description: Entry point for test cases in stif module
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testmapimage.h"
#include "testmnmapimage.h"
#include "testmnmapimageparams.h"
#include <swinstapi.h>

_LIT( KProviderSisxPath, "e:\\testing\\data\\MnTestProvider.sisx" );

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestmapimage::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestmapimage::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestmapimage::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestmapimage::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CMnMapImageNewL", Ctestmapimage::TestMapImageNewLL ),
        ENTRY( "TestSetMapImageParams", Ctestmapimage::TestSetMapImageParams ),
        ENTRY( "TestSetShowOptions", Ctestmapimage::TestSetShowOptions ),
        ENTRY( "TestSetTargetOrigin", Ctestmapimage::TestSetTargetOrigin ),
        ENTRY( "TestGetImageCoordinate", Ctestmapimage::TestGetImageCoordinate ),
        ENTRY( "TestSupportedImageTypes", Ctestmapimage::TestSupportedImageTypes ),
        ENTRY( "TestGetWorldCoordinate", Ctestmapimage::TestGetWorldCoordinate ),
        ENTRY( "TestSetExitObserverL", Ctestmapimage::TestSetExitObserverL ),
        ENTRY( "TestRender1L", Ctestmapimage::TestRender1L ),
        ENTRY( "TestRender2L", Ctestmapimage::TestRender2L ),
        ENTRY( "TestCancelRendering", Ctestmapimage::TestCancelRendering ),
        
        ENTRY( "TestTMnMapImageParams", Ctestmapimage::TestTMnMapImageParams ),
        ENTRY( "TestSetGetCenterPoint", Ctestmapimage::TestSetGetCenterPoint ),
        ENTRY( "TestSetGetVisibleArea", Ctestmapimage::TestSetGetVisibleArea ),
        ENTRY( "TestSetGetSize", Ctestmapimage::TestSetGetSize ),
        ENTRY( "TestSetGetDirection", Ctestmapimage::TestSetGetDirection ),
        ENTRY( "TestSetGetImageType", Ctestmapimage::TestSetGetImageType ),
        ENTRY( "TestSetGetProjectionId", Ctestmapimage::TestSetGetProjectionId ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// This method will install the provider application
//
// -----------------------------------------------------------------------------
//   
TInt Ctestmapimage::InstallProvider( CStifItemParser& /*aItem*/  )
    {
   // iLog->Log("Installing [%S]...", aPackagePath );

    TInt err = KErrNone;
    SwiUI::RSWInstSilentLauncher installer;

    err = installer.Connect();
    if( KErrNone != err )
    	{
    	iLog->Log(_L("Connection to installer failed"));
    	return err;
    	}
    //CHECK_EQUAL( err, KErrNone, "Connection to installer failed" );

    SwiUI::TInstallOptionsPckg options;
    options().iUpgrade = SwiUI::EPolicyAllowed;
    options().iPackageInfo = SwiUI::EPolicyAllowed;
    options().iOverwrite = SwiUI::EPolicyAllowed;
    options().iKillApp = SwiUI::EPolicyAllowed;

    err = installer.SilentInstall( KProviderSisxPath, options );
    if( KErrNone != err )
    	{
    	iLog->Log(_L("Provider Installation failed"));
    	installer.Close();
    	return err;
    	}

    //CHECK_EQUAL( err, KErrNone, "Installation failed" );
	iLog->Log(_L("Provider Installation passed"));
    installer.Close();
    return err;
    }


// -----------------------------------------------------------------------------
// Ctestmapimage::TestMapImageNewLL
// Test case for CMnMapImage::NewL
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestMapImageNewLL(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.TestNewLL();

   }

// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetMapImageParams
// Test case for CMnMapImage::SetMapImageParams & CMnMapImage::MapImageParams
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetMapImageParams(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckSetMapImageParams();

   }
   
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetShowOptions
// Test case for CMnMapImage::SetShowOptions & CMnMapImage::ShowOptions
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetShowOptions(
   CStifItemParser& aItem )
   {

       CTestMapImage image(iLog);
        return image.CheckSetShowOptions(aItem);

   }

// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetTargetOrigin
// Test case for CMnMapImage::SetTargetOrigin & CMnMapImage::TargetOrigin
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetTargetOrigin(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckSetTargetOrigin();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestGetImageCoordinate
// Test case for CMnMapImage::GetImageCoordinate
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestGetImageCoordinate(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckGetImageCoordinate();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSupportedImageTypes
// Test case for CMnMapImage::SupportedImageTypes
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSupportedImageTypes(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckSupportedImageTypes();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestGetWorldCoordinate
// Test case for CMnMapImage::GetWorldCoordinate
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestGetWorldCoordinate(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckGetWorldCoordinate();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetExitObserverL
// Test case for CMnMapImage::SetExitObserverL & CMnMapImage::RemoveExitObserver
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetExitObserverL(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckSetExitObserverL();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestRender1L
// Test case for CMnMapImage::RenderL whose completion is informed via aStatus parameter.
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestRender1L(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckRender1L();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestRender2L
// Test case for CMnMapImage::RenderL whose completion is informed via aCallback
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestRender2L(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckRender2L();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestCancelRendering
// Test case for CMnMapImage::CancelRendering 
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestCancelRendering(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImage image(iLog);
        return image.CheckCancelRendering();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestTMnMapImageParams
// Test case for constructor of TMnMapImageParams 
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestTMnMapImageParams(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckTMnMapImageParams();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetCenterPoint
// Test case for TMnMapImageParams::SetCenterPoint & TMnMapImageParams::GetCenterPoint
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetCenterPoint(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetCenterPoint();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetVisibleArea
// Test case for TMnMapImageParams::SetVisibleArea & TMnMapImageParams::GetVisibleArea
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetVisibleArea(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetVisibleArea();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetSize
// Test case for TMnMapImageParams::SetSize & TMnMapImageParams::Size
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetSize(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetSize();

   }

// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetDirection
// Test case for TMnMapImageParams::SetDirection & TMnMapImageParams::Direction
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetDirection(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetDirection();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetImageType
// Test case for TMnMapImageParams::SetImageType & TMnMapImageParams::ImageType
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetImageType(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetImageType();

   }
   
// -----------------------------------------------------------------------------
// Ctestmapimage::TestSetGetProjectionId
// Test case for TMnMapImageParams::SetProjectionId & TMnMapImageParams::ProjectionId
// -----------------------------------------------------------------------------
//

TInt Ctestmapimage::TestSetGetProjectionId(
   CStifItemParser& /*aItem*/ )
   {

        CTestMapImageParams imageParams(iLog);
        return imageParams.CheckSetGetProjectionId();

   }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
