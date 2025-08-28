#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;

	if (s1)
		len = ft_strlen(s1);
	else
		len = 0;
	if (s2)
		len += ft_strlen(s2);
	else
		len += 0;
	str = ft_calloc(len + 1, 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, ft_strlen(s1));
	ft_memcpy(str + ft_strlen(s1), s2, ft_strlen(s2));
	return (str);
}
