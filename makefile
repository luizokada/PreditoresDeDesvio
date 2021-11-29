# Makefile
CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
include $(CONFIG_ROOT)/makefile.config
TEST_TOOL_ROOTS := bp_gag
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_gap
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_gas
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_pap
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_pas
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_pag
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_sag
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_sap
include $(TOOLS_ROOT)/Config/makefile.default.rules
TEST_TOOL_ROOTS := bp_sas
include $(TOOLS_ROOT)/Config/makefile.default.rules
