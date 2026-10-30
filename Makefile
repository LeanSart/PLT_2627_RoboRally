CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# All classes of the project (main excluded)
CLASSES = $(filter-out src/prj/UMLDiagram/main.cpp,$(wildcard src/prj/UMLDiagram/*.cpp))
HEADERS = $(wildcard src/prj/UMLDiagram/*.h)

# Executable par fichier tests/test_*.cpp
TEST_SRCS = $(wildcard test/test_*.cpp)
TESTS = $(patsubst test/%.cpp,bin/%.exe,$(TEST_SRCS))

.PHONY: all run test sanitize valgrind clean

all: bin/main.exe

bin/main.exe: src/prj/UMLDiagram/main.cpp $(CLASSES) $(HEADERS)
	# mkdir bin
	$(CXX) $(CXXFLAGS) src/prj/UMLDiagram/main.cpp $(CLASSES) -o $@ $(SFML_LIBS)

# Regle generique : bin/test_xxx.exe est construit depuis tests/test_xxx.cpp
bin/%.exe: src/prj/tests/%.cpp $(wildcard test/*.hpp) $(CLASSES) $(HEADERS)
	mkdir bin
	$(CXX) $(CXXFLAGS) -Itests $< $(CLASSES) -o $@ $(SFML_LIBS)

run: bin/main.exe
	./bin/main.exe

test: $(TESTS)
	@for t in $(TESTS); do echo "=== $$t ==="; ./$$t || exit 1; done

# Verification des fuites memoire et des comportements indefinis
sanitize: $(CLASSES) $(HEADERS)
	mkdir bin
	@for src in $(TEST_SRCS); do \
		out=bin/sanitize_$$(basename $$src .cpp).exe; \
		echo "=== build $$out ==="; \
		$(CXX) $(CXXFLAGS) -fsanitize=address,undefined -Itests $$src $(CLASSES) -o $$out $(SFML_LIBS) || exit 1; \
		./$$out || exit 1; \
	done

valgrind: $(TESTS)
	@for t in $(TESTS); do \
		echo "=== valgrind $$t ==="; \
		valgrind --leak-check=full --error-exitcode=1 ./$$t || exit 1; \
	done

ifeq ($(OS),Windows_NT)
    RM = -rmdir /s /q
else
    RM = rm -rf
endif

clean:
	$(RM) bin/main.exe