#include "libft.h"
/**/
typedef struct {
	int a;
	char b;
}	simple_struct;
/**/

simple_struct * new_ss() {
	simple_struct * ss;

	ss = (simple_struct *)malloc(sizeof(simple_struct));
	if (!ss)
		return (NULL);
	return (ss);
}

/**/
void handle_abstraction(void * ptr) {
	simple_struct * ss;

	ss = (simple_struct *)ptr;
	if (!ss) {
		perror("casting failed");
		exit(3);
	}
}
/**/

/**
void handle_abstraction(simple_struct * ptr) {
	if (!ptr) {
		perror("Failed\n");
		exit(3);
	}
}
**/

int main(int ac, char *av[]) {
	if (ac != 2)
		return (1);

	long int iterations;
	t_list ** head;
	simple_struct * ss;


	head = (t_list **)calloc(sizeof(t_list *), 1);
	iterations = ft_atoi(av[1]);
	printf("Test with [%ld] - Abstract Usage\n", iterations);
	for (int i = 0; i < iterations; i++) {
		ss = new_ss();
		if (!ss) {
			perror("malloc failed\n");
			return (2);
		}
		ft_lstadd_back(head, ft_lstnew(ss));
	}
	ft_lstiter(*head, handle_abstraction);
	return (0);
}

