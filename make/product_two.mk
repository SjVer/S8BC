PRODUCT_TWO_SRCS = $(wildcard $(SRCDIR)/$(PRODUCT_TWO_NAME)/*.c)
PRODUCT_TWO_OBJS = $(addprefix $(PRODUCT_TWO_OBJDIR)/, $(notdir $(patsubst %.c, %.o, $(PRODUCT_TWO_SRCS))))
PRODUCT_TWO_INCDIR = $(INCDIR)/$(PRODUCT_TWO_NAME)

YELLOW = \033[0;33m$$(tput bold)
PURPLE = \033[1;35m$$(tput bold)
NC= \033[0m$$(tput sgr0)

$(PRODUCT_TWO_OBJDIR)/%.o: $(SRCDIR)/$(PRODUCT_TWO_NAME)/%.c
	@printf "$(YELLOW)[$(PRODUCT_TWO_NAME)]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into $(YELLOW)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -I $(INCDIR) -I $(PRODUCT_TWO_INCDIR) -o $@ -c $<
	@printf "\b\b done!\n"

$(PRODUCT_TWO_BIN): $(PRODUCT_TWO_OBJS)
	@printf "$(YELLOW)[$(PRODUCT_TWO_NAME)]$(NC) compiling $(YELLOW)$(notdir $<)$(NC) into product $(PURPLE)$(notdir $@)$(NC)..."
	@$(CC) $(CXXFLAGS) -o $@ $^ $(OBJS) $(LDFLAGS)
	@printf "\b\b done!\n"