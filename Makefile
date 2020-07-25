#######################################
#######################################
#-------------Makefile----------------#
#######--------Tetris-------###########
#######################################
#######################################
.PHONY: clean
VPATH=include:compiled
OUTPATH=./compiled/
CXX = g++
SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))
DEPEND=$(patsubst %.cpp,%.d,$(SRC))
EXEC =Tetris
$(EXEC): $(OBJ)
	cd $(OUTPATH);$(CXX) $(OBJ) -o $(EXEC)
%.o: %.cpp
	$(CXX) -c $< -o $(OUTPATH)$@
include $(DEPEND)
#在g++ -MM的输出中添加.d的目标,以实现自动创建依赖
%.d : %.cpp
	@rm -rf $@;$(CXX) -MM $< > $(OUTPATH)$@.temp;cd $(OUTPATH);\
	sed 's,$(patsubst %.cpp,%.o,$<),$(patsubst %.cpp,%.o,$<) $@ ,g' <$@.temp > $@;rm -rf $(patsubst %.cpp,%.d,$<).temp
clean:
	cd ./compiled;rm -rf $(OBJ) $(EXEC) $(DEPEND)