/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Location Notation Preferences Plugin Co-ordinate display adapter
*
*/

#ifndef C_LOCNOTPREFCOORDINATEDISPLAYADAPTER_H_
#define C_LOCNOTPREFCOORDINATEDISPLAYADAPTER_H_

//  System Includes
#include <e32base.h>

// User Includes

// Forward Declarations
class MLocNotPrefAdapterObserver;
class CRepository;

// Constant Declarations

// Class Definition

/**
 * Adapter to the Co-ordinate Display settings storage
 */
class CLocNotPrefCoordinateDisplayAdapter : public CActive
    {
    public:
        /**
         * Enumeration for the positon of the Co-ordinate display values in display order
         */         
        enum TCordDisSetListPosition
            {
            /**
             * Enumeration to denote Location information is to be displayed 
             * at postion 0
             */
            EPosDMSS = 0x00000000,
            
            /**
             * Enumeration to denote Location information is to be displayed
             * at postion 1
             */             
            EPosDMM,
            
            /**
             * Enumeration to denote Location information is to be displayed
             * at postion 2
             */ 
            EPosDD             
            };            
    public:
        /**
         * Creates new Co-ordinate display adapter
         *
         * @param  aAdapterObsrv                        Observer to the Settings adapter
         * @return CLocNotPrefCoordinateDisplayAdapter& Reference to the application view
         */                                                 
        static CLocNotPrefCoordinateDisplayAdapter* NewL( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv );


        /**
         * Creates new Co-ordinate display adapter
         * Leaves the object on the Clean up stack         
         *
         * @param  aAdapterObsrv                        Observer to the Settings adapter
         * @return CLocNotPrefCoordinateDisplayAdapter& Reference to the application view
         */                                                 
        static CLocNotPrefCoordinateDisplayAdapter* NewLC( 
                            MLocNotPrefAdapterObserver& aAdapterObsrv );    

        
        /**
         * Destructor
         */
        ~CLocNotPrefCoordinateDisplayAdapter ();
        
        /**
         * Returns the position of the Co-ordinate display format settings value.
         *
         * @return TCordDisSetListPosition Current postion of the Co-ordinate display
         *                                 settings value.
         */
        TCordDisSetListPosition GetCurrentDisplayFormatL();
        
        /**
         * Sets the value of the Co-ordinate display format settings based on the 
         * display position.
         *
         * @param aSettingsPos  Position of the new Co-ordinate display settings value.
         */
        void SetCurrentDisplayFormatL( TCordDisSetListPosition aSettingsPos );

    protected:
        /**
         * Inherited from CActive
         */
        void RunL();
        
        /**
         * Inherited from CActive
         */
        void DoCancel();
                    
    private:
        /**
         * Constructor
         */
        CLocNotPrefCoordinateDisplayAdapter( MLocNotPrefAdapterObserver&    aAdapterObsrv );

        /**
         * Second Phase Constructor
         */
        void ConstructL();
        
        /**
         * Start notification from the Central repository server for any changes in the
         * Co-ordinate display format key value
         */
        void StartNotification();
        
    private:
        /**
         * Enumeration for Co-ordinate display values
         */
        enum TCoordinateDisplayValue
            {
            /**
             * Enumeration to denote Location information is to be displayed in
             * Degrees and Decimal Degrees
             */ 
            EDD = 0x00000000,
            /**
             * Enumeration to denote Location information is to be displayed in
             * Degrees, Minutes and Decimal Minutes
             */             
            EDMM,
            /**
             * Enumeration to denote Location information is to be displayed in
             * Degrees, Minutes, Seconds and Decimal Seconds
             */             
            EDMSS
            };
                
        /**
         * Reference to the Adapter observer
         */
        MLocNotPrefAdapterObserver&     iObserver;
        
        /**
         * Reference to the Central repository object
         *
         * Owns
         */
        CRepository*                    iRepository; 
    };          

#endif // C_LOCNOTPREFCOORDINATEDISPLAYADAPTER_H_

