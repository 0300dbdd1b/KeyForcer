#ifndef PLATFORM_H
 #define PLATFORM_H

 #if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
 #endif

 #if defined(__linux__)
    #define PLATFORM_LINUX
 #endif

 #if defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_DARWIN
 #endif

/********************************************************/

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
#endif

#ifdef PLATFORM_LINUX
    #include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <openssl/evp.h>
	#include <openssl/rand.h>
	#include <openssl/sha.h>

	#include "BIP39.h"
#endif

#ifdef PLATFORM_DARWIN
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <openssl/evp.h>
	#include <openssl/rand.h>
	#include <openssl/sha.h>

	#include "BIP39.h"
#endif



#endif // PLATFORM_H

