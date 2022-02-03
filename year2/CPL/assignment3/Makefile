INC_DIR = include
SRC_DIR = src
SOURCES = $(sort $(shell find $(SRC_DIR) -name '*.cc'))
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = parser
CXX = g++
CFLAGS = -g -Wall -Wextra -Wpedantic -std=c++17
CPPFLAGS = $(addprefix -I, $(INC_DIR))
.PHONY: all clean debug release
release: CFLAGS += -O0 -DNDEBUG
release: all
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all
all: $(TARGET)
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $@ $^
-include $(DEPS)
%.o: %.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<
