#######################################
#######################################
#-------------Makefile----------------#
#########------for debug----###########
#######################################
#######################################
CXX = g++
OBJ = game.o game_core.o kbdec.o model.o size.o
.PHONY: clean
EXEC =Tetris
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o Tetris

clean:
	rm -rf $(OBJ) $(EXEC)