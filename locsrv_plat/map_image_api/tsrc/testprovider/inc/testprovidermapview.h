/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Map View for map and navigation test provider
*
*/



#ifndef __MNTP_MAPVIEW_H_
#define __MNTP_MAPVIEW_H_

#include <coecntrl.h>

class CTestProviderMapView : public CCoeControl
    {
    public: // New methods

        static CTestProviderMapView* NewL( const TRect& aRect );
        static CTestProviderMapView* NewLC( const TRect& aRect );

        virtual ~CTestProviderMapView();

    public:  // Functions from base classes

        void Draw( const TRect& aRect ) const;
        
    private: // Constructors

        void ConstructL(const TRect& aRect);

        CTestProviderMapView();
    
    private:

    };

#endif // __MNTP_MAPVIEW_H_

