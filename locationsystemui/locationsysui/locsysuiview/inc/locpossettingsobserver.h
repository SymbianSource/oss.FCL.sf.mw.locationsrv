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
* Description:  Observer to Postioning Settings UI.
*
*/


#ifndef M_LOCPOSSETTINGS_H_
#define M_LOCPOSSETTINGS_H_

// System Include
#include <e32base.h>

// User Includes

// Forward Declarations

// Class Definition
/**
 * Observer to Postioning Settings UI.
 * This class provides notification regarding the termination of Positioning 
 * settibgs UI.
 * @lib locsysuiview.lib
 * @since S60 v3.2
 */
class MLocPosSettingsObserver
	{
		/**
		 * Notifies the termination of the Psotioning Settings UI
		 *
		 * @since S60 v3.2
		 * @param aErrorCode The termination reason. KErrNone for normal 
		 *					 terminations. In case of error or pre-mature
		 *					 aborting System wide Error codes.
		 */
		 virtual void PositioningSettingClosed( TInt aErrorCode )			= 0;	    
	};

#endif // M_LOCPOSSETTINGS_H_

