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
void* maxcurl_new(t_symbol* url);

/**
 * Destructor
 */
void maxcurl_free();                          

/**
 * React to "bang" message from MaxMSP client
 *
 * @param t_maxcurl *mxcrl
 *    The MaxCurl object instance 
 */
void maxcurl_bang(t_maxcurl *mxcrl);

/**
 * Performs a cURL and returns the data from the page
 *
 * @param const char *url
 *    The url for which you would like to get data from
 *
 * @return char*
 *    Returns all of the bulk data retrieved from the cURL request, or NULL
 *    if the request was unsuccessful
 */
char* _maxcurl_doCurl(char* url);

// "String" struct for cURL
typedef struct __s {
  char* buffer;
  size_t size;
} t_curl_databuffer;

// "String intialization" method
void t_curl_databuffer_new(t_curl_databuffer* dbuf);

// Reset the buffer
void t_curl_databuffer_reset(t_curl_databuffer* dbuf);

/**
 * callback function used by _maxcurl_doCurl. see the libCurl API for more
 * info on cURL callback functions
 *
 * @see _maxcurl_doCurl
 */
size_t maxcurl_callback(void* data, size_t size, size_t nmemb,
                         t_curl_databuffer* userdata);

#endif