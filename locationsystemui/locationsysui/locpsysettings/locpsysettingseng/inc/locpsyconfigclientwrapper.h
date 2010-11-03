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
* Description: 
 *         psy configuration ui server app wrapper implementation.
 *
*/

#ifndef LOCPSYCONFIGCLIENTWRAPPER_H_
#define LOCPSYCONFIGCLIENTWRAPPER_H_

#include <AknServerApp.h>

/**
 * This class provides the place holder for the structural information of the
 * of the Configuration UI application.
 */
class CLocPsyConfigUiReg : public CBase
	{
	public:
	    /**
	     * Two phase constructor
	     *
	     * @param  aApp                  Application UID
	     * @param  aResource             Resource ID
	     * @return CLocPsyConfigUiReg*   Reference to the object created
	     */
		static CLocPsyConfigUiReg* NewL(       TUid     aApp,
		                                 const TDesC8&  aResource );
		                                 
        /**
         * Accessor function for the PSY UID
         * 
         * @return TInt     PSY UID
         */ 		                                 
		TInt RegId();
		
		/**
		 * Accessor function for the Application UID
		 *
		 * @return TUid     Application UID
		 */
		TUid AppUid();

	private:
	    /**
	     * C++ Default Constructor
	     */
		CLocPsyConfigUiReg();
		
		/**
		 * Second phase constructor
	     * @param  aApp                  Application UID
	     * @param  aResource             Resource ID		 
		 */
		void ConstructL(       TUid    aApp,
		                 const TDesC8& aResource );

	private:
	    /**
	     * Application UID
	     */
		TUid iAppUid;
		
		/**
		 * PSY UID
		 */
		TInt iRegId;
	};

/**
 * This class provides the discovery mechanism to discover the PSY
 * Configuration UI corresponding to the registration information
 */
class CLocPsyConfigUiServiceClient : public CBase
	{
	public:
	    /**
	     * Two phase constructor
	     *
	     * @param  aReg                             Application registration info
	     * @param  aObserver                        Observer to notify the termination
	     *                                          of the application
	     * @return CLocPsyConfigUiServiceClient*    Reference to the object created
	     */
		static CLocPsyConfigUiServiceClient* NewLC(
			                              CLocPsyConfigUiReg&        aReg, 
			                              MAknServerAppExitObserver& aObserver );
	public:
	    /**
	     * Static Discovery mechanism
	     * 
	     * @param  TInt                 PSY UID
	     * @return CLocPsyConfigUiReg*  Registration Info
	     */
		static CLocPsyConfigUiReg* DiscoverLC( const TInt PsyUid );
	};

#endif // LOCPSYCONFIGCLIENTWRAPPER_H_
