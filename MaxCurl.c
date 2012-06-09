
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
	
	c = class_new("MaxCurl", (method)maxcurl_new, (method)maxcurl_free, 
                sizeof(t_maxcurl), 0L, 0);
	class_addmethod(c, (method)maxcurl_bang, "bang", 0);
	
  // allows the class to be searched when a user types it into the box
	class_register(CLASS_BOX, c);
	s_maxcurl_class = c;
	return EXIT_SUCCESS;
}

void *maxcurl_new(t_atom *url) {
  curl_global_init(CURL_GLOBAL_ALL);
	t_maxcurl *maxcurl_proto = (t_maxcurl *)object_alloc(s_maxcurl_class);
	maxcurl_proto->m_url = (char *)&url->a_w;
  maxcurl_proto->m_outlet = outlet_new(maxcurl_proto, NULL);
	return maxcurl_proto;
}

void maxcurl_free() {
  curl_global_cleanup();
}

const char* _maxcurl_doCurl(const char *url, 
                            size_t (*callback)(char*, size_t, size_t, void*)) {
  char *result_buffer = NULL;
  curl = curl_easy_init();
  if (!curl) {
    error("cURL Error: Could not set up cURL client");
  }
  
  curl_easy_setopt(curl, CURLOPT_URL, *url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, result_buffer);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
  
  resp = curl_easy_perform(curl);
  
  // TODO: If resp == Error, complain
  
  return "null";
}

void maxcurl_bang(t_maxcurl *x) {
	post("Requested url: %s", x->m_url);
}