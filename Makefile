# command options
RM = rm
MKDIR = mkdir
TC = touch

# ===================== variables =====================

SRCDIR = src
BINDIR = bin

PRODUCTS = as

Y = \033[0;33m$$(tput bold)
P = \033[1;35m$$(tput bold)
N = \033[0m$$(tput sgr0)

# ===================== targets =====================

define make-target

.PHONY: $1
$1: $(BINDIR)/$1

.PHONY: $(BINDIR)/$1
$(BINDIR)/$1: makedirs
	@printf "$(Y)[$1]$(N) "
	odin build $(SRCDIR)/$1 -out:$$@

endef

$(foreach p,$(PRODUCTS),$(eval $(call make-target,$(p))))

# ===================== TOOLS =====================

test-as: as
	@printf "# "
	$(BINDIR)/as test/test.s -out:bin/test.rom -v

test-vm: vm
	@printf "# "
	$(BINDIR)/vm bin/test.rom -vd

verilog: makedirs
	iverilog sim/*.v -o bin/sim

makedirs:
	@$(MKDIR) -p $(BINDIR)

clean:
	@$(RM) -rf $(BINDIR)
