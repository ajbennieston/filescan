#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int is_printable(char ch);
void hexdump_region(const char *data, size_t length, size_t offset);
int open_file(const char * filename);
void close_file(int file_descriptor);
void *mmap_file(int file_descriptor, size_t fsize);
size_t file_size(int file_descriptor);
void scan_for_string(const char *begin, size_t size, const char *string, size_t context);

int main(int argc, char **argv)
{
    const char *search_string;
    const char *filename;
    int file_descriptor;
    const char *data;
    size_t fsize;
    size_t context = 64;

    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s string filename [context-length]\n", argv[0]);
        return 1;
    }
    search_string = argv[1];
    filename = argv[2];
    if ( argc == 4 )
        context = atoi(argv[3]);

    file_descriptor = open_file(filename);
    if (file_descriptor == -1)
    {
        perror(filename);
        return 1;
    }

    fsize = file_size(file_descriptor);

    data = mmap_file(file_descriptor, fsize);
    if (data == NULL)
    {
        perror(filename);
        return 1;
    }

    scan_for_string(data, fsize, search_string, context);

    close_file(file_descriptor);
}

void hexdump_region(const char *data, size_t length, size_t offset)
{
    const char *end = data + length;
    int count = 0;
    char buffer[17];
    char *bufpos = &buffer[0];
    int printed_chars = 0;

    while (data != end) {
        printf("%016zx: ", offset);

        do {
            if (count % 2 == 0)
                printed_chars += printf("%2.2hhx", *data);
            else
                printed_chars += printf("%2.2hhx ", *data);
            if (is_printable(*data))
                *bufpos = *data;
            else
                *bufpos = '.';

            ++count;
            ++data;
            ++offset;
            ++bufpos;
        } while (count % 16 != 0 && data != end);

        *bufpos = '\0';
        while (printed_chars++ != 40)
            printf(" ");
        printf("  %s\n", buffer);
        bufpos = buffer;
        printed_chars = 0;
    };
}

int open_file(const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY | O_LARGEFILE);
    return fd;
}

size_t file_size(int file_descriptor)
{
    struct stat s;
    if (fstat(file_descriptor, &s) == -1)
        return 0;
    else
        return s.st_size;
}

void *mmap_file(int file_descriptor, size_t fsize)
{
    void *addr;

    if (file_descriptor == -1)
        return NULL;

    addr = mmap(NULL, fsize, PROT_READ, 
            MAP_SHARED, file_descriptor, 0);
    if (addr == MAP_FAILED)
        return NULL;

    return addr;
}

void close_file(int file_descriptor)
{
    close(file_descriptor);
}

void scan_for_string(const char *begin, size_t size, const char *string, size_t context)
{
    size_t bytes_scanned = 0;
    size_t s_len = strlen(string);
    size_t end = size - s_len;
    const char * data = begin;
    while (bytes_scanned <= end)
    {
        if (strncmp(string, data, s_len) == 0)
        {
            printf("---- Offset: %zu (%#016zx) ----\n", 
                    bytes_scanned, bytes_scanned);
            if (bytes_scanned + context < end)
            {
                if (bytes_scanned >= context)
                    hexdump_region(data - context, 2 * context, bytes_scanned - context);
                else
                    hexdump_region(begin, 2 * context, 0);
            }
            else
            {
                if (bytes_scanned >= context)
                    hexdump_region(data - context, size - bytes_scanned + context, bytes_scanned - context);
                else
                    hexdump_region(begin, size, 0);
            }
            printf("\n");
        }
        ++data;
        ++bytes_scanned;
    }
}

int is_printable(char ch)
{
    return isprint(ch) && !isspace(ch);
}
