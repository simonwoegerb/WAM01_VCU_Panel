CXX := g++
CXXFLAGS := -Wall -Wextra -O2 -MMD -MP

TARGET := WAM01_VCU_Panel 

SRC_DIR := src
BUILD_DIR := build
IMGUI_DIR := imgui

# -------------------------
# Your app sources
# -------------------------
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# -------------------------
# ImGui sources
# -------------------------
IMGUI_SRC := \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

IMGUI_OBJ := $(patsubst $(IMGUI_DIR)/%.cpp,$(BUILD_DIR)/imgui_%.o,$(IMGUI_SRC))

# -------------------------
# Includes
# -------------------------
INCLUDES := -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

# -------------------------
# Libraries (Linux)
# -------------------------
LIBS := -lglfw -lGL -ldl -lpthread

# -------------------------
# Default target
# -------------------------
all: $(TARGET)

# -------------------------
# Link step
# -------------------------
$(TARGET): $(OBJ) $(IMGUI_OBJ)
	$(CXX) $^ -o $@ $(LIBS)

# -------------------------
# App compile rule
# -------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# ImGui compile rule (FIXED)
# -------------------------
$(BUILD_DIR)/imgui_%.o: $(IMGUI_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# Dependency files
# -------------------------
DEP := $(OBJ:.o=.d) $(IMGUI_OBJ:.o=.d)
-include $(DEP)

# -------------------------
# Clean
# -------------------------
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
