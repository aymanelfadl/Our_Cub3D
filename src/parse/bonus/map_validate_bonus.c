#include "parser_internal.h"
#include "parser_bonus.h"

static int is_allowed_char_bonus(char c)
{
    // Group similar checks with OR operator
    if (c == '0' || c == '1' || c == ' ')
        return (1);
    
    if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
        return (1);
    
    if (c == 'D' || c == '2')
        return (1);
    
    return (0);
}

int parser_validate_map_chars_bonus(const char *line)
{
    int i;
    
    if (!line)
        return (PARSE_ERR_MAP_INVALID_CHAR);
    
    i = 0;
    while (line[i])
    {
        // Positive check instead of negative
        if (is_allowed_char_bonus(line[i]) == 0)
            return (PARSE_ERR_MAP_INVALID_CHAR);
        i++;
    }
    
    return (PARSE_OK);
}