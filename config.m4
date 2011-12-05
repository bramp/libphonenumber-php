PHP_ARG_ENABLE(phonenumber,
	[Whether to enable libphonenumber support],
	[ --enable-phonenumber   Enable libphonenumber support])

if test "$PHP_PHONENUMBER" = "yes"; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(PHONENUMBER_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, PHONENUMBER_SHARED_LIBADD)
  PHP_NEW_EXTENSION(phonenumber, phonenumber.cc, $ext_shared)
fi
