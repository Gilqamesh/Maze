EXECUTABLE = mazegenerator

SOURCES = $(shell find . -name "*.c")
HEADERS = $(shell find . -name "*.h")

OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
DEPENDS = $(patsubst %.c, %.d, $(SOURCES))

CFLAGS = -g -Wall -Wextra -Werror
LFLAGS = 

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

.phony: all
all: $(EXECUTABLE)

.phony: clean
clean:
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

.phony: re
re: clean all
