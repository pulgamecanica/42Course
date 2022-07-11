/*
 *	EXECUTE: gcc -o cgi_atoi_base cgl_atoi_base.c cgi_atoi_base.c -lcgl -I .
*/

/*
	CGL library test proggie
	$Id: testcgl.c,v 1.7 2000/12/13 06:16:38 harding Exp $
*/

/*
 * Copyright (c) 1998-2000 Carson S.K. Harding
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cgl.h"


int	ft_atoi_base(char *str, char *base);

int main(int argc, char **argv)
{
	int	len1;
	int	len2;
	char	**string_l;
	char	**base_l;
	char	*str;
	char	*base;

	if (cgl_initenv() == -1) {
		cgl_perror(stderr, "testcgl");
		exit(1);
	}

	if (strcmp(cgl_Env->request_method, "POST") == 0) {
		cgl_content_header("text/plain");
		
		fflush(stdout);
		if (cgl_initformdata() == -1) {
			cgl_perror(stderr, "testcgl form data");
			exit(1);
		}
		printf("<h5>Calculator<h5>\n");
		fflush(stdout);
		string_l = cgl_getvalues(&len1, "string");
		if (len1 == -1) {
			cgl_perror(stderr, "testcgl string");
 			exit(1);
		}
		if (len1 > 0) {
			str = strdup(string_l[0]);
		} else {
			printf("(Wrong input...)\n");
			fflush(stdout);
			exit(1);
		}
		base_l = cgl_getvalues(&len2, "base");
		if (len2 == -1) {
			cgl_perror(stderr, "testcgl base");
 			exit(1);
		}
		if (len2 > 0) {
			base = strdup(base_l[0]);
		} else {
			printf("(Wrong input...)\n");
			fflush(stdout);
			exit(1);
		}
		printf("<p>String: <span class=\"str\">%s</span></p>\n", str);
		fflush(stdout);
		printf("<p>Base: <span class=\"base\"><span>%s</span></span></p>\n", base);
		fflush(stdout);
		printf("<h3>Decimal Result: <span class=\"result\">%d</span></h3s>\n", ft_atoi_base(str, base));
		free(str);
		free(base);
		fflush(stdout);
		cgl_freeformdata();
	}
	else {
		cgl_perror(stderr, "(Wrong methog...This CGI accepts POST's requests only)");
		printf("(Wrong methog...This CGI accepts POST's requests only)\n");
	}
	fflush(stdout);
	return (0);
}

/* END */
