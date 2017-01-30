dnl Function to detect if libregf dependencies are available
AC_DEFUN([AX_LIBREGF_CHECK_LOCAL],
  [dnl Headers included in libregf/libregf_file.h, libregf/libregf_key.h
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
  dnl Check for internationalization functions in libregf/libregf_i18n.c 
  AC_CHECK_FUNCS([bindtextdomain])

  dnl Check if library should be build with verbose output
  AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

  dnl Check if library should be build with debug output
  AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])
    ])
  ])
])

dnl Function to detect if regftools dependencies are available
AC_DEFUN([AX_REGFTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Check if tools should be build as static executables
  AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_SUBST(
        [LIBREGF_DLL_IMPORT],
        ["-DLIBREGF_DLL_IMPORT"])
    ])
  ])
])

