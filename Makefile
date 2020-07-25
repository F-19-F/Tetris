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
#wildcard命令可以使统配符号生效
SRC=$(wildcard *.cpp)
#patsubst命令用来改后缀
OBJ=$(patsubst %.cpp,%.o,$(SRC))
#DEPEND用来指明各个源文件需要的依赖关系文件
DEPEND=$(patsubst %.cpp,%.d,$(SRC))
.PHONY: clean
EXEC =Tetris
$(EXEC):$(DEPEND) $(OBJ)
	cd ./compiled;$(CXX) $(OBJ) -o $(EXEC)
%.d : %.cpp
	$(CXX) -MM $< > ./compiled/$@;
%.o : %.cpp
	$(CXX) -c $< -o ./compiled/$@
clean:
	cd ./compiled;rm -rf $(OBJ) $(EXEC)