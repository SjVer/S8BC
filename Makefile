# compiler options
CC = clang-15
MUTE = unused-command-line-argument
DEFS = COMPILER=\"$(CC)\" APP_VERSION_PATCH=\"$(shell git rev-parse --short HEAD)\"
CXXFLAGS = -std=c11 -Iinclude -Wall
LDFLAGS = 

# command options
RM = rm
MKDIR = mkdir
TC = touch

# ===================== variables =====================

EXT = .c
SRCDIR = src
INCDIR = include
BINDIR = bin
OBJDIR = $(BINDIR)/obj

PRODUCTS = $(notdir $(wildcard $(SRCDIR)/*))

SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)

CXXFLAGS += $(addprefix -Wno-,$(MUTE)) $(addprefix -D,$(DEFS))

Y = \033[0;33m$$(tput bold)
P = \033[1;35m$$(tput bold)
N = \033[0m$$(tput sgr0)

# ===================== targets =====================

define make-target
$1-SRCDIR = $(SRCDIR)/$1
$1-INCDIR = $(INCDIR)/$1
$1-OBJDIR = $(OBJDIR)/$1

$1-SRC = $$(wildcard $$($1-SRCDIR)/*$(EXT))
$1-OBJ = $$($1-SRC:$$($1-SRCDIR)/%$(EXT)=$$($1-OBJDIR)/%.o)

$1: $(BINDIR)/$1

$(BINDIR)/$1: $(OBJ) $$($1-OBJ)
	@printf "$(Y)[$1]$(N) compiling final product $(P)$$(notdir $$@)$(N)..."
	@$(CC) $(CXXFLAGS) -o $$@ $$^ $(LDFLAGS)
	@printf "\b\b done!\n"

$$($1-OBJDIR)/%.o: $$($1-SRCDIR)/%$(EXT) | makedirs
	@printf "$(Y)[$1]$(N) compiling $(P)$1/$$(notdir $$<)$(N)..."
	@$(CC) $(CXXFLAGS) -I$(INCDIR) -o $$@ -c $$<
	@printf "\b\b done!\n"

clean-$1:
	@$(RM) -rf $$($1-OBJDIR)
	@$(RM) $(BINDIR)/$1
endef

$(foreach p,$(PRODUCTS),$(eval $(call make-target,$(p))))

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) | makedirs
	@printf "$(Y)[..]$(N) compiling $(P)$(notdir $<)$(N)..."
	@$(CC) $(CXXFLAGS) -I$(INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

.PHONY: dis
dis:
	odin run dis -out:$(BINDIR)/dis -- bin/test.rom

# ===================== TOOLS =====================

test-as: as
	$(BINDIR)/as test/test.s -o bin/test.rom -v

test-vm: vm
	$(BINDIR)/vm bin/test.rom -vd

verilog: makedirs
	iverilog sim/*.v -o bin/sim

makedirs:
	@$(MKDIR) -p $(BINDIR)
	@$(foreach p,$(PRODUCTS),$(MKDIR) -p $(OBJDIR)/$(p);)

clean:
	@$(RM) -rf $(BINDIR)
