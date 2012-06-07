
/**
 * @file MaxCurl.c
 *
 * Implementation for MaxCurl. Responsible for instantiating, registering, and 
 * object-ifying within Max's runtime
 */

#include "MaxCurl.h"

// global pointer to our class definition that is set up in main()
static t_class *s_maxcurl_class; 
CURL *curl = NULL;
CURLcode resp = 0;

int main() {
	t_class *c = NULL;
	
	c = class_new("MaxCurl", (method)maxcurl_new, (method)NULL, 
                sizeof(t_maxcurl), 0L, 0);
	class_addmethod(c, (method)maxcurl_bang, "bang", 0); 
	
  // allows the class to be searched when a user types it into the box
	class_register(CLASS_BOX, c);
	s_maxcurl_class = c;
	return EXIT_SUCCESS;
}

void *maxcurl_new() {
	t_maxcurl *x = (t_maxcurl *)object_alloc(s_maxcurl_class);
	x->url = NULL;
	return x;
}

const char* _maxcurl_doCurl(const char *url, 
                            size_t (*callback)(char*, size_t, size_t, void*)) {
  // TODO: Implement
  return "null";
}

void maxcurl_bang(t_maxcurl *x) {
	post("Requested url: %s", x->url);
}