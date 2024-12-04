compiler = g++

flags = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal 			\
		-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 				\
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe 					\
		-fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body 					\
		-Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op 							\
		-Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith \
		-Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 	\
		-Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

includes = ./inc/Errors.h ./inc/Utils.h ./inc/acinator.h ./inc/ErrConsts.h Stack/inc/Stack.h Stack/src/Defines.h

sources = ./src/acinator.cpp ./src/Utils.cpp Stack/src/Stack.cpp

objects = $(sources:.cpp=.o)

execute = Acinator.exe

all: $(sources) $(execute) run

run:
	rm -rf ./Logs/Dump.txt
	rm -rf ./Logs/GraphDump.dot
	rm -rf ./Logs/GraphDump.svg
	rm -rf ./Logs/GraphDump.html
	@$(execute)
	@echo -e -en '\E[;32m'"\n\t\t\t\033[1mRunning...\t\t\t\033[0m\n\n"
	ren ../Acinator/Logs/GraphDump.svg GraphDump.html
	
$(execute): $(objects) 
	$(compiler) $(objects) -o $@

.cpp.o: $(includes)
	@$(compiler) $(flags) $< -o $@

clean:
	rm -rf ./src/*.o
	rm -rf *.exe