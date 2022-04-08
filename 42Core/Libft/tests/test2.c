#include "../libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	print_list(t_list *elem)
{
	printf("Content: %p : next: [%p]", elem->content, elem->next);
	printf("	String Rep : %s\n", elem->content);
}

void	print_all(t_list *start)
{
	t_list	*n;
	int	i;

	i = 0;
	n = start;
	printf("LIST (size: %d)->\n", ft_lstsize(start));
	while (n != NULL)
	{
		printf("	#%d ", i++);
		print_list(n);
		n = n->next;
	}
}

void	delete_str(void	*ptr)
{
	if (ptr == NULL)
		return ;
	free(ptr);
}

t_list	*create_list()
{
	t_list  *e1;
	t_list  *e2;
	t_list  *e3;

	e1 = ft_lstnew(ft_strdup("Hello Dude"));
	e2 = ft_lstnew(ft_strdup("HWLLOOOO DUDE"));
	e3 = ft_lstnew(ft_strdup("You are crazy"));
	e1->next = e2;
	e2->next = e3;
	e3->next = NULL;
	return (e1);
}

void	str_up(void *s)
{
	char	*str;
	int	i;

	str = s;
	i = 0;
	while (str[i] != '\0')
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
}

void    *str_troll(void *s)
{
	void	*p;
	char	*str;

	(void)s;
	str = "HELLO";
	p = str;
	return (p);
}

int	main(int ac, char **av)
{
	t_list	*e1;
	t_list	*e2;
	t_list	*e3;
	t_list  *e4;

	if (ac == 2)
	{
		print_list(ft_lstnew(av[1]));
		e1 = create_list();
		e2 = create_list();
		e3 = create_list();
		print_all(e1);
		ft_lstadd_front(&e1, ft_lstnew(ft_strdup("Another Elem")));
		print_all(e1);
		ft_lstadd_back(&e1, ft_lstnew(ft_strdup("LAST ELEM")));
		print_all(e1);
		printf("Deleted element 1\n");
		ft_lstdelone(e1->next, &delete_str);
		print_all(e1);
		ft_lstclear(&e2->next, &delete_str);
		printf("Delete All elements from e2\n");
		print_all(e2);
		printf("Iterate through the list\n");
		ft_lstiter(e3, &str_up);
		print_all(e3);
		printf("Mapi to get a new List\n");
		e4 = ft_lstmap(e3, &str_troll, &delete_str);
		print_all(e4);
	}
	else
		printf("Usage: ./test <one_arg>");
	printf("\n");
}
