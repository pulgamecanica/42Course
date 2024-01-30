gcc -fsanitize=address -o test image_utils.c animate.c main.c -ldl -lglfw -lm -lpthread -LMLX42/build -lmlx42 -Llibft/ -lft -IMLX42/include -Ilibft/includes && ./test 
