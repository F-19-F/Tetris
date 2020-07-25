#######################################
#######################################
#-------------Makefile----------------#
#########------for debug----###########
#######################################
#######################################
#“$@”表示目标文件的挨个值#################
#“$<”表示依赖文件的挨个值#################
VPATH=include:compiled
CXX = g++
OUTPATH=./compiled/
#wildcard命令可以使统配符号生效
SRC=$(wildcard *.cpp)
#patsubst命令用来改后缀
OBJ=$(patsubst %.cpp,%.o,$(SRC))
#DEPEND用来指明各个源文件需要的依赖关系文件
DEPEND=$(patsubst %.cpp,%.d,$(SRC))
.PHONY: clean
EXEC =Tetris
$(EXEC): $(OBJ)
	cd $(OUTPATH);$(CXX) $(OBJ) -o $(EXEC)
%.o: %.cpp
	$(CXX) -c $< -o $(OUTPATH)$@
#就是将g++ -MM的输出加上.d自身的依赖
include $(DEPEND)
#下面的命令，第一行删除前面的旧.d文件(如果存在的话)，并生成临时文件，在末尾切换到输出目录
#第二行在g++的输出的基础上添加一个对应.d的目标文件，并将修改后的内容储存至.d文件中
%.d : %.cpp
	rm -rf $@;$(CXX) -MM $< > $(OUTPATH)$@.temp;cd $(OUTPATH);\
	sed 's,$(patsubst %.cpp,%.o,$<),$(patsubst %.cpp,%.o,$<) $@ ,g' <$@.temp > $@;rm -rf $(patsubst %.cpp,%.d,$<).temp
test:
	@echo $(SRC)
clean:
	cd ./compiled;rm -rf $(OBJ) $(EXEC) $(DEPEND)