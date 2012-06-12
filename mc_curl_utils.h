/**
* @file mc_curl_utils.h
*
* Contains utility types and functions used for cURLing in MaxCurl object
*
* @package MaxCurl
*
* @author Travis Kaufman
*/
#ifndef MaxCurl_mc_curl_utils_h
#define MaxCurl_mc_curl_utils_h
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "MaxCurl.h"

// "String" struct for cURL
typedef struct __s {
  char* buffer;
  size_t size;
} t_curl_databuffer;

// "String intialization" method
void t_curl_databuffer_new(t_curl_databuffer* s);

/**
 * callback function used by _maxcurl_doCurl. see the libCurl API for more
 * info on cURL callback functions
 *
 * @see _maxcurl_doCurl
 */
size_t _maxcurl_callback(void* data, size_t size, size_t nmemb, 
                         t_curl_databuffer* userdata);

/**
 * Get cURL client ready to perform an "easy" cURL
 *
 * @param CURL* crl
 *    The cURL object
 *
 * @param CURLcode cc
 *    CURLcode object that will hold the response state
 *
 * @param char* url
 *    The string representing the url that you want to cURL
 *
 * @param t_curl_databuffer* dbuf
 *    The buffer which will store the data retrieved by cURL
 *
 * @param char* ebuf
 *    The buffer which will store any errors output by cURL
 *
 */
void _maxcurl_curl_easy_bootstrap(CURL* crl, 
                                   CURLcode* cc,
                                   char* url,
                                   t_curl_databuffer* dbuf,
                                   char* ebuf);
#endif
