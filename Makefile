### CHANGE ME ###
PROJECT := timestable
ENTRY := $(PROJECT)_main.c
VERSION := 1.0.0

### ------------  DO NOT CHANGE BELOW ------------ ###

# START - Makefile configuration
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
# END - Makefile configuration

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
CFLAGS += -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
CFLAGS += -Wformat=2 -Wconversion -Wdouble-promotion -Wfloat-equal
CFLAGS += -fstack-protector-all
CFLAGS += -std=c99 -D_DEFAULT_SOURCE
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

# Directories
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

# Target executables
TARGET := $(BIN_DIR)/$(PROJECT)
TEST_TARGET := $(BIN_DIR)/$(PROJECT)_test

# Default target (debug build)
.PHONY: all
all: debug

# Debug target
.PHONY: debug
debug: CFLAGS += -g3 -O0
debug: directories $(TARGET)

# Production target
.PHONY: prod
prod: CFLAGS += -O2 -DNDEBUG
prod: directories $(TARGET)

# Release target with version info
.PHONY: release
release: CFLAGS += -O2 -DNDEBUG -DVERSION=\"$(VERSION)\"
release: clean directories $(TARGET)
	@echo "Created release version $(VERSION)"

# Include dependency files if they exist
-include $(DEP_FILES)
-include $(TEST_DEP_FILES)

# Timestamp for header dependency checking
HEADERS := $(wildcard $(INC_DIR)/*.h)
HEADER_STAMP := $(BUILD_DIR)/.header_timestamp

# Check if headers have changed
$(HEADER_STAMP): $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	@touch $(HEADER_STAMP)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_STAMP)
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Compile test files
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADER_STAMP)
	@mkdir -p $(TEST_OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -I$(TEST_DIR) -MMD -MP -c $< -o $@

# Link object files
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $(TARGET)"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Build complete: $(TARGET)"

# Build and link test executable
$(TEST_TARGET): $(TEST_OBJ_FILES) $(COMMON_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $(TEST_TARGET)"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Test build complete: $(TEST_TARGET)"

# Create build directories
.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DOC_DIR)
	@echo "Build directories created"

# Test target
.PHONY: test
test: CFLAGS += -g3 -O0 -DTEST
test: directories $(TEST_TARGET)
	@echo "Running tests..."
	@$(TEST_TARGET)

# Coverage target
# .PHONY: coverage
# coverage: CFLAGS += -g3 -O0 -DTEST --coverage
# coverage: LDFLAGS += --coverage
# coverage: directories $(TEST_TARGET)
# 	@mkdir -p $(COV_DIR)
# 	@echo "Running tests with coverage..."
# 	@$(TEST_TARGET)
# 	@echo "Generating coverage report..."
# 	@gcov -o $(TEST_OBJ_DIR) $(TEST_SRC_FILES)
# 	@lcov --capture --directory . --output-file $(COV_DIR)/coverage.info
# 	@genhtml $(COV_DIR)/coverage.info --output-directory $(COV_DIR)
# 	@echo "Coverage report generated in $(COV_DIR)"

# Static analysis with clang-tidy
# .PHONY: analyze
# analyze:
# 	@echo "Running static analysis..."
# 	@-clang-tidy $(SRC_FILES) $(HEADERS) -- $(CFLAGS) $(INCLUDES)

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
.PHONY: docs
docs:
	@echo "Generating documentation..."
	@mkdir -p $(DOC_DIR)
	@-doxygen Doxyfile
	@echo "Generating PDF..."
	@$(MAKE) -C $(DOC_DIR)/latex pdf
	@echo "PDF documentation generated in $(DOC_DIR)"

# Install target
.PHONY: install
install: prod
	@echo "Installing to /usr/local/bin/$(PROJECT)"
	@install -d /usr/local/bin
	@install -m 755 $(TARGET) /usr/local/bin/$(PROJECT)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Clean all artifacts including docs
.PHONY: distclean
distclean: clean
	@echo "Cleaning documentation"
	@rm -rf $(DOC_DIR)
	@echo "All artifacts cleaned"

# Rebuild everything
.PHONY: rebuild
rebuild: clean all

# Run the application
.PHONY: run
run: all
	@echo "Running $(PROJECT)..."
	@$(TARGET)

# Print project information
.PHONY: info
info:
	@echo "Project: $(PROJECT)"
	@echo "Version: $(VERSION)"
	@echo "Source files: $(notdir $(SRC_FILES))"
	@echo "Test files: $(notdir $(TEST_SRC_FILES))"
	@echo "Include files: $(notdir $(HEADERS))"
	@echo "Compiler: $(CC)"
	@echo "Compiler flags: $(CFLAGS)"
	@echo "Linker flags: $(LDFLAGS)"
	@echo "Libraries: $(LDLIBS)"

# Check for compiler and tool availability
.PHONY: check-env
check-env:
	@echo "Checking build environment..."
	@which $(CC) >/dev/null || (echo "ERROR: $(CC) not found"; exit 1)
	@echo "$(CC) found: $$(which $(CC))"
	@echo "$(CC) version: $$($(CC) --version | head -n1)"
	@which make >/dev/null || (echo "ERROR: make not found"; exit 1)
	@echo "make found: $$(which make)"
	@echo "make version: $$(make --version | head -n1)"
	@which doxygen >/dev/null || (echo "ERROR: doxygen not found"; exit 1)
	@echo "doxygen found: $$(which doxygen)"
	@echo "doxygen version: $$(doxygen --version)"
	@which pdflatex >/dev/null || (echo "WARNING: pdflatex not found - PDF documentation will not be generated"; exit 1)
	@echo "pdflatex found: $$(which pdflatex)"
	@echo "pdflatex version: $$(pdflatex --version | head -n1)"
	@echo "Environment check complete"

# Help target
.PHONY: help
help:
	@echo "Makefile for $(PROJECT) (v$(VERSION))"
	@echo ""
	@echo "Basic Targets:"
	@echo "  all          - Build the project with debug flags (default)"
	@echo "  debug        - Build with debug info and no optimization"
	@echo "  prod         - Build with optimizations for production"
	@echo "  release      - Create a versioned release build"
	@echo "  test         - Build and run tests"
	@echo "  clean        - Remove build artifacts"
	@echo "  distclean    - Remove all generated files including docs"
	@echo "  rebuild      - Clean and rebuild"
	@echo "  run          - Build and run the application"
	@echo ""
	@echo "Advanced Targets:"
	@echo "  # coverage     - Run tests with coverage reporting (currently disabled)"
	@echo "  # analyze      - Run static code analysis (currently disabled)"
	@echo "  # asan         - Build with Address Sanitizer"
	@echo "  # tsan         - Build with Thread Sanitizer"
	@echo "  docs         - Generate documentation"
	@echo "  install      - Install the application to /usr/local/bin"
	@echo "  check-env    - Check the build environment"
	@echo "  info         - Display project information"
	@echo ""
	@echo "Use 'make V=1' for verbose output"

# Support for verbose mode
ifneq ($(V),1)
.SILENT:
endif

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
	$(info $$INCLUDES is [${INCLUDES}])
	$(info $$TARGET is [${TARGET}])
	$(info $$TEST_TARGET is [${TEST_TARGET}])