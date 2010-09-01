/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Responsible for launching SUPL session details
*
*/

#ifndef LPDVERIFIERSETTINGSLAUNCHER_H
#define LPDVERIFIERSETTINGSLAUNCHER_H

// INCLUDES
#include <e32base.h>
#include <locsettingsuiclient.h>

// CLASS DECLARATION
/**
*  CLpdVerifierSettingsLauncher class
*  This class is a wrapper for launching Position settings - SUPL session details
*/
class CLpdVerifierSettingsLauncher : public CActive
	{
	private : // Constructors and destructor
	      /**
	       * C++ default constructor.
	       */
	    CLpdVerifierSettingsLauncher();
	      
	public:
	      /**
	       * Destructor.
	       */
	      ~CLpdVerifierSettingsLauncher();		
	
	public:	
		/**
         * Two-phased constructor.
         *
         */
	     static CLpdVerifierSettingsLauncher* NewL();
			      
	private: 
		 /**
          * Second phase of construction.
          */
	      void ConstructL();
	      
	public: // new functions
	      void LaunchL( TInt aSessionId );	      
	      	      
	protected: // from CActive
	      void RunL();
	      
	      void DoCancel();
	      
	      void CancelRequest();
	      
	private:
	    // Pointer to Location setings client library
        CLocSettingsUiClient*       iClientLibrary;
        
        // Launch parameters
        HBufC*       iLaunchParams;
	};
	
#endif // LPDVERIFIERSETTINGSLAUNCHER_H

// End of file