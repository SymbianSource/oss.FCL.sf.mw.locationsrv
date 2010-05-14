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
* Description:  Declaration of settings api classes
*
*/



#ifndef SUPLSERVER_H
#define SUPLSERVER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <epos_suplterminal.h>
#include <epos_CSuplSettings.h>
#include <epos_MSuplSettingsObserver.h>
#include <epos_csuplsettingparams.h>

//#include <epos_csuplsettingsengine.h>

class CObserverTest;
class MSuplSettingsObserver;
class CServerParamValues;
class CSuplSettingsInternal;
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( KsuplserverLogPath, "c:\\logs\\testframework\\suplsettings\\" ); 
// Log file
_LIT( KsuplserverLogFile, "suplsettings.txt" ); 

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CSettingsApi;
class CTerminalInitiatedRunSession;
class MSettingsObserver
{
	public:
	virtual void CompleteInitialize(TInt err)=0;
};
// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CSettingsApi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CSettingsApi) : public CScriptBase, public MSuplSettingsObserver, public MSettingsObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSettingsApi* NewL( CTestModuleIf& aTestModuleIf );
        
        /**
        * Destructor.
        */
        virtual ~CSettingsApi();

    public: // New functions
        
        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
      
		//	void 	HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TBool aDBNotifier = EFalse);
			void HandleSuplSettingsChangeL(TSuplSettingsEventType aEvent,TInt aSlpId = -1 );
                       
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        void GetImsiTest(CSuplSettings*);
       
        void CompleteInitialize(TInt err);
                
    protected:  // New functions
        
        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes
        
        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CSettingsApi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );
    
        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();
        
        /**
        * Test methods are listed below. 
        */
        
        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        //TInt TestConnect( CStifItemParser& aItem );
       //priya
        TInt InitializeImsi();
        
        TInt ListenForChanges();
        TInt CancelInitializeTest();
        TInt GetHslpFromImsi();
        TInt IsImsiChangedTest();
        TInt RemoveObserver();
        TInt GetImsiWithoutInitialize();
        TInt RemoveBeforeSetObserver();
        TInt IsImsiChangedWithoutInitialize();
        TInt GenerateHslpFromImsiWithoutInitialize();
        TInt GenerateHslpWithoutInitialize();
			  TInt SetUsageAutomatic();
        TInt SetUsageAlwaysAsk();
        TInt SetUsageAutoInHMNw();
        TInt SetUsageDisabled();
        TInt GetUsageAutomatic();
        TInt GetUsageAlwaysAsk();
        TInt GetUsageAutoInHMNw();
        TInt GetUsageDisabled();
      	TInt SetUsageOtherThanAppropriate();
        TInt RemoveServer(CStifItemParser& aItem);
        TInt RemoveServer_Invalid_negative(CStifItemParser& aItem);
        TInt RemoveServer_Id_NotExisting(CStifItemParser& aItem);
        TInt RemoveServer_Invalid_zero(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_Increase(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_Decrease(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_invalid_positive(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_priority0(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_Negative(CStifItemParser& aItem);
        TInt CSettingsApi::ChangePriority_InvalidServer(CStifItemParser& aItem);
      	TInt CSettingsApi::ChangePriority_invalidserverpriority(CStifItemParser& aItem);
        TInt CSettingsApi::GetDefaultServer();
				TInt CSettingsApi::AddNewServerWithDefault(CStifItemParser& aItem);
				TInt CSettingsApi::AddNewServerWithoutDefault(CStifItemParser& aItem);
				TInt CSettingsApi::AddNewServer_Duplicate(CStifItemParser& aItem);
				TInt CSettingsApi::GetSlpInfoFromId(TInt64 Slpid);
				TInt CSettingsApi::GetSlpInfoFromId(CStifItemParser& aItem);
				TInt CSettingsApi::GetSlpInfoFromId_invalid_positive(CStifItemParser& aItem);
				TInt CSettingsApi::GetSlpInfoFromId_invalid_negative(CStifItemParser& aItem);
				TInt CSettingsApi::GetSlpInfoFromId_invalid_zero(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress_invalidpositive(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress_invalidnegative(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress_invalidzero(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress_AddressNull(CStifItemParser& aItem);
				TInt CSettingsApi::SetServerAddress_InvalidAddressId(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerAddress(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerAddress_invalidpositive(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerAddress_invalidnegative(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerAddress_invalidzero(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerAddress(TInt64 aSlpId1);
				TInt CSettingsApi::SetIAPName_InvalidAddressId(CStifItemParser& aItem);
				TInt CSettingsApi::SetIAPName_invalidzero(CStifItemParser& aItem);
				TInt CSettingsApi::SetIAPName_invalidnegative(CStifItemParser& aItem);
				TInt CSettingsApi::SetIAPName_invalidpositive(CStifItemParser& aItem);
				TInt CSettingsApi::SetIAPName(CStifItemParser& aItem);
				 TInt CSettingsApi::GetIAPName_invalidzero(CStifItemParser& aItem);
				 TInt CSettingsApi::GetIAPName_invalidnegative(CStifItemParser& aItem);
				 TInt CSettingsApi::GetIAPName_invalidpositive(CStifItemParser& aItem);
				 TInt CSettingsApi::GetIAPName(CStifItemParser& aItem);
				 TInt CSettingsApi::GetIAPName(TInt64 aSlpId1);
				 TInt CSettingsApi::SetServerEnabledFlag_Etrue(CStifItemParser& aItem);
				 TInt CSettingsApi::SetServerEnabledFlag_EFalse(CStifItemParser& aItem);
				 TInt CSettingsApi::SetServerEnabledFlag_id_negative(CStifItemParser& aItem);
				 TInt CSettingsApi::SetServerEnabledFlag_id_positive(CStifItemParser& aItem);
				  TInt CSettingsApi::SetServerEnabledFlag_id_zero(CStifItemParser& aItem);
				 TInt CSettingsApi::GetServerEnabledFlag_id_negative(CStifItemParser& aItem);
				  TInt CSettingsApi::GetServerEnabledFlag_id_positive(CStifItemParser& aItem);
				  TInt CSettingsApi::GetServerEnabledFlag_id_zero(CStifItemParser& aItem);
				  TInt CSettingsApi::GetServerEnabledFlag_EFalse(CStifItemParser& aItem);
				  TInt CSettingsApi::GetServerEnabledFlag_Etrue(CStifItemParser& aItem);
				  TInt CSettingsApi::GetUsageInHomeNwFlag(TInt64 aSlpId1);
				  TInt CSettingsApi::SetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem);
				  TInt CSettingsApi::SetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem);
				  TInt CSettingsApi::SetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem);
				  TInt CSettingsApi::SetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem);
				   TInt CSettingsApi::SetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem);
					 TInt CSettingsApi::GetUsageInHomwNwFlag_Etrue(CStifItemParser& aItem);
				   TInt CSettingsApi::GetUsageInHomwNwFlag_EFalse(CStifItemParser& aItem);
				   TInt CSettingsApi::GetUsageInHomwNwFlag_id_negative(CStifItemParser& aItem);
				   TInt CSettingsApi::GetUsageInHomwNwFlag_id_positive(CStifItemParser& aItem);
				   TInt CSettingsApi::GetUsageInHomwNwFlag_id_zero(CStifItemParser& aItem);
				   TInt CSettingsApi::GetUsageInHomwNwFlag(TInt64 aSlpId1);
				 TInt CSettingsApi::AddNewServer_aServerAddress_Null(CStifItemParser& aItem);
				  TInt CSettingsApi::AddNewServer_aIapNAme_Null(CStifItemParser& aItem);
				 TInt CSettingsApi::SetDefaultServer_aServerAddress_Null(CStifItemParser& aItem);
	 TInt CSettingsApi::SetDefaultServerWithDefaultValues(CStifItemParser& aItem);
				 TInt CSettingsApi::SetAllParameter_aServerAddress_Null(CStifItemParser& aItem);
  TInt CSettingsApi::SetAllParameter_WithDefaultValues(CStifItemParser& aItem);
				 TInt CSettingsApi::SetAllParameter_Invalid_Positive(CStifItemParser& aItem);
				 TInt CSettingsApi::SetAllParameter_Invalid_Negative(CStifItemParser& aItem);
				 TInt CSettingsApi::SetAllParameter_Invalid_Zero(CStifItemParser& aItem);
				TInt CSettingsApi::GetServerEnabledFlag(TInt64 aSlpId1);
				TInt CSettingsApi::SetEditableFlag_Etrue(CStifItemParser& aItem);
				TInt CSettingsApi::SetEditableFlag_EFalse(CStifItemParser& aItem);
				TInt CSettingsApi::SetEditableFlag_id_negative(CStifItemParser& aItem);
				TInt CSettingsApi::SetEditableFlag_id_positive(CStifItemParser& aItem);
				TInt CSettingsApi::SetEditableFlag_id_zero(CStifItemParser& aItem);
				TBool CSettingsApi::IsSLPExists(CStifItemParser& aItem);
				TBool CSettingsApi::IsSLPExists_invalid_negative(CStifItemParser& aItem);
				TBool CSettingsApi::IsSLPExists_invalid_positive(CStifItemParser& aItem);
				TBool CSettingsApi::IsSLPExists_invalid_zero(CStifItemParser& aItem);
				TInt CSettingsApi::GetAllSlp_sync(CStifItemParser& aItem);
				TInt CSettingsApi::GetAllSlp_async(CStifItemParser& aItem);
				TInt CSettingsApi::SetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem);
				TInt CSettingsApi::SetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem);
				TInt CSettingsApi::SetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem);
				TInt CSettingsApi::SetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem);
				TInt CSettingsApi::SetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem);
				TInt CSettingsApi::GetSimChangeRemoveFlag_Etrue(CStifItemParser& aItem);
				TInt CSettingsApi::GetSimChangeRemoveFlag(TInt64 aSlpId1);
				TInt CSettingsApi::GetSimChangeRemoveFlag_EFalse(CStifItemParser& aItem);
				TInt CSettingsApi::GetSimChangeRemoveFlag_id_negative(CStifItemParser& aItem);
				TInt CSettingsApi::GetSimChangeRemoveFlag_id_positive(CStifItemParser& aItem);
				TInt CSettingsApi::GetSimChangeRemoveFlag_id_zero(CStifItemParser& aItem);
				TInt CSettingsApi::GetEditableFlag_Etrue(CStifItemParser& aItem);
				TInt CSettingsApi::GetEditableFlag(TInt64 aSlpId1);
				TInt CSettingsApi::GetEditableFlag_EFalse(CStifItemParser& aItem);
				TInt CSettingsApi::GetEditableFlag_id_negative(CStifItemParser& aItem);
				TInt CSettingsApi::GetEditableFlag_id_positive(CStifItemParser& aItem);
				TInt CSettingsApi::GetEditableFlag_id_zero(CStifItemParser& aItem);
				TInt CSettingsApi::SetFallBack_ETrue();
				TInt CSettingsApi::SetFallBack_EFalse();
				TInt CSettingsApi::GetFallBack();
				TInt CSettingsApi::IsSLPExistsString();
				TInt CSettingsApi::IsSLPExistsString_Null();
				TInt CSettingsApi::IsSLPExistsString_invalid();
				TInt CSettingsApi::SlpCount();
				TInt CSettingsApi::GetSlpInfoFromAddress(CStifItemParser& aItem);
				TInt CSettingsApi::GetSlpInfoFromAddress_invalid(CStifItemParser& aItem);
				TInt CSettingsApi::ProductConfiguredServer1();
				TInt CSettingsApi::ProductConfiguredServer2();
				TInt CSettingsApi::ProductConfiguredServer3();
				TInt CSettingsApi::ProductConfiguredServer4();
		
				TInt SetGetTriggerParamsL( CStifItemParser& aItem ); 
				
                
        virtual TInt GetAllSessionSetApiL( CStifItemParser& aItem );
       
              
        
       
        virtual TInt SetSessionObserverL( CStifItemParser& aItem );
        virtual TInt SetSessionObserverTwiceL( CStifItemParser& aItem );
        virtual TInt RemoveSessionObserverL( CStifItemParser& aItem );
        virtual TInt RemoveSessionObserverWithoutSetL( CStifItemParser& aItem );
        virtual TInt CheckSessionChangeEventsL( CStifItemParser& aItem );
        
        virtual TInt GetASessionParamNullL( CStifItemParser& aItem );
        virtual TInt GetASessionParamInValidL( CStifItemParser& aItem );
        virtual TInt CancelTriggerSessionL( CStifItemParser& aItem );
        
       
        virtual TInt SetTriggerEmptySessionNameL( CStifItemParser& aItem );
        virtual TInt SetTriggerLongSessionNameL( CStifItemParser& aItem );

        TInt CSettingsApi::ChangeSuplTriggerServiceStatusON();
            

        TInt CSettingsApi::ChangeSuplTriggerServiceStatusOFF();
           

        TInt CSettingsApi::GetSuplTriggerServiceStatusON();


        TInt CSettingsApi::GetSuplTriggerServiceStatusOFF();

        TInt CSettingsApi::SetstatusOtherThanAppropriate();
    public : // from MSuplSettingsObserver
        virtual void HandleSuplTriggerStatusChangeL(
                       CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus );
      
				   
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;
         
        // Reserved pointer for future extension
        //TAny* iReserved;
		MSuplSettingsObserver *iObs;
    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
        CActiveScheduler *iScheduler;
        	CSuplSettings *iSettings;
        	CSuplSettingsInternal *cSettings;
        	CTerminalInitiatedRunSession *imyAO;
        	TInt iTestid;
        	TInt iErrCode;
        	TInt iSlpId;
        	
};
    

class CTerminalInitiatedRunSession : public CActive
	{
	public :

    CTerminalInitiatedRunSession();
	static CTerminalInitiatedRunSession* NewL(MSettingsObserver *aObserver);
		
	void ConstructL(MSettingsObserver *aObserver);
	
	~CTerminalInitiatedRunSession();
	void RunSession(CSuplSettings *aSettings);
	//void GetPositionL(TSuplPosition& aPosition);
	
	void RunL();
	void DoCancel();
	void CancelRunSession(CSuplSettings *aSettings);
	void StartListening();
	
	private:
	
   CSuplSettings *iSettings;
   CSettingsApi *iTest;
   	CSuplSettingsInternal *cSettings;
   MSettingsObserver *iObserver;
   RTimer iTimer;
    
	};
class CObserverTest: public MSuplSettingsObserver
{
	public:

	
	private:
	MSuplSettingsObserver *iObserver;
	
	
	 private:    // Constructor
        /**
        * C++ Constructor
        */
        CObserverTest(MSuplSettingsObserver *aObserver);

    public:     // Destructor
        /**
        * C++ Destructor
        */
        ~CObserverTest();

        /**
        * Two-phase constructor
        */
        static CObserverTest* NewL(MSuplSettingsObserver *aObserver);

    private:
        /**
        * EPOC default constructor
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CObserverTest( const CObserverTest& );
	
};
#endif      // SUPLSERVER_H            
// End of File


