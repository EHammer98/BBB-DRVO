#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_PATH "/sys/devices/platform/led_extern/led"

void write_led(const char *value) {
    int fd = open(LED_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open led for writing");
        return;
    }

    if (write(fd, value, strlen(value)) == -1) {
        perror("Failed to write value to led");
    }

    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <on|off>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "on") == 0) {
        write_led("1");  // Assuming '1' turns the LED on
    } else if (strcmp(argv[1], "off") == 0) {
        write_led("0");  // Assuming '0' turns the LED off
    } else {
        fprintf(stderr, "Invalid argument. Use 'on' or 'off'.\n");
        return 1;
    }

    return 0;
}
