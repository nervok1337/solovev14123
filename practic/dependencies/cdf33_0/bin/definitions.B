#------------------------------------------------------------------------------
#
#  NSSDC/CDF					    Definitions for using CDF.
#						    Bourne shell.
#
#  Version 1.2, 19-Mar-96, Hughes STX.
#
#  Modification History:
#
#   V1.0  14-Jun-95, J Love	Original version.
#   V1.1  18-Jul-95, J Love	Added CDFexport.
#   V1.2  19-Mar-96, J Love	CDF V2.6.
#   V1.2  05-Aug-08, M Liu      CDF V3.3.
#
#-----------------------------------------------------------------------------
# Before using this script file, change the definition of CDF_BASE for where
# CDF was installed on your machine (ie. the directory containing the `bin',
# `lib', and `include' directories).  All CDF users can then use the command
#
#	$ . <path>/bin/definitions.B
#
# to setup the necessary enviroment variables (where <path> is the full
# directory path leading to where CDF was installed).  
#
# Aliases are not available with the Bourne shell.  The toolkit programs must
# be invoked by specifying the actual name of the executable.  The executables
# are found in ${CDF_BIN} (after the installation step of building the CDF
# distribution).  For example, to execute CDFedit the following would be used:
#
#       $ ${CDF_BIN}/cdfedit <qualifiers/options> <parameter(s)>
#
# Note that `{CDF_BIN}' could also also be added to a user's path allowing
# the toolkit programs to be executed simply by specifying their names.
#-----------------------------------------------------------------------------

export CDF_BASE; CDF_BASE=/home/nervok/Programmer/C/2/RSTLite/dependencies/cdf33_0

export CDF_INC; CDF_INC=$CDF_BASE/include
export CDF_LIB; CDF_LIB=$CDF_BASE/lib
export CDF_BIN; CDF_BIN=$CDF_BASE/bin
export CDF_HELP; CDF_HELP=$CDF_BASE/lib/cdf/help

# export IDL_STARTUP; IDL_STARTUP=$CDF_BIN/idlstart.pro

alias cdfedit=$CDF_BIN/cdfedit
alias cdfexport=$CDF_BIN/cdfexport
alias cdfconvert=$CDF_BIN/cdfconvert
alias cdfinquire=$CDF_BIN/cdfinquire
alias cdfstats=$CDF_BIN/cdfstats
alias cdfdump=$CDF_BIN/cdfdump
alias cdfirsdump=$CDF_BIN/cdfirsdump
alias cdfcompare=$CDF_BIN/cdfcompare
alias skeletontable=$CDF_BIN/skeletontable
alias skeletoncdf=$CDF_BIN/skeletoncdf
alias cdfdir=$CDF_BIN/cdfdir
alias cdfirsdump=$CDF_BIN/cdfirsdump
alias cdfmerge=$CDF_BIN/cdfmerge
alias cdfbrowse="$CDF_BIN/cdfedit -prompt -browse"
alias cdfvalidate=$CDF_BIN/cdfvalidate
