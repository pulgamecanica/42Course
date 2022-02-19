norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_ft ..."
gcc -Wall -Werror -Wextra tests/test00.c ex00/ft_ft.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_ultimate_ft ..."
gcc -Wall -Werror -Wextra tests/test01.c ex01/ft_ultimate_ft.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_swap ..."
gcc -Wall -Werror -Wextra tests/test02.c ex02/ft_swap.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_div_mod[14, 6] ..."
gcc -Wall -Werror -Wextra tests/test03.c ex03/ft_div_mod.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_ultimate_div_mod[25, 7] ..."
gcc -Wall -Werror -Wextra tests/test04.c ex04/ft_ultimate_div_mod.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_putstr ..."
gcc -Wall -Werror -Wextra tests/test05.c ex05/ft_putstr.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex06 ft_strlen ..."
gcc -Wall -Werror -Wextra tests/test06.c ex06/ft_strlen.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex07 ft_rev_int_tab.c[55, 32, 23, 53, 10, 6] ..."
gcc -Wall -Werror -Wextra tests/test07.c ex07/ft_rev_int_tab.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex08 ft_sort_int_tab.c ..."
gcc -Wall -Werror -Wextra tests/test08.c ex08/ft_sort_int_tab.c && ./a.out
echo -e "\n- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
