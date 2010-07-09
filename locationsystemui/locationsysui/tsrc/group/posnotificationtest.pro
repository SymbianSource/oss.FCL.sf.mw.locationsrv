# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description: Notification tests pro file
# include <platform_paths.hrh>
CONFIG += qtestlib
TEMPLATE = app
TARGET = posnotificationtest
DEPENDPATH += .
QT += testlib
LIBS += -lqttest
LIBS += -leposnwprv
LIBS += -leposprvtyp
HEADERS += posnotificationtest.h \
          testconsole.h
SOURCES += posnotificationtest.cpp
FORMS += 
RESOURCES += 
symbian:TARGET.UID3 = 0xEC1EFEF4
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>"
TARGET.CAPABILITY = All \
    -TCB
