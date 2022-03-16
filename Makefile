CXX = g++
CXXFLAGS = -Wall -g -lncurses
OBJMODULES = chess_board.o chess_pieces.o log.o

%.o: %.cpp %.hpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

chess: main.cpp $(OBJMODULES)
		$(CXX) $(CXXFLAGS) $^ -o $@
