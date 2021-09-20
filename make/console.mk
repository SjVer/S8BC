CONSOLE_SRCS = $(wildcard $(SRCDIR)/console/*.c)
CONSOLE_OBJS = $(addprefix $(CONSOLE_OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(CONSOLE_SRCS))))
CONSOLE_INCDIR = $(INCDIR)/$(CONSOLE_NAME)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(CONSOLE_OBJDIR)/%.o: $(SRCDIR)/console/%.c
	@printf "$(YELLOW)[console ]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -I $(CONSOLE_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(CONSOLE_BIN): $(CONSOLE_OBJS)
	@printf "$(YELLOW)[console ]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(OBJS) $(LDFLAGS)
	@printf "\b\b done!\n"