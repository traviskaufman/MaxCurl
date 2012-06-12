
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
tkstring curl_result_buffer;
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
  post("URL: %s", url->s_name);
  t_maxcurl *maxcurl_proto = (t_maxcurl *)object_alloc(s_maxcurl_class);
	maxcurl_proto->m_url = url->s_name;
  maxcurl_proto->m_outlet = outlet_new(maxcurl_proto, NULL);
  curl_global_init(CURL_GLOBAL_ALL);
  tkstring_new(&curl_result_buffer);
  // TODO: Refactor curl stuff
  
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
  resp = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _maxcurl_callback);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_result_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
  if (resp != CURLE_OK)
    error("Curl error %d: %s", resp, curl_error_buffer);
  resp = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if (resp == CURL_SUCCESS) {
    return curl_result_buffer.buffer;
  }
  return NULL;
}

size_t _maxcurl_callback(void* data, size_t size, size_t nmemb, 
                         tkstring* userdata) {
  size_t new_size = userdata->size + (size*nmemb);
  userdata->buffer = realloc(userdata->buffer, new_size+1);
  memcpy(userdata->buffer+userdata->size, data, size*nmemb);
  userdata->buffer[new_size] = '\0';
  userdata->size = new_size;
  if (DEBUG) {
    if (data)
      post("data: %s", data);
    if (userdata)
      post("userdata: %s", userdata->buffer);
  }
  return size*nmemb;
}

void maxcurl_bang(t_maxcurl *x) {
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

void tkstring_new(tkstring *s) {
  s->size = 0;
  s->buffer = malloc(s->size+1);
  s->buffer[0] = '\0';
}