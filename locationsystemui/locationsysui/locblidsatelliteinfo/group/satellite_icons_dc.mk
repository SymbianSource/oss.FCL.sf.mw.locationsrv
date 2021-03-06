#
# Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: Icon make file for BLID Satellite Info
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=\epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\satellite.mif
HEADERFILENAME=$(HEADERDIR)\satellite.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
		/c8,8 qgn_prop_blid_active \
		/c8,8 qgn_prop_blid_bg \
		/c8,8 qgn_prop_blid_inactive \
		/c8,8 qgn_prop_blid_sat_firma \
		/c8,8 qgn_prop_blid_sat_firma_active \
		/c8,8 qgn_prop_blid_sat_firma_inact \
		/c8,8 qgn_prop_blid_sat_firma_moon \
		/c8,8 qgn_prop_blid_sat_firma_sun \
		/c8,8 qgn_graf_blid \
		/c8,8 qgn_graf_blid_ringfix
				
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing

