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
* Description:  Callback interface class.
*
*/


#ifndef MLPDBKUPEVTOBSERVER_H
#define MLPDBKUPEVTOBSERVER_H

// INCLUDES
#include <e32def.h>
#include <eiklbv.h>


// CLASS DECLARATION

/**
*  Callback interface class.
*  Implementor of this class gives a reference to itself when
*  creating a global query dialog. A callback method is then
*  called when the query is dismissed.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class MLpdBkupEvtObserver
    {
    public:

        /**
        * Pure virtual callback method.
        */
        virtual void ExitDialogL() = 0;
    };

#endif  // MLPDBKUPEVTOBSERVER_H

// End of File
