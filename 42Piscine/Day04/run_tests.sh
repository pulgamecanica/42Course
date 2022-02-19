norminette ex*/ft* -R CheckForbiddenSourceHeader
echo -e "\n- - - - - - - - - - - "
echo "Ex00 ft_strcpy ..."
gcc -Wall -Werror -Wextra tests/test00.c ex00/ft_strcpy.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex01 ft_strncpy ..."
gcc -Wall -Werror -Wextra tests/test01.c ex01/ft_strncpy.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex02 ft_str_is_alpha ..."
gcc -Wall -Werror -Wextra tests/test02.c ex02/ft_str_is_alpha.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex03 ft_str_is_numeric ..."
gcc -Wall -Werror -Wextra tests/test03.c ex03/ft_str_is_numeric.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex04 ft_str_is_lowercase ..."
gcc -Wall -Werror -Wextra tests/test04.c ex04/ft_str_is_lowercase.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex05 ft_str_is_uppercase ..."
gcc -Wall -Werror -Wextra tests/test05.c ex05/ft_str_is_uppercase.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex06 ft_str_is_printable ..."
gcc -Wall -Werror -Wextra tests/test06.c ex06/ft_str_is_printable.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex07 ft_strupcase ..."
gcc -Wall -Werror -Wextra tests/test07.c ex07/ft_strupcase.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex08 ft_strlowcase.c ..."
gcc -Wall -Werror -Wextra tests/test08.c ex08/ft_strlowcase.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex09 ft_strcapitalize ..."
gcc -Wall -Werror -Wextra tests/test09.c ex09/ft_strcapitalize.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex10 ft_strlcpy ..."
gcc -Wall -Werror -Wextra tests/test10.c ex10/ft_strlcpy.c  && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex11 ft_putstr_non_printable ..."
gcc -Wall -Werror -Wextra tests/test11.c ex11/ft_putstr_non_printable.c && ./a.out
echo -e "\n- - - - - - - - - - - "
echo -e "Ex12 ft_print_memory ..."
gcc -Wall -Werror -Wextra tests/test12.c ex12/ft_print_memory.c && ./a.out
echo -e "- - - - - - - - - - - "
rm a.out
echo "Nice Job :D !!!"
