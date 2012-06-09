/**
 * @file MaxCurl.h
 * 
 * Header file for MaxCurl object
 * 
 * @author Travis Kaufman
 */
#ifndef MaxCurl_MaxCurl_h
#define MaxCurl_MaxCurl_h

// All includes that we will need
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "ext.h"
#include "ext_obex.h"

/////// MaxCurl Object Interface ///////

// The MaxCurl "Class" Struct
typedef struct _maxcurl {
	t_object s_obj; // t_object header
	char *m_url; // URL that will be cURL'd
  void *m_outlet; // MaxMSP Outlet
} t_maxcurl;

/////// Methods ///////

/**
 * Constructor
 */
void* maxcurl_new(t_atom *url);

/**
 * Destructor
 */
void maxcurl_free();

/**
 * Performs a cURL and returns the data from the page
 *
 * @param const char *url
 *    The url for which you would like to get data from
 *
 * @param size_t (*callback)(char*, size_t, size_t, void*)
 *    The callback function to be performed after the cURL is executed
 *    Function must subscribe to the following prototype:
 *      size_t callback(char* ptr, size_t size, size_t nmemb, void *userdata)
 *    See this link for more info:
 *    http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTWRITEFUNCTION 
 */
const char* _maxcurl_doCurl(const char *url, 
                            size_t (*callback)(char*, size_t, size_t, void*));

/**
 * React to "bang" message from MaxMSP client
 *
 * @param t_maxcurl *mxcrl
 *    The MaxCurl object instance 
 */
void maxcurl_bang(t_maxcurl *mxcrl);

#endif