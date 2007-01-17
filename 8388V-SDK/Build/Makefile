#                Copyright 2006, Marvell International Ltd.
# This code contains confidential information of Marvell Semiconductor, Inc.
# No rights are granted herein under any patent, mask work right or copyright
# of Marvell or any third party.
# Marvell reserves the right at its sole discretion to request that this code
# be immediately returned to Marvell. This code is provided "as is".
# Marvell makes no warranties, express, implied or otherwise, regarding its
# accuracy, completeness or performance.

#
# Makefile for building firmware from SDK
#
# History:
#    5/3/06   sanjayg         Created
#


TOOLS_DIR=../Tools/bin

# note sdk.mak is automatically created while building SDK
include sdk.mak

#C compiler flags
CFLAGS += -I$(SDK_WLAN_INCLUDE_DIR)/ -I$(SDK_WLAN_CUSTOM_APP_INCLUDE_DIR)/ \
          -I$(SDK_TCP_INCLUDE_DIR)

# list of objects
ifeq (1, $(BUILT_NO_HOST))
OBJS = customApp.o mli.o version_nohost.o eping.o userif.o hostsme_sm.o cmd_parser.o peersta_api.o
else
OBJS = customApp.o mli.o version_sdk.o
endif

# compilation rules
%.o: $(SDK_WLAN_CUSTOM_APP_SRC_DIR)/%.s
	$(AS) $(ASFLAGS) -o $*.o $<
	
%.o: $(SDK_WLAN_CUSTOM_APP_SRC_DIR)/%.c
	$(CC) -c -o $*.o $(CFLAGS) $(EXTRACFLAGS) $<

# build target rule
wlansdk8388: $(OBJS)
	$(LD) $(LDFLAGS) -o $(EXE_NAME) $(OBJS)  $(SDK_OBJS) $(SDK_LIBS)
	@echo "/* $(MOD_NAME)_$(MAC_REV)/$(WLAN_RF) (Ver. $(FWVERSION)) built `date` by `whoami` */" > $(FMIMAGE)	
	$(TOOLS_DIR)/arm-elf-breakhlp.exe -s512 -h $(EXE_NAME)
	$(TOOLS_DIR)/bin2hdr rom.bin $(MOD_NAME).h
	@cat $(MOD_NAME).h >> $(FMIMAGE)
	@mv $(FMIMAGE) $(MOD_NAME)_$(MAC_REV)_$(WLAN_RF).h
	mv rom.bin $(PRODUCT_NAME).img
	$(TOOLS_DIR)/arm-elf-breakhlp.exe -s512 -h helper.axf
	mv rom.bin helper.img
	$(TOOLS_DIR)/rom profile
	$(TOOLS_DIR)/rom booter 
	$(TOOLS_DIR)/rom helper_profile 
	@cat booter.bin > $(PRODUCT_NAME).bin
	@cat csb0.bin >> $(PRODUCT_NAME).bin
	@cat csb0.bin >> $(PRODUCT_NAME).bin
	@cat sect7.bin >> $(PRODUCT_NAME).bin
	@cp $(PRODUCT_NAME).bin $(PRODUCT_NAME)_mfg.bin
	@$(TOOLS_DIR)/bined -s -f $(PRODUCT_NAME)_mfg.bin -w0x7fffc=0x5FFFFFFA


# build target rule
wlansdk8388v: $(OBJS)
	$(LD) $(LDFLAGS) -o $(EXE_NAME) $(OBJS) $(SDK_OBJS) $(SDK_LIBS)
	@echo "/* $(MOD_NAME)_$(MAC_REV)/$(WLAN_RF) (Ver. $(FWVERSION)) built `date` by `whoami` */" > $(FMIMAGE)	
	$(TOOLS_DIR)/arm-elf-breakhlp.exe -s512 -h $(EXE_NAME)
	$(TOOLS_DIR)/bin2hdr rom.bin $(MOD_NAME).h
	@cat $(MOD_NAME).h >> $(FMIMAGE)
	@mv $(FMIMAGE) $(MOD_NAME)_$(MAC_REV)_$(WLAN_RF).h
	@rm $(MOD_NAME).h
	mv rom.bin wlan8388v.img
	$(TOOLS_DIR)/rom profile

clean:
	-rm -f $(EXE_NAME) *.o $(PRODUCT_NAME).bin  $(PRODUCT_NAME)_mfg.bin



