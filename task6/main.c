#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Item {
    int x;
    char name[32];
};

int main(int argc, char **argv)
{
    int count = (argc > 1) ? atoi(argv[1]) : -1;
    if(count < 1 || count >= INT_MAX)
        return 1;
    size_t bytes = (size_t)count * sizeof(struct Item);

    struct Item *ptr = malloc(4 * sizeof(struct Item));
    if (!ptr) return 1;
    printf("count=%d, bytes=%zu\n", count, bytes);

    ptr = realloc(ptr, bytes);
    if (!ptr) {
        printf("realloc failed\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        ptr[i].x = i * 2;
        strncpy(ptr[i].name, "example", sizeof(ptr[i].name - 1));
    }

    printf("done: %d items, first=%d\n", count, ptr[0].x);
    free(ptr);
    return 0;
}

