/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:     Class definition for Time Manager 
*
*/






#ifndef C_LBTCIDCHANGENOTIFIER_H
#define C_LBTCIDCHANGENOTIFIER_H


#include <e32base.h>
#include <flogger.h>
#include <Etel3rdParty.h>
#include <etelmm.h>

class MLbtCidChangeObsrvr
    {
public:
    virtual void HandleCIDChangeL(const CTelephony::TNetworkInfoV1 &aNwInfo) = 0;
    };

/**
 * This is a helper class that is utilized by strategy engine to initiate 
 * requests for notification once a designated interval of time has elapsed.
 * This class also provides the interface to determine elapsed time 
 * (in microseconds). 
 */
NONSHARABLE_CLASS( CLbtCidChangeNotifier ) : public CActive
    {
public:

    /**
     * Factory function that instantiates an object of CLbtStrategyTimeManager
     */
    static CLbtCidChangeNotifier* NewL(RFileLogger &aLog, MLbtCidChangeObsrvr *aObs);
    
    /**
     * Destructor
     */
    ~CLbtCidChangeNotifier();
    
public:

    /**
     * Initiates a request for notification after the specified time interval
     * has elapsed
     */
     void Start( );
     
     /**
      * Cancels an outstanding request for notifications
      */
     void DoCancel();
     
private:

    /**
     * Default constructor
     */
    CLbtCidChangeNotifier(RFileLogger &aLog, MLbtCidChangeObsrvr *aObs);
    
    /**
     * 2nd phase constuctor for instantiating member variables
     */
    void ConstructL();
    
    void RunL();
private:

    CTelephony::TNetworkInfoV1Pckg iNwInfoPckg;
    CTelephony::TNetworkInfoV1 iNwInfo;
    
    CTelephony  *iTelephony;
     
     RFileLogger &iLog;
     
     MLbtCidChangeObsrvr *iObs;
     
    };

#endif  // C_LBTSTRATEGYTIMER_H
