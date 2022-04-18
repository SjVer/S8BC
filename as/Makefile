AS_SRCDIR = $(AS_DIR)/src
AS_INCDIR = $(AS_DIR)/include

AS_SRC = $(shell find $(AS_SRCDIR)/ -type f -name '*$(EXT)')
AS_OBJ = $(AS_SRC:$(AS_SRCDIR)/%$(EXT)=$(AS_OBJDIR)/%.o)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(AS_OBJDIR)/%.o: $(AS_SRCDIR)/%$(EXT)
	@printf "$(YELLOW)[as]$(NC) compiling $(PURPLE)$(notdir $<)$(NC)..."
	@$(CC) $(CXXFLAGS) -I$(AS_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(AS_BIN): $(AS_OBJ)
	@printf "$(YELLOW)[as]$(NC) compiling final product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@printf "\b\b done!\n"