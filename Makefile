TARGET := out/Main.exe
YAML_TARGET := out/Yaml.o
OBJS := out/Main.o out/Window.o out/Renderer.o out/Vector2.o out/Map.o out/Simulation.o $(YAML_TARGET)

C_FLAGS := -Wall -pedantic -g
INCLUDE := -Isrc/include -Iinclude

ifeq ($(OS),Windows_NT)
	LIBRARIES := -Lsrc/lib -lmingw32 -lSDL2main -lSDL2
else
	LIBRARIES := -lSDL2 -lSDL2_gfx
endif



all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(C_FLAGS) $(INCLUDE) -o $@ $^ $(LIBRARIES)

$(YAML_TARGET): src/include/yaml/Yaml.cpp
	g++ $(C_FLAGS) -c -o $@ $^

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