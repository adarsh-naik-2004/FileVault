# Compiler and Flags
CXX = g++
CXXFLAGS = -g -I./include -Wall -Wextra -std=c++17

# Directories
SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Source Files and Object Files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Target Executable
TARGET = $(BINDIR)/FileVault

# Default Build Target
all: $(TARGET)
	@echo "Build complete"

# Linking the Final Executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compiling Source Files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create Necessary Directories
$(BUILDDIR) $(BINDIR):
	mkdir -p $@

# Clean Build Files
clean:
	rm -rf $(BUILDDIR)/* $(TARGET)
