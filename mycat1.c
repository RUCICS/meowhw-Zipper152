#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to open file %s\n", argv[1]);
        exit(1);
    }

    char c;
    ssize_t bytes_read;
    while (bytes_read = (read(fd, &c, 1)) > 0)
    {
        if (write(STDOUT_FILENO, &c, 1) != 1)
        {
            fprintf(stderr, "Failed to write\n");
            close(fd);
            exit(1);
        }
    }
    if (bytes_read == -1)
    {
        fprintf(stderr, "Failed to read\n");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}