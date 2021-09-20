PRODUCT_ONE_SRCS = $(wildcard $(SRCDIR)/$(PRODUCT_ONE_NAME)/*.c)
PRODUCT_ONE_OBJS = $(addprefix $(PRODUCT_ONE_OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(PRODUCT_ONE_SRCS))))
PRODUCT_ONE_INCDIR = $(INCDIR)/$(PRODUCT_ONE_NAME)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(PRODUCT_ONE_OBJDIR)/%.o: $(SRCDIR)/$(PRODUCT_ONE_NAME)/%.c
	@printf "$(YELLOW)[$(PRODUCT_ONE_NAME)]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -I $(PRODUCT_ONE_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(PRODUCT_ONE_BIN): $(PRODUCT_ONE_OBJS)
	@printf "$(YELLOW)[$(PRODUCT_ONE_NAME)]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(OBJS) $(LDFLAGS)
	@printf "\b\b done!\n"