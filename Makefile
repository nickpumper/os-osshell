CXX= g++
CXXFLAGS= -std=c++11

INCLUDE= 
LIB= 

OBJDIR= obj
SRCDIR= src
BINDIR= bin

OBJS= $(addprefix $(OBJDIR)/, osshell.o)
EXEC= $(addprefix $(BINDIR)/, osshell)

mkdirs:= $(shell mkdir -p $(OBJDIR) $(BINDIR))

# BUILD EVERYTHING
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDE)


# REMOVE OLD FILES
clean:
	rm -f $(OBJS) $(EXEC)

test: 
	g++ -o ./obj/tests.o -c -std=c++11 ./src/tests.cpp 
	g++ -o ./obj/osshell.o -c -std=c++11 ./src/osshell.cpp 
	g++ -o ./bin/test  ./obj/tests.o ./obj/osshell.o
