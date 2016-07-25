#
# System.make: settings extracted from the oasys configuration
#
# System.make.  Generated from System.make.in by configure.

#
# Programs
#
AR		= ar
RANLIB		= ranlib
DEPFLAGS	= -MMD -MP -MT "$*.o $*.E"
INSTALL 	= /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA 	= ${INSTALL} -m 644
PYTHON		= /usr/bin/python
PYTHON_BUILD_EXT= 
XSD_TOOL	= 

#
# System-wide compilation flags including the oasys-dependent external
# libraries.
#
SYS_CFLAGS          = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
SYS_EXTLIB_CFLAGS   =  -I/opt/tcl/include -I/opt/db/include
SYS_EXTLIB_LDFLAGS  =  -ldl -lm  -L/opt/tcl/lib -ltcl8.5 -lexpat -lxerces-c -lz  -L/opt/db/lib -ldb-5.3  -lodbc -lpthread 

#
# Library-specific compilation flags
TCL_LDFLAGS     = -L/opt/tcl/lib -ltcl8.5

