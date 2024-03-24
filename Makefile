TARGET := out/Main.exe
OBJS := out/Main.o out/Window.o out/Renderer.o out/Vector2.o out/Map.o

C_FLAGS := -Wall -g
INCLUDE := -Isrc/include -Iinclude

ifeq ($(OS),Windows_NT)
	LIBRARIES := -Lsrc/lib -lmingw32 -lSDL2main -lSDL2
else
	LIBRARIES := -lSDL2 -lSDL2_gfx
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(C_FLAGS) $(INCLUDE) -o $@ $^ $(LIBRARIES)

out/%.o: src/%.cpp
	g++ $(C_FLAGS) $(INCLUDE) -c -o $@ $^

ifeq ($(OS),Windows_NT)

run: $(TARGET)
	call out\Main.exe

else

run: $(TARGET)
	$(TARGET)

clean:
	rm -f out/*

endif