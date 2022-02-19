norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_strncmp ..."
gcc -Wall -Werror -Wextra ex00/ft_strcmp.c  tests/test00.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_strncmp ..."
gcc -Wall -Werror -Wextra ex01/ft_strncmp.c  tests/test01.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_strcat ..."
gcc -Wall -Werror -Wextra ex02/ft_strcat.c tests/test02.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_strncat ..."
gcc -Wall -Werror -Wextra ex03/ft_strncat.c tests/test03.c && ./a.out | cat -e
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_strstr ..."
gcc -Wall -Werror -Wextra ex04/ft_strstr.c tests/test04.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_strlcat..."
gcc -Wall -Werror -Wextra ex05/ft_strlcat.c tests/test05.c && ./a.out
echo -e "\n- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
