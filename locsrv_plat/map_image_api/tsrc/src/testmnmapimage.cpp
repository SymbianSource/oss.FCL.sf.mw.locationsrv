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
* Description:  Helper class for CMnMapImage class
*
*/


// System Includes


// User Includes
#include "testmnmapimage.h"
#include <e32std.h>
#include <e32const.h>
#include <mnmapimage.h>
#include <mnproviderfinder.h>
#include <mnprovider.h>
#include <mnmapimageparams.h>
#include <LbsPosition.h>
#include <fbs.h>

// Constant Declarations



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestMapImage ::CTestMapImage (CStifLogger* aLog):iLog(aLog),CActive(0)
    {
        CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CTestMapImage ::~CTestMapImage ()
    {
        if(IsActive())
            {
                Cancel();
            }
        
    }

// ---------------------------------------------------------------------------
// Test case for CMnMapImage::NewL
// ---------------------------------------------------------------------------

    TInt CTestMapImage::TestNewLL()
        {
            RPointerArray<CMnProvider> providerArray;
	        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
	       // TBool flag = EFalse;
	        /*TUid KTestProviderUid = TUid::Uid( 0x0313200B );
	        for( TInt i = 0; i < providerArray.Count(); i++ )
	            {
	            if( providerArray.operator[](i)->Uid() == KTestProviderUid )
	                {
	                flag = ETrue;
	                break;
	                }
	            }
	         */
	         CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
             delete mapImage;
	            
	        
	        return KErrNone;
        }
        
// ---------------------------------------------------------------------------
//Test case for CMnMapImage::SetMapImageParams & CMnMapImage::MapImageParams
// ---------------------------------------------------------------------------

    TInt CTestMapImage::CheckSetMapImageParams() const
        {
            RPointerArray<CMnProvider> providerArray;
            TMnMapImageParams setMapImageParams;
            TInt error = KErrNone;
            
	        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
	        
	      /*  TUid KTestProviderUid = TUid::Uid( 0x0313200B );
	        for( TInt i = 0; i < providerArray.Count(); i++ )
	            {
	            if( providerArray.operator[](i)->Uid() == KTestProviderUid )
	                {
	                    break;
	                    
	                }
	            }
	        */    
	        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));   
	        
	        
            TCoordinate coordinate(31.6,62.3);
            setMapImageParams.SetCenterPoint(coordinate);
            setMapImageParams.SetVisibleArea(20,30);
            
            TSize size(1,2);
            setMapImageParams.SetSize(size);
            
            setMapImageParams.SetDirection(15.0);
            setMapImageParams.SetImageType(TMnMapImageParams::ETypeSatelliteImage);
            setMapImageParams.SetProjectionId(5);
            
            mapImage->SetMapImageParams(setMapImageParams);
            TMnMapImageParams mapImageParams = mapImage->MapImageParams();
            
            const CMnMapImage* mapImage1 = mapImage;
            
            const TMnMapImageParams param2( mapImage1->MapImageParams());
            
            TCoordinate coordinate1;
            mapImageParams.GetCenterPoint(coordinate1);
            if((coordinate1.Latitude()!=coordinate.Latitude())||
           			(coordinate1.Longitude()!=coordinate.Longitude()))
               {
                
          		error = KErrGeneral; 	
               }

            TReal32 width,height;
            mapImageParams.GetVisibleArea(width,height);
            if(width!=20||height!=30)
           	   {
        		error = KErrGeneral; 	
           	   }
            
            if((mapImageParams.Size()!=size)||(mapImageParams.Direction()!=15.0)||
           	   (mapImageParams.ImageType()!=TMnMapImageParams::ETypeSatelliteImage)||
           	    (mapImageParams.ProjectionId()!=5))
           	   {
        		error = KErrGeneral;  	
           	   }
            
            delete mapImage;
            return error;
            
        }
                        
      
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::SetShowOptions & CMnMapImage::ShowOptions
// ---------------------------------------------------------------------------

TInt CTestMapImage::CheckSetShowOptions(CStifItemParser& aItem)
    {
        TInt showOption = 0;
        TInt err = aItem.GetNextInt(showOption);
        CMnMapImage::TShowOptions setOption = CMnMapImage::EShowOptionNone;
        CMnMapImage::TShowOptions option = CMnMapImage::EShowOptionNone;
        TInt error = KErrNone;
        
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));   
        
        if(!err)
            {
                switch(showOption)
                    {
                    case 0: setOption = CMnMapImage::EShowOptionNone;
                            break;
                    case 1: setOption = CMnMapImage::EShowOptionText;
                            break;
                    case 2: setOption = CMnMapImage::EShowOptionPois;
                            break;
                    case 3: setOption = CMnMapImage::EShowOptionLandmarks;
                            break;
                    case 4: setOption = CMnMapImage::EShowOptionCurrentLocation;
                            break;
                    case 5: setOption = CMnMapImage::EShowOptionCountryBorders;
                            break;
                    case 6: setOption = CMnMapImage::EShowOptionsMandatory;
                            break;
                    default: break;
                    
                    }
                    
                 mapImage->SetShowOptions(setOption);
                 option = mapImage->ShowOptions();
                 if(option == setOption)
                    {
                        
                        error = KErrNone;
                    }
                  else
                    error = KErrGeneral;
                 
                    
                    
            }
          else
                error = KErrGeneral;
          
          delete mapImage;
          return error;
    }
    
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::SetTargetOrigin & CMnMapImage::TargetOrigin
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckSetTargetOrigin()
    {
        TPoint setOrigin(10,10);
        TPoint origin;
        TInt error = KErrNone;
        
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0)); 
        
        mapImage->SetTargetOrigin(setOrigin);
        origin = mapImage->TargetOrigin();
        
        if(origin == setOrigin)
            {
                error = KErrNone;
            }
        else
                error = KErrGeneral;
        
        delete mapImage;
        return error;
        
        
        
        
    }
    
 // ---------------------------------------------------------------------------
// Test case for CMnMapImage::GetImageCoordinate
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckGetImageCoordinate()
    {
        TCoordinate coordinate(10,20);
        TPoint point;
        TInt error = KErrNone;
        TMnMapImageParams setMapImageParams;
        
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
        
        setMapImageParams.SetImageType(TMnMapImageParams::ETypeSatelliteImage);
        TSize size(20,30);
        setMapImageParams.SetSize(size);
        mapImage->SetMapImageParams(setMapImageParams); 
        
        error = mapImage->GetImageCoordinate(coordinate,point);
        
        delete mapImage;
        return error;
    }
    
  // ---------------------------------------------------------------------------
// Test case for CMnMapImage::SupportedImageTypes
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckSupportedImageTypes()
    {
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
      
        TInt error = KErrNone;
                
        if(mapImage->SupportedImageTypes())
         
            {
                error = KErrNone;
            }
        else
            error = KErrGeneral;
        
        delete mapImage;
        return error;
        
    }
  
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::GetWorldCoordinate
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckGetWorldCoordinate()
    {
        TCoordinate coordinate;
        TPoint point(10,10);
        TInt error = KErrNone;
        TMnMapImageParams setMapImageParams;
        
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray ,CMnProvider::EServiceMapImage);
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
        
        setMapImageParams.SetImageType(TMnMapImageParams::ETypeSatelliteImage);
        TSize size(20,30);
        setMapImageParams.SetSize(size);
        mapImage->SetMapImageParams(setMapImageParams); 
        
        error = mapImage->GetWorldCoordinate(point,coordinate);
        
        delete mapImage;
        return error;
    }
 
 // ---------------------------------------------------------------------------
// From MAknServerAppExitObserver
// ---------------------------------------------------------------------------
 
 void CTestMapImage::HandleServerAppExit(TInt /*aReason*/)
    {
        //no implementation
    }
    
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::SetExitObserverL & CMnMapImage::RemoveExitObserver
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckSetExitObserverL()
    {
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
        
        mapImage->SetExitObserverL(*this);
        mapImage->RemoveExitObserver();
        
        delete mapImage;
        return KErrNone;
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
void CTestMapImage::RunL()
    {
        if(iStatus != KErrCancel)
            {
                CActiveScheduler::Stop();	
            }
    }
// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
  void CTestMapImage::DoCancel()
    {

        iMapImage->CancelRendering();
    
        
    }  
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::RenderL whose completion is informed via aStatus parameter.
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckRender1L()
    {
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray ,CMnProvider::EServiceMapImage);
        iMapImage = CMnMapImage::NewL( *providerArray.operator[](0));
      
        RFbsSession fbs;
        fbs.Connect();
        
        TSize size1(10, 20);
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
        CleanupStack::PushL(bitmap);
        bitmap->Create( size1, EColor16M );
        
        TMnMapImageParams params;
        TSize size2(5,10);
        params.SetCenterPoint( TCoordinate( 0, 0 ) );
        params.SetSize( size2 );
        params.SetVisibleArea( 5, 10 );
        iMapImage->SetMapImageParams(params);
        iMapImage->RenderL(*bitmap, iStatus);
        SetActive();
        CActiveScheduler::Start();
        
        CleanupStack::PopAndDestroy(bitmap);
        delete iMapImage;
        fbs.Disconnect();
        
        return iStatus.Int();
        
    }
    
  
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::RenderL whose completion is informed via aCallback
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckRender2L()
    {
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        CMnMapImage* mapImage = CMnMapImage::NewL( *providerArray.operator[](0));
        CleanupStack::PushL(mapImage);
        RFbsSession fbs;
        fbs.Connect();
        
        TSize size1(10, 20);
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
        CleanupStack::PushL(bitmap);
        bitmap->Create( size1, EColor16M );
        
        TMnMapImageParams params;
        TSize size2(5,10);
        params.SetCenterPoint( TCoordinate( 0, 0 ) );
        params.SetSize( size2 );
        params.SetVisibleArea( 5, 10 );
        mapImage->SetMapImageParams(params);
        
        mapImage->RenderL(*bitmap, *this);
        iError = KRequestPending;
        //SetActive();
        CActiveScheduler::Start();
        
        CleanupStack::PopAndDestroy(bitmap);
        CleanupStack::PopAndDestroy(mapImage);
        fbs.Disconnect();
        
        return iError;
        
    }  
    
// ---------------------------------------------------------------------------
// From MMnMapImageRenderCallback
// ---------------------------------------------------------------------------
void CTestMapImage::HandleRenderingCompletedL( TInt aStatus, CMnMapImage& /*aMapImage*/ )
    {
            iError = aStatus;
            CActiveScheduler::Stop();
    }
    
    
    
    
    
// ---------------------------------------------------------------------------
// Test case for CMnMapImage::CancelRendering 
// ---------------------------------------------------------------------------
TInt CTestMapImage::CheckCancelRendering()
    {
        RPointerArray<CMnProvider> providerArray;
        MnProviderFinder::FindProvidersL( providerArray,CMnProvider::EServiceMapImage );
        iMapImage = CMnMapImage::NewL( *providerArray.operator[](0));
      
        RFbsSession fbs;
        fbs.Connect();
        
        TSize size1(10, 20);
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
        CleanupStack::PushL(bitmap);
        bitmap->Create( size1, EColor16M );
        
        TMnMapImageParams params;
        TSize size2(5,10);
        params.SetCenterPoint( TCoordinate( 0, 0 ) );
        params.SetSize( size2 );
        params.SetVisibleArea( 5, 10 );
        iMapImage->SetMapImageParams(params);
        iMapImage->RenderL(*bitmap, iStatus);
        SetActive();
        Cancel();
              
        CleanupStack::PopAndDestroy(bitmap);
        delete iMapImage;
        fbs.Disconnect();
        
        return iStatus.Int();
        
    }
    