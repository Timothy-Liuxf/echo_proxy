# ===========================================================================
#     https://www.gnu.org/software/autoconf-archive/ax_cc_for_build.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CC_FOR_BUILD
#
# DESCRIPTION
#
#   Find a build-time compiler. Sets CC_FOR_BUILD and EXEEXT_FOR_BUILD.
#
#   AX_CC_FOR_BUILD is deprecated; it expands into AX_PROG_CC_FOR_BUILD
#   which renames EXEEXT_FOR_BUILD to BUILD_EXEEXT.
#
# LICENSE
#
#   Copyright (c) 2010 Reuben Thomas <rrt@sc3d.org>
#   Copyright (c) 1999 Richard Henderson <rth@redhat.com>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <https://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 5

AC_DEFUN([AX_CC_FOR_BUILD],
[dnl
m4_warn([obsolete], [$0 is deprecated, please use AX_PROG_CC_FOR_BUILD instead])
AC_REQUIRE([AX_PROG_CC_FOR_BUILD])
dnl Backwards compatibility.
EXEEXT_FOR_BUILD=$BUILD_EXEEXT
AC_SUBST(EXEEXT_FOR_BUILD)])dnl