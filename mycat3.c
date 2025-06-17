#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>

char *align_malloc(size_t size, size_t alignment)
{
    void *ptr;
    if (posix_memalign(&ptr, alignment, size) != 0)
    {
        return NULL;
    }
    return (char *)ptr;
}

void align_free(void *ptr)
{
    free(ptr);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to open file %s\n", argv[1]);
        exit(1);
    }

    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
    {
        fprintf(stderr, "Failed to get pagesize, using default 4KB\n");
        page_size = 4096;
    }

    char *buffer = align_malloc(page_size, page_size);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to malloc\n");
        close(fd);
        exit(1);
    }

    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, page_size)) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
        {
            fprintf(stderr, "Failed to write\n");
            align_free(buffer);
            close(fd);
            exit(1);
        }
    }
    if (bytes_read == -1)
    {
        fprintf(stderr, "Failed to read\n");
        align_free(buffer);
        close(fd);
        exit(1);
    }

    align_free(buffer);
    close(fd);
    return 0;
}