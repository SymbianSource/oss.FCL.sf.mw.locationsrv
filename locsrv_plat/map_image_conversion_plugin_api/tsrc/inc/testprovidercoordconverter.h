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
* Description:  CTestProviderCoordConverter class
*
*/


#ifndef MNTP_COORDINATECONVERTER_H
#define MNTP_COORDINATECONVERTER_H

#include <mncoordinateconverterbase.h>

/**
 *  This class provides implementations for CMnCoordinateConverterBase
 *  class.
 *
 *  @lib testmapimgconpluginapi.lib
 *  @since S60 v5.0
 */

class CTestProviderCoordConverter : public CMnCoordinateConverterBase
    {
    public:
      /**
     	* Two-phased constructor.
     	*/
     	static CTestProviderCoordConverter* NewLC();
     	
     	/**
    	* Destructor.
    	*/
    	~CTestProviderCoordConverter();

    public: // From CMnCoordinateConverterBase

        TInt GetImageCoordinate( 
            const TCoordinate& aWorldCoordinate, 
            TPoint& aImagePoint );
        
        TInt GetWorldCoordinate( 
            const TPoint& aImagePoint, 
            TCoordinate& aWorldCoordinate );
                    
    protected:    
     
        /**
   		* constructor.
   		*/
   		CTestProviderCoordConverter();
        
  
    };

#endif //MNTP_COORDINATECONVERTER_H
