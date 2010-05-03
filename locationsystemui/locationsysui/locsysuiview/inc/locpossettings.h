/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Positioning Settings implementation of Location UIs. 
*
*/


#ifndef C_LOCPOSSETTINGS_H_
#define C_LOCPOSSETTINGS_H_

// System Include
#include <e32base.h>

// User Includes
#include "locationui.h"

// Forward Declarations
class CLocSysUiView;
 
// Class Definition
/**
 * Positioning Settings implementation of Location UIs. This class extends the 
 * Location UI interface for Positioning Settings UI.
 *
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
class CLocPosSettings : public CBase,
                        public MLocationUI
	{
	public:	
        /**
         * Two Phase Constructor.
         *
         * @return	CLocPosSettings*	Pointer to the created object.
         */
        IMPORT_C static CLocPosSettings* NewL();
       
        /**
         * Two Phase Constructor. The funcion leaves the CLocationSettings object
         * on the CleanupStack
         *
         * @return	CLocPosSettings*	Pointer to the created object.
         */         
        IMPORT_C static CLocPosSettings* NewLC();
        
        /**
         * C++ Destructor
         */
        virtual ~CLocPosSettings();
       
    public:
    
        /**
         * Inherited from MLocationUI
         *
         */
        void LaunchLocationUIL( TInt		            aUiInputParams,
                                MLocationUIObserver*    aObserver );
    
        /**
         * Inherited from MLocationUI
         *
         */
        void LaunchLocationUIL( const TDesC&                    aUiInputParams,
                                MLocationUIObserver*    aObserver );

        /**
         * Inherited from MLocationUI
         *
         */
        void Close();
            
    private:
        /**
         * Overloaded constructor
         */         
        CLocPosSettings();
        
        /**
         * Second phase of two phase construction
         */
        void ConstructL();        
                
    private:	
	    /**
	     * Postioning Settings View. The ownership lies with the App UI
	     */
	    CLocSysUiView*          iView;
	};

#endif // C_LOCPOSSETTINGS_H_

