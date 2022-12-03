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

<img width="1471" alt="Screen Shot 2022-12-03 at 10 53 57" src="https://user-images.githubusercontent.com/28810331/205437391-c323870e-3ac8-47c1-a519-66ec6b30e244.png">
<img width="2048" alt="Screen Shot 2022-12-03 at 1 12 08" src="https://user-images.githubusercontent.com/28810331/205437398-a52713d7-5470-4dd1-abed-e3a3a2480098.png">
