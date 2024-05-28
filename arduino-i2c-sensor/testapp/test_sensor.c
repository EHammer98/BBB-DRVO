#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buf[256];

    fd = open("/sys/class/i2c-dev/i2c-1/device/1-0008/distance", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    read(fd, buf, sizeof(buf));
    printf("Distance: %s cm\n", buf);

    close(fd);
    return 0;
}
