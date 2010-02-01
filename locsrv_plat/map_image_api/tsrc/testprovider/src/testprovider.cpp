/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Entry Point for map and navigation test provider
*
*/


#include <e32base.h>
#include <eikstart.h>

#include "debug.h"

#include "testproviderapplication.h"


LOCAL_C CApaApplication* NewApplication()
{
    LOG( "NewApplication" );
    return new CTestProviderApplication;
}

TInt E32Main()
{
    return EikStart::RunApplication( NewApplication );
}

