# product options
AS_NAME = as
VM_NAME = vm

# compiler options
CC = clang++
MUTE = # varargs write-strings sign-compare unused-function comment dangling-gsl unknown-warning-option c++17-extensions
DEFS = 
CXXFLAGS = -Iinclude -Wall $(addprefix -Wno-,$(MUTE)) $(addprefix -D,$(DEFS))
LDFLAGS = 

# command options
RM = rm
MKDIR = mkdir
TC = touch

# dir stuff
EXT = .cpp
MAINDIR = $(PWD)
AS_DIR = as
VM_DIR = vm
BINDIR = $(MAINDIR)/bin
OBJDIR = $(BINDIR)/obj
AS_OBJDIR = $(OBJDIR)/$(AS_NAME)
VM_OBJDIR = $(OBJDIR)/$(VM_NAME)

# file stuff
AS_BIN = $(BINDIR)/$(AS_NAME)
VM_BIN = $(BINDIR)/$(VM_NAME)

# make stuff
export CC CXXFLAGS LDFLAGS RM MKDIR  EXT
export AS_NAME AS_DIR AS_OBJDIR AS_BIN 
export VM_NAME VM_DIR VM_OBJDIR VM_BIN

.MAIN: all
all: $(AS_NAME) $(VM_NAME)

YELLOW = \033[0;33m$$(tput bold)
NC= \033[0m$$(tput sgr0)

# targets

$(AS_NAME): makedirs
	@$(MAKE) --no-print-directory -f $(AS_DIR)/Makefile $(AS_BIN)

$(VM_NAME): makedirs
	@$(MAKE) --no-print-directory -f $(VM_DIR)/Makefile $(VM_BIN)


makedirs:
	@$(MKDIR) -p $(BINDIR)
	@$(MKDIR) -p $(AS_OBJDIR)
	@$(MKDIR) -p $(VM_OBJDIR)

clean:
	@$(RM) -rf $(BINDIR)
#	@printf "$(YELLOW)cleaned!\n$(NC)"


# new file creator
newfile: guard-name | guard-type
	$(TC) $(type)/src/$(name).cpp
	$(TC) $(type)/include/$(name).hpp


guard-%: # make sure variable exists
	@ if [ "${${*}}" = "" ]; then \
		echo "Argument $* not given (name=\"name\" and type=\"<empty>|$(AS_NAME)|$(VM_NAME)\" required)"; \
		exit 1; \
	fi

