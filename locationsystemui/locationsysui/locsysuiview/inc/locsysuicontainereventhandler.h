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
* Description:  Handles Events from the container 
*
*/


#ifndef M_LOCSYSUICONTAINEREVENTHANDLER_H_
#define M_LOCSYSUICONTAINEREVENTHANDLER_H_

// System Include
#include <e32base.h>

/**
 * Handles events from the Location System UI Container
 *
 * @lib locsysuiview.lib
 * @since S60 v3.1
 */
class MLocSysUiContainerEventHandler
	{
	public:
		/**
		 * Handles the Item selection event from the container
		 * @since S60 v3.1
		 */
	 	 virtual void ListBoxItemSelectedL() 	= 0;
	 	 
	 	/**
		 * Handles the Screen Size update
		 * @since S60 v3.1
		 */
		 virtual void HandleScreenSizeChange() 	= 0; 
	};

#endif // M_LOCSYSUICONTAINEREVENTHANDLER_H_

