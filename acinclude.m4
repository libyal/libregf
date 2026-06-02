dnl Checks for required headers and functions
dnl
dnl Version: 20260602

dnl Function to detect if libregf dependencies are available
AC_DEFUN([AX_LIBREGF_CHECK_LOCAL],
  [dnl Check for internationalization functions in libregf/libregf_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])

  dnl Headers included in libregf/libregf_file.h, libregf/libregf_key.h
  dnl libregf/libregf_key_item_values.h and libregf/libregf_value_item_values.h
  AC_CHECK_HEADERS([wctype.h])

  dnl Functions used in libregf/libregf_file.h, libregf/libregf_key.h
  dnl libregf/libregf_key_item_values.h and libregf/libregf_value_item_values.h
  AC_CHECK_FUNCS([towupper])

  AS_IF(
    [test "x$ac_cv_func_towupper" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: towupper],
      [1])
    ])
  ])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBREGF_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBREGF_DLL_EXPORT],
        ["-DLIBREGF_DLL_EXPORT"])

      AC_SUBST(
        [LIBREGF_DLL_IMPORT],
        ["-DLIBREGF_DLL_IMPORT"])
      ])
    ])
  ])

