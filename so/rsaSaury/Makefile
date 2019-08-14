COMP := g++

TARGETDIR := bin
TARGET := librsaSaury.so
INCLUDES := -I.

COMPILE_FLAGS :=-shared -fPIC -O2 -Wall -Wextra -lssl -lcrypto

#源文件
SRCS := main.cpp 

#.o文件
OBJS = main.o 

#将所有.o文件编译为目标文件    编译.so需要加 -shared -fPIC
all:$(OBJS)  
	$(COMP) $(COMPILE_FLAGS) -o $(TARGET) $(OBJS)

    	#将librsaSaury.so复制到当前目录下的Bin目录下，所以提前创建一个Bin文件夹
	cp $(TARGET) ./$(TARGETDIR)/

    	#删除  .o  和so文件
	$(RM) *.o $(TARGET) #删除当前目录下的.o文件和.so

$(OBJS):
	$(COMP) -fPIC $(INCLUDES) -c $(SRCS) -o $(OBJS)

# make  clean
	$(RM) *.o $(TARGET)
