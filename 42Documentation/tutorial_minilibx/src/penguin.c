#include "config.h"

# define YELLOW 0xF0CA0C
# define ORANGE 0xED840C

void	draw_square(t_square square, t_img *img);

void	penguin(t_img *img)
{
	t_square s1;
	for (int row1 = 160; row1 <= 240; row1+=10) {
		s1 = (t_square){row1, 100, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row2 = 150; row2 <= 250; row2+=10) {
		s1 = (t_square){row2, 110, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row3 = 140; row3 <= 160; row3+=10) {
		s1 = (t_square){row3, 120, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row3 = 170; row3 <= 230; row3+=10) {
		s1 = (t_square){row3, 120, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row3 = 240; row3 <= 260; row3+=10) {
		s1 = (t_square){row3, 120, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row4 = 140; row4 <= 150; row4+=10) {
		s1 = (t_square){row4, 130, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row4 = 160; row4 <= 240; row4+=10) {
		s1 = (t_square){row4, 130, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row4 = 250; row4 <= 260; row4+=10) {
		s1 = (t_square){row4, 130, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row5 = 140; row5 <= 260; row5+=10) {
		s1 = (t_square){row5, 140, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){140, 140, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){170, 140, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){180, 140, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){220, 140, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){230, 140, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){260, 140, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row6 = 140; row6 <= 260; row6+=10) {
		s1 = (t_square){row6, 150, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){140, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){160, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){170, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){190, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){210, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){220, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){240, 150, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){260, 150, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row7 = 140; row7 <= 260; row7+=10) {
		s1 = (t_square){row7, 160, 10, 0x000000};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){150, 160, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){200, 160, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){250, 160, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row8 = 140; row8 <= 260; row8+=10) {
		s1 = (t_square){row8, 170, 10, 0x000000};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){150, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){160, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){190, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){200, 170, 10, YELLOW}; /*YELLOW*/
		draw_square(s1, img);
		s1 = (t_square){210, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){240, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){250, 170, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row9 = 140; row9 <= 260; row9+=10) {
		s1 = (t_square){row9, 180, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){140, 180, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){260, 180, 10, 0x000000};
		draw_square(s1, img);
		s1 = (t_square){190, 180, 10, YELLOW};
		draw_square(s1, img);
		s1 = (t_square){200, 180, 10, YELLOW};
		draw_square(s1, img);
		s1 = (t_square){210, 180, 10, YELLOW};
		draw_square(s1, img);
	}
	for (int row10 = 130; row10 <= 270; row10+=10) {
		s1 = (t_square){row10, 190, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row10 = 160; row10 <= 240; row10+=10) {
		s1 = (t_square){row10, 190, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row10 = 180; row10 <= 220; row10+=10) {
		s1 = (t_square){row10, 190, 10, YELLOW};
		draw_square(s1, img);
	}
	for (int row11 = 120; row11 <= 280; row11+=10) {
		s1 = (t_square){row11, 200, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row11 = 160; row11 <= 240; row11+=10) {
		s1 = (t_square){row11, 200, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row11 = 190; row11 <= 210; row11+=10) {
		s1 = (t_square){row11, 200, 10, YELLOW};
		draw_square(s1, img);
	}
	for (int row12 = 110; row12 <= 280; row12+=10) {
		s1 = (t_square){row12, 210, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row12 = 160; row12 <= 240; row12+=10) {
		s1 = (t_square){row12, 210, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row13 = 100; row13 <= 290; row13+=10) {
		s1 = (t_square){row13, 220, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row13 = 160; row13 <= 240; row13+=10) {
		s1 = (t_square){row13, 220, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){140, 220, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){260, 220, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row14 = 100; row14 <= 290; row14+=10) {
		s1 = (t_square){row14, 230, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row14 = 160; row14 <= 240; row14+=10) {
		s1 = (t_square){row14, 230, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){130, 230, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){140, 230, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){260, 230, 10, 0xFFFFFF};
		draw_square(s1, img);
		s1 = (t_square){270, 230, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row15 = 150; row15 <= 250; row15+=10) {
		s1 = (t_square){row15, 240, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row15 = 160; row15 <= 240; row15+=10) {
		s1 = (t_square){row15, 240, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row16 = 150; row16 <= 250; row16+=10) {
		s1 = (t_square){row16, 250, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row16 = 170; row16 <= 230; row16+=10) {
		s1 = (t_square){row16, 250, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
	for (int row17 = 160; row17 <= 240; row17+=10) {
		s1 = (t_square){row17, 260, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row18 = 160; row18 <= 240; row18+=10) {
		s1 = (t_square){row18, 270, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row18 = 170; row18 <= 230; row18+=10) {
		s1 = (t_square){row18, 270, 10, ORANGE};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){200, 270, 10, 0x000000};
		draw_square(s1, img);
	}
	for (int row19 = 160; row19 <= 240; row19+=10) {
		s1 = (t_square){row19, 280, 10, 0x000000};
		draw_square(s1, img);
	}
	{
		s1 = (t_square){200, 280, 10, 0xFFFFFF};
		draw_square(s1, img);
	}
}
