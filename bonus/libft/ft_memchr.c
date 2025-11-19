#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	uc;

	uc = (unsigned char)c;
	str = (unsigned char *)s;
	while (n > 0)
	{
		if (*str == uc)
			return ((void *)str);
		str++;
		n--;
	}
	return (NULL);
}
