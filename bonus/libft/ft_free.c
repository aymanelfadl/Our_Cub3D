#include "libft.h"

void	ft_free(void **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
}
