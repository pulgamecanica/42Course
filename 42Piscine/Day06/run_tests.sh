norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_strlen ..."
gcc -Wall -Werror -Wextra tests/test00.c ex00/ft_strlen.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_putstr ..."
gcc -Wall -Werror -Wextra tests/test01.c ex01/ft_putstr.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_putnbr ..."
gcc -Wall -Werror -Wextra tests/test02.c ex02/ft_putnbr.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_atoi ..."
gcc -Wall -Werror -Wextra tests/test03.c ex03/ft_atoi.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_putnbr_base ..."
gcc -Wall -Werror -Wextra tests/test04.c ex04/ft_putnbr_base.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_atoi_base..."
gcc -Wall -Werror -Wextra tests/test05.c ex05/ft_atoi_base.c && ./a.out
echo -e "\n- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
