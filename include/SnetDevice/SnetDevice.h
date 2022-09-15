//
// SnetDevice.h
//
// Library: SnetDevice
// Package: Macro
// Module:  Core
//
// Platform and architecture identification macros
// and platform-specific definitions for Windows.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef _SnetDevice_H
#define _SnetDevice_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_WIN32) && defined(_DLL)
#if !defined(SNETDEVICE_DLL) && !defined(SNETDEVICE_STATIC)
#define SNETDEVICE_DLL
#endif
#endif


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SnetDevice_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SnetDevice_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if (defined(_WIN32) || defined(_WIN32_WCE)) && defined(SNETDEVICE_DLL)
#if defined(SnetDevice_EXPORTS)
#define SnetDevice_API __declspec(dllexport)
#else
#define SnetDevice_API __declspec(dllimport)
#endif
#endif


#if !defined(SnetDevice_API)
#if !defined(SNETDEVICE_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
#define SnetDevice_API __attribute__ ((visibility ("default")))
#else
#define SnetDevice_API
#endif
#endif


//
// Automatically link SnetDevice library.
//
#if defined(_MSC_VER)
#if defined(SNETDEVICE_DLL)
#if defined(_DEBUG)
#define SNETDEVICE_LIB_SUFFIX "d.lib"
#else
#define SNETDEVICE_LIB_SUFFIX ".lib"
#endif
#elif defined(_DLL)
#if defined(_DEBUG)
#define SNETDEVICE_LIB_SUFFIX "mdd.lib"
#else
#define SNETDEVICE_LIB_SUFFIX "md.lib"
#endif
#else
#if defined(_DEBUG)
#define SNETDEVICE_LIB_SUFFIX "mtd.lib"
#else
#define SNETDEVICE_LIB_SUFFIX "mt.lib"
#endif
#endif

#if !defined(SNETDEVICE_NO_AUTOMATIC_LIBS) && !defined(SnetDevice_EXPORTS)
#pragma comment(lib, "SnetDevice" SNETDEVICE_LIB_SUFFIX)
#endif
#endif

//
// Include platform-specific definitions
//
#if defined(_WIN32)
#include "SnetDevice/Platform_WIN32.h"
#endif



#if defined(SNETDEVICE_NO_DEPRECATED)
#define SNETDEVICE_DEPRECATED
#elif defined(_GNUC_)
#define SNETDEVICE_DEPRECATED __attribute__((deprecated))
#elif defined(__clang__)
#define SNETDEVICE_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define SNETDEVICE_DEPRECATED __declspec(deprecated)
#else
#define SNETDEVICE_DEPRECATED
#endif

#include <cassert>
#include <list>
#include <map>
#include <vector>
#include <string>

#endif  // !_SNETDEVICE_COMMON_PCH_H

extern "C" {
	SnetDevice_API int add(int a, int b);
}