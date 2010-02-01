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
* Description:  Helper class for TMnMapImageParams class
*
*/


#ifndef CTESTMNMAPIMAGEPARAMS_H
#define CTESTMNMAPIMAGEPARAMS_H

// System Includes
#include <StifLogger.h>
#include <StifParser.h>
#include <StifParser.h>
#include <mnmapimageparams.h>




// CLASS DECLARATION
/**
 *  
 *
 *  This class is a helper class used to handle the functions of TMnMapImageParams
 *  class. 
 *
 *  @lib testmapimage.lib
 *  @since S60 v5.0
 */

class CTestMapImageParams
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CTestMapImageParams(CStifLogger* aLog);

        /**
        * Destructor. 
        */
        ~CTestMapImageParams();

    public:         
        
                   
		  
        // New functions  
        /**
        * Test case for constructor of TMnMapImageParams
        */
        TInt CheckTMnMapImageParams();
        /**
        * Test case for TMnMapImageParams::SetCenterPoint &
        * TMnMapImageParams::GetCenterPoint
        */
        TInt CheckSetGetCenterPoint();
        /**
        * Test case for TMnMapImageParams::SetVisibleArea &
        * TMnMapImageParams::GetVisibleArea
        */
        TInt CheckSetGetVisibleArea();
        /**
        * Test case for TMnMapImageParams::SetSize &
        * TMnMapImageParams::Size
        */
        TInt CheckSetGetSize();
        /**
        * Test case for TMnMapImageParams::SetDirection &
        * TMnMapImageParams::Direction
        */
        TInt CheckSetGetDirection();
        /**
        * Test case for TMnMapImageParams::SetImageType &
        * TMnMapImageParams::ImageType
        */
        TInt CheckSetGetImageType();
        /**
        * Test case for TMnMapImageParams::SetProjectionId &
        * TMnMapImageParams::ProjectionId
        */
        TInt CheckSetGetProjectionId();
        
        
        
		 
		  
	private:
	    
	   
	   
	    
    private:  //data
	   
	    /**
	     * Pointer to Stif Logger - Does not Own
	     */           
        CStifLogger* 			iLog;
        
     };     

#endif      //  CTESTMNMAPIMAGEPARAMS_H

// End of File
