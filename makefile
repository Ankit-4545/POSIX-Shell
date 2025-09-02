CXX = g++
CXXFLAGS = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

main: posix.cpp ls.cpp search.cpp bg_fg.cpp
	$(CXX) posix.cpp ls.cpp search.cpp bg_fg.cpp $(CXXFLAGS) $(LDFLAGS) 
