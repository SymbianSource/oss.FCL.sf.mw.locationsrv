/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides Location PSY configuration UI class.
*
*/


#ifndef CLOCPSYCONFIGCLIENTIMPL_H_
#define CLOCPSYCONFIGCLIENTIMPL_H_

#include <AknLaunchAppService.h>
#include <AknServerApp.h>

#include "locpsyconfigclientwrapper.h"

/**
* This class provides the implementation to launch the PSY COnfiguration UI as
* an embedded application. It uses the Symbian & Series60 AppArc Framework to
* launch the application.
*/
class CLocPsyConfigUiServiceClientImpl : public CLocPsyConfigUiServiceClient
	{
	public:
	    /**
	     * C++ Overloaded Constuctor
	     *
	     * @param aReg      Configuration UI registration information
	     * @param aObserver Observer to notify the termination of the
	     *                  config UI service
	     */
	    CLocPsyConfigUiServiceClientImpl( CLocPsyConfigUiReg&        aReg, 
	                                      MAknServerAppExitObserver& aObserver );
	    
	    /**
	     * Destructor
	     */
		~CLocPsyConfigUiServiceClientImpl();
		
		/**
		 * Create the Application launcher
		 */
		void StartServerL();
		
		/**
		 * Close the application launcher
		 *
		 * @param reason Reason for closing the application
		 */
		void CloseServer( TInt reason );
		
		/**
		 * Accessor function for the PSY ID
		 * 
		 * @return TInt     PSY ID
		 */
		TInt PsyId();
		
		/**
		 * Accessor function for the Application UID
		 *
		 * @return TUid     Application UID
		 */
		TUid AppUid();
		
	private:
	    /**
	     * Application launher
	     * Owns
	     */
		CAknLaunchAppService*           iLaunchAppService;
		
		/**
		 * Observer to notify the exit of the application
		 * Reference
		 */
		MAknServerAppExitObserver*      iObserver;
		
		/**
		 * Configuration UI registration information
		 * Reference
		 */
		CLocPsyConfigUiReg&             iAppRegInfo;
	};

#endif // CLOCPSYCONFIGCLIENTIMPL_H_
