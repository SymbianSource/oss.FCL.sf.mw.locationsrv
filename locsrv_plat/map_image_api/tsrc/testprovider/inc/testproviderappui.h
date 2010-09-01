/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     See class description below
*
*/


#ifndef __TESTPROVIDER_APPUI_H_
#define __TESTPROVIDER_APPUI_H_

#include <aknviewappui.h>

class CTestProviderAppView;
class CTestProviderNaviView;
class CTestProviderMapView;

class CAknNavigationDecorator;
class CAknNavigationControlContainer;

/**
*  An instance of class CTestProviderAppUi is the UserInterface part of the Eikon
*  application framework for the TestProvider reference application
*/
class CTestProviderAppUi : public CAknAppUi
    {
    public: // Constructors and destructor

        /**
        * C++ default constructor. This needs to be public due to the way
        * the framework constructs the AppUi.
        */
        CTestProviderAppUi();

        /**
        * Symbian 2nd phase constructor. This needs to be public due to the way
        * the framework constructs the AppUi.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CTestProviderAppUi();

        CTestProviderNaviView& ShowNaviViewL();
        CTestProviderMapView& ShowMapViewL();
        TBool IsAnyViewOpen();
        TBool IsChainedMode();
        void Exit( TInt aReason );
        
    public: // from CEikAppUi

        /**
        * HandleCommandL handles user menu selections.
        *
        * @param aCommand the enumerated code for the option selected
        */
        void HandleCommandL(TInt aCommand);
        
        TBool ProcessCommandParametersL( CApaCommandLine &aCommandLine );

    public: // from MEikMenuObserver

        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);

    private:

        /**
        * Performs the construction phase of this object that is dependent
        * that the application engine is initialized.
        */
        void PostConstructL();

        CEikStatusPane* StatusPane();

        void SetNaviPaneTitleL( const TDesC& aPaneTitle );
        
    private: // Data
        CTestProviderAppView* iAppView;
        CTestProviderNaviView* iNaviView;
        CTestProviderMapView* iMapView;

        //Own:
        CAknNavigationDecorator* iNaviLabel;

        //Ref:
        CAknNavigationControlContainer* iNaviPane;
        
        TInt iParentWg;
/*
    private:
        class CShutter : public CTimer
            {
            public:
                CShutter( 
                    CTestProviderAppServer& aServer,
                    CTestProviderAppUi& aAppUi );
                void ConstructL();                
                void RunL();
                void Go();

            protected:
                CTestProviderAppServer& iServer;                
                CTestProviderAppUi& iAppUi;
            };
            
    private:            
        CShutter* iShutter;
*/        
    };

#endif // __TESTPROVIDER_APPUI_H_

