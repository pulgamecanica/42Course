# Pong (Digital Table-Tenis)

Atari originally released it in 1972.
It was created by Allan Alcorn.

# The Game

The two paddles hit a ball and bounce it to the direction of the oponent. The score is displayed at the top of the screen.

Mathematically it is very simple. You need to represent two squares that move horizontally, then a ball with a position in the screen.
The ball has a speed, and it moves in 4 directions North-East-South-West, i.e it has a horizontal speed and a vertical speed.
When the ball hits the top or bottom borders, it changes it's vertical speed to the oposite direction, thus appearing to bounce.
If the paddle (square) hits the ball, the horizontal speed is inverted, thus making it seam as it bounced.

		while not gameOver
			move Player1 & move Player2
			if ball hit bottom || ball hit top {
				ball invert vertical speed
			}
			if player1 hit ball || player2 hit ball {
				ball invert horizontal speed
			}
			move ball

# Images

