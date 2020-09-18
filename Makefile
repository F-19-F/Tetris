#######################################
#######################################
#-------------Makefile----------------#
#######--------Tetris-------###########
#######################################
#######################################
.PHONY: clean
VPATH=./include:./compiled
OUTPATH=./compiled/
CXX = g++
SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))
DEPENDS=$(patsubst %.cpp,%.d,$(SRC))
DEPEND=$(EXEC).D
EXEC =Tetris
$(EXEC): $(OBJ)
	cd $(OUTPATH);$(CXX) -pthread $(OBJ) -o $(EXEC)
%.o: %.cpp
	$(CXX) -c $< -o $(OUTPATH)$@
include $(DEPEND)
#在g++ -MM的输出中添加.d的目标,以实现自动创建依赖
$(DEPEND) : $(DEPENDS)
	@rm -rf $(DEPEND);cd $(OUTPATH);cat $(DEPENDS) >>../$(DEPEND)
%.d : %.cpp
	@rm -rf $(OUTPATH)$@;$(CXX) -MM $< > $(OUTPATH)$@.temp;cd $(OUTPATH);\
	sed 's,$(patsubst %.cpp,%.o,$<),$(patsubst %.cpp,%.o,$<) $@ ,g' <$@.temp > $@;rm -rf $(patsubst %.cpp,%.d,$<).temp
test :
	echo $(SRC)
clean:
	rm -rf $(DEPEND);cd ./compiled;rm -rf $(OBJ) $(EXEC) $(DEPENDS)