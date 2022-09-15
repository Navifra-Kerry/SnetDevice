//
// UnWindows.h
//
// Library: SnetDevice
// Package: Core
// Module:  UnWindows
//
// Simple wrapper around the <windows.h> header file.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SnetDevice_UnWindows_INCLUDED
#define SnetDevice_UnWindows_INCLUDED


// Reduce bloat
#if defined(_WIN32)
	#if !defined(WIN32_LEAN_AND_MEAN) && !defined(ELBATS_BLOATED_WIN32)
		#define WIN32_LEAN_AND_MEAN
	#endif
#endif


#if !defined(SNETDEVICE_NO_WINDOWS_H)
    #include <windows.h>
    #ifdef __MINGW32__
        #include <Winsock2.h>
        #include <Iphlpapi.h>
        #include <ws2tcpip.h>
    #endif // __MINGW32__
#endif


#endif // SnetDevice_UnWindows_INCLUDED
