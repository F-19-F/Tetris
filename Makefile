#######################################
#######################################
#-------------Makefile----------------#
#########------for debug----###########
#######################################
#######################################
#“$@”表示目标文件的挨个值#################
#“$<”表示依赖文件的挨个值#################
VPATH=include:/compiled
CXX = g++
OUTPATH=compiled/
#wildcard命令可以使统配符号生效
SRC=$(wildcard *.cpp)
#patsubst命令用来改后缀
OBJ=$(patsubst %.cpp,%.o,$(SRC))
#DEPEND用来指明各个源文件需要的依赖关系文件
DEPEND=$(patsubst %.cpp,%.d,$(SRC))
#生成临时文件
TEMP=$(patsubst %.cpp,%.temp,$(SRC))
.PHONY: clean
EXEC =Tetris
$(EXEC):$(DEPEND) $(OBJ)
	cd ./compiled;$(CXX) $(OBJ) -o $(EXEC)
#就是将g++ -MM的输出加上.d自身的依赖
%.d : %.temp
	cd $(OUTPATH);sed 's,\.o[ :]*,$(patsubst %.temp,,$<).o $@ : ,g' <$< > $@;rm -rf $<
%.temp : %.cpp
	$(CXX) -MM $< > $(OUTPATH)$@
%.o : %.cpp
	$(CXX) -c $< -o $(OUTPATH)$@
include $(DEPEND)
test:
	@echo $(SRC);echo $(TEMP)
clean:
	cd ./compiled;rm -rf $(OBJ) $(EXEC) $(TEMP) $(DEPEND)