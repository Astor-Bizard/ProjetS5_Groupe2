AC_DEFUN([VD_CONFIG_HOST_SUBDIRS],
[AC_CONFIG_SUBDIRS([$1])]
[_vd_subdirs=
 for ac_dir in : $subdirs; do test "x$ac_dir" = x: && continue
   if test x"$ac_dir" = x"$1" ; then :
   else
     _vd_subdirs="$_vd_subdirs $ac_dir"
   fi
 done
 subdirs="$_vd_subdirs"
 host_subdirs="$host_subdirs $1" 
]
[_vd_host="$2"])


AC_DEFUN([VD_OUTPUT_HOST_SUBDIRS],
[
if test "$no_recursion" != yes; then

  # Remove --cache-file, --srcdir, and --disable-option-checking arguments
  # so they do not pile up. Same for --host.
  ac_sub_configure_args=
  ac_prev=
  eval "set x $ac_configure_args"
  shift
  for ac_arg
  do
    case x$ac_prev in
    x) ;;
    *) ac_prev=; continue ;;
    esac
    case $ac_arg in
    -cache-file | --cache-file | --cache-fil | --cache-fi \
    | --cache-f | --cache- | --cache | --cach | --cac | --ca | --c)
      ac_prev=cache_file ;;
    -cache-file=* | --cache-file=* | --cache-fil=* | --cache-fi=* \
    | --cache-f=* | --cache-=* | --cache=* | --cach=* | --cac=* | --ca=* \
    | --c=*)
      ;;
    --config-cache | -C)
      ;;
    -srcdir | --srcdir | --srcdi | --srcd | --src | --sr)
      ac_prev=srcdir ;;
    -srcdir=* | --srcdir=* | --srcdi=* | --srcd=* | --src=* | --sr=*)
      ;;
    -prefix | --prefix | --prefi | --pref | --pre | --pr | --p)
      ac_prev=prefix ;;
    -prefix=* | --prefix=* | --prefi=* | --pref=* | --pre=* | --pr=* | --p=*)
      ;;
    --disable-option-checking)
      ;;
    --host | --hos | --ho)
      ac_prev=host ;;
    --host=* | --hos=* | --ho=*)
      ;;
    host_alias=*)
      ;;
    *)
      case $ac_arg in
      *\'*) ac_arg=`AS_ECHO(["$ac_arg"]) | sed "s/'/'\\\\\\\\''/g"` ;;
      esac
      AS_VAR_APPEND([ac_sub_configure_args], [" '$ac_arg'"]) ;;
    esac
  done
      ac_arg="--host=$_vd_host"
      case $ac_arg in
      *\'*) ac_arg=`AS_ECHO(["$ac_arg"]) | sed "s/'/'\\\\\\\\''/g"` ;;
      esac
      AS_VAR_APPEND([ac_sub_configure_args], [" '$ac_arg'"])

  # Always prepend --prefix to ensure using the same prefix
  # in subdir configurations.
  ac_arg="--prefix=$prefix"
  case $ac_arg in
  *\'*) ac_arg=`AS_ECHO(["$ac_arg"]) | sed "s/'/'\\\\\\\\''/g"` ;;
  esac
  ac_sub_configure_args="'$ac_arg' $ac_sub_configure_args"

  # Pass --silent
  if test "$silent" = yes; then
    ac_sub_configure_args="--silent $ac_sub_configure_args"
  fi

  # Always prepend --disable-option-checking to silence warnings, since
  # different subdirs can have different --enable and --with options.
  ac_sub_configure_args="--disable-option-checking $ac_sub_configure_args"

  ac_popdir=`pwd`
  for ac_dir in : $host_subdirs; do test "x$ac_dir" = x: && continue

    # Do not complain, so a configure script can configure whichever
    # parts of a large source tree are present.
    test -d "$srcdir/$ac_dir" || continue

    ac_msg="=== configuring for host=$_vd_host build in $ac_dir (`pwd`/$ac_dir)"
    _AS_ECHO_LOG([$ac_msg])
    _AS_ECHO([$ac_msg])
    AS_MKDIR_P(["$ac_dir"])
    _AC_SRCDIRS(["$ac_dir"])

    cd "$ac_dir"

    # Check for guested configure; otherwise get Cygnus style configure.
    if test -f "$ac_srcdir/configure.gnu"; then
      ac_sub_configure=$ac_srcdir/configure.gnu
    elif test -f "$ac_srcdir/configure"; then
      ac_sub_configure=$ac_srcdir/configure
    elif test -f "$ac_srcdir/configure.in"; then
      # This should be Cygnus configure.
      ac_sub_configure=$ac_aux_dir/configure
    else
      AC_MSG_WARN([no configuration information is in $ac_dir])
      ac_sub_configure=
    fi

    # The recursion is here.
    if test -n "$ac_sub_configure"; then
      AC_MSG_NOTICE([running $SHELL $ac_sub_configure $ac_sub_configure_args --srcdir=$ac_srcdir])
      # The eval makes quoting arguments work.
      eval "\$SHELL \"\$ac_sub_configure\" $ac_sub_configure_args \
	   --srcdir=\"\$ac_srcdir\"" ||
	AC_MSG_ERROR([$ac_sub_configure failed for $ac_dir])
    fi

    cd "$ac_popdir"
  done
fi
])
