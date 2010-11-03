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
* Description: Main View for map and navigation test provider
*
*/



#ifndef __MNTP_NAVIVIEW_H_
#define __MNTP_NAVIVIEW_H_

#include <coecntrl.h>

class CTestProviderNaviView : public CCoeControl
    {
    public: // New methods

        static CTestProviderNaviView* NewL( const TRect& aRect );
        static CTestProviderNaviView* NewLC( const TRect& aRect );
        virtual ~CTestProviderNaviView();

    public:  // Functions from base classes

        void Draw( const TRect& aRect ) const;
    private: // Constructors

        void ConstructL(const TRect& aRect);
        CTestProviderNaviView();
    
    private:

    };

#endif // __MNTP_NAVIVIEW_H_

