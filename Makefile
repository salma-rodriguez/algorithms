CFLAGS=-Wall -I./include -O3
COMPILER=gcc
MKDIR=bin
TARGET=$(MKDIR)/test

VPATH=

define CC
	@echo " [CC] $@" && ${COMPILER}
endef

SOURCES += test.c
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
