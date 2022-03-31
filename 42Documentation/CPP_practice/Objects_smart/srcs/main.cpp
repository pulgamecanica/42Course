// Automatic script
#include <iostream>
#include <string>
#include "figures.hpp"

#define W 40
#define H 30

using namespace std;

void	put_grid(char grid[H][W])
{
	for(int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
			cout << grid[i][j] << " ";
		cout << endl;
	}
}

void	draw_circle(Circle c, char grid[H][W])
{
	point	p;

	for(int i = 0; i < H; i++)
	{
		for(int j = 0; j < W; j++)
		{
			p.y = i;p.x = j;
			if (c.belongs_to(p))
				grid[i][j] = c.get_chr();
		}
	}
}

void    draw_rectangle(Rectangle r, char grid[H][W])
{
        point   p;

	for(int i = 0; i < H; i++)
	{
		for(int j = 0; j < W; j++)
		{
			p.y = i;p.x = j;
			if (r.belongs_to(p))
				grid[i][j] = r.get_chr();
		}
	}
}

int	main(void)
{
//	cout << "Hello Friend\nPulgamecanica greets you :D\n";
	point	p;
	char	grid[H][W];

	for(int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
			grid[i][j] = '*';
	}
	p.x = 10;p.y = 20;
	Circle c(p, 10, 'i');
	draw_circle(c, grid);

	p.x = 15;p.y = 12;
	Rectangle r(p, 8, 14, ' ');
	draw_rectangle(r, grid);

/*
        p.x = 15;p.y = 10;
	c.set(p, 5, 'x');
        draw_circle(c, grid);

	p.x = 40;p.y = 8;
        c.set(p, 15, 'R');
        draw_circle(c, grid);

	p.x = 22;p.y = 30;
        c.set(p, 8, ':');
        draw_circle(c, grid);

	p.x = 45;p.y = 10;
        c.set(p, 6, ' ');
        draw_circle(c, grid);

	p.x = 45;p.y = 22;
        r.set(p, 5, 10, 'K');
        draw_rectangle(r, grid);
*/
        put_grid(grid);
	cout << "At this point we can see that this is getting too tedious\n";
	cout << "It's no use to create one function for EVERY single figure\n";
	cout << "We could use an abstract object Figure or an interface...";
	return (0);
}
