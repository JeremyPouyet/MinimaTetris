##
## Practical aliases
##
CC		= g++
RM		= rm -rf
SRCS_DIR	= ./srcs

##
## code like a boss
##
CPPFLAGS	+= -W -Wextra -Wall

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

LIBS		= -lSDL2 -lSDL2_mixer -lSDL2_ttf

##
## compilation options
##
PRGRM		= MinimaTetris

SRCS	= $(SRCS_DIR)/main.cpp \
	$(SRCS_DIR)/Rendering.cpp \
	$(SRCS_DIR)/Tetromino.cpp \
	$(SRCS_DIR)/AudioManager.cpp \
	$(SRCS_DIR)/Tetris.cpp

OBJS	= $(SRCS:.cpp=.o)

##
## compilation
##
all:		$(PRGRM)

prgrm:		$(PRGRM)

$(PRGRM):	$(OBJS)
		$(CC) -o $@ $(OBJS) $(LIBS)

##
## Clean
##
RM_OBJS	= $(OBJS)

RM_BINS	= $(PRGRM)

clean:
	$(RM) $(RM_OBJS)

fclean:	clean
	$(RM) $(RM_BINS)

re:	fclean all

##
## avoid problems
##
.PHONY:	all clean fclean re rclean prgrm $(PRGRM)
