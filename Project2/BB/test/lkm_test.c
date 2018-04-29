/*
 * lkm_test.c
 * Quick test of controller LKM
 * 
 * Author: Connor Shapiro
 */

#include <stdint.h>
#include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FAILURE     (-1)
#define READ_INIT   (1)
#define TEST_VAL    (0xAA55)
#define TEST_DEV    ("/dev/controller")

int main(int argc, char *argv[])
{
    uint16_t test_value, read_value;
    int fd;

    test_value = TEST_VAL;
    read_value = READ_INIT;

    fd = open(TEST_DEV, O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open.\n");
        return FAILURE;
    }

    read(fd, &read_value, sizeof(read_value));
    printf("Initial device value is %u\n", read_value);

    write(fd, &test_value, sizeof(test_value));
    read(fd, &read_value, sizeof(read_value));
    printf("New device value is %u\n", read_value);

    close(fd);

    return 0;


}