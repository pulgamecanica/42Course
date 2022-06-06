# [Snake Challenge](https://pulgamecanica.herokuapp.com/posts/10/show)
***

### This exercise was a challenge I created and posted on my blog
#### *SPOILER ALERT* This is my solution to the challenge, I describe bellow how I solved the challenge.

#### About the food:
- I decided to add an element to the game struct **t_point food;**

		It's a point that represents the position of the food

#### About the snake movement:
- The snake had to move in 4 directions: North, South, East and West.
- The Snake is not able to move to the oposite direction.
- I decided to represent the direction with a point, as a vector.

		North(0, -1) - South(0, 1) - West(-1, 0) - East(1, 0)
- Then I added a **t_point direction** to the snake struct, this point
    represents the direction as explained above.
- I controll the speed by not doing anything until update is called "speed"
    times, but this would mess up the direction, because, it means that not
    necessarily the snake will move after the user changes direction. It changes
    only after "speed" times. That is why I added a **t_bool can_change** that
    is set to false after changing direction and it's set to true after moving.
    This way, after changing direction, the user will be able to change
    direction only after the snake moved.

#### Index
- Functions called on update

		static t_point next_position(t_snake *snake); Returns the next direction according to the direction of the snake.
		static void fix_position(t_game *game); Fix the snake position in case it goes beyond the window limits.
		static void shift_snake(t_snake *snake); Shift the snake positions to move.
		static void update_food(t_game *game); Check if the snake catched the food, increase the snake len, and the speed.
		static void snake_died(t_game *game); Check if the snake died.
- Functions called on key_down

		static void change_direction(t_game *game, int key_pressed); Change direction if possible.
