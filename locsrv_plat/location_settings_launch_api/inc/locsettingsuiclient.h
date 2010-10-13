/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client Side handle for using the Location System UI services
*
*/


#ifndef C_LOCSETTINGSUICLIENT_H
#define C_LOCSETTINGSUICLIENT_H

// System Includes
#include <e32base.h>

// Forward Declarations
class RLocSettingsUiService;
class CLocLaunchUiAO;
class CEikonEnv;

// Global Constants

// UIDs for Launching the Location Based Settings UIs through the Launch
// API. The UIDs of all the Settings UI plug-ins that plug into this framework
// need to be consolidated here.

/**
 * UID for launching Position Method Settings 
 */
const TInt  KLocPsySettingsUID  = 0x10275062;

/**
 * UID for launching SUPL Settings UI
 */
const TInt  KLocSUPLSettingsUID = 0x10275091;

/**
 * UID for launching Location Notation Prefences Settings UI
 */
const TInt  KLocNotPrefSettingsUID = 0x1020690F;

// Parameters for Individual Location Based Settings UIs. All parameter 
// enumerations of all Settings UI plug-ins that plug into this framwork
// need to be consolidated here

/**
 * Default Value for all paramter enumerations. This should be the first 
 * enumeration value for all the parameter enumerations that we define
 */
const TInt  KDefaultParamValue  = 0x00000000;

/**
 * SUPL Settings UI parameter enumerations
 */
enum TLocSUPLSettingsParams
    {
    ELocSUPLDefault         = KDefaultParamValue,   // Default Settings View
    ELocSUPLIAPSelection    = 0x00000001,           // IAP Selection dialog
    ELocSUPLStateSelection  = 0x00000002,           // State enable dialog 
    ELocSuplSettingsPage    = 0x00000010,           // SUPL settings page
    ELocSuplIAPDialog       = 0x00000020            // SUPL IAP dialog
    };
    
 
/**
 *  Client side resource class to launch Location settings UI.
 *
 *  Client side interface class used by all clients to
 *  launch Location settings UIs. For UI applications the settings UIs 
 *  can be launched as embedded application. Frameworks can launch the
 *  settings UIs in a separate UI server.
 *
 *  The client application / framework can also cancel the launched
 *  settings UI by calling the Cancel method.
 *
 *  @lib locsettingsuiclient.lib
 *  @since S60 v3.1
 */
class CLocSettingsUiClient : public CBase
    {

public:
    /**
     * Two Phase constructor 
     * 
     * @since S60 v3.1
     * @return The reference of the newly allocated CLocSettingsUiClient 
     *         object
     */
    IMPORT_C static CLocSettingsUiClient* NewL();

    /**
     * Two Phase constructor. Leaves the allocated object on the Clean-up
     * stack 
     * 
     * @since S60 v3.1
     * @return The reference of the newly allocated CLocSettingsUiClient 
     *         object
     */
    IMPORT_C static CLocSettingsUiClient* NewLC();

    /**
     * C++ Destructor
     * 
     * @since S60 v3.1
     */
    virtual ~CLocSettingsUiClient();

public: // Exported functions related to the handling of Location sub-setting UI.
    /**
     * Launch a Settings UI in an ui app server. 
     * The API is used to launch a settings UI. It is expected to be used 
     * by Framework executables that do not have a UI environment.
     * There can be only one outstanding Launch request at any instant of 
     * time. This is an asynchronous function. If a session is already 
     * running then the function leaves with KErrInUse.
     *
     * @since S60 v3.1
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParams            Opaque parameters that is handed over to
     *                           the Settings UI. Flags that define 
     *                           specific configuration of the Settings UI,
     *                           that the user of the API wants to launch. 
     *                           The interpretation of these flag values is a
     *                           part of the understanding between the API 
     *                           user and the Settings UI
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    IMPORT_C void LaunchSettingsUiL( TUid            aImplementationUid, 
                                     TInt            aParams,
                                     TRequestStatus& aStatus );
    
    /**
     * Launch a Settings UI as an embedded application.
     * The API is used to launch a settings UI as an embeded. This is
     * expected to be used by UI applications. There can be only 
     * one outstanding Launch request at any instant of time. This is
     * an asynchronous function.
     *
     * @since S60 v3.1
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParams            Opaque parameters that is handed over to
     *                           the Settings UI. Flags that define 
     *                           specific configuration of the Settings UI,
     *                           that the user of the API wants to launch.
     *                           The interpretation of these flag values is a
     *                           part of the understanding between the API 
     *                           user and the Settings UI
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    IMPORT_C void LaunchSettingsUiAsEmbeddedAppL( 
                                    TUid            aImplementationUid, 
                                    TInt            aParams,
                                    TRequestStatus& aStatus );    
    /**
     * Launch a Settings UI in an ui app server. 
     * The API is used to launch a settings UI. It is expected to be used 
     * by Framework executables that do not have a UI environment.
     * There can be only one outstanding Launch request at any instant of 
     * time. This is an asynchronous function. If a session is already 
     * running then the function leaves with KErrInUse.
     *
     * @since S60 v9.2
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParamsString      Opaque parameters that is handed over to
     *                           the Settings UI. This allows the API user  
     *                           to pass buffer to the Settings UI.
     *                           The interpretation of this buffer is the
     *                           part of the understanding between the API 
     *                           user and the Settings UI
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    IMPORT_C void LaunchSettingsUiL( TUid            aImplementationUid, 
                                     const TDesC&            aParamsString,
                                     TRequestStatus& aStatus );
    
    /**
     * Launch a Settings UI as an embedded application.
     * The API is used to launch a settings UI as an embeded. This is
     * expected to be used by UI applications. There can be only 
     * one outstanding Launch request at any instant of time. This is
     * an asynchronous function.
     *
     * @since S60 v9.2
     * @param aImplementationUid The ECOM Implementation UID of the 
     *                           settings UI ECOM plug-in that has to be 
     *                           launched.
     * @param aParamsString      Opaque parameters that is handed over to
     *                           the Settings UI. This allows the API user  
     *                           to pass buffer to the Settings UI.
     *                           The interpretation of this buffer is the
     *                           part of the understanding between the API 
     *                           user and the Settings UI
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.
     *
     */
    IMPORT_C void LaunchSettingsUiAsEmbeddedAppL( 
                                    TUid            aImplementationUid, 
                                    const TDesC&            aParamsString,
                                    TRequestStatus& aStatus );    
    
    /**
     * Cancels an already launched settings UI.
     *
     * @since S60 v3.1
     * @return a Symbian OS Error Code in case a settings UI was not launched.
     * Returns KErrNone if successful. 
     */
    IMPORT_C TInt CancelLaunchedSettingsUi();

public: // Exported functions pertaining to the handling of Postioning Settings

    /**
     * Launch Positioning Settings UI.
     * The API is used to launch Positioning Settings UI. It is expected to 
     * be used by Framework executables that do not have a UI environment.
     * There can be only one outstanding Launch request at any instant of 
     * time. This is an asynchronous function. If a session is already 
     * running then the function leaves with KErrInUse.
     *
     * @since S60 v3.2
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.     
     *
     */
    IMPORT_C void LaunchPosSettingsL( TRequestStatus& aStatus );
    
    /**
     * Launch Positioning Settings UI as an embedded application.
     * The API is used to launch Positioning Settings UI. his is
     * expected to be used by UI applications. There can be only 
     * one outstanding Launch request at any instant of time. This is
     * an asynchronous function. If a session is already 
     * running then the function leaves with KErrInUse.
     *
     * @since S60 v3.2
     * @param aStatus            Request status. On completion contains: 
     *                           KErrNone, if successful, otherwise one of
     *                           the other system-wide error codes.     
     *
     */    
    IMPORT_C void LaunchPosSettingsAsEmbeddedAppL(  TRequestStatus& aStatus );
 
    /**
     * Closes an already launched Positioning Settings UI.
     *
     * @since S60 v3.2
     * @return Symbian OS Error Code in case Positioning settings UI
     *         was not launched.
     * Returns KErrNone if successful. 
     */   
    IMPORT_C TInt ClosePosSettings();
    
private:
    /**
     * C++ Default Constructor
     */
    CLocSettingsUiClient();

    /**
     * Second phase of Two phase construction
     */
    void ConstructL();

private:

    /**
     * Handle to the Client side resource to the Settings UI Server
     * Owns
     */
    RLocSettingsUiService*          iLocService;
     
    /**
     * Active Object to initiate asynchronous requests
     * Owns
     */
    CLocLaunchUiAO*                 iLaunchUiAO;
     
    /**
     * Reference to the UI environment from where this client is launched.
     * It will be NULL if the Client Application is launched from frameworks
     */
    CEikonEnv*                        iCEikonEnv; 
     
    /** Unused variable for future expansion. */
    TAny* iReserved;

    };

#endif // C_LOCSETTINGSUICLIENT_H
