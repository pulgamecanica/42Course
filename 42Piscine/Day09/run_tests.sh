norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_strudp ..."
gcc -Wall -Werror -Wextra ex00/ft_strdup.c tests/test00.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_range ..."
gcc -Wall -Werror -Wextra ex01/ft_range.c tests/test01.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_ultimate_range ..."
gcc -Wall -Werror -Wextra ex02/ft_ultimate_range.c tests/test02.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_strjoin ..."
gcc -Wall -Werror -Wextra ex03/ft_strjoin.c tests/test03.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_convert_base ..."
gcc -Wall -Werror -Wextra ex04/ft_convert_base.c ex04/ft_convert_base2.c tests/test04.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_split..."
echo -e "\n- - - - - - - - - - - "
gcc -Wall -Werror -Wextra ex05/ft_split.c tests/test05.c && ./a.out
rm a.out
echo "Nice Job :D !!!"
