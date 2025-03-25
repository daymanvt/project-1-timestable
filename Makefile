### --------------------------------------------------------------------- ###
### PROJECT CONFIGURATION (CHANGE ME)									  ###
### --------------------------------------------------------------------- ###
PROJECT := timestable
ENTRY := $(PROJECT)_main.c
VERSION := 1.0.0


### --------------------------------------------------------------------- ###
### SHELL CONFIGURATION (DO NOT CHANGE)								      ###
### --------------------------------------------------------------------- ###
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules


### --------------------------------------------------------------------- ###
### COMPILER AND LINKER FLAGS 											  ###
### --------------------------------------------------------------------- ###

# Cross compilation support
CROSS_COMPILE ?=
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
RANLIB := $(CROSS_COMPILE)ranlib

# Warning flag configurations
WARNINGS_BASIC := -Wall -Wextra -Wpedantic
WARNINGS_EXTRA := $(WARNINGS_BASIC) -Waggregate-return -Wcast-align -Wcast-qual \
                 -Wdisabled-optimization -Wdouble-promotion -Wfloat-equal -Wformat=2 \
                 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls \
                 -Wshadow -Wstrict-overflow=5 -Wundef -Wno-unused
WARNINGS_HARDCORE := $(WARNINGS_EXTRA) -Wconversion -Wsign-conversion -Wpadded \
                    -Wswitch-enum -Wwrite-strings -Wvla

# Default to basic warnings
WARNINGS ?= basic
ifeq ($(WARNINGS),extra)
    CFLAGS += $(WARNINGS_EXTRA)
else ifeq ($(WARNINGS),hardcore)
    CFLAGS += $(WARNINGS_HARDCORE)
else
    CFLAGS += $(WARNINGS_BASIC)
endif

# Build type configurations
BUILD_TYPE ?= debug
ifeq ($(BUILD_TYPE),release)
    CFLAGS += -O2 -DNDEBUG
else ifeq ($(BUILD_TYPE),size)
    CFLAGS += -Os -DNDEBUG
else ifeq ($(BUILD_TYPE),fast)
    CFLAGS += -Ofast -DNDEBUG
else
    CFLAGS += -g3 -O0 -DDEBUG
endif

# Common compiler flags
CFLAGS += -std=c99 -D_DEFAULT_SOURCE
CFLAGS += -fstack-protector-all
INCLUDES := -Iinclude
LDFLAGS :=

# Libraries that require explicit linking
LDLIBS += -lm          # Math library (math.h)
# LDLIBS += -lpthread    # POSIX Threads library (pthread.h)
# LDLIBS += -lrt         # Real-time extensions library
# LDLIBS += -ldl         # Dynamic linking library (dlfcn.h)
# LDLIBS += -lnsl        # Network services library
# LDLIBS += -lsocket     # Socket library (for some UNIX systems)
# LDLIBS += -lc_p        # GNU C Library Extensions (profiling)
# LDLIBS += -lncurses    # Terminal control library
# LDLIBS += -lreadline   # Command-line editing library
# LDLIBS += -ldb         # Berkeley DB library
# LDLIBS += -lz          # Compression library (zlib)


### --------------------------------------------------------------------- ###
### DIRECTORIES AND SOURCE FILES										  ###
### --------------------------------------------------------------------- ###

SRC_DIR := src
INC_DIR := include
TEST_DIR := test
BUILD_DIR := build
BIN_DIR := bin
OBJ_DIR := $(BUILD_DIR)/obj
TEST_OBJ_DIR := $(BUILD_DIR)/test_obj
DOC_DIR := docs
COV_DIR := $(BUILD_DIR)/coverage

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

# Test files
TEST_SRC_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES := $(patsubst $(TEST_DIR)/%.c,$(TEST_OBJ_DIR)/%.o,$(TEST_SRC_FILES))
TEST_DEP_FILES := $(TEST_OBJ_FILES:.o=.d)

# Object files for testing (exclude program entry point)
PROG_ENTRY := $(OBJ_DIR)/$(basename $(ENTRY)).o
COMMON_OBJ_FILES := $(filter-out $(PROG_ENTRY), $(OBJ_FILES))

### --------------------------------------------------------------------- ###
### DEFAULT TARGETS 													  ###
### --------------------------------------------------------------------- ###

TARGET := $(BIN_DIR)/$(PROJECT)
TEST_TARGET := $(BIN_DIR)/$(PROJECT)_test

# Default target
.PHONY: all
all: check-tools $(TARGET)

### TOOL VERIFICATION ###
REQUIRED_TOOLS := gcc make
OPTIONAL_TOOLS := clang-format doxygen pdflatex gcov lcov
.PHONY: check-tools
check-tools:
	$(foreach tool,$(REQUIRED_TOOLS),\
		$(if $(shell which $(tool)),,$(error "$(tool) not found in PATH")))
	$(foreach tool,$(OPTIONAL_TOOLS),\
		$(if $(shell which $(tool)),,$(warning "$(tool) not found in PATH")))

### --------------------------------------------------------------------- ###
### BUILD TARGETS 														  ###
### --------------------------------------------------------------------- ###

# Include dependency files if they exist
-include $(DEP_FILES)
-include $(TEST_DEP_FILES)

# Timestamp for header dependency checking
HEADERS := $(wildcard $(INC_DIR)/*.h)
HEADER_STAMP := $(BUILD_DIR)/.header_timestamp

# Check if headers have changed
$(HEADER_STAMP): $(HEADERS)
	mkdir -p $(BUILD_DIR)
	touch $(HEADER_STAMP)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_STAMP)
	mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Compile test files
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADER_STAMP)
	mkdir -p $(TEST_OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -I$(TEST_DIR) -MMD -MP -c $< -o $@

# Link object files
$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	@echo "Linking $(TARGET)"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Build complete: $(TARGET)"

# Build and link test executable
$(TEST_TARGET): $(TEST_OBJ_FILES) $(COMMON_OBJ_FILES)
	mkdir -p $(BIN_DIR)
	@echo "Linking $(TEST_TARGET)"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Test build complete: $(TEST_TARGET)"


### --------------------------------------------------------------------- ###
### ADDITIONAL TARGETS 												      ###
### --------------------------------------------------------------------- ###

# Test target
.PHONY: test
test: CFLAGS += -g3 -O0 -DTEST
test: $(TEST_TARGET)
	mkdir -p $(TEST_DIR)
	@echo "Running tests..."
	$(TEST_TARGET)

### CODE QUALITY ###
.PHONY: format
format:
	@echo "Formatting source code..."
	-clang-format -i $(SRC_FILES) $(HEADERS) $(TEST_SRC_FILES)
	@echo "Formatting complete"

### UTILITY TARGETS ###
.PHONY: clean
clean:
	@echo "Cleaning build artifacts"
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

.PHONY: distclean
distclean: clean
	@echo "Cleaning documentation"
	rm -rf $(DOC_DIR)
	@echo "All artifacts cleaned"

.PHONY: rebuild
rebuild: clean all

### HELP ###
.PHONY: help
help:
	@echo "Makefile for $(PROJECT) (v$(VERSION))"
	@echo ""
	@echo "Build Configuration:"
	@echo "  BUILD_TYPE=debug|release|size|fast (default: debug)"
	@echo "  WARNINGS=basic|extra|hardcore (default: basic)"
	@echo "  CROSS_COMPILE=<prefix> (for cross-compilation)"
	@echo ""
	@echo "Basic Targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  rebuild      - Clean and rebuild"
	@echo "  format       - Format source code"
	@echo "  check-tools  - Verify required tools are available"
	@echo ""
	@echo "Advanced Targets:"
	@echo "  # coverage     - Run tests with coverage reporting (currently disabled)"
	@echo "  # analyze      - Run static code analysis (currently disabled)"
	@echo "  # asan         - Build with Address Sanitizer"
	@echo "  # tsan         - Build with Thread Sanitizer"
	@echo "  docs         - Generate documentation"
	@echo "  install      - Install the application to /usr/local/bin"
	@echo "  print        - Display project information"
	@echo "Examples:"
	@echo "  make                     - Debug build with basic warnings"
	@echo "  make BUILD_TYPE=release  - Release build"
	@echo "  make WARNINGS=extra      - Build with extra warnings"
	@echo "  make format              - Format the code"
	@echo ""
	@echo "Use 'make V=1' for verbose output"

# Print variables target (for debugging)
.PHONY: print
print:
	$(info $$PROJECT is [${PROJECT}])
	$(info $$VERSION is [${VERSION}])
	$(info $$SRC_FILES is [${SRC_FILES}])
	$(info $$OBJ_FILES is [${OBJ_FILES}])
	$(info $$TEST_SRC_FILES is [${TEST_SRC_FILES}])
	$(info $$TEST_OBJ_FILES is [${TEST_OBJ_FILES}])
	$(info $$COMMON_OBJ_FILES is [${COMMON_OBJ_FILES}])
	$(info $$CFLAGS is [${CFLAGS}])
	$(info $$LDFLAGS is [${LDFLAGS}])
	$(info $$LDLIBS is [${LDLIBS}])
	$(info $$INCLUDES is [${INCLUDES}])
	$(info $$TARGET is [${TARGET}])
	$(info $$TEST_TARGET is [${TEST_TARGET}])

# Support for verbose mode
ifneq ($(V),1)
.SILENT:
endif

### --------------------------------------------------------------------- ###
### THESE WILL BE ENABLED AS THE NEED ARISES 							  ###
### --------------------------------------------------------------------- ###

# Coverage target
# .PHONY: coverage
# coverage: CFLAGS += -g3 -O0 -DTEST --coverage
# coverage: LDFLAGS += --coverage
# coverage: directories $(TEST_TARGET)
# 	mkdir -p $(COV_DIR)
# 	@echo "Running tests with coverage..."
# 	$(TEST_TARGET)
# 	@echo "Generating coverage report..."
# 	gcov -o $(TEST_OBJ_DIR) $(TEST_SRC_FILES)
# 	lcov --capture --directory . --output-file $(COV_DIR)/coverage.info
# 	genhtml $(COV_DIR)/coverage.info --output-directory $(COV_DIR)
# 	@echo "Coverage report generated in $(COV_DIR)"

# Static analysis with clang-tidy
# .PHONY: analyze
# analyze:
# 	@echo "Running static analysis..."
# 	-clang-tidy $(SRC_FILES) $(HEADERS) -- $(CFLAGS) $(INCLUDES)

# Sanitizer builds
# .PHONY: asan
# asan: CFLAGS += -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
# asan: LDFLAGS += -fsanitize=address,undefined
# asan: directories $(TARGET)
# 	@echo "Address Sanitizer build complete"

# .PHONY: tsan
# tsan: CFLAGS += -g3 -fsanitize=thread
# tsan: LDFLAGS += -fsanitize=thread
# tsan: directories $(TARGET)
# 	@echo "Thread Sanitizer build complete"

# Documentation target
# .PHONY: docs
# docs:
# 	@echo "Generating documentation..."
# 	mkdir -p $(DOC_DIR)
# 	-doxygen Doxyfile
# 	@echo "Generating PDF..."
# 	$(MAKE) -C $(DOC_DIR)/latex pdf
# 	@echo "PDF documentation generated in $(DOC_DIR)"

# Install target
# .PHONY: install
# install: prod
# 	@echo "Installing to /usr/local/bin/$(PROJECT)"
# 	install -d /usr/local/bin
# 	install -m 755 $(TARGET) /usr/local/bin/$(PROJECT)