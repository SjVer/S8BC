# product options
COMPILER_NAME = comp
CONSOLE_NAME = vm

# compiler options
CC = gcc
CXXFLAGS = -std=c11 -Wall -lm
LDFLAGS = 

# command options
RM = rm
MKDIR = mkdir
TC = touch

# dir stuff
MAINDIR = $(PWD)
SRCDIR = $(MAINDIR)/src
INCDIR = $(MAINDIR)/include
BINDIR = $(MAINDIR)/bin
OBJDIR = $(BINDIR)/obj
COMPILER_OBJDIR = $(OBJDIR)/$(COMPILER_NAME)-obj
CONSOLE_OBJDIR = $(OBJDIR)/$(CONSOLE_NAME)-obj

# file stuff
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(SRCS))))
COMPILER_BIN = $(BINDIR)/$(COMPILER_NAME)
CONSOLE_BIN = $(BINDIR)/$(CONSOLE_NAME)


# make stuff
export CC CXXFLAGS LDFLAGS RM MKDIR 
export MAKEDIR SRCDIR INCDIR BINDIR SRCS OBJS
export COMPILER_NAME COMPILER_OBJDIR COMPILER_BIN 
export CONSOLE_NAME CONSOLE_OBJDIR CONSOLE_BIN
.MAIN: all
all: $(COMPILER_NAME) $(CONSOLE_NAME)

YELLOW = \033[0;33m$$(tput bold)
NC= \033[0m$$(tput sgr0)

# targets

# $(OBJS): $(SRCS) | makedirs
$(OBJDIR)/%.o: $(SRCDIR)/%.c | makedirs
	@printf "$(YELLOW)[general    ]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(COMPILER_NAME): $(OBJS) | makedirs
	@$(MAKE) --no-print-directory -f $(MAINDIR)/make/compiler.mk $(COMPILER_BIN)

$(CONSOLE_NAME): $(OBJS) | makedirs
	@$(MAKE) --no-print-directory -f $(MAINDIR)/make/console.mk $(CONSOLE_BIN)


makedirs:
	@$(MKDIR) -p $(BINDIR)
	@$(MKDIR) -p $(COMPILER_OBJDIR)
	@$(MKDIR) -p $(CONSOLE_OBJDIR)

clean:
	@$(RM) -rf $(BINDIR)
	@printf "$(YELLOW)Cleaned!\n$(NC)"



# new file creator
newfile: guard-name | guard-type
	$(TC) $(SRCDIR)/$(type)/$(name).c
	$(TC) $(INCDIR)/$(type)/$(name).h


guard-%: # make sure variable exists
	@ if [ "${${*}}" = "" ]; then \
		echo "Environment variable $* not set (name=\"name\" and type=\"<empty>|$(COMPILER_NAME)|$(CONSOLE_NAME)\" required)"; \
		exit 1; \
	fi

