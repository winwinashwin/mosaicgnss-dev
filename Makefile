.SHELL        := /bin/sh
.DEFAULT_GOAL := all

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wshadow -O3 -DNDEBUG -DLOG_LEVEL=4

MKDIR    := mkdir -p
RM       := rm -rf

DEP_DIR  := ./build/deps
OBJ_DIR  := ./build/obj
SRC_DIR  := ./src
BIN_DIR  := ./build

SRCS  := $(shell find $(SRC_DIR) -name "*.cpp" -type f)
OBJS  := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.obj)
DEPS  := $(SRCS:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

LD_LIBS  := pcap

LD_FLAGS := $(addprefix -l, $(LD_LIBS))
DEPFLAGS  = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

TARGET   := stream

$(DEPS):
	@$(MKDIR) $(@D)

-include $(DEPS)

$(TARGET): $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(@D)
	$(CXX) -c $(CXXFLAGS) $(DEPFLAGS) -o $@ $<

$(BIN_DIR)/$(TARGET): $(OBJS)
	@$(MKDIR) $(@D)
	$(CXX) -o $@ $^ $(LD_FLAGS)

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(DEPS) $(BIN_DIR)/$(TARGET)
