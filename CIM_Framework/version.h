/****************************************************************
Copyright (c) 2018, Intel Corporation. All rights reserved.
*****************************************************************/
#ifndef CIMFRAMEWORK_VERSION_H_
#define CIMFRAMEWORK_VERSION_H_

#define MAJOR_VERSION              99
// Minor version must be changed if external API is changed
#define MINOR_VERSION              9
// This value is changed when a change is made that does not affect the API
#define QUICK_FIX_NUMBER           9 
#define VER_BUILD                  7001


#define _MAKE_VER_STRING(maj, min, submin, bld)    #maj "." #min "." #submin "." #bld
#define MAKE_VER_STRING(maj, min, submin, bld)    _MAKE_VER_STRING(maj, min, submin, bld)

#define VER_PRODUCTVERSION          MAJOR_VERSION,MINOR_VERSION,QUICK_FIX_NUMBER,VER_BUILD
#define VER_PRODUCTVERSION_STR      MAKE_VER_STRING(MAJOR_VERSION, MINOR_VERSION, QUICK_FIX_NUMBER, VER_BUILD)

#endif
