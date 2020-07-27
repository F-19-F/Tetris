#######################################
#######################################
#-------------Makefile----------------#
########----支持c/c++混编----###########
#######################################
#######################################
#######“$@”表示目标文件的挨个值#########
#######“$<”表示依赖文件的挨个值#########
######请确保compiled文件夹存在#########
###########目前支持一级目录############
.PHONY: clean
VPATH=include:compiled
OUTPATH=./compiled/
FLAGS=


#########以下内容请勿修改###############
CXX = g++
#wildcard命令可以使统配符号生效
SRC=$(wildcard *.c) $(wildcard *.cpp)
#patsubst命令用来改后缀,递归改后缀，先改cpp为o，再将c改为o
OBJ=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))
#DEPEND用来指明各个源文件需要的依赖关系文件
DEPENDS=$(OBJ:.o=.d)
DEPEND=$(EXEC).D
EXEC =Tetris
$(EXEC): $(OBJ)
	cd $(OUTPATH);$(CXX) $(FLAGS) $(OBJ) -o $(EXEC)
%.o: %.cpp
	$(CXX) -c $< -o $(OUTPATH)$@
%.o: %.c
	$(CXX) -c $< -o $(OUTPATH)$@
include $(DEPEND)
$(DEPEND) : $(DEPENDS)
	@rm -rf $(DEPEND);cd $(OUTPATH);cat $(DEPENDS) >>../$(DEPEND)
#下面的命令，第一行删除前面的旧.d文件(如果存在的话)，并生成临时文件，在末尾切换到输出目录
#第二行在g++的输出的基础上添加一个对应.d的目标文件，并将修改后的内容储存至.d文件中
%.d : %.cpp
	@rm -rf $(OUTPATH)$@;$(CXX) -MM $< > $(OUTPATH)$@.temp;cd $(OUTPATH);\
	sed 's,$(patsubst %.cpp,%.o,$<),$(patsubst %.cpp,%.o,$<) $@ ,g' <$@.temp > $@;rm -rf $(patsubst %.cpp,%.d,$<).temp
%.d :%.c
	@rm -rf $(OUTPATH)$@;$(CXX) -MM $< > $(OUTPATH)$@.temp;cd $(OUTPATH);\
	sed 's,$(patsubst %.c,%.o,$<),$(patsubst %.c,%.o,$<) $@ ,g' <$@.temp > $@;rm -rf $(patsubst %.c,%.d,$<).temp
clean:
	rm -rf $(DEPEND);cd ./compiled;rm -rf $(OBJ) $(EXEC) $(DEPENDS)