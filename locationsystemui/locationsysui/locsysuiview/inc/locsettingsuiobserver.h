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
* Description:  Observer SettingsUI interface class. Provides callback to notify
*				 the termination of the Settings UI application
*
*/


#ifndef M_LOCSETTINGSUIOBSERVER_H_
#define M_LOCSETTINGSUIOBSERVER_H_

// System Include 
#include <e32base.h>

// Class Definition
/**
 * Observer class that provides notification from the Settings UI.
 *
 * @lib locsysuiengine.lib
 * @since S60 v3.1
 */
class MLocSettingsUiObserver
	{
	public:
        /**
         * Notifies the termination of the Settings UI
         *
         * @since S60 3.1
         * @param aErrorCode The termination reason. KErrNone for normal 
         *					 terminations. In case of error or pre-mature
         *					 aborting System wide Error codes.
         */
        virtual void SettingClosed( TInt aErrorCode )	= 0;
	};

#endif //M_LOCSETTINGSUIOBSERVER_H_

