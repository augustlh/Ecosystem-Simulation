#ifndef ECOSIM_H
#define ECOSIM_H

typedef unsigned int uint;

#define ECOSIM_CATCH_AND_CALL(exception, func) \
    catch (exception) { func; }

#include <SDL3/SDL.h>

#ifdef _WIN32

#ifdef ECOSIM_EXPORT
#define ECOSIM_API __declspec(dllexport)
#else
#define ECOSIM_API __declspec(dllimport)
#endif

#else

#define ECOSIM_API

#endif /* _WIN32 */

#endif /* ECOSIM_H */