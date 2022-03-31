#include <iostream>
using namespace std;

struct s_point {
	short int	x;
	short int	y;
};

typedef s_point		point;

static point	*add_points(point *p1, point *p2) {
	point	*p;

	p = new (nothrow) point;
	if (!p)
	{
		cout << "Point memory couldn't be allocated";
		return (NULL);
	}
	p->x = p1->x + p2->x;
	p->y = p1->y + p2->y;
	return (p);
}

void	pointer_04() {
	point p1, p2;
	point *p;

	p1.x = 9;p1.y = 10;
	p2.x = -8;p2.y = 1;
	p = add_points(&p1, &p2);
	cout << "Result: " << "[" << p->x << ", " << p->y << "].";
	delete p;
}

void	pointer_03() {
	int	*ptr_n;

	ptr_n = new (nothrow) int[3];	// allocate memory with nothrow, so it can throw nullptr on failure
	if (ptr_n == 0)
	{
		cout << "Allocation Failed" << endl;
		return;
	}
	for (int i = 0; i < 3; i++, ptr_n++)
		*ptr_n = i;	// (ptr_n + i) = i; // only if i = 0
	ptr_n -= 3;
	cout << ptr_n[0] << ptr_n[1] << ptr_n[2] << endl;
	delete[] ptr_n; // destroy with [] because it's an array of elements
}


// NEW [one element]
void	pointer_02() {
	char	*str;
	char	c('a');

	str = new char;	// allocating one char bytes (1 byte)
	*str = c;
	cout << str[0] << endl;
	delete str;	// must destroy allocated memory
}

void	pointer_01(void) {
	int *ptr_n;
	int n(5);

	ptr_n = &n;
	cout << *ptr_n << endl;
}
