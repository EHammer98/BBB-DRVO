CC=gcc
CFLAGS=-Wall
TARGET=test_sensor

all: $(TARGET)

$(TARGET): test_sensor.c
	$(CC) $(CFLAGS) test_sensor.c -o $(TARGET)

clean:
	rm -f $(TARGET)
