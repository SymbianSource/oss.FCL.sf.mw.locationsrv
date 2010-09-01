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


#ifndef MNTP_APPSERVER_H
#define MNTP_APPSERVER_H

#include <aknserverapp.h>
#include <e32property.h> 

class CTestProviderNavigationService;
class CTestProviderMapViewService;
class CTestProviderGeocodingService;

#ifdef RD_MAPNAV_BITMAP_ACCESS
class CTestProviderMapImageService;
#endif

class CTestProviderAppUi;
class CTestCommandListener;

/**
*  An instance of CTestProviderAppServer is the application part of the Eikon
*  application framework for the MapNav TestProvider
*/
class CTestProviderAppServer : public CAknAppServer
    {
    public:
        CTestProviderAppUi* AppUi() const;
        ~CTestProviderAppServer();
        
        void ConstructL( const TDesC& aFixedServerName );
        
        TBool IsFree();
        
        void HandleTestCommandL();
        TInt GetTestCommand() 
            {
            return iTestCommand;
            };

    public: // from CAknAppServer
        IMPORT_C CApaAppServiceBase* CreateServiceL(TUid aServiceType) const;
        IMPORT_C void HandleAllClientsClosed();

    protected:        
        CTestProviderGeocodingService* CreateGeocodingServiceL() const;
        CTestProviderNavigationService* CreateNavigationServiceL() const;
        CTestProviderMapViewService* CreateMapViewServiceL() const;
#ifdef RD_MAPNAV_BITMAP_ACCESS
        CTestProviderMapImageService* CreateMapImageServiceL() const;
#endif        
        void DumpEmptyL() const;
        
        static TInt TestCommandCallback( TAny* aPtr );
        
    private:
        mutable TBool iBusy;
        mutable CTestProviderGeocodingService* iGeocodingService;
        mutable CTestProviderNavigationService* iNavigationService;
        mutable CTestProviderMapViewService* iMapViewService;
#ifdef RD_MAPNAV_BITMAP_ACCESS
        mutable CTestProviderMapImageService* iMapImageService;
#endif        
        CTestCommandListener*   iListener;
        TInt iTestCommand;
    };

#endif // MNTP_APPSERVER_H

