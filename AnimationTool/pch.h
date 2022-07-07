// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
#pragma once
#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma comment(linker, "/ENTRY:wWinMainCRTStartup /subsystem:console")

#include "framework.h"
#include "glad/glad.h"
#include "ozz/options/options.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <tchar.h>

static bool GL_ARB_instanced_arrays_supported = true;
static CEvent CreationReached(FALSE, TRUE);
#ifndef GL_OES_read_format
#define GL_OES_read_format 1
#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B



#endif /* GL_OES_read_format */


#endif //PCH_H


