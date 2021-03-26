#include <stdio.h>
#include <string.h>

static char* basename(char* name)
{
    char* p = strrchr(name, '/');

    if (p)
        return p + 1;

    return name;
}

void list_version(char* name)
{
    fprintf(stderr, "%s [%s]\n", basename(name), BUILD_VERSION);
}
