#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

int is_valid_extension(const char *path)
{
    int len;

    if (!path)
        return (ERR_ARGS);
    len = 0;
    while (path[len])
        len++;
    if (len < 4)
        return (1);  
    if (path[len - 4] == '.' && path[len - 3] == 'c' &&
        path[len - 2] == 'u' && path[len - 1] == 'b')
        return (0);
    return (1);
}
