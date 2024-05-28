#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>  // Include this to use 'errno'

int main() {
    int fd;
    char buf[256];

    fd = open("/sys/class/i2c-dev/i2c-1/device/1-0008/distance", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;  // Now 'errno' is recognized
    }

    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return errno;
    }
    printf("Distance: %s cm\n", buf);

    close(fd);
    return 0;
}
