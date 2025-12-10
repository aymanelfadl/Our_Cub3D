/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:34:09 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:34:09 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*nextnode;

	if (!lst || !del)
		return ;
	tmp = *lst;
	while (tmp)
	{
		nextnode = tmp->next;
		ft_lstdelone(tmp, del);
		tmp = nextnode;
	}
	*lst = NULL;
}
