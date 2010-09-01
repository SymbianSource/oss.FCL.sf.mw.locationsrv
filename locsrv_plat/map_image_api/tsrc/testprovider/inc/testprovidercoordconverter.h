/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Co ordinate converter for map and navigation test provider
*
*/
#ifndef MNTP_COORDINATECONVERTER_H
#define MNTP_COORDINATECONVERTER_H

#include <mncoordinateconverterbase.h>

class CTestProviderCoordConverter : public CMnCoordinateConverterBase
    {
    public:
        static CTestProviderCoordConverter* NewL();

        ~CTestProviderCoordConverter();

    protected: // From CMnCoordinateConverterBase

        TInt GetImageCoordinate( 
            const TCoordinate& aWorldCoordinate, 
            TPoint& aImagePoint );
        
        TInt GetWorldCoordinate( 
            const TPoint& aImagePoint, 
            TCoordinate& aWorldCoordinate );
                    
    protected:    
        CTestProviderCoordConverter();
        
    private:
    };

#endif /*MNTP_COORDINATECONVERTER_H*/
