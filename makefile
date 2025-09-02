CXX = g++
CXXFLAGS = -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -lncurses

main: posix.cpp ls.cpp search.cpp bg_fg.cpp pipe.cpp io.cpp
	$(CXX) posix.cpp ls.cpp search.cpp bg_fg.cpp pipe.cpp io.cpp $(CXXFLAGS) $(LDFLAGS) 
