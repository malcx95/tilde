CXX=clang++

tilde: tilde.cpp
	$(CXX) tilde.cpp -o tilde -lsfml-graphics -lsfml-window -lsfml-system
