dnl Checks for required headers and functions
dnl
dnl Version: 20230220

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

  dnl Headers included in regftools/log_handle.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
  ])

  dnl Headers included in regftools/regfmount.c
  AC_CHECK_HEADERS([errno.h sys/time.h])

  dnl Functions included in regftools/mount_file_system.c and regftools/mount_file_entry.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_FUNCS([clock_gettime getegid geteuid time])
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

