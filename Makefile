COMPILER=gcc
MKDIR=bin
TARGET=$(MKDIR)/test
OPTIMIZE=-g
VPATH=list array

CFLAGS=-Wall -Wextra -I./include $(OPTIMIZE)

define CC
	@echo " [CC] $@" && $(COMPILER)
endef

# test
SOURCES += test.c
# array
SOURCES += array.c
SOURCES += search.c
# list
SOURCES += fifo.c
SOURCES += dequeue.c
SOURCES += linked_list.c

$(shell `mkdir -p $(MKDIR)`)

OBJECTS	= $(addprefix $(MKDIR)/,${SOURCES:.c=.o})

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/test

$(MKDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	${RM} -r $(MKDIR)

.PHONY: clean
