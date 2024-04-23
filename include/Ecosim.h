#ifndef ECOSIM_H
#define ECOSIM_H

typedef unsigned int uint;

#define ECOSIM_CATCH_AND_CALL(exception, func) \
    catch (exception) { func; }

#include <SDL3/SDL.h>

#endif /* ECOSIM_H */