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
* Description: Application View for map and navigation test provider
*
*/



#ifndef __MNTP_APPVIEW_H_
#define __MNTP_APPVIEW_H_

#include <coecntrl.h>

class CTestProviderAppView : public CCoeControl
    {
    public: // New methods

        /**
        * NewL.
        * Two-phased constructor.
        * Create a CTestProviderAppView object, which will draw itself to aRect.
        * @param aRect The rectangle this view will be drawn to.
        * @return a pointer to the created instance of CTestProviderAppView.
        */
        static CTestProviderAppView* NewL( const TRect& aRect );

        /**
        * NewLC.
        * Two-phased constructor.
        * Create a CTestProviderAppView object, which will draw itself
        * to aRect.
        * @param aRect Rectangle this view will be drawn to.
        * @return A pointer to the created instance of CTestProviderAppView.
        */
        static CTestProviderAppView* NewLC( const TRect& aRect );

        /**
        * ~CTestProviderAppView
        * Virtual Destructor.
        */
        virtual ~CTestProviderAppView();

    public:  // Functions from base classes

        /**
        * From CCoeControl, Draw
        * Draw this CAnimationAppView to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;
        
        //TInt CountComponentControls() const { return 1;};
        
    protected:
        /*
        CCoeControl* ComponentControl(TInt aIndex) const
            {
                return (aIndex == 0) ? iList : NULL;
            };
        */
    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        * Perform the second phase construction of a
        * CTestProviderAppView object.
        * @param aRect The rectangle this view will be drawn to.
        */
        void ConstructL(const TRect& aRect);

        /**
        * CTestProviderAppView.
        * C++ default constructor.
        */
        CTestProviderAppView();
    
    private:

    };

#endif // __MNTP_APPVIEW_H_

