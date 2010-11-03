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


#ifndef __TESTPROVIDER_APPLICATION_H_
#define __TESTPROVIDER_APPLICATION_H_

#include <eikapp.h>

class CTestProviderAppServer;

/**
*  An instance of CTestProviderApplication is the application part of the Eikon
*  application framework for the MapNav TestProvider
*/
class CTestProviderApplication : public CEikApplication
    {
    public:  // from CEikApplication

        /** 
        * AppDllUid returns the application DLL UID value.
        *
        *  @return the UID of this Application/Dll
        */
        TUid AppDllUid() const;
        
        ~CTestProviderApplication();

    protected: // from CEikApplication

        /** 
        * CreateDocumentL creates a CApaDocument object and return a pointer to it
        *
        * @return a pointer to the created document
        */
        CApaDocument* CreateDocumentL();
        
        void NewAppServerL(CApaAppServer *&aAppServer);
    };

#endif // __TESTPROVIDER_APPLICATION_H_

