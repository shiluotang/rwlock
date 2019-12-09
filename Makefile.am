ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall -Werror
AM_CFLAGS	=
AM_CXXFLAGS	=
AM_LDFLAGS	=

lib_LTLIBRARIES = librwlock.la
librwlock_la_SOURCES = ./src/rwlock.c
librwlock_la_LDFLAGS = -no-undefined

check_PROGRAMS = test1
test1_SOURCES = ./src/test1.c
test1_LDADD = librwlock.la

TESTS = $(check_PROGRAMS)
