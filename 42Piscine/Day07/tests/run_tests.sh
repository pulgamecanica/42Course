norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_iterative_factorial ..."
gcc -Wall -Werror -Wextra tests/test00.c ex00/ft_iterative_factorial.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_recursive_factorial ..."
gcc -Wall -Werror -Wextra tests/test01.c ex01/ft_recursive_factorial.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_iterative_power ..."
gcc -Wall -Werror -Wextra tests/test02.c ex02/ft_iterative_power.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_recursive_power ..."
gcc -Wall -Werror -Wextra tests/test03.c ex03/ft_recursive_power.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_fibbonacci ..."
gcc -Wall -Werror -Wextra tests/test04.c ex04/ft_fibonacci.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_sqrt ..."
gcc -Wall -Werror -Wextra tests/test05.c ex05/ft_sqrt.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex06 ft_is_prime ..."
gcc -Wall -Werror -Wextra tests/test06.c ex06/ft_is_prime.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex07 ft_find_next_prime  ..."
gcc -W -W -W tests/test07.c ex07/ft_find_next_prime.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex08 ft_queens_puzzle.c ..."
#gcc -W -W -W tests/test08.c ex08/ft_ten_queens_puzzle.c && ./a.out
echo -e "\n- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
