/**
* @file mc_curl_utils.c
*
* mc_curl_utils.h implementation
* 
* @author Travis Kaufman
*/
#include "mc_curl_utils.h"

void t_curl_databuffer_new(t_curl_databuffer *dbuf) {
  dbuf->size = 0;
  dbuf->buffer = malloc(dbuf->size+1);
  dbuf->buffer[0] = '\0';
}

size_t _maxcurl_callback(void* data, size_t size, size_t nmemb, 
                         t_curl_databuffer* userdata) {
  size_t new_size = userdata->size + (size*nmemb);
  userdata->buffer = realloc(userdata->buffer, new_size+1);
  memcpy(userdata->buffer+userdata->size, data, size*nmemb);
  userdata->buffer[new_size] = '\0';
  userdata->size = new_size;
  return size*nmemb;
}

void _maxcurl_curl_easy_bootstrap(CURL* crl, 
                                   CURLcode* cc,
                                   char* url,
                                   t_curl_databuffer* dbuf,
                                   char* ebuf) {
  crl = curl_easy_init();
  if (!crl) {
    error("cURL Error: Could not set up cURL client");
  }
  *cc = curl_easy_setopt(crl, CURLOPT_URL, url);
  if (*cc != CURLE_OK)
    error("Curl error %d: %s", *cc, ebuf);
  *cc = curl_easy_setopt(crl, CURLOPT_WRITEFUNCTION, _maxcurl_callback);
  if (*cc != CURLE_OK)
    error("Curl error %d: %s", *cc, ebuf);
  *cc = curl_easy_setopt(crl, CURLOPT_WRITEDATA, dbuf);
  if (*cc != CURLE_OK)
    error("Curl error %d: %s", *cc, ebuf);
  *cc = curl_easy_setopt(crl, CURLOPT_ERRORBUFFER, ebuf);
  if (*cc != CURLE_OK)
    error("Curl error %d: %s", *cc, ebuf);
}