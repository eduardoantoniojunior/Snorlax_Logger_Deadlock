CC = gcc
CFLAGS = -Wall -pthread
TARGET = snorlax_log.out

all: $(TARGET)

$(TARGET): snorlax_log.c
	$(CC) $(CFLAGS) -o $(TARGET) snorlax_log.c

run: $(TARGET)
	./$(TARGET)
	@echo "\n[LOG] Conteúdo de trace.log:"
	@cat trace.log

clean:
	rm -f $(TARGET) trace.log
