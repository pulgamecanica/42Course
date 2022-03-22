#include "get_next_line.h"
#include "ft_printf.h"

void	main_menu()
{
	ft_printf("		====>MAIN MENU<====\n");
	ft_printf("	1) Play Game\n");
	ft_printf("	2) Free Text\n");
	ft_printf("	3) Read File\n");
	ft_printf("	0) EXIT\n");
}

void	game_menu()
{
	ft_printf("		====>GAME<====\n");
	ft_printf("	1) Say Hello\n");
	ft_printf("	2) Talk to Mama\n");
	ft_printf("	3) Talk to Papa\n");
	ft_printf("	4) Talk to Granny!\n");
	ft_printf("	5) Kill'em all...\n");
	ft_printf("	6) Show MENU\n");
	ft_printf("	0) EXIT GAME\n");
}

void	start_game()
{
	char	*line;
	int	granny_count = 0;

	game_menu();
	while ((line = get_next_line(0)))
	{
		line = ft_strtrim(line, "       ");
		if (*line == '1')
		{		
			ft_printf("Hello to you toooo!\n");
		}
		else if (*line == '2')
		{
			if (granny_count)
			{
				ft_printf("HOW DARE YOU YELL AT YOUR GRANNY! DONT TALK TO ME!\n");
				continue ;
			}
			else
			{
				ft_printf("Dear, what do you want?\n");
				line = get_next_line(0);
				if (ft_strnstr(line, "love", ft_strlen(line)))
					ft_printf("I love you too, honey!\n");
				else
					ft_printf("So you don't love ME!? :(\n");
					
			}
		}
		else if (*line == '3')
			if (granny_count)
                        {
                                ft_printf("You LITTLE RAT! Why u scream to grand'ma!!!\n");
                                continue ;
                        }
                        else
                        {
                                ft_printf("Tell me son... what's on your mind?\n");
                                line = get_next_line(0);
                                if (ft_strnstr(line, "fuck", ft_strlen(line)))
                                        ft_printf("AHHHHHH you little piece of shit come here right now!!! I'll smack your ass!\n");
                                else
                                        ft_printf("So you don't love ME!? :(\n");

                        }
		else if (*line == '4')
		{
			ft_printf("Hello my sweet, here's your granny, ask my anything...\n");
			line = get_next_line(0);
			if (ft_strnstr(line, "!!!", ft_strlen(line)))
			{
				ft_printf("Ahhhhhhh don't scream at me! what did I do to you, you damn bastard!\n");
				granny_count++;
			}
			else
				ft_printf("Hugh!? What did you say!? No no since 1998!\n");
		}
		else if (*line == '5')
		{
			ft_printf("Done.... how sad.... bye bye\n");
			break ;
		}
		else if (*line == '6')
			game_menu();
		else if (*line == '0')
			break ;
		else
			ft_printf("Sorry that's not a valid option... :(\n");
	}
}

int	main(void)
{
	char	*line;

	main_menu();
	while ((line = get_next_line(0)))
	{
		line = ft_strtrim(line, " 	");
		if (*line == '1')
			start_game();
		else if (*line == '2')
			while (ft_printf("%s", get_next_line(0)) > 1);
		else if (*line == '3')
			ft_printf("Working on it\n");
		else if (*line == '0')
			break ;
		else
			ft_printf("Sorry that's not a valid option... :(\n");
		ft_printf("Option choosen: %s\n", line);
		main_menu();
	}
}
