// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#ifdef DrawText
#undef DrawText
#endif
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#ifdef GetObject
#undef GetObject
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef GetUserName
#undef GetUserName
#endif
#ifdef CreateFont
#undef CreateFont
#endif
#ifdef Button
#undef Button
#endif


#include <windowsx.h>
#include <intrin.h> 
#include <fstream>

#include <iostream>
#include <fstream>
#include <ppltasks.h>
#include <windowsx.h>
#include <psapi.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <list>
#include <string>
#include <map>
#include <TlHelp32.h>
#include <thread>
#include <math.h>
#include <cmath>
#include <vector>
#include <d2d1_1.h>
#include <dwrite.h>
// only add headers here that wont frequently be updated. such as frameworks
#include "Colour.h"
#include "Graphics.h"
#include "font.h"
#include "Vector.h"
#include "Input.h"
#endif //PCH_H