
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
  curl = curl_easy_init();
  if (!curl) {
    error("cURL Error: Could not set up cURL client");
  }
  resp = curl_easy_setopt(curl, CURLOPT_URL, url);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, maxcurl_callback);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_result_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  
  
  t_curl_databuffer_reset(&curl_result_buffer);
  resp = curl_easy_perform(curl);
  
  if (DEBUG && resp != CURLE_OK) {
    error("CURL Error: %s", curl_easy_strerror(resp));
  } else {
    post("Done. Data Size: %d.", curl_result_buffer.size);
  }
  curl_easy_cleanup(curl);
  if (resp == CURL_SUCCESS) {
    return curl_result_buffer.buffer;
  }
  return NULL;
}

void maxcurl_bang(t_maxcurl *x) {
  if (DEBUG) {
    post("Requested url: %s", x->m_url);
  }
  char* crlRes = _maxcurl_doCurl(x->m_url);
  if (DEBUG) {
    post("crlRes: %s", crlRes);
    
  }
  t_atom outlet_data[1];
  t_max_err setsym_result = (crlRes != NULL) ? 
                        atom_setsym(outlet_data, gensym(crlRes)) : 
                        atom_setsym(outlet_data, gensym("curl error"));
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

/**
 * @file mc_curl_utils.c
 *
 * mc_curl_utils.h implementation
 *
 * @author Travis Kaufman
 */

void t_curl_databuffer_new(t_curl_databuffer *dbuf) {
  dbuf->size = 0;
  dbuf->buffer = malloc(dbuf->size+1);
  dbuf->buffer[0] = '\0';
}

void t_curl_databuffer_reset(t_curl_databuffer *dbuf) {
  dbuf->buffer = realloc(dbuf->buffer, 1);
  dbuf->size = 0;
  dbuf->buffer[0] = '\0';
}

size_t maxcurl_callback(void* data, size_t size, size_t nmemb,
                         t_curl_databuffer* userdata) {
  size_t new_size = userdata->size + (size*nmemb);
  userdata->buffer = realloc(userdata->buffer, new_size+1);
  if (userdata->buffer == NULL) {
    error("OUT OF MEMORY!");
    return 0;
  }
  
  memcpy(userdata->buffer + userdata->size, data, size*nmemb);
  userdata->buffer[new_size] = '\0';
  userdata->size = new_size;
  return size*nmemb;
}