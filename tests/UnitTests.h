#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <iostream>
#include <stdlib.h>

#define TEST_FUNCTION(func)                        \
    {                                              \
        std::cout << "Testing: " << #func << '\n'; \
        func();                                    \
    }

#define FAIL_IF_FALSE(cond, testid)                                                        \
    {                                                                                      \
        std::cout << "\t[" << ((cond) ? "SUCCESS" : "FAILURE") << "]  " << testid << "\n"; \
        if (!(cond))                                                                       \
            exit(-1);                                                                      \
    }

#endif /* UNITTESTS_H */