# product options
PRODUCT_ONE_NAME = product_one
PRODUCT_TWO_NAME = product_two

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
PRODUCT_ONE_OBJDIR = $(OBJDIR)/$(PRODUCT_ONE_NAME)-obj
PRODUCT_TWO_OBJDIR = $(OBJDIR)/$(PRODUCT_TWO_NAME)-obj

# file stuff
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(SRCS))))
PRODUCT_ONE_BIN = $(BINDIR)/$(PRODUCT_ONE_NAME)
PRODUCT_TWO_BIN = $(BINDIR)/$(PRODUCT_TWO_NAME)


# make stuff
export CC CXXFLAGS LDFLAGS RM MKDIR 
export MAKEDIR SRCDIR INCDIR BINDIR SRCS OBJS
export PRODUCT_ONE_NAME PRODUCT_ONE_OBJDIR PRODUCT_ONE_BIN 
export PRODUCT_TWO_NAME PRODUCT_TWO_OBJDIR PRODUCT_TWO_BIN
.MAIN: all
all: $(PRODUCT_ONE_NAME) $(PRODUCT_TWO_NAME)

YELLOW = \033[0;33m$$(tput bold)
NC= \033[0m$$(tput sgr0)

# targets

# $(OBJS): $(SRCS) | makedirs
$(OBJDIR)/%.o: $(SRCDIR)/%.c | makedirs
	@printf "$(YELLOW)[general    ]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(PRODUCT_ONE_NAME): $(OBJS) | makedirs
	@$(MAKE) --no-print-directory -f $(MAINDIR)/make/$(PRODUCT_ONE_NAME).mk $(PRODUCT_ONE_BIN)

$(PRODUCT_TWO_NAME): $(OBJS) | makedirs
	@$(MAKE) --no-print-directory -f $(MAINDIR)/make/$(PRODUCT_TWO_NAME).mk $(PRODUCT_TWO_BIN)


makedirs:
	@$(MKDIR) -p $(BINDIR)
	@$(MKDIR) -p $(PRODUCT_ONE_OBJDIR)
	@$(MKDIR) -p $(PRODUCT_TWO_OBJDIR)

clean:
	@$(RM) -rf $(BINDIR)
	@printf "$(YELLOW)Cleaned!\n$(NC)"



# new file creator
newfile: guard-name | guard-type
	$(TC) $(SRCDIR)/$(type)/$(name).c
	$(TC) $(INCDIR)/$(type)/$(name).h


guard-%: # make sure variable exists
	@ if [ "${${*}}" = "" ]; then \
		echo "Environment variable $* not set (name=\"name\" and type=\"<empty>|$(PRODUCT_ONE_NAME)|$(PRODUCT_TWO_NAME)\" required)"; \
		exit 1; \
	fi

