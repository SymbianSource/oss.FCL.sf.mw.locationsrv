/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Input Parameters to the Settings UI.
*
*/


#ifndef C_LOCSETTINGSUIPARAMS_H_
#define C_LOCSETTINGSUIPARAMS_H_

// System Include
#include <e32base.h>

// Forward Declarations
class MLocSettingsUiObserver;
class CAknViewAppUi;

// Class Defintions

/**
* Input Parameters to the Settings UI. 
* 
* The parameters are passed through the E-Comm frameworks Create function. 
* The E-Comm framework is not expected to understand the parameters, it 
* passes on the same to the creation funtion of the actual implementation.
* The ownership of the object is vested with the class that calls the 
* creation function
*
* @lib locsysuiview.lib
* @since S60 v3.1
*/
class CLocSettingsUiParams : public CBase
	{				   
	public:
        /**
         * Creates the Settings Parameters. 
         *
         * @param aObserver			  Settings UI Observer 	
         * @param aAppUi			  Pointer to the Application UI class that 
         *							  launched the Settings Application
         */
        inline static CLocSettingsUiParams* NewL( MLocSettingsUiObserver&	aObserver,
        							 		      CAknViewAppUi&			aAppUi );
        /**
         * Creates the Settings Parameters. 
         *
         * @param aObserver			  Settings UI Observer 	
         * @param aAppUi			  Pointer to the Application UI class that 
         *							  launched the Settings Application
         */
        inline static CLocSettingsUiParams* NewLC( MLocSettingsUiObserver&	aObserver,
        							  		       CAknViewAppUi&			aAppUi );

        /**
         * Destructor
         */
        inline virtual ~CLocSettingsUiParams();
        
        /**
         * SettingsUIObserver Accessor function
         *
         * @return MLocSettingsUiObserver&	Reference to the Settings UI Observer
         */
        inline MLocSettingsUiObserver&	SettingsUiObserver();

        /**
         * CAknViewAppUi Accessor function
         *
         * @return CAknViewAppUi&	Reference to the AppUi Object
         */
        inline CAknViewAppUi&	AppUi();

	private:
	    /** 
         * Overloaded Constructor
         *
         * @param aObserver		  Settings UI Observer 	
         * @param aAppUi			  Pointer to the Application UI class that 
         *					      launched the Settings Application
         */
        inline CLocSettingsUiParams( MLocSettingsUiObserver&	aObserver,
        			    	         CAknViewAppUi&		        aAppUi );
        			    	         
        /**
         * Second Phase construction
         */
        inline void ConstructL();

	private:
	    /**
         * Observer for the Settings UI Class
         */	 
		MLocSettingsUiObserver&		iSettingsUiObserver;
		
		/**
         * Reference to the Application Ui to which the Settings UI would belong to
         */
		CAknViewAppUi&				iAppUi;
	};

#include "locsettingsuiparams.inl"

#endif // C_LOCSETTINGSUIPARAMS_H_
 
