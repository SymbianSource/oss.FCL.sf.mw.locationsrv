rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of the License "Symbian Foundation License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

md e:\results

md e:\results\LandMarksTestModule

ATSINTERFACE.EXE -testmodule testscripter -config c:\testframework\LandMarksTestModule1.cfg

copy c:\Logs\TestFramework\TestReport.txt e:\results\LandMarksTestModule\TestReport_LandMarksTestModule1.txt
