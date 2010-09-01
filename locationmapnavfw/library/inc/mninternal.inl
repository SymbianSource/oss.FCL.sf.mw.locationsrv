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
* Description:  Internal inline methods
*
*/


// -----------------------------------------------------------------------------
//  Used to panic server implementation
// -----------------------------------------------------------------------------
//
inline void PanicServer( TInt aReason )
    {
    User::Panic( KMnPanicServerFault, aReason );
    }

// -----------------------------------------------------------------------------
//  Used to panic client from server side
// -----------------------------------------------------------------------------
//
inline void PanicClient( const RMessage2& aMessage, TInt aReason )
    {
    aMessage.Panic( KMnPanicClientFault, aReason );
    }

// -----------------------------------------------------------------------------
//  Used for internal panic, debugging time only
// -----------------------------------------------------------------------------
//
inline void PanicSelf( TInt aReason )
    {
    User::Panic( KMnPanicInternal, aReason );
    }
