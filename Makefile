TARGET := out/Main.exe
OBJS := out/Main.o out/Window.o

C_FLAGS := -Wall -g
INCLUDE := -Isrc/include -Iinclude
LIBRARIES := -Lsrc/lib -lmingw32 -lSDL2main -lSDL2

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(C_FLAGS) $(INCLUDE) -o $@ $^ $(LIBRARIES)

out/%.o: src/%.cpp
	g++ $(C_FLAGS) $(INCLUDE) -c -o $@ $^

run: $(TARGET)
	@call out\Main.exe