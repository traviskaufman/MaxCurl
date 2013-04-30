MAXCURL: A cURL client for MaxMSP
=================================

Overview
--------
MaxCurl is a cURL client built for MaxMSP (built in 5, but should work in 6 as well).
It's goal is to be cross-platform, non-blocking, and able to easily 
transmit info from the web right into your Max Patch; all you need is an internet connection! 

This external is still in its alpha stage; unix only, messy, and single-threaded,
but in the future it will be x-platform, and very clean and modular.

Development
-----------
Clone this repo and then build in XCode. Note that you will probably need to
modify the project search paths (especially for the Max SDK) and you will also
have to have libcurl installed, which I would be very surprised if you're a
developer and you don't.

Usage
-----
After building the .mxo, make sure it's locatable by Max and then create a new
object and type "MaxCurl URL" where "URL" is the url that you want to curl.
After that the outlet assist should take you through the rest. Note that on
success right now it outputs a "curlresult" message followed by the payload, so
you can use something like "route curlresult" to actually get the payload.

Caveats
-------
* This is still in its infancy. It's ridiculously buggy and in no way
recommended for production. Hopefully that will change soon.

* <strong>Do not</strong> try to attach the output to the right inlet of a
message object, a textedit window, or any other structure that has a fixed
memory size for displaying data. Max *will* segfault (at least Max 5 will).
