#include "../ft_printf.h"

void	printf_c()
{
	printf("-----Printf %%c-----(X.0 = ft_printf)\n");
	printf("1: ['c']: %c\n", 'c');
	ft_printf("1.0: ['c']: %c\n", 'c');
        printf("2: [4]: %c\n", 0);
	ft_printf("2.0: [4]: %c\n", 0);
        printf("3: [10]: %c\n", 10);
	ft_printf("3.0: [10]: %c\n", 10);
        printf("4: [-2]: %c\n", -2);
	ft_printf("4: [-2]: %c\n", -2);
	printf("NO NULL :(\n");
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_u()
{
	printf("-----Printf %%u-----(X.0 = ft_printf)\n");
        printf("1: [-222]: %u\n", -222);
	ft_printf("1.0: [-222]: %u\n", -222);
        printf("2: [2147483647]: %u\n", 2147483647); //DOESNT SUPPORT: 2147483648
        ft_printf("2.0: [2147483647]: %u\n", 2147483647); //DOESNT SUPPORT: 2147483648
	printf("3: [0]: %u\n", 0);
	ft_printf("3.0: [0]: %u\n", 0);
        printf("4: [9]: %u\n", 9);
	ft_printf("4.0: [9]: %u\n", 9);
	printf("NO NULL :(\n");
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_i()
{
	printf("-----Printf %%i-----(X.0 = ft_printf)\n");
	printf("1: [-222]: %i\n", -222);
	ft_printf("1.0: [-222]: %i\n", -222);
        printf("2: [2147483647]: %i\n", 2147483647); //DOESNT SUPPORT: 2147483648
	ft_printf("2.0: [2147483647]: %i\n", 2147483647); //DOESNT SUPPORT: 2147483648
        printf("3: [0]: %i\n", 0);
	ft_printf("3.0: [0]: %i\n", 0);
        printf("4: [9]: %i\n", 9);
	ft_printf("4: [9]: %i\n", 9);
        printf("NO NULL:(\n");
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_d()
{
	printf("-----Printf %%d-----(X.0 = ft_printf)\n");
	printf("1: [-222]: %d\n", -222);
	ft_printf("1.0: [-222]: %d\n", -222);
        printf("2: [2147483647]: %d\n", 2147483647); //DOESNT SUPPORT: 2147483648
	ft_printf("2.0: [2147483647]: %d\n", 2147483647); //DOESNT SUPPORT: 2147483648
        printf("3: [0]: %d\n", 0);
	ft_printf("3.0: [0]: %d\n", 0);
        printf("4: [900]: %d\n", 900);
	ft_printf("4.0: [900]: %d\n", 900);
        printf("NO NULL :(\n");
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_p()
{
	int	i;
	double e;
	char	*str = "Hello";
	void *c;

	c = &str;
	i = 99;
	e = 33.3;
	printf("-----Printf %%p-----(X.0 = ft_printf)\n");
	printf("1: [int *]: %p\n", &i);
	ft_printf("1.0: [int *]: %p\n", &i);
        printf("2: [double *]: %p\n", &e);
	ft_printf("2.0: [double *]: %p\n", &e);
        printf("3: [ *]: %p\n", str);
	ft_printf("3.0: [ *]: %p\n", str);
        printf("4: [void *]: %p\n", c);
	ft_printf("4.0: [void *]: %p\n", c);
	printf("NULL: %p\n", NULL);
	ft_printf("FT_NULL: %p\n", NULL);
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_s()
{
	printf("-----Printf %%s-----(X.0 = ft_printf)\n");
	printf("1: [-22.2]: %s\n", "-22.2");
	ft_printf("1.0: [-22.2]: %s\n", "-22.2");
        printf("2: []: %s\n", "");
	ft_printf("2.0: []: %s\n", "");
        printf("3: [%%Hello %%]: %s\n", "%%Hello %%");
	ft_printf("3.0: [%%Hello %%]: %s\n", "%%Hello %%");
	printf("NULL: %s\n", NULL);
	ft_printf("FT_NULL: %s\n", NULL);
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}
void	printf_x()
{
	printf("-----Printf %%x-----(X.0 = ft_printf)\n");
	printf("1: [-222]: %x\n", -22);
	ft_printf("1.0: [-222]: %x\n", -22);
        printf("2: [2147483647]: %x\n", 2147483647); //DOESNT SUPPORT: 2147483648 BECAUSE ITS UNSIGNED INT!!!!!
	ft_printf("2.0: [2147483647]: %x\n", 2147483647); //DOESNT SUPPORT: 2147483648 BECAUSE ITS UNSIGNED INT!!!!!
        printf("3: [0]: %x\n", 0);
	ft_printf("3.0: [0]: %x\n", 0);
        printf("4: [9]: %x\n", 9);
	ft_printf("4.0: [9]: %x\n", 9);
        printf("NO NULL :(\n", NULL, NULL,  NULL);
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}

void	printf_X()
{
	printf("-----Printf %%X-----(X.0 = ft_printf)\n");
	printf("1: [-222]: %x\n", -22);
	ft_printf("1.0: [-222]: %x\n", -22);
        printf("2: [2147483647]: %x\n", 2147483647); //DOESNT SUPPORT: 2147483648 BECAUSE ITS UNSIGNED INT!!!!!
	ft_printf("2.0: [2147483647]: %x\n", 2147483647); //DOESNT SUPPORT: 21474836480 BECAUSE ITS UNSIGNED INT!!!!!
        printf("3: [0]: %x\n", 0);
	ft_printf("3.0: [0]: %x\n", 0);
        printf("4: [9]: %x\n", 9);
	ft_printf("4.0: [9]: %x\n", 9);
        printf("NO NULL :(\n");
	printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
}

void	testMFW()
{
	int     a = 10;
        printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST MINIMUM FIELD WIDTH-\\_/\\_/\\_/\\_/\\_/\\.\nThe MFW is The minimum width (characters) printed by the conversion, to my understanding this must allways go AFTER any flag and BEFORE the presition )if minnimum is lower than the whole argument return then remains unchanged)\n");
        printf("One c argument as [%%3c]: [%3c]", 'a');
        printf("			<The result is two spaces characters printed before the char 'a'.....>\n");
        printf("One p argument as [%%16p]: [%16p]", &a);
        printf("	<The result is 2 spaces since the pointer takes 14 characters!.....>\n");
        printf("One s argument as [%%6s]: [%6s]", "Hello");
        printf("		<The result is one space before the string, since hello has 5 chars!.....>\n");
        printf("One u negative argument as [%%3u]: [%3u]\n", -40);
        printf("Another u positive argument as [%%4u]: [%4u]", 999);
        printf("	<The result was unchanged for the first one, since it is bigger than the mfw the second one compensated with one space!.....>\n");
        printf("One i negative argument as [%%5i]: [%5i]\n", -4054);
        printf("Another i positive argument as [%%7i]: [%7i]\n", 90909);
        printf("Another i ZERO argument as [%%1i]: [%1i]", 0);
        printf("		<The result for the first & last one is unchanged since it took exactly the mfw, the seoncd one has 2 more spaces to compensate!...>\n");
        printf("One x argument as [%%3x]: [%3x]", 2222);
        printf("			<The result is unchanged since it prints exactly mfw chars!.....>\n");
        printf("One X argument as [%%5X]: [%5X]", 2222);
        printf("		<The result is two more spaces to compensate since it prints only 3 chars and the mfw is 5!.....>");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG SPACE-/–\\/–\\/–\\/–\\/–\\/.\n");

}

void	testflagSP()
{
	int	a = 10;
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST FLAG SPACE-\\_/\\_/\\_/\\_/\\_/\\.\nThe space flag is used before a number in order to force it's signed format (Apparently it's never overwritter or missbeheaved...) IT HAS NO EFFECT IF THE FLAG PLUS IS USED\n");
        printf("One c argument as [%% c]: [% c]", 'a');
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One p argument as [%% p]: [% p]", &a);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One s argument as [%% s]: [% s]", "Hello");
	printf("		<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One u negative argument as [%% u]: [% u]\n", -40);
	printf("Another u positive argument as [%% u]: [% u]", 999);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One i negative argument as [%% i]: [% i]\n", -4054);
	printf("Another i positive argument as [%% i]: [% i]\n", 90909);
	printf("Another i ZERO argument as [%% i]: [% i]", 0);
	printf("		<The result is a space before a positive number including ZERO negatives remain unchanged>\n");
	printf("One d negative argument as [%% d]: [% d]\n", -1);
	printf("Another d positive argument as [%% d]: [% d]\n", 909);
	printf("Another d ZERO argument as [%% d]: [% d]", 0);
	printf("		<The result is a space before a positive number including ZERO negatives remain unchanged>\n");
	printf("One x argument as [%% x]: [% x]", 2222);
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One X argument as [%% X]: [% X]", 2222);
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("-----WITH MINIMUM FIELD WIDTH TESTS-----\n");
	printf("Since the Space Flag only had a defined behaviour for %%d and %%i then I assume it should only work with those\n");
	printf("Mix with flag MFW [Hello Robot <%% 10i>, I guess my fav num is <%% 3d>. I have <%% 1d> friends :(]\n");
	printf("RESULT: Hello Robot <% 10i>, I guess my fav num is <% 3d>. I have <% 1d> friends :(\n", -445566, 999, 0);
	printf("The result is expected but with one corner case: When the mwf is equals to the characters printer then we must ADD one space, so short, ther should at least exist one space (for the sign)!.....");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG SPACE-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void    testflagMINUS()
{
        int     a = 10;
        printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST FLAG MINUS-\\_/\\_/\\_/\\_/\\_/\\.\nThe minus flag is used to make a left adjustment (left justify) this would only have an effect when we use a MFW else the result is unchanged...\n");
	printf("One c argument as [%%3-c]: [%-3c]", 'a');
        printf("			<The result is two spaces characters printed after the char 'a'.....>\n");
        printf("One p argument as [%%-16p]: [%-16p]", &a);
        printf("	<The result is 2 spaces at the end since the pointer takes 14 characters!.....>\n");
        printf("One s argument as [%%-6s]: [%-6s]", "Hello");
        printf("		<The result is one space after the string, since hello has 5 chars!.....>\n");
        printf("One u negative argument as [%%-3u]: [%-3u]\n", -40);
        printf("Another u positive argument as [%%-4u]: [%-4u]", 999);
        printf("	<The result was unchanged for the first one, since it is bigger than the mfw the second one compensated with one space at the end!.....>\n");
        printf("One i negative argument as [%%- 5i]: [%- 5i]\n", -4054);
        printf("Another i positive argument as [%%-7i]: [%-7i]\n", 90909);
        printf("Another i ZERO argument as [%%1 -i]: [%1 -i]", 0);
        printf("	<The result for the first one is unchanged since negatives have no effect undder SPACE flag and it's exactly mwf chars, last is one has one space since the flag SPACE requires one space for the signed and the left JUSTIFY HAS NOOOO EFFECT, the seoncd one has 2 more spaces to compensate at the end!...>\n");
	printf("We can conclude that the space flag OVERWRITES the flag MINUS, but what if we had more than one space to compensate????\n");
	printf("Another i ZERO argument as [%%2 -i]: [%2 -i]\n", 0);
	printf("Another i ZERO argument as [%%2- i]: [%2- i]\n", 0);
	printf("As we can see the order doesn't matter and the mwf didn't compensate because it wrote the space with the sign...but if we had more than one space to compensate??\n");
	printf("Another i ZERO argument as [%%3 -i][%%4- i]: [%3 -i][%4- i]\n", 0, 9);
	printf("As we can confirm, only the spaces needed to complete the mfw were used to to justify to the left....\n");
        printf("One x argument as [%%-3x]: [%-3x]", 2222);
        printf("			<The result is unchanged since it prints exactly mfw chars!.....>\n");
	printf("One X argument as [%%- 5X]: [%- 5X]", 2222);
	printf("		<The result is two more spaces to compensate at the end since it prints only 3 chars and the mfw is 5 with left justify & space has undefined behaviour for the X format!.....>");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG MINUS-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	testflag0()
{
        int     a = 10;
        printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST ZERO MINUS-\\_/\\_/\\_/\\_/\\_/\\.\nThe ZERO flag is used to place zero's instead of spaces for the MFW unless the flag MINUS is used...\n");
        printf("One c argument as [%%03c]: [%03c]", 'a');
        printf("			<The result is two zero characters printed before the char 'a'.....>\n");
        printf("One p argument as [%%016p]: [%016p]", &a);
        printf("	<The result is 2 zeros BEFORE THE HEXADECIMAL BUT AFTER THE PREFIX 0x since the pointer takes 14 characters!.....>\n");
        printf("One s argument as [%%06s]: [%06s]", "Hello");
        printf("		<The result is one zero before the string, since hello has 5 chars!.....>\n");
        printf("One u negative argument as [%%03u]: [%03u]\n", -40);
        printf("Another u positive argument as [%%04u]: [%04u]", 999);
        printf("	<The result was unchanged for the first one, since it is bigger than the mfw the second one compensated with one zero at the begining!.....>\n");
        printf("One i negative argument as [%%0 6i]: [%0 6i]\n", -4054);
        printf("Another i positive argument as [%%07i]: [%07i]\n", 90909);
        printf("Another i ZERO argument as [%%1 0i]: [%1 0i]", 0);
        printf("	<The result for the first one is one zero BEFORE the number but AFTER the '-' from the NUMBER, second one has two zeros before the number and last is one has one zero since the flag SPACE requires one space for the signed!....");
        printf("We can conclude that the zero flag substitute BEFORE a prefix, but what if we had more than one space to compensate does is goes to the prefix or before????\n");
        printf("Another i ZERO argument as [%%2 0i]: [%2 0i]\n", 0);
        printf("Another i ZERO argument as [%%02 i]: [%02 i]\n", 0);
        printf("As we can see the order doesn't matter BUT it should not be at the right of the and the MFW... It didn't compensate because it wrote the space with the sign...but if we had more than one space to compensate??\n");
        printf("Another i ZERO argument as [%%3 0i][%%04 i]: [%3 0i][%04 i]\n", 0, 9);
        printf("As we can see, it wrote the zeros in order to copensate between the space and the NUMBER, in conclusion the SPACE allways goes before....\n");
        printf("One x argument as [%%03x]: [%03x]", 2222);
        printf("			<The result is unchanged since it prints exactly mfw chars!.....>\n");
        printf("One X argument as [%%0 5X]: [%0 5X]", 2222);
        printf("		<The result is two more spaces to compensate at the end since it prints only 3 chars and the mfw is 5 with left justify & space has undefined behaviour for the X format!.....>");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG ZERO-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	testflagPLUS()
{
	int	a = 10;
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST FLAG PLUS-\\_/\\_/\\_/\\_/\\_/\\.\nThe space flag is used to print a SIGN (+-) when a signed format is used (Apparently it's never overwritter or missbeheaved...)\n");
        printf("One c argument as [%%+c]: [%+c]", 'a');
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One p argument as [%%+p]: [%+p]", &a);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One s argument as [%%+s]: [%+s]", "Hello");
	printf("		<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One u negative argument as [%%+u]: [%+u]\n", -40);
	printf("Another u positive argument as [%%+u]: [%+u]", 999);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One i negative argument as [%%+i]: [%+i]\n", -4054);
	printf("Another i positive argument as [%%+i]: [%+i]\n", 90909);
	printf("Another i ZERO argument as [%%+i]: [%+i]", 0);
	printf("		<The result is a + sign before the positive numbers (ZERO INCLUDED) and a - sign BEFORE the NEGATIVE NUMBERS>\n");
	printf("One d negative argument as [%%+d]: [%+d]\n", -1);
	printf("Another d positive argument as [%%+d]: [%+d]\n", 909);
	printf("Another d ZERO argument as [%%+d]: [%+d]", 0);
	printf("		<The result is a + sign before the positive numbers (ZERO INCLUDED) and a - sign BEFORE the NEGATIVE NUMBERS>\n");
	printf("One x argument as [%% x]: [%+x]", 2222);
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One X argument as [%% X]: [%+X]", 2222);
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("-----WITH MINIMUM FIELD WIDTH, ZERO, AND SPACE AND MINUS TESTS-----\n");
	printf("Since the PLUS Flag only had a defined behaviour for %%d and %%i then I assume it should only work with those\n");
	printf("Mix with flag MFW & SPACE[Hello Robot <%%+ 10i>, I guess my fav num is <%%+ d>. I have <%% +1d> friends :(]\n");
	printf("RESULT: Hello Robot <%+ 10i>, I guess my fav num is <%+ d>. I have <% +1d> friends :(\n", -445566, 999, 0);
	printf("When PLUS sign is used, the SPACE flag has NO EFFECT at all what so ever!.....\n");
	printf("Mix with flag MFW & MINUS [Good Bye Human <%%+- 10i>, I guess my fav num is <%%-+ d>. I have <%% +-1d> friends :(]\n");
	printf("RESULT: Good Bye Human <%+- 10i>, I guess my fav num is <%-+ d>. I have <% +-1d> friends :(\n", -445566, 999, 0);
	printf("Behaviour expected.... it justify to the left only the remaning spaces!....");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG PLUS-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	testflagCARDINAL()
{
	int	a = 10;
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST FLAG CARDINAL-\\_/\\_/\\_/\\_/\\_/\\.\nThe CARDINAL flag is used before a 'xX' flag to print it's alternative form Followed either by a 0x or 0X!.....(The ZERO flag should place zeros in between)\n");
        printf("One c argument as [%%#c]: [%#c]", 'a');
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One p argument as [%%#p]: [%#p]", &a);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One s argument as [%%#s]: [%#s]", "Hello");
	printf("		<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One u negative argument as [%%#u]: [%#u]\n", -40);
	printf("Another u positive argument as [%%#u]: [%#u]", 999);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One i negative argument as [%%#i]: [%#i]\n", -4054);
	printf("Another i positive argument as [%%#i]: [%#i]\n", 90909);
	printf("Another i ZERO argument as [%%#i]: [%#i]", 0);
	printf("		<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One d negative argument as [%%#d]: [%#d]\n", -1);
	printf("Another d positive argument as [%%#d]: [%#d]\n", 909);
	printf("Another d ZERO argument as [%%#d]: [%#d]", 0);
	printf("		<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One x argument as [%%#x]: [%#x]", 2222);
	printf("		<The result is the alternative form of Hexadecimal Number.....>\n");
	printf("One X argument as [%% X]: [% X]", 2222);
	printf("			<The result is the alternative form of Hexadecimal Number.....>\n");
	printf("-----WITH MINIMUM FIELD WIDTH TESTS-----\n");
	printf("Since the Space Flag only had a defined behaviour for %%x and %%X then I assume it should only work with those\n");
	printf("Mix with flag MFW [Hello Robot <%%#010x>, I guess my fav num is <%%#5X>. I have <%%#3x> friends :(]\n");
	printf("RESULT: Hello Robot <%#010x>, I guess my fav num is <%#5X>. I have <%#3x> friends :(\n", -445566, 999, 0, 999, 00);
	printf("The result is expected but with one corner case: When the mwf is equals to the characters printer then we must ADD one space, so short, ther should at least exist one space (for the sign)!.....");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG CARDINAL-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	testflagDOT()
{
	int	a = 10;
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST FLAG DOT-\\_/\\_/\\_/\\_/\\_/\\.\nThe DOT flag is used before a number to print MAX WIDTH DECIMALS\n");
        printf("One c argument as [%%.2c]: [%.2c]", 'a');
	printf("			<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One p argument as [%%.1p]: [%.1p]", &a);
	printf("	<The result was unchanged & a warning of undefined behaviour is given!.....>\n");
	printf("One s argument as [%%3.2s]: [%3.2s]", "Hello");
	printf("		<The result was a MAX WIDTH PRECITION for the STRING, you should not print more than PRESITION chars!.....>\n");
	printf("One u negative argument as [%%.1u]: [%.1u]\n", -40);
	printf("Another u positive argument as [%%.1u]: [%.1u]", 999);
	printf("	<The result is that the number MUST BE AT LEAST MAX WIDTH PRESITION LONG, if not, then the flag ZERO with n is used!.....>\n");
	printf("One i negative argument as [%%- .10i]: [%- .10i]\n", -4054);
	printf("Another i positive argument as [%%.6i]: [%.6i]\n", 90909);
	printf("Another i ZERO argument as [%%-4.2i]: [%-4.2i]", 0);
	printf("	<The result is that the number MUST BE AT LEAST MAX WIDTH PRESITION LONG, if not, then the flag ZERO with n is used if the flag MINUS is precent then its used only if there is space remaining.....>\n");
	printf("One d negative argument as [%%.0d]: [%.0d]\n", -1);
	printf("Another d positive argument as [%%.2d]: [%.2d]\n", 909);
	printf("Another d ZERO argument as [%%.10d]: [%.10d]", 0);
	printf("<Same as i!.....>\n");
	printf("One x argument as [%%#.5x]: [%#.5x]", 2222);
	printf("		<The result is same as i But if used with the # it prints BEFORE the sufix.....>\n");
	printf("One X argument as [%%.5X]: [%.5X]", 2222);
	printf("		<The result is same as i But if used with the # it prints BEFORE the sufix......>\n");
	printf("-----WITH MINIMUM FIELD WIDTH TESTS-----\n");
	/*
	TODDO
	printf("Since the Space Flag only had a defined behaviour for %%x and %%X then I assume it should only work with those\n");
	printf("Mix with flag MFW [Hello Robot <%%#010x>, I guess my fav num is <%%#5X>. I have <%%#3x> friends :(]\n");
	printf("RESULT: Hello Robot <%#010x>, I guess my fav num is <%#5X>. I have <%#3x> friends :(\n", -445566, 999, 0, 999, 00);
	printf("The result is expected but with one corner case: When the mwf is equals to the characters printer then we must ADD one space, so short, ther should at least exist one space (for the sign)!.....");
	*/
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST FLAG CARDINAL-/–\\/–\\/–\\/–\\/–\\/.\n");

}

void	test1()
{
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST1-\\_/\\_/\\_/\\_/\\_/\\.\n");
	printf("Hello World");
	printf("\nVS\n");
	ft_printf("Hello World");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST1-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	test2()
{
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST2-\\_/\\_/\\_/\\_/\\_/\\.\n");
	printf("This is %s and I like %p pointers...\\%%", "correct", NULL);
	printf("\nVS\n");
	ft_printf("This is %s and I like %p pointers...\\%%", "correct", NULL);
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST2-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	test3()
{
	int	a;
	int	b;

	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST3-\\_/\\_/\\_/\\_/\\_/\\.\n");
	a = printf(" %x ", -1);
	printf("- RETURN VALUE: %d", a);
	printf("\nVS\n");
	b = ft_printf(" %x ", -1);
	ft_printf("- RETURN VALUE: %d", b);
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST3-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	test4()
{
	int	a;
	int	b;

	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST4-\\_/\\_/\\_/\\_/\\_/\\.\n");
	a = printf(" %p ", NULL);
	printf("- RETURN VALUE: %d", a);
	printf("\nVS\n");
	b = ft_printf(" %p ", NULL);
	ft_printf("- RETURN VALUE: %d", b);
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST4-/–\\/–\\/–\\/–\\/–\\/.\n");
}

void	test5()
{
	printf("\n./\\_/\\_/\\_/\\_/\\_/-TEST5-\\_/\\_/\\_/\\_/\\_/\\.\n");
	printf("Hello World");
	printf("\nVS\n");
	ft_printf("Hello World");
	printf("\n.\\/–\\/–\\/–\\/–\\/–\\-ENDTEST5-/–\\/–\\/–\\/–\\/–\\/.\n");
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{

		printf("ACTUAL BEHAVIOUR:\n--->%s<---\n", av[1]);
		printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
		printf_c();printf_u();printf_i();printf_d();printf_p();printf_s();printf_x();printf_X();
		printf("The number of flags is: %d\n", count_conversions(av[1]));
		printf(".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");

		printf("TEST s\n");
		((void(*)())ft_getf('s'))("I just returned a function!!!", 1);
		printf("\n.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
		printf("TEST d\n");
		((void(*)())ft_getf('d'))(2232323, 1);
		printf("\n.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");

		printf("%s %i%%\n%X\n", av[1], 100, 33333);
		ft_printf("%s %i%%\n%X\n", av[1], 100, 33333);
		test1();test2();test3();test4();test5();

//		test3();test4();
		testMFW();
		testflagSP();
		testflagMINUS();
		testflag0();
		testflagPLUS();
		testflagCARDINAL();
		testflagDOT();
	}
	(void)ac;
	(void)av;
}
