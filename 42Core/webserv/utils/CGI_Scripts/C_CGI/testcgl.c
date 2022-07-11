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

int main(int argc, char **argv)
{
	int	i;
	int	animalsc;
	char	**animalsv;


	if (cgl_initenv() == -1) {
		cgl_perror(stderr, "testcgl");
		exit(1);
	}


	if (strcmp(cgl_Env->request_method, "GET") == 0) {

		cgl_put_cookie("testcgl", "testvalue", NULL, 
		    "/cgi-bin/testcgl", NULL, 0);
		cgl_html_header();

		cgl_html_begin("CGL Library Test");
		cgl_put_heading(2, "CGL Library Test");

		printf("<form action=/cgi-bin/cgl_cgi.c method=POST>\n");
		printf("Select animal(s): ");
		printf("  <select name=animal multiple valign=top>\n");
		printf("    <option>Giraffe\n");
		printf("    <option>Lion\n");
		printf("    <option>Zebra\n");
		printf("    <option>Hyena\n");
		printf("  </select>\n");
		printf("<p>\n");
		printf("Type some text: ");
		printf("<input type=text size=20 maxlen=40 name=text>\n");
		printf("<p>\n");
		cgl_put_hidden("hidden", "ahiddenvalue");
		printf("<input type=submit value=\"Test Query\">\n");
		printf("</form>\n");

		cgl_html_end();

	} else {

		cgl_content_header("text/plain");

		printf("Environment:\n");
		cgl_dumpenv(stdout);
		printf("\n==============================================\n\n");
		fflush(stdout);

		if (cgl_initformdata() == -1) {
			cgl_perror(stderr, "testcgl form data");
			exit(1);
		}

		printf("Form data:\n");
		cgl_dumpform(stdout);
		printf("\n==============================================\n\n");
		fflush(stdout);

		/*
 		if (cgl_initcookies() == -1) {
			cgl_perror(stderr, "testcgl coockies");
 			exit(1);
		}

		printf("Cookies:\n");
		cgl_dumpdata(cgl_Cookies, stdout);
		fflush(stdout);

		printf("\n==============================================\n\n");

		*/

		printf("Search test (find \"animal\"):\n");
		animalsv = cgl_getvalues(&animalsc, "animal");
		if (animalsc == -1) {
			cgl_perror(stderr, "testcgl");
 			exit(1);
		}

		if (animalsc > 0) {
			for (i = 0; i < animalsc; i++)
				printf("  %s\n", animalsv[i]);
		} else
			printf("  (no animals found)\n");

		printf("\n==============================================\n\n");

		fflush(stdout);

		cgl_freeformdata();

	}

	return 0;
}

/* END */
