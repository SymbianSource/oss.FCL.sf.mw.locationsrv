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


#ifndef CTESTMNMAPIMAGE_H
#define CTESTMNMAPIMAGE_H

// System Includes
#include <StifLogger.h>
#include <StifParser.h>
#include <StifParser.h>
#include <AknServerApp.h>
#include <mnmapimage.h>
#include <fbs.h>



// CLASS DECLARATION
/**
 *  
 *
 *  This class is a helper class used to handle the functions of CMnMapImage
 *  class. 
 *
 *  @lib testmapimage.lib
 *  @since S60 v5.0
 */

class CTestMapImage:public MAknServerAppExitObserver,public CActive,public MMnMapImageRenderCallback
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CTestMapImage(CStifLogger* aLog);

        /**
        * Destructor. 
        */
        ~CTestMapImage();

    public:         
        
                   
		  
        // New functions  
        /**
        * Test case for CMnMapImage::NewL
        */
        TInt TestNewLL();
        /**
        * Test case for CMnMapImage::SetMapImageParams & CMnMapImage::MapImageParams
        */
        TInt CheckSetMapImageParams() const;
        /**
        * Test case for CMnMapImage::SetShowOptions & CMnMapImage::ShowOptions
        */
        TInt CheckSetShowOptions(CStifItemParser& aItem);
         /**
        * Test case for CMnMapImage::SetTargetOrigin & CMnMapImage::TargetOrigin
        */
        TInt CheckSetTargetOrigin();
        /**
        * Test case for CMnMapImage::TestGetImageCoordinate
        */
        TInt CheckGetImageCoordinate();
	    /**
        * Test case for CMnMapImage::SupportedImageTypes
        */
        TInt CheckSupportedImageTypes();
        /**
        * Test case for CMnMapImage::GetWorldCoordinate
        */
        TInt CheckGetWorldCoordinate();
        /**
        * From MAknServerAppExitObserver
        */
        void HandleServerAppExit  (  TInt  aReason   );
        /**
        * Test case for CMnMapImage::SetExitObserverL & CMnMapImage::RemoveExitObserver
        */
        TInt CheckSetExitObserverL(); 
        /**
        * From CActive
        */
        void RunL();
        void DoCancel();
        /**
        * Test case for CMnMapImage::RenderL whose completion is informed via aStatus parameter.
        */
        TInt CheckRender1L(); 
        /**
        * Test case for CMnMapImage::RenderL whose completion is informed via aCallback
        */
        TInt CheckRender2L(); 
        /**
        * From MMnMapImageRenderCallback
        */
        void HandleRenderingCompletedL( TInt aStatus, CMnMapImage& /*aMapImage*/ );
        /**
        * Test case for CMnMapImage::CancelRendering 
        */
        TInt CheckCancelRendering(); 
        
	
		  
	private:
	    
	   
	   
	    
    private:  //data
	   
	    /**
	     * Pointer to Stif Logger - Does not Own
	     */           
        CStifLogger* 			iLog;
        TInt iError;
        CMnMapImage* iMapImage ;
        
     };     

#endif      //  CTESTMNMAPIMAGE_H

// End of File
