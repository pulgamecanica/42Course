/*
	CGI Library
	$Id: cgl.h,v 1.9 2000/12/13 06:16:38 harding Exp $

	data structures, prototypes, and globals
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

/* 
 * environment passed to CGI program
 */
struct	cglenv {
	char *auth_type;
	char *content_length;
	char *content_type;
	char *document_root;
	char *gateway_interface;
	char *http_accept;
	char *http_cookie;
	char *http_pragma;
	char *http_user_agent;
	char *path_info;
	char *path_translated;
	char *query_string;
	char *remote_addr;
	char *remote_host;
	char *remote_ident;
	char *remote_port;
	char *remote_user;
	char *request_method;
	char *request_uri;
	char *script_filename;
	char *script_name;
	char *server_admin;
	char *server_name;
	char *server_port;
	char *server_protocol;
	char *server_software;
};
typedef struct cglenv cglEnv;


/* 
 * Function prototypes...
 */

   /*
    * initializing environment and form data, retrieving form values
    */

int	cgl_init(void);			/* calls cgl_init*() below          */
int	cgl_initenv(void);		/* malloc and populate cgl_Env.     */
int	cgl_initformdata(void);		/* get form data into cgl_Formdata  */
int 	cgl_initcookies(void);		/* collect cookies into cgl_Cookies */

void	cgl_freeall(void);		/* free form data and buffer        */
void	cgl_freeenv(void);		/* free environment pointers        */
void	cgl_freeformdata(void);		/* free form data and buffer        */
void	cgl_freecookies(void);		/* free cookie data and buffer      */

char 	*cgl_getvalue(char *name);      /* return pointer to value          */
char	**cgl_getvalues
	   (int *count, char *name);    /* return array of pointers to      */
					/* values for name. 		    */
char 	*cgl_getcookie(char *name);     /* return pointer to value          */
char	**cgl_getcookies
	   (int *count, char *name);    /* return array of pointers to      */
					/* values for name. 		    */

   /*
    * encoding and decoding...
    */

int	cgl_urlencode(char *s, FILE *fw);
void	cgl_urldecode(char *s);
int	cgl_urlescape(char *s, FILE *fw);
int	cgl_urlunescape(char *s);
int	cgl_htmlencode(char *s, FILE *fw);
void	cgl_htmldecode(char *s);
int	cgl_htmlescape(const char *s, FILE *fw);
void	cgl_htmlunescape(char *s);
char	cgl_hex2char(char *s);
void	cgl_charify(char *s, char from, char to);


   /*
    * output...
    */

void	cgl_html_header(void);
void	cgl_content_header(char *type);
void	cgl_nph_header(char *version, char *status);
void	cgl_status_header(char *status);
void	cgl_location_header(char *location);
void	cgl_pragma_header(char *s);
int	cgl_accept_image(void);

void	cgl_html_begin(char *title);
void	cgl_html_end(void);
void	cgl_put_heading(int level, char *heading);
void	cgl_put_hidden(char *name, char *value);

int	cgl_put_cookie(char *name, char *opaque, 
	    char *expires, char *path, char *domain, int secure);

char	*cgl_cookietime(time_t *t);
char 	*cgl_stradd(char *s1, char *s2);
char 	*cgl_mkpath(char *p, char *f, char *x);

void	cgl_perror(FILE *fw, char *s);
char	*cgl_strerror(void);
void	cgl_dump(FILE *fw);
void	cgl_dumpenv(FILE *fw);
void	cgl_dumpform(FILE *fw);

char	*cgl_getenv(char *);
char	*cgl_strdup(const char *s);


/* ------------------------------------------------------------------------ *
 * HASHED LISTS
 * ------------------------------------------------------------------------ */



/* name/value pair in hashed list*/
typedef struct hashedlistnode cglnode;
struct hashedlistnode {
	cglnode	*next;
	cglnode	*prev;
	cglnode	*bucket;
	cglnode	*dupe;
	char	*name;
	void	*value;
};


/* the header and associated control data for a hashed list */
typedef struct hashedlist cgllist;
struct	hashedlist {
	cglnode	*head;			/* pointers to members of list       */
	cglnode	*tail;
	cglnode	*cur;
	struct {
		int	h_control;	/* to hash or not to hash            */
#define CGL_HASH_OFF	-1		/* to preset, set cgl_def_hcontrol   */
#define CGL_HASH_AUTO	0
		int	h_unique;	/* no dupes!                         */
#define CGL_HASH_DUPES	0
#define CGL_HASH_UNIQUE	1
		int	h_mincount;	/* start hashing if this many        */
#define CGL_MINTOHASH	11		/* to preset, set cgl_def_mincount   */
		int	h_count;	/* number of unique names            */
		int	h_collisions;	/* count of hash collisions          */
		int	h_used;		/* buckets used                      */
		int	h_resize;	/* times resized                     */
		int	h_size;	        /* size of hash table                */
		cglnode	**h_table; 	/* array of bucket ptrs              */
	} hash;
	int	count;		/* number of name/value pairs in list        */
	int	maxcount;	/* maximum number of pairs accepted          */
#define CGL_MAXCOUNT	1000	/* default maxcount: preset cgl_def_maxcount */
	int	source;		/* source of data XXXXXX                     */
#define CGL_METHOD_NONE	0
#define CGL_METHOD_POST	1
#define CGL_METHOD_GET	2
};


char	*cgl_getnodevalue(cgllist *cdata, char *name);
char	**cgl_getnodevalues(cgllist *cdata, int *count, char *name);
cglnode	*cgl_fetchnode(cgllist *cdata, char *name);

int 	cgl_insertnode(cgllist *cdata, char *name, void *value, int where);
#define CGL_INSERT_TAIL	0			/* after cdata->tail      */
#define CGL_INSERT_HEAD	1			/* before cdata->head     */
#define CGL_INSERT_PREV	2			/* before cdata->cur      */
#define CGL_INSERT_NEXT	3			/* after cdata->cur       */

int	cgl_deletenode(cgllist *cdata);

int	cgl_hashlist(cgllist *cdata, int hashsz);
void	cgl_freedata(cgllist *cdata);
cgllist	*cgl_newlist(void);
cgllist	*cgl_newhash(void);

cglnode	*cgl_firstnode(cgllist *cdata);
cglnode	*cgl_nextnode(cgllist *cdata);
cglnode	*cgl_prevnode(cgllist *cdata);
cglnode	*cgl_lastnode(cgllist *cdata);

void	cgl_dumpdata(cgllist *cdata, FILE *fw);
void	cgl_dumphstats(cgllist *cdata, FILE *fw);
void	cgl_dumphash(cgllist *cdata, FILE *fw); 

void	cgl_dumprawdata(cgllist *cdata, FILE *fw);
void	cgl_dumprawhash(cgllist *cdata, FILE *fw);



/* ------------------------------------------------------------------------ *
 * GLOBALS
 *
 * initialized by cgl_init(), cgl_initenv(), cgl_initcookies(),
 * and cgl_initformdata()
 * ------------------------------------------------------------------------ */

extern	cglEnv	*cgl_Env;	/* for data from server-set environment     */
extern	int	cglerrno;	/* internal library error code              */
extern	int	cglsyserr;	/* save system errno                        */
extern	char	*cglerrlist[];	/* list of error descriptions for cglerrno  */
extern	cgllist	*cgl_Formdata;	/* data read from form (POST or GET)        */
extern	cgllist	*cgl_Cookies;	/* cookies garnered                         */

/*
 * default control values and data limits
 */
extern	int cgl_def_hcontrol;		/* default hashing behaviour         */
extern	int cgl_def_mincount;		/* start hashing at                  */
extern	int cgl_def_maxcount;		/* max nodes hashed list             */
#define CGL_MAXDATA (1024*1024)
extern	int cgl_def_maxdata;		/* max bytes we will accept from form*/


/* END */

