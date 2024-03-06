# CXXFLAGS = -Ideps/include -std=c++0x
# LXXFLAGS = -Ldeps/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# output: main.o window.o rect.o
#     g++ main.o window.o rect.o -o output $(LXXFLAGS) 

# main.o: main.cpp 
#     g++ main.cpp -c $(CXXFLAGS) 

# window.o: window.cpp Window.h
#     g++ -c window.cpp $(CXXFLAGS)

# rect.o: rect.cpp rect.h
    # g++ -c rect.cpp $(CXXFLAGS) 