#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Arm Examples, 1.0, Guillaume.Huard@imag.fr, arm-examples)
AC_CONFIG_AUX_DIR([../build-aux])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([example1.s])
AC_CONFIG_HEADER([config.h])

AC_MSG_NOTICE([configuring ARM examples])

# Checks for programs.
AC_PROG_CC
AM_PROG_AS
AM_PROG_CC_C_O

AC_MSG_CHECKING([whether the compiler is targeting ARM])
if $CC -dumpmachine | grep arm > /dev/null ; then
  AC_MSG_RESULT([yes])
  have_arm_compiler=yes
else
  AC_MSG_RESULT([no])
  have_arm_compiler=no
  AC_MSG_WARN([An ARM cross-compiler is required to compile the examples])
fi
AM_CONDITIONAL([HAVE_ARM_COMPILER],[test $have_arm_compiler = yes])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
