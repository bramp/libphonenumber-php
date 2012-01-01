#include "php_phonenumber.h"

#include <string>

//#define USE_GOOGLE_BASE
#include "phonenumbers/phonemetadata.pb.h"
#include "phonenumbers/phonenumber.h"
#include "phonenumbers/phonenumber.pb.h"
#include "phonenumbers/phonenumberutil.h"

using std::string;
using i18n::phonenumbers::PhoneNumberUtil;

PhoneNumberUtil * pn; // Single shared PhoneNumberUtil. Lets hope it is thread safe!

zend_class_entry *phonenumber_util_ce;

/*
void GetSupportedRegions(set<string>* regions) const;
void ConvertAlphaCharactersInNumber(string* number) const;
void NormalizeDigitsOnly(string* number) const;
void GetNationalSignificantNumber(const PhoneNumber& number,
                                  string* national_significant_num) const;
int GetLengthOfGeographicalAreaCode(const PhoneNumber& number) const;
int GetLengthOfNationalDestinationCode(const PhoneNumber& number) const;
void Format(const PhoneNumber& number,
            PhoneNumberFormat number_format,
            string* formatted_number) const;
void FormatByPattern(
    const PhoneNumber& number,
    PhoneNumberFormat number_format,
    const RepeatedPtrField<NumberFormat>& user_defined_formats,
    string* formatted_number) const;


void FormatNationalNumberWithCarrierCode(const PhoneNumber& number,
                                         const string& carrier_code,
                                         string* formatted_number) const;
void FormatNationalNumberWithPreferredCarrierCode(
    const PhoneNumber& number,
    const string& fallback_carrier_code,
    string* formatted_number) const;
void FormatNumberForMobileDialing(
    const PhoneNumber& number,
    const string& region_calling_from,
    bool with_formatting,
    string* formatted_number) const;
void FormatOutOfCountryCallingNumber(
    const PhoneNumber& number,
    const string& calling_from,
    string* formatted_number) const;
void FormatInOriginalFormat(const PhoneNumber& number,
                            const string& region_calling_from,
                            string* formatted_number) const;

void FormatOutOfCountryKeepingAlphaChars(
    const PhoneNumber& number,
    const string& calling_from,
    string* formatted_number) const;
bool TruncateTooLongNumber(PhoneNumber* number) const;
PhoneNumberType GetNumberType(const PhoneNumber& number) const;

bool IsValidNumber(const PhoneNumber& number) const;
bool IsValidNumberForRegion(
    const PhoneNumber& number,
    const string& region_code) const;
void GetRegionCodeForNumber(const PhoneNumber& number,
                            string* region_code) const;
int GetCountryCodeForRegion(const string& region_code) const;
void GetRegionCodeForCountryCode(int country_code, string* region_code) const;
bool IsNANPACountry(const string& region_code) const;
void GetNddPrefixForRegion(const string& region_code,
                           bool strip_non_digits,
                           string* national_prefix) const;
ValidationResult IsPossibleNumberWithReason(const PhoneNumber& number) const;

bool IsPossibleNumber(const PhoneNumber& number) const;
bool IsPossibleNumberForString(
    const string& number,
    const string& region_dialing_from) const;
bool GetExampleNumber(const string& region_code,
                      PhoneNumber* number) const;
bool GetExampleNumberForType(const string& region_code,
                             PhoneNumberType type,
                             PhoneNumber* number) const;
ErrorType Parse(const string& number_to_parse,
                const string& default_region,
                PhoneNumber* number) const;
ErrorType ParseAndKeepRawInput(const string& number_to_parse,
                               const string& default_region,
                               PhoneNumber* number) const;
MatchType IsNumberMatch(const PhoneNumber& first_number,
                        const PhoneNumber& second_number) const;
MatchType IsNumberMatchWithTwoStrings(const string& first_number,
                                      const string& second_number) const;
MatchType IsNumberMatchWithOneString(const PhoneNumber& first_number,
                                     const string& second_number) const;
AsYouTypeFormatter* GetAsYouTypeFormatter(const string& region_code) const;
*/


/*

zend_object_handlers object_handlers;

struct holder_object {
    zend_object std;
    PhoneNumberUtil *num;
};

void free_storage(void *object TSRMLS_DC)
{
	holder_object *obj = (holder_object *)object;
    //delete obj->car;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    holder_object *obj = (holder_object *)emalloc(sizeof(holder_object));
    memset(obj, 0, sizeof(holder_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->default_properties, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

    retval.handle = zend_objects_store_put(obj, NULL, free_storage, NULL TSRMLS_CC);
    retval.handlers = &object_handlers;

    return retval;
}
*/


/*
  // INTERNATIONAL and NATIONAL formats are consistent with the definition
  // in ITU-T Recommendation E. 123. For example, the number of the Google
  // Zürich office will be written as "+41 44 668 1800" in INTERNATIONAL
  // format, and as "044 668 1800" in NATIONAL format. E164 format is as per
  // INTERNATIONAL format but with no formatting applied e.g. +41446681800.
  // RFC3966 is as per INTERNATIONAL format, but with all spaces and other
  // separating symbols replaced with a hyphen, and with any phone number
  // extension appended with ";ext=".
  enum PhoneNumberFormat {
    E164,
    INTERNATIONAL,
    NATIONAL,
    RFC3966
  };

  // Type of phone numbers.
  enum PhoneNumberType {
    FIXED_LINE,
    MOBILE,
    // In some regions (e.g. the USA), it is impossible to distinguish between
    // fixed-line and mobile numbers by looking at the phone number itself.
    FIXED_LINE_OR_MOBILE,
    // Freephone lines
    TOLL_FREE,
    PREMIUM_RATE,
    // The cost of this call is shared between the caller and the recipient, and
    // is hence typically less than PREMIUM_RATE calls. See
    // http://en.wikipedia.org/wiki/Shared_Cost_Service for more information.
    SHARED_COST,
    // Voice over IP numbers. This includes TSoIP (Telephony Service over IP).
    VOIP,
    // A personal number is associated with a particular person, and may be
    // routed to either a MOBILE or FIXED_LINE number. Some more information can
    // be found here: http://en.wikipedia.org/wiki/Personal_Numbers
    PERSONAL_NUMBER,
    PAGER,
    // Used for "Universal Access Numbers" or "Company Numbers". They may be
    // further routed to specific offices, but allow one number to be used for a
    // company.
    UAN,
    // A phone number is of type UNKNOWN when it does not fit any of the known
    // patterns for a specific region.
    UNKNOWN
  };

  // Types of phone number matches. See detailed description beside the
  // IsNumberMatch() method.
  enum MatchType {
    INVALID_NUMBER,  // NOT_A_NUMBER in the java version.
    NO_MATCH,
    SHORT_NSN_MATCH,
    NSN_MATCH,
    EXACT_MATCH,
  };

  enum ErrorType {
    NO_PARSING_ERROR,
    INVALID_COUNTRY_CODE_ERROR,  // INVALID_COUNTRY_CODE in the java version.
    NOT_A_NUMBER,
    TOO_SHORT_AFTER_IDD,
    TOO_SHORT_NSN,
    TOO_LONG_NSN,  // TOO_LONG in the java version.
  };

  // Possible outcomes when testing if a PhoneNumber is possible.
  enum ValidationResult {
    IS_POSSIBLE,
    INVALID_COUNTRY_CODE,
    TOO_SHORT,
    TOO_LONG,
  };
*/

PHP_METHOD(PhoneNumberUtil, isAlphaNumber)
{
	char *number;
	int   number_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &number, &number_len ) == FAILURE) {
		RETURN_NULL();
	}

	string num(number, number_len);

	RETURN_BOOL( pn->IsAlphaNumber(number) );
}

PHP_METHOD(PhoneNumberUtil, GetSupportedRegions) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, ConvertAlphaCharactersInNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, NormalizeDigitsOnly) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetNationalSignificantNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetLengthOfGeographicalAreaCode) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetLengthOfNationalDestinationCode) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, Format) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatByPattern) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatNationalNumberWithCarrierCode) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatNationalNumberWithPreferredCarrierCode) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatNumberForMobileDialing) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatOutOfCountryCallingNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatInOriginalFormat) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, FormatOutOfCountryKeepingAlphaChars) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, TruncateTooLongNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetNumberType) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsValidNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsValidNumberForRegion) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetRegionCodeForNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetCountryCodeForRegion) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetRegionCodeForCountryCode) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsNANPACountry) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetNddPrefixForRegion) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsPossibleNumberWithReason) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsPossibleNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsPossibleNumberForString) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetExampleNumber) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetExampleNumberForType) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, Parse) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, ParseAndKeepRawInput) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsNumberMatch) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsNumberMatchWithTwoStrings) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, IsNumberMatchWithOneString) { RETURN_NULL(); }
PHP_METHOD(PhoneNumberUtil, GetAsYouTypeFormatter) { RETURN_NULL(); }

function_entry phonenumber_util_methods[] = {
    //PHP_ME(PhoneNumberUtil,   __construct,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(PhoneNumberUtil, isAlphaNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetSupportedRegions,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, ConvertAlphaCharactersInNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, NormalizeDigitsOnly,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetNationalSignificantNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetLengthOfGeographicalAreaCode,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetLengthOfNationalDestinationCode,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, Format,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatByPattern,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatNationalNumberWithCarrierCode,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatNationalNumberWithPreferredCarrierCode,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatNumberForMobileDialing,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatOutOfCountryCallingNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatInOriginalFormat,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, FormatOutOfCountryKeepingAlphaChars,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, TruncateTooLongNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetNumberType,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsValidNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsValidNumberForRegion,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetRegionCodeForNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetCountryCodeForRegion,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetRegionCodeForCountryCode,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsNANPACountry,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetNddPrefixForRegion,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsPossibleNumberWithReason,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsPossibleNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsPossibleNumberForString,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetExampleNumber,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetExampleNumberForType,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, Parse,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, ParseAndKeepRawInput,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsNumberMatch,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsNumberMatchWithTwoStrings,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, IsNumberMatchWithOneString,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)
    PHP_ME(PhoneNumberUtil, GetAsYouTypeFormatter,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_ALLOW_STATIC)

    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(phonenumber)
{
#ifdef USE_GOOGLE_BASE
	pn = PhoneNumberUtil::GetInstance();
#else
	pn = new PhoneNumberUtil();
#endif
	//pn->SetLogger();// TODO

	zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "PhoneNumberUtil", phonenumber_util_methods);
    phonenumber_util_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(phonenumber)
{
	delete pn;
	return SUCCESS;
}

zend_module_entry vehicles_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_PHONENUMBER_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(phonenumber),
    PHP_MSHUTDOWN(phonenumber),
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
