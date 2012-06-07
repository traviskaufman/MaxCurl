
/*
 ===========================================================================================
 Name        : maxcurlleMax.c
 Author      : Cycling '74
 Version     : 5.1.6
 Copyright   : Not mine
 Description : Making the maxcurlleMax Object from C74s Example folder trying to set Xcode up
 ===========================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "ext.h"
#include "ext_obex.h"

static t_class *s_maxcurl_class; // global pointer to our class definition that is set up in main()

typedef struct _maxcurl {
	t_object s_obj; // t_object header
	long s_value; // something else
} t_maxcurl;

void* maxcurl_new();
const char* _maxcurl_doCurl(const char *url);
void maxcurl_bang(t_maxcurl *x);
void maxcurl_del();

int main() {
	t_class *c = NULL;
	
	c = class_new("MaxCurl", (method)maxcurl_new, (method)maxcurl_del, sizeof(t_maxcurl), 0L, 0);
	class_addmethod(c, (method)maxcurl_bang, "bang", 0); // added the method "bang" invoking maxcurl_bang
	
	class_register(CLASS_BOX, c); // allows the class to be searched when a user types it into the box
	
	s_maxcurl_class = c;
	
	return EXIT_SUCCESS;
}

void *maxcurl_new() {
	t_maxcurl *x = (t_maxcurl *)object_alloc(s_maxcurl_class);
	x->s_value = 0;
	return x;
}

const char* _maxcurl_doCurl(const char *url) {
    // TODO: Implement
    return "null";
}

void maxcurl_bang(t_maxcurl *x) {
	post("value is %ld", x->s_value);
}