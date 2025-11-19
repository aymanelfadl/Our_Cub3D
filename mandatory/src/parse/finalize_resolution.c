#include "parser_internal.h"

void    parser_finalize_resolution(t_config *cfg)
{
    if (!cfg)
        return;
    if (cfg->resolution_width <= 0)
        cfg->resolution_width = WINDOW_WIDTH;
    if (cfg->resolution_height <= 0)
        cfg->resolution_height = WINDOW_HEIGHT;
    cfg->have_resolution = 1;
}
