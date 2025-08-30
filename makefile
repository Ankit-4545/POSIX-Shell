CXX = g++
CXXFLAGS = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

main: posix.cpp ls.cpp
	$(CXX) posix.cpp ls.cpp $(CXXFLAGS) $(LDFLAGS) 
