/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface which allows subscribers to consume key event or 
*               pass the event to other view class.
*       
*
*/


#ifndef MLOCPSYKEYPROCESSOR_H
#define MLOCPSYKEYPROCESSOR_H

// INCLUDES
#include <w32std.h>

// CLASS DECLARATION

/**
*	An interface a CCoeControl class can implement to forward the keyevents
*   before the actual control gets the key events. This is useful in
*   situations where the control consumes keys which you need.
*/
class MLocPsyKeyProcessor
    {
    public:
     /**
     * process the keyevent befor it reaches the control
     * @param aKeyEvent the event fired
     * @param aType the type of event
     * @return ETrue if key event was consumed, otherwise EFalse
     */
    virtual TBool ProcessKeyEventL( const TKeyEvent& aKeyEvent,
                                    TEventCode aType ) = 0;
    };

#endif // MLOCPSYKEYPROCESSOR_H

// End of File
