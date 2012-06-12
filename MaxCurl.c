
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
const int CURL_SUCCESS = 0;
t_curl_databuffer curl_result_buffer;
char *curl_error_buffer = NULL;
const bool DEBUG = TRUE;

int main() {
	t_class *c = NULL;
	
	c = class_new("MaxCurl", (method)maxcurl_new, (method)maxcurl_free, 
                sizeof(t_maxcurl), 0L, A_DEFSYM, 0);
	class_addmethod(c, (method)maxcurl_bang, "bang", 0);
	
  // allows the class to be searched when a user types it into the box
	class_register(CLASS_BOX, c);
	s_maxcurl_class = c;
	return EXIT_SUCCESS;
}

void *maxcurl_new(t_symbol* url) {
  if (DEBUG) {
    post("URL: %s", url->s_name);
  }
  t_maxcurl *maxcurl_proto = (t_maxcurl *)object_alloc(s_maxcurl_class);
	maxcurl_proto->m_url = url->s_name;
  maxcurl_proto->m_outlet = outlet_new(maxcurl_proto, NULL);
  curl_global_init(CURL_GLOBAL_ALL);
  t_curl_databuffer_new(&curl_result_buffer);
  
	return maxcurl_proto;
}

void maxcurl_free() {
  curl_global_cleanup();
  free(curl_result_buffer.buffer);
  free(&curl_result_buffer);
}

char* _maxcurl_doCurl(char *url) {
  _maxcurl_curl_easy_bootstrap(curl, 
                               &resp,
                               url, 
                               &curl_result_buffer, 
                               curl_error_buffer);
  resp = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (resp == CURL_SUCCESS) {
    return curl_result_buffer.buffer;
  }
  return NULL;
}

void maxcurl_bang(t_maxcurl *x) {
  // TODO: This isn't doing anything. Make it work with threading
  if (x->is_curling) {
    error("Error: cURL operation in progress");
    return;
  }
  if (DEBUG) {
    post("Requested url: %s", x->m_url);
  }
  x->is_curling = TRUE;
  char* crlRes = _maxcurl_doCurl(x->m_url);
  x->is_curling = FALSE;
  if (DEBUG) {
    post("crlRes: %s", crlRes);
  }
  t_atom outlet_data[1];
  t_max_err setsym_result = (crlRes != NULL) ? 
                        atom_setsym(outlet_data, gensym(crlRes)) : 
                        atom_setsym(outlet_data, gensym(curl_error_buffer));
  if (setsym_result == MAX_ERR_NONE) {
    if (DEBUG) {
      post("No atom_setsym errors");
    }
    outlet_anything(x->m_outlet, gensym("curlresult"), 1, outlet_data);
  }
  else {
    error("%s", setsym_result);
  }
}