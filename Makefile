TARGET_EXEC ?= exec 

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++11 -Wall -Wextra -g #-Werror

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	g++ $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

all : run

run :
	$(BUILD_DIR)/exec -inst tiny -alg BnB -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny -alg Approx -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny -alg LS1 -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny -alg LS2 -time 10 -seed 0

	$(BUILD_DIR)/exec -inst tiny_1 -alg BnB -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny_1 -alg Approx -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny_1 -alg LS1 -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst tiny_1 -alg LS2 -time 10 -seed 0

	$(BUILD_DIR)/exec -inst karate -alg BnB -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst karate -alg Approx -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst karate -alg LS1 -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst karate -alg LS2 -time 10 -seed 0

	$(BUILD_DIR)/exec -inst jazz -alg BnB -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg Approx -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg LS1 -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg LS2 -time 10 -seed 0

	$(BUILD_DIR)/exec -inst email -alg BnB -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg Approx -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg LS1 -time 10 -seed 0
#	$(BUILD_DIR)/exec -inst email -alg LS2 -time 10 -seed 0

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
