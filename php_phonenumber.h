#ifndef PHP_HELLO_H
#define PHP_HELLO_H 1

#define PHP_PHONENUMBER_EXTNAME "phonenumber"
#define PHP_PHONENUMBER_EXTVER  "1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

extern zend_module_entry phonenumber_module_entry;
#define phpext_phonenumber_ptr &phonenumber_module_entry;

#endif


