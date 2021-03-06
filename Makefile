COMPILER=gcc
MKDIR=bin
TARGET=$(MKDIR)/test
OPTIMIZE=-g
VPATH=list array math map
RMFLAGS=-r $(MKDIR)

CFLAGS=-Wall -Wextra -I./include $(OPTIMIZE)

define CC
	@echo " [CC] $@" && $(COMPILER)
endef

define RR
	@echo " [RM] $(MKDIR)" && ${RM}
endef

# test
SOURCES += test.c

# array
SOURCES += array.c
SOURCES += search.c
SOURCES += sort.c
SOURCES += quick-uf.c

# math
SOURCES += math.c

# list
SOURCES += bag.c
SOURCES += fifo.c
SOURCES += lifo.c
SOURCES += dequeue.c
SOURCES += linked_list.c

# map
SOURCES += map.c

$(shell `mkdir -p $(MKDIR)`)

OBJECTS	= $(addprefix $(MKDIR)/,${SOURCES:.c=.o})

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/test

$(MKDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RR) -r $(MKDIR)

.PHONY: clean
