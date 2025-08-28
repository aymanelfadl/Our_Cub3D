#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*head;

	head = lst;
	len = 0;
	if (lst)
	{
		while (head)
		{
			len++;
			head = head->next;
		}
	}
	return (len);
}
