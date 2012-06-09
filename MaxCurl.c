
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
char *curl_result_buffer = NULL;
// TODO: Errorbuffer
const bool DEBUG = TRUE;

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
  // TODO: Refactor curl stuff
  curl = curl_easy_init();
  if (!curl) {
    error("cURL Error: Could not set up cURL client");
  }
  
  curl_easy_setopt(curl, CURLOPT_URL, *url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _maxcurl_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_result_buffer);
  
	t_maxcurl *maxcurl_proto = (t_maxcurl *)object_alloc(s_maxcurl_class);
	maxcurl_proto->m_url = (char *)&url->a_w;
  maxcurl_proto->m_outlet = outlet_new(maxcurl_proto, NULL);
	return maxcurl_proto;
}

void maxcurl_free() {
  curl_global_cleanup();
}

const char* _maxcurl_doCurl(const char *url) {
  
  resp = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (resp == CURL_SUCCESS) {
    return curl_result_buffer;
  }
  // TODO: else return errorbuffer
  
  return "null";
}

size_t _maxcurl_callback(char* data, size_t size, size_t nmemb, 
                         void *userdata) {
  // TODO: Implement (see https://github.com/bagder/curl/blob/master/docs/examples/htmltitle.cc)
  return 1;
}

void maxcurl_bang(t_maxcurl *x) {
  if (DEBUG)
    post("Requested url: %s", x->m_url);
}