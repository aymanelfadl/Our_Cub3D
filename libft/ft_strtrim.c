#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*trim;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (ft_strchr(set, s1[start]) && s1[start])
		start++;
	if (s1[start] == '\0')
		return (ft_strdup(""));
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[end]) && end > start)
		end--;
	len = end - start + 1;
	trim = malloc(len + 1);
	if (trim == NULL)
		return (NULL);
	ft_strlcpy(trim, s1 + start, len + 1);
	return (trim);
}
