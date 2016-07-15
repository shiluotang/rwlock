ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall
AM_CFLAGS	= 
AM_CXXFLAGS	= 

lib_LTLIBRARIES = librwlock.la
librwlock_la_SOURCES = ./src/rwlock.c
librwlock_la_LDFLAGS = -no-undefined

noinst_PROGRAMS = test1
test1_SOURCES = ./src/main.c
test1_LDADD = librwlock.la
