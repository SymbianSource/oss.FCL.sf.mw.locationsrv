/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  test case implementations.
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <LbsPosition.h>
#include "testmapimgconpluginapi.h"
#include "testprovidercoordconverter.h"

const TInt KTestProviderUid = 0x03133038;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestmapimgconpluginapi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestmapimgconpluginapi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "CMnCoordinateConverterBase_MapImageParams", Ctestmapimgconpluginapi::CMnCoordinateConverterBase_MapImageParams ),
        ENTRY( "CMnCoordinateConverterBase_GetImageCoordinate", Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetImageCoordinate ),
        ENTRY( "CMnCoordinateConverterBase_GetWorldCoordinate", Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetWorldCoordinate ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::CMnCoordinateConverterBase_MapImageParams
// This test case will test MapImageParams method of CMnCoordinateConverterBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt Ctestmapimgconpluginapi::CMnCoordinateConverterBase_MapImageParams(
   CStifItemParser& /*aItem*/)
   {
   CTestProviderCoordConverter* testProviderCoordConverter=
    								CTestProviderCoordConverter::NewLC();
   TMnMapImageParams mapImageParams=testProviderCoordConverter->MapImageParams(); 	
   CleanupStack::PopAndDestroy(testProviderCoordConverter);
   return KErrNone;
   }
   
// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetImageCoordinate
// This test case will test GetImageCoordinate method of CMnCoordinateConverterBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetImageCoordinate(
   CStifItemParser& /*aItem*/)
   {
   CTestProviderCoordConverter* testProviderCoordConverter=
    								CTestProviderCoordConverter::NewLC();
   TCoordinate coordinate;
   TPoint point;
   testProviderCoordConverter->GetImageCoordinate(coordinate,point); 
   	
   CleanupStack::PopAndDestroy(testProviderCoordConverter);
   return KErrNone;
   }   
   
// -----------------------------------------------------------------------------
// Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetWorldCoordinate
// This test case will test GetWorldCoordinate method of CMnCoordinateConverterBase
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt Ctestmapimgconpluginapi::CMnCoordinateConverterBase_GetWorldCoordinate(
   CStifItemParser& /*aItem*/)
   {
   CTestProviderCoordConverter* testProviderCoordConverter=
    								CTestProviderCoordConverter::NewLC();
   TCoordinate coordinate;
   TPoint point;
   testProviderCoordConverter->GetWorldCoordinate(point,coordinate); 
   	
   CleanupStack::PopAndDestroy(testProviderCoordConverter);
   return KErrNone;
   } 

//  [End of File] - Do not remove
