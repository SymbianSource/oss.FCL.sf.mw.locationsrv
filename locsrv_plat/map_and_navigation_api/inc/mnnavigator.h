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
* Description:  CMnNavigator class
*
*/


#ifndef MN_NAVIGATOR_H
#define MN_NAVIGATOR_H

#include <e32base.h>

class CApaServerAppExitMonitor;
class MAknServerAppExitObserver;
class CMnProvider;
class CPosLandmark;
class RMnNavigationServiceClient;

/** @brief Provides navigation features.
 *
 *  To create an instance of the class, use @ref NewL or @ref NewChainedL methods. They
 *  require instance of CMnProvider, defined in Map and Navigation Provider Discovery API.
 *
 *  @since 3.1
 *  @lib mnclientlib.dll
 *  @ingroup MapNavAPI
 */
class CMnNavigator : public CBase
    {
    public :
        /** Destructor */
        IMPORT_C ~CMnNavigator();

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed standalone.
         *
         *  @param[in] aProvider Provider Application to be used for navigation.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      navigation services (CMnProvider::EServiceNavigation).
         */
        IMPORT_C static CMnNavigator* NewL( CMnProvider& aProvider );

        /** @brief Creates new instance of the class. Provider Application
         *  will be executed chained (unless Provider Application defines that
         *  it should be started in background).
         *
         *  @param[in] aProvider Provider Application to be used for navigation.
         *  @return Pointer to new instance of the class.
         *
         *  @leave KErrNotSupported Selected Provider Application does not support
         *      navigation services (CMnProvider::EServiceNavigation).
         */
        IMPORT_C static CMnNavigator* NewChainedL( CMnProvider& aProvider );

        /** @brief Starts navigation from current location to specified destination.
         *
         *  This method completes as soon as navigation request reaches Provider
         *  Application. Navigation is held then in Provider Application's process.
         *
         *  Navigation destination is specified as landmark. This landmark
         *  does not have to contain coordinate information, in which case
         *  its address information will be used, if present. If neither
         *  coordinate nor address information is present in the landmark,
         *  then this method leaves with KErrArgument.
         *
         *  This is synchronous request. It is completed as soon as navigation
         *  starts on Provider Application side.
         *
         *  @param[in] aDestination Navigation destination.
         *
         *  @leave KErrArgument if neither coordinate nor address information is present in the landmark
         */
        IMPORT_C void NavigateToL( const CPosLandmark& aDestination );

        /** @brief Sets observer of provider application lifetime.
         * 	Client will receive exit event from provider application.
         *  By default exit events from provider application are not monitored.
         *  @param[in] aObserver Exit observer.
         *  @leave KErrAlreadyExists if observer is already set. 
         */
        IMPORT_C void SetExitObserverL( MAknServerAppExitObserver& aObserver );

        /** @brief Removes observer of provider application lifetime. */
        IMPORT_C void RemoveExitObserver();

    private :
        /** C++ constructor */
        CMnNavigator();

        void ConstructCommonL( CMnProvider& aProvider );
        void ConstructNewL( CMnProvider& aProvider );
        void ConstructChainedL( CMnProvider& aProvider );

    private :

        RMnNavigationServiceClient* iSession;
        CApaServerAppExitMonitor* iExitMonitor;
    };

#endif // MN_NAVIGATOR_H

