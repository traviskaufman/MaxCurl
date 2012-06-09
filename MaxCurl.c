
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
char *curl_error_buffer = NULL;
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

void *maxcurl_new(char* url) {
  t_maxcurl *maxcurl_proto = (t_maxcurl *)object_alloc(s_maxcurl_class);
	maxcurl_proto->m_url = url;
  maxcurl_proto->m_outlet = outlet_new(maxcurl_proto, NULL);
  curl_global_init(CURL_GLOBAL_ALL);
  // TODO: Refactor curl stuff
  
	return maxcurl_proto;
}

void maxcurl_free() {
  curl_global_cleanup();
}

char* _maxcurl_doCurl(char *url) {
  curl = curl_easy_init();
  if (!curl) {
    error("cURL Error: Could not set up cURL client");
  }
  
  resp = curl_easy_setopt(curl, CURLOPT_URL, url);
  if (resp != CURLE_OK)
    error("Curl error %d", resp);
  resp = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _maxcurl_callback);
  if (resp != CURLE_OK)
    error("Curl error %d", resp);
  resp = curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_result_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d", resp);
  resp = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d", resp);
  resp = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (resp == CURL_SUCCESS) {
    return curl_result_buffer;
  }// TODO: Errorbuffer
  return curl_error_buffer;
}

size_t _maxcurl_callback(char* data, size_t size, size_t nmemb, 
                         char* userdata) {
  size_t new_size = 
  if (DEBUG)
    post("userdata: %s", userdata);
  if (userdata) {
    realloc(userdata, (sizeof(userdata))+(size*nmemb)+1);
    strcat(userdata, data);
  }
  else
    userdata = data;
  return size*nmemb;
}

void maxcurl_bang(t_maxcurl *x) {
  if (DEBUG) {
    post("Requested url: %s", x->m_url);
  }
  char* crlRes = _maxcurl_doCurl("http://www.google.com");
  if (DEBUG) {
    post("crlRes: %s", crlRes);
  }
  /*t_symbol* mc_curl_result = gensym(crlRes);
  t_atom outlet_data;
  atom_setsym(&outlet_data, mc_curl_result);
  outlet_anything(x->m_outlet, gensym("curlresult"), 1, &outlet_data);*/
}