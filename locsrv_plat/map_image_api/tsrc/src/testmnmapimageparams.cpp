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
#include "testmnmapimageparams.h"

// Constant Declarations

    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestMapImageParams ::CTestMapImageParams (CStifLogger* aLog):iLog(aLog)
    {
        
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CTestMapImageParams ::~CTestMapImageParams ()
    {
        
        
    }

// ---------------------------------------------------------------------------
// Test case for constructor of TMnMapImageParams
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckTMnMapImageParams()
    {
        TMnMapImageParams params;
                
        return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetCenterPoint & TMnMapImageParams::GetCenterPoint
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetCenterPoint()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        TCoordinate coordinate(31.6,62.3);
        params.SetCenterPoint(coordinate);
        
        TCoordinate coordinate1;
        params.GetCenterPoint(coordinate1);
        
        if((coordinate1.Latitude()!=coordinate.Latitude())||
        			(coordinate1.Longitude()!=coordinate.Longitude()))
               {
                
          		error = KErrGeneral; 	
               }        
        else 
            error = KErrNone;
        
        return error;
    }
    
// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetVisibleArea & TMnMapImageParams::GetVisibleArea
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetVisibleArea()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        params.SetVisibleArea(20,30);
        
        TReal32 width,height;
        params.GetVisibleArea(width,height);
        if(width!=20||height!=30)
           	   {
        		error = KErrGeneral; 	
           	   }
        else
             error = KErrNone;
        
        return error;
        
    }

// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetSize & TMnMapImageParams::Size
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetSize()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        
        TSize size(1,2);
        params.SetSize(size);
        
        if(params.Size()!=size)
            {
                error = KErrGeneral;
            }
        else
            error = KErrNone;
        
        return error;
    }

// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetDirection & TMnMapImageParams::Direction
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetDirection()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        
        TReal32 direction = 20.0;
        params.SetDirection(direction);
                
        if(params.Direction() != direction)
            {
                error = KErrGeneral;
            }
        else
            error = KErrNone;
       
        return error;        
    }

// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetImageType& TMnMapImageParams::ImageType
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetImageType()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        
        params.SetImageType(TMnMapImageParams::ETypeSatelliteImage);
        
        if(params.ImageType()!=TMnMapImageParams::ETypeSatelliteImage)
            {
                error = KErrGeneral;
            }
        else
            error = KErrNone;
        
        return error;
    }
    
// ---------------------------------------------------------------------------
// Test case for TMnMapImageParams::SetProjectionId & TMnMapImageParams::ProjectionId
// ---------------------------------------------------------------------------
TInt CTestMapImageParams ::CheckSetGetProjectionId()
    {
        TMnMapImageParams params;
        TInt error = KErrNone;
        
        params.SetProjectionId(15);
        
        if(params.ProjectionId() != 15)
            {
                error = KErrGeneral;
            }
        else
            error = KErrNone;
        
        return error;
    }