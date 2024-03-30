#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#define ECOSIM_CATCH_AND_CALL(exception, func) \
    catch (exception) { func; }

#endif /* EXCEPTIONS_H */