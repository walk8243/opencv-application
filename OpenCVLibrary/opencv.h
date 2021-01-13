#pragma once
#include <string>

#ifdef OPENCVLIBRARY_EXPORTS
#define OPENCVLIBRARY_API __declspec(dllexport)
#else
#define OPENCVLIBRARY_API __declspec(dllimport)
#endif

extern "C" OPENCVLIBRARY_API bool is_same_image(const std::string refFilepath, const std::string targetFilepath, const bool debugFlag) throw(std::runtime_error);
