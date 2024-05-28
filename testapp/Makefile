CC=gcc
CFLAGS=-Wall

# Define the binary output name
TARGET=led_control

all: $(TARGET)

$(TARGET): led_control.c
	$(CC) $(CFLAGS) led_control.c -o $(TARGET)

clean:
	rm -f $(TARGET)
