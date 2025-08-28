#include "libft.h"

int	ft_atoi(const char *str)
{
	int		sign;
	size_t	result;

	result = 0;
	sign = 1;
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
			|| *str == '\f' || *str == '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	if (result > LONG_MAX)
		return (0 - (sign == 1));
	return (result * sign);
}
