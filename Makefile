##
## Practical aliases
##
BUILD		= ./build
BIN		= ./bin
CC		= g++
MKDIR		= mkdir -p
RM		= rm -rf
SRCS_DIR	= ./srcs

##
## code like a boss
##
CPPFLAGS	+= -W -Wextra -Wall -O2

##
## header location
##
CPPFLAGS 	+= -I./srcs/

##
## c++ version
##
CPPFLAGS	+= -std=c++11

##
## libraries
##

LIBS		= -lSDL2 -lSDL2_mixer -lSDL2_ttf -lm

##
## compilation options
##
PRGRM		= MinimaTetris

SRCS	= $(wildcard $(SRCS_DIR)/*.cpp)

OBJS	= $(patsubst $(SRCS_DIR)/%.cpp,$(BUILD)/%.o,$(SRCS))

##
## compilation
##
all:		dir $(BIN)/$(PRGRM)

dir:
		$(MKDIR) $(BIN) $(BUILD)

$(BIN)/$(PRGRM):	$(OBJS)
			$(CC) $(OBJS) -o $@ $(LIBS)

$(OBJS):	$(BUILD)/%.o : $(SRCS_DIR)/%.cpp
		$(CC) $(CPPFLAGS) -c $< -o $@

install:
	sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
	$(MAKE) all

##
## Clean
##
RM_OBJS	= $(BUILD)

RM_BIN	= $(BIN)

clean:
	$(RM) $(RM_OBJS)

fclean:	clean
	$(RM) $(RM_BIN)

re:	fclean all

##
## avoid problems
##
.PHONY:	all $(BIN)/$(PRGRM) clean dir fclean install $(OBJS) re
