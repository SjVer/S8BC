VM_SRCDIR = $(VM_DIR)/src
VM_INCDIR = $(VM_DIR)/include

VM_SRC = $(shell find $(VM_SRCDIR)/ -type f -name '*$(EXT)')
VM_OBJ = $(VM_SRC:$(VM_SRCDIR)/%$(EXT)=$(VM_OBJDIR)/%.o)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(VM_OBJDIR)/%.o: $(VM_SRCDIR)/%$(EXT)
	@printf "$(YELLOW)[vm]$(NC) compiling $(PURPLE)$(notdir $<)$(NC)..."
	@$(CC) $(CXXFLAGS) -I$(VM_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(VM_BIN): $(VM_OBJ)
	@printf "$(YELLOW)[vm]$(NC) compiling final product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@printf "\b\b done!\n"