###
# Manage variables for compilation
##
PROJECT = Routing.exe

COMP 	= gcc
CFLAGS 	= -Wall
LDFLAGS = 

OBJDIR 	= obj
SRCDIR 	= src

SRCS 	= $(wildcard $(SRCDIR)/*.c)
OBJS 	= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))	

###
# Compiling sources
##
all: $(PROJECT)

$(PROJECT): buildrepo $(OBJS)
	$(COMP) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMP) $(CFLAGS) -c $< -o $@

###
# Cleaning for compilation
##
clean:
	rm -f $(OBJDIR)/*.o
	
###
# Build repository
##
buildrepo:
	@mkdir -p $(OBJDIR)