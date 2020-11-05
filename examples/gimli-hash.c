#include <lithium/gimli_hash.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _WIN32
#define PLAT_FLAGS O_BINARY
#else
#define PLAT_FLAGS 0
#endif

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <input-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned char output[32];

    gimli_hash_state state;
    gimli_hash_init(&state);

    int msgfd = open(argv[1], O_RDONLY | PLAT_FLAGS);
    if (msgfd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    static unsigned char buf[4096];
    ssize_t nread;

    while ((nread = read(msgfd, buf, sizeof buf)) > 0)
    {
        gimli_hash_update(&state, buf, (size_t)nread);
    }

    if (nread < 0)
    {
        perror("read");
        return EXIT_FAILURE;
    }
    close(msgfd);

    gimli_hash_final(&state, output, sizeof output);

    for (size_t i = 0; i < sizeof output; ++i)
    {
        printf("%02hhx", output[i]);
    }
    printf("\n");
}
