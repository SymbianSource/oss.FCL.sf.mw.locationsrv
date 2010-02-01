/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnNavigationServiceBase class
*
*/


#ifndef MN_NAVIGATIONSERVICEBASE_H
#define MN_NAVIGATIONSERVICEBASE_H

#include "mnservicebase.h"

class CPosLandmark;

/** @brief Base class for navigation service implementations (KMnAppNavigationService). 
 * 
 *  Provider Application must derive its navigation service 
 *  class from this one. This base implementation
 *  hides IPC communication details.
 *
 *  Provider Application must return pointer to instance of its class, derived
 *  from this class, in response to CAknAppService::CreateServiceL() call,
 *  if given parameter is KMnAppNavigationService. 
 *
 *  This class defines navigation request handler as pure 
 *  virtual method, which derived class must implement. 
 *  Implementation must obey specification of @ref CMnNavigator.
 *
 *  See parent @ref CMnServiceBase class documentation for 
 *  other details.
 *
 *  @since 3.1
 *  @lib mnservicelib.lib
 *  @ingroup MnProviderAPI
 */
class CMnNavigationServiceBase : public CMnServiceBase
    {
    public :
    protected :
        /** C++ constructor */
        IMPORT_C CMnNavigationServiceBase();
        /** Destructor */
        IMPORT_C ~CMnNavigationServiceBase();

        /** This method must be called from derived class' ConstructL to
         *  allow internal initialization of this class. */
        IMPORT_C void BaseConstructL();

        /** @brief Handles client's request for navigation. 
         *
         *  Derived class must implement this method obeying specification of 
         *  @ref CMnNavigator::NavigateToL() and if KMnSvcFeatureNavigateTo
         *  feature is specified in SERVICE_INFO.
         *
         *  This is synchronous request.
         *  Client's request will be completed when this method exits, therefore
         *  it should exit immediately after navigation is started.
         *
         *  @param[in] aDestination The landmark, defines destination of the
         *                      navigation requested.
         *  
         *  @leave KErrNotSupported Should leave if this feature is not supported.
         */
        virtual void HandleNavigateToL(const CPosLandmark& aDestination) =0;

    protected: // From CAknAppServiceBase

        /** \internal */
        IMPORT_C virtual void ServiceL(const RMessage2& aMessage);
    };

#endif // MN_NAVIGATIONSERVICEBASE_H

