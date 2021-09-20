COMPILER_SRCS = $(wildcard $(SRCDIR)/compiler/*.c)
COMPILER_OBJS = $(addprefix $(COMPILER_OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(COMPILER_SRCS))))
COMPILER_INCDIR = $(INCDIR)/$(COMPILER_NAME)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(COMPILER_OBJDIR)/%.o: $(SRCDIR)/compiler/%.c
	@printf "$(YELLOW)[compiler]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -I $(COMPILER_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(COMPILER_BIN): $(COMPILER_OBJS)
	@printf "$(YELLOW)[compiler]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(OBJS) $(LDFLAGS)
	@printf "\b\b done!\n"