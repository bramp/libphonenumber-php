#include "php_phonenumber.h"

zend_class_entry *phonenumber_util_ce;

PHP_METHOD(PhoneNumberUtil, __construct)
{
	PhoneNumberUtil::GetInstance();
}

PHP_METHOD(PhoneNumberUtil, isAlphaNumber)
{
	Car *car;
	car_object *obj = (car_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	car = obj->car;
	if (car == NULL) {
		RETURN_NULL();
    	}
	RETURN_BOOLEAN(pn->IsAlphaNumber("hello"));
}

function_entry phonenumber_util_methods[] = {
    PHP_ME(PhoneNumberUtil,   __construct,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(PhoneNumberUtil, isAlphaNumber,   NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(phonenumber)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "PhoneNumberUtil", phonenumber_util_methods);
    phonenumber_util_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}

zend_module_entry vehicles_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_PHONENUMBER_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(phonenumber),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_PHONENUMBER_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PHONENUMBER
extern "C" {
ZEND_GET_MODULE(phonenumber)
}
#endif
