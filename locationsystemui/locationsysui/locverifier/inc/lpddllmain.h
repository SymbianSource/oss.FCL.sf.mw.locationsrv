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
* Description:  Dll entry point and plugin creation functions.
*
*/


#ifndef CLPDDLLMAIN_H
#define CLPDDLLMAIN_H

// INCLUDES
#include <e32base.h>
#include <eiknotapi.h>


// FUNCTION PROTOTYPES
IMPORT_C CArrayPtr<MEikSrvNotifierBase2>* NotifierArray();

#endif // CLPDDLLMAIN_H

// End of File
