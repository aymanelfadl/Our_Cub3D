#include "parser_bonus.h"

char *get_bonus_error_msg(int code)
{
    static char *bonus_errors[] = {
        [ERR_INVALID_DOOR] = "Door placed in invalid position",
        [ERR_INVALID_SPRITE] = "Sprite placed in invalid position",
        [ERR_DOOR_TEXTURE] = "Invalid door texture file",
        [ERR_SPRITE_TEXTURE] = "Invalid sprite texture file",
        [ERR_BONUS_CONFIG] = "Invalid bonus configuration"
    };
    
    if (code >= ERR_INVALID_DOOR && code <= ERR_BONUS_CONFIG)
        return (bonus_errors[code - ERR_INVALID_DOOR]);
    
    /* Fall back to mandatory errors */
    return (get_error_msg(code));
}