norminette ex0*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_putchar['t'] ..."
gcc -Wall -Werror -Wextra tests/test00_0.c ex00/ft_putchar.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_putchar['A'] ..."
gcc -Wall -Werror -Wextra tests/test00_1.c ex00/ft_putchar.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_print_alphabet ..."
gcc -Wall -Werror -Wextra tests/test01.c ex01/ft_print_alphabet.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_print_reverse_alphabet ..."
gcc -Wall -Werror -Wextra tests/test02.c ex02/ft_print_reverse_alphabet.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_print_numbers ..."
gcc -Wall -Werror -Wextra tests/test03.c ex03/ft_print_numbers.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_is_negative[-10] ..."
gcc -Wall -Werror -Wextra tests/test04_0.c ex04/ft_is_negative.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_is_negative[0] ..."
gcc -Wall -Werror -Wextra tests/test04_1.c ex04/ft_is_negative.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_is_negative[10] ..."
gcc -Wall -Werror -Wextra tests/test04_2.c ex04/ft_is_negative.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_print_comb ..."
gcc -Wall -Werror -Wextra tests/test05.c ex05/ft_print_comb.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex06 ft_print_comb2 ..."
gcc -Wall -Werror -Wextra tests/test06.c ex06/ft_print_comb2.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex07 ft_putnbr.c[-2147483648] ..."
gcc -Wall -Werror -Wextra tests/test07_0.c ex07/ft_putnbr.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex07 ft_putnbr.c[2147483647] ..."
gcc -Wall -Werror -Wextra tests/test07_1.c ex07/ft_putnbr.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex08 ft_print_combn.c[1] ..."
gcc -Wall -Werror -Wextra tests/test08_0.c ex08/ft_print_combn.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex08 ft_print_combn.c[3] ..."
gcc -Wall -Werror -Wextra tests/test08_1.c ex08/ft_print_combn.c && ./a.out
echo -e "\n- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
