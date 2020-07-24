#######################################
#######################################
#-------------Makefile----------------#
#########------for debug----###########
#######################################
#######################################
#“$<”表示目标文件的挨个值#################
#“$@”表示依赖文件的挨个值#################
VPATH=./include:./compiled
CXX = g++
OBJ = game.o game_core.o kbdec.o model.o size.o
.PHONY: clean
EXEC =Tetris
$(EXEC): $(OBJ)
	cd ./compiled;$(CXX) $(OBJ) -o $(EXEC)
%.o : %.cpp
	$(CXX) -c $< -o ./compiled/$@
clean:
	cd ./compiled;rm -rf $(OBJ) $(EXEC)