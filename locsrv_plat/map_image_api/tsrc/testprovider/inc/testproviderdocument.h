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


#ifndef MNTP_DOCUMENT_H
#define MNTP_DOCUMENT_H

#include <akndoc.h>

class CTestProviderAppUi;
class CEikApplication;

/**
*  An instance of CTestProviderDocument is the application part of the Avkon
*  application framework for the TestProvider reference application
*/
class CTestProviderDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aApp an eikon application instance
        */
        static CTestProviderDocument* NewL( CEikApplication& aApp );

        /**
        * Destructor.
        */
        ~CTestProviderDocument();

    public: // from CAknDocument

        /**
        * CreateAppUiL creates a CTestProviderAppUi object and return a pointer 
        * to it.
        *
        * @return a pointer to the created instance of the AppUi created
        */
        CEikAppUi* CreateAppUiL();

    private:

        /**
        * C++ constructor.
        */
        CTestProviderDocument(CEikApplication& aApp);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    };

#endif // MNTP_DOCUMENT_H

