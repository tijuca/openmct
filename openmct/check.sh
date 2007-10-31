#!/bin/bash
# $Id; check.sh by carsten 16.10.2007 ver0.1

# This is a script that should check some progs and helping tools
# theese are needed dor building OpenMCT Images .
# You found some errors?  Fine, post it on http://forum.openmct.org
# 
# 
# This script is licensed under GPL!

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


# 
# 

##################################################################################################
#                                  setting up some variables                                     #
##################################################################################################
# Version and Date of the script itself
VER=0.1
DATE=16.10.2007

# check for the file .bashrc or equal
if [ -f $HOME/.bashrc ]; then
    BASHRC=.bashrc
else
    BASHRC=
fi

CUT=`which cut`
GREP=`which grep`
DIE=0

##################################################################################################
#                                          Subroutines                                           #
##################################################################################################

function check_hostapp () {

test -f `which autoconf` && AUTOCONF=`autoconf --version | $GREP "autoconf " | $CUT -f4 -d " "` || AUTOCONF=
test -f `which automake` && AUTOMAKE=`automake --version | $GREP "automake " | $CUT -f4 -d " "` || AUTOMAKE=
test -f `which gawk` && AWK=`gawk --version | $GREP "Awk " | $CUT -f3 -d " "` || AWK=
test -f `which  gcc` && GCC=`gcc --version | $GREP "gcc " | $CUT -f3 -d " "` || GCC=
test -f `which  gettext` && GETTEXT=`gettext --version | $GREP gettext | $CUT -f4 -d " "` || GETTEXT=
test -f `which libtool` && LIBTOOL=`libtool --version | $GREP "libtool)" | $CUT -f4 -d " "` || LIBTOOL=
test -f `which make` && MAKE=`make --version | $GREP Make | $CUT -f3 -d " "` || MAKE=
test -f `which perl` && PERL=`perl --version | $GREP "perl, " | $CUT -f4 -d " " | $CUT -c 2-` || PERL=
test -f `which sed` && SED=`sed --version | $GREP "sed " | $CUT -f4 -d " "` || SED=
test -f `which tex` && TEX=`tex --version | $GREP "TeX " | head -n 1 | $CUT -f4 -d " " | $CUT -c -5` || TEX=

test -f `which bunzip2` && BUNZIP2=`bunzip2 --help 2> btmp; $GREP Version < btmp | $CUT -c 50-54; rm btmp` || BUNZIP2=
test -f `which gunzip` && GUNZIP=`gunzip --version | $GREP gunzip | $CUT -f2 -d " "` || GUNZIP=
test -f `which tar` && TAR=`tar --version | $GREP tar | $CUT -f4 -d " "` || TAR=

echo "Step 2(a):"
echo "Checking for main applications (in alphabetical order) ..."
echo

##################################################################################################
#                                            autoconf                                            #
##################################################################################################

AUTOCONFVERSION=0		# temp variable
LC=0				# temp variable (LC means 'L'ast 'C'haracter')
MINOR=				# temp variable

echo -n "autoconf >=2.57a     : "$AUTOCONF

if [ -z $AUTOCONF ]; then				# testing if string $AUTOCONF is empty
    echo -e "\t\t\t<---- \033[1;31mNo autoconf installed!\033[0m"
    DIE=1
    AUTOCONFVERSION=1
fi
if [ $AUTOCONFVERSION -eq 0 ]; then
    AUTOCONF=${AUTOCONF##?.}				# splitting the autoconfversion
    MINOR=$AUTOCONF

    if [ ${#AUTOCONF} -eq 3 ]; then			# if the the len of minorversion = 3
	LC=${AUTOCONF##??}				# memory the last character
	AUTOCONF=${AUTOCONF%%?}				# removing last character
    fi
    if [ ! $MINOR == "57a" ];then			# ok. never seen a version 2.57b :-/ .. dirty but it works!
        if  ([ ! LC == "a" ] && [ $AUTOCONF -eq 57 ]) || [ $AUTOCONF -lt 56 ]; then
	    echo -e " \t\t<---- \033[1;31mautoconf is too OLD!\033[0m"
	    DIE=1
	fi
    fi
fi
if [ $AUTOCONFVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                           automake                                             #
##################################################################################################

AUTOMAKEVERSION=0		# temp variable
#AUTOMAKE=1.8.1			# for testing the script

echo -n "automake >=1.4       : "$AUTOMAKE

AUTOMAKE=${AUTOMAKE##?.} ; AUTOMAKE=${AUTOMAKE%%.?}	# splitting the automakeversion

if [ -z $AUTOMAKE ]; then
    echo -e " \t\t<---- \033[1;31mNo automake installed!\033[0m"
    AUTOMAKEVERSION=1
    DIE=1

elif [ $AUTOMAKE -eq 3 ];then
    echo -e " \t\t<---- \033[1;31mautomake is too OLD!\033[0m"
    AUTOMAKEVERSION=1

elif [ $AUTOMAKE -eq 8 ]; then
    echo -e "\033[1;31m \t\t<---- Attention!"
    echo "        With this version of automake it may be happen that varius side effects are possibile!"
fi
if [ $AUTOMAKEVERSION -eq 0 ]; then
    echo -e
fi

###################################################################################################
#                                            gawk/awk                                             #
###################################################################################################

AWKVERSION=0			# temp variable
#AWK=3.0.9			# for testing the script

echo -n "awk      >=3.1.0     : "$AWK

MAJOR=${AWK%.*.*}		# the majornumber from gawk
AWK=${AWK##?.}
MINOR=${AWK%%.*}		# the minornumber from gawk
MICRO=${AWK##*.}		# the micronumber from gawk


if [ -z $AWK ]; then
    echo -e " \t\t<---- \033[1;31mNo gawk installed!\033[0m"
    AWKVERSION=1
    DIE=1
fi


if [ $AWKVERSION -eq 0 ]; then
    if [ $MAJOR -lt 3 ] || ([ $MAJOR -eq 3 ] && [ $MINOR -lt 1 ]); then			
        echo -e " \t\t<---- \033[1;31mawk is too OLD!\033[0m"
	AWKVERSION=1
        DIE=1
    fi
fi

if [ $AWKVERSION -eq 0 ]; then
    echo -e
fi

###################################################################################################
#                                               gcc                                               #
###################################################################################################

GCCVERSION=0			# temp variable
#GCC=3.0				# for testing the script

echo -n "gcc      >=3.1.0     : "$GCC

LEN=${#GCC}			# Are there other possibilitys like x.x.x ?? I dont now. Better check the len of this!
MAJOR=${GCC%%.*}   		# the majornumber from gcc
GCC=${GCC##?.}
MINOR=${GCC%%.*}    		# the minornumber from gcc
if [  $LEN -eq 5 ]; then
    MICRO=${GCC##*.}    	# the micronumber from gcc
fi

if [ -z $GCC ]; then
    echo -e " \t\t<---- \033[1;31mNo gcc installed!\033[0m"
    GCCVERSION=1
    DIE=1
fi

if [ $GCCVERSION -eq 0 ]; then
    if 	[ $MAJOR -lt 3 ] || ([ $MAJOR -eq 3 ] && [ $MINOR -lt 1 ]); then			
            echo -e " \t\t<---- \033[1;31mgcc is too OLD!\033[0m"	
	    GCCVERSION=1
            DIE=1
    fi
fi

if [ $GCCVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                            gettext                                             #
##################################################################################################

GETTEXTVERSION=0		# temp variable
#GETTEXT=0.12.0			# for testing the script

echo -n "gettext  >=0.12.1    : "$GETTEXT

MAJOR=${GETTEXT%.*.*}		# the majornumber from gettext
GETTEXT=${GETTEXT##?.}
MINOR=${GETTEXT%%.*}		# the minornumber from gettext
MICRO=${GETTEXT##*.}		# the micronumber from gettext

if [ -z $GETTEXT ]; then
    echo -e " \t\t<---- \033[1;31mNo gettext installed!\033[0m"
    GETTEXTVERSION=1
    DIE=1
fi

if [ $GETTEXTVERSION = 0 ]; then
    if ([ $MINOR -eq 12 ] && [ $MICRO -lt 1 ]); then
        echo -e " \t\t<---- \033[1;31mgettext is too OLD!\033[0m"
        GETTEXTVERSION=1
        DIE=1
    fi
fi

if [ $GETTEXTVERSION -eq 0 ]; then
    echo -e
fi

###################################################################################################
#                                            libtool                                              #
###################################################################################################

LIBTOOLVERSION=0		# temp variable
#LIBTOOL=			# for testing the script

echo -n "libtool  >=1.4.2     : "$LIBTOOL

MAJOR=${LIBTOOL%.*.*}		# the majornumber from libtool
LIBTOOL=${LIBTOOL##?.}
MINOR=${LIBTOOL%%.*}		# the minornumber from libtool
MICRO=${LIBTOOL##*.}		# the micronumber from libtool


if [ -z $LIBTOOL ]; then
    echo -e " \t\t<---- \033[1;31mNo libtool installed!\033[0m"
    LIBTOOLVERSION=1
    DIE=1
fi


if [ $LIBTOOLVERSION = 0 ]; then
    if ([ $MINOR -eq 4 ] && [ $MICRO -lt 2 ]); then
        echo -e " \t\t<---- \033[1;31mlibtool is too OLD!\033[0m"
	LIBTOOLVERSION=1
        DIE=1
    fi
fi

if [ $LIBTOOLVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                              make                                              #
##################################################################################################

MAKEVERSION=0			# temp variable
#MAKE=3.20			# for testing the script

echo -n "make     >=3.79      : "$MAKE
MAKE=${MAKE##?.} ; MAKE=${MAKE%%.?}					# splitting the makeversion

if [ -z $MAKE ]; then
    echo -e " \t\t<---- \033[1;31mNo make installed!\033[0m"
    MAKEVERSION=1
    DIE=1
elif [ $MAKE -eq 78 ] || [ $MAKE -lt 78 ]; then				# testing if makeversion 3.x is >= 78
    echo -e " \t\t<---- \033[1;31mmake is too OLD!\033[0m"
    MAKEVERSION=1
    DIE=1
fi
if [ $MAKEVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                              Perl                                              #
##################################################################################################

PERLVERSION=0			# temp variable
#PERL=3.9.0			# for testing this script

echo -n "Perl     >=5.0.0     : "$PERL

MAJOR=${PERL%.*.*}		# the majornumber from Perl
LIBTOOL=${PERL##?.}
MINOR=${PERL%%.*}		# the minornumber from Perl
MICRO=${PERL##*.}		# the micronumber from Perl


if [ -z $PERL ]; then
    echo -e " \t\t<---- \033[1;31mNo Perl installed!\033[0m"
    PERLVERSION=1
    DIE=1
fi


if [ $PERLVERSION -eq 0 ]; then
    if [ $MAJOR -lt 5 ]; then
        echo -e " \t\t<---- \033[1;31mPERL is too OLD!\033[0m"
	PERLVERSION=1
        DIE=1
    fi
fi

if [ $PERLVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                              sed                                               #
##################################################################################################

SEDVERSION=0			# temp variable
#SED=3.0.6			# for testing this script

echo -n "Sed      >=4.1.0     : "$SED

MAJOR=${SED%.*.*}		# the majornumber from Sed
SED=${SED##?.}
MINOR=${SED%%.*}		# the minornumber from Sed
MICRO=${SED##*.}		# the micronumber from Sed


if [ -z $SED ]; then
    echo -e " \t\t<---- \033[1;31mNo Sed installed!\033[0m"
    SEDVERSION=1
    DIE=1
fi


if [ $SEDVERSION -eq 0 ]; then
    if [ $MAJOR -lt 4 ] || ([ $MAJOR -eq 4 ] && [ $MINOR -lt 1 ]); then			
        echo -e " \t\t<---- \033[1;31mSed is too OLD!\033[0m"
	SEDVERSION=1
        DIE=1
    fi
fi

if [ $SEDVERSION -eq 0 ]; then
    echo -e
fi



##################################################################################################
#                                              tex                                               #
##################################################################################################
# NOT TESTET IN THE MOMENT!!! MAYBE WE NEED IT LATER? 

TEXVERSION=0			# temp variable
#TEX=7.0.9			# for testing this script

echo -n "tex      >=7.1.0     : "$TEX

MAJOR=${TEX%.*.*}		# the majornumber from tex
TEX=${TEX##?.}
MINOR=${TEX%%.*}		# the minornumber from tex
MICRO=${TEX##*.}		# the micronumber from tex


if [ -z $TEX ]; then
    echo -e " \t\t<---- \033[1;31mNo tex installed!\033[0m"
    TEXVERSION=1
    DIE=1
fi


if [ $TEXVERSION -eq 0 ]; then
    if [ $MAJOR -lt 7 ] || ([ $MAJOR -eq 7 ] && [ $MINOR -lt 1 ]); then			
        echo -e " \t\t<---- \033[1;31mtex is too OLD!\033[0m"
	TEXVERSION=1
        DIE=1
    fi
fi

if [ $TEXVERSION -eq 0 ]; then
    echo -e
fi


#                                    end of main applications                                    #
##################################################################################################


echo
echo "Step 2(b):"
echo "Check some needed applications for the rest of this script and for later things ..."
echo "(also in alphabetical order)"
echo

##################################################################################################
#                                            bunzip2                                             #
##################################################################################################

BUNZIP2VERSION=0		# temp variable
#BUNZIP2=			# for testing this script
echo -n "bunzip2              : "$BUNZIP2

if [ -z $BUNZIP2 ]; then
    echo -e " \t\t<---- \033[1;31mNo bunzip2 installed!\033[0m"
    BUNZIP2VERSION=1
    DIE=1
fi
if [ $BUNZIP2VERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                           gunzip                                               #
##################################################################################################

GUNZIPVERSION=0			# temp variable
#GUNZIP=			# for testing this script
echo -n "gunzip               : "$GUNZIP

if [ -z $GUNZIP ]; then
    echo -e " \t\t<---- \033[1;31mNo gunzip installed!\033[0m"
    GUNZIPVERSION=1
    DIE=1
fi
if [ $GUNZIPVERSION -eq 0 ]; then
    echo -e
fi

##################################################################################################
#                                              tar                                               #
##################################################################################################

TARVERSION=0			# temp variable
#TAR=				# for testing this script
echo -n "tar                  : "$TAR

if [ -z $TAR ]; then
    echo -e " \t\t<---- \033[1;31mNo tar installed!\033[0m"
    TARVERSION=1
    DIE=1
fi
if [ $TARVERSION -eq 0 ]; then
    echo -e
fi

#                              end of helping applications                                       #
##################################################################################################

if [ $DIE -eq 1 ]; then
    echo
    echo -e "\033[1;31mSome checks failed! See above thats going wrong!"
    echo -e "Install the missed applications an then restart the script again!\033[0m"
    echo
    exit
fi

}


##################################################################################################
#                                            MAIN FUNCTION                                       #

clear
echo "############################################"
echo "# CHECKSCRIPT FOR PROGS AND HELPERAPS      #"
echo "# version: $VER  from $DATE            #"
echo "############################################"
echo
echo "Whe doing now some Checks!"
echo
#----------------------------------- Skript running as root ? -----------------------------------#
echo -e "Step 1:"
echo -e "Checking some properties ...\n"
echo -n "Script runs with root rights ? .."
if [ "$UID" -eq 0 ]
then
    echo -e "\t\033[1;5;31m Yes!"
    echo -e "You can not run this script as root!\033[0m"
    exit 1
else
    echo -e "\t\t\033[1;32mo.k.\033[0m No, run as user '$USER'"
fi
#sleep 1
echo
check_hostapp
#sleep 1
if [ $DIE -eq 0 ]; then
    echo
    echo -e "\033[1;32mAll checks passed! Thats fine!\033[0m"
    echo -e "Now build your own Open\033[1;35mM\033[1;33mC\033[1;32mT\033[0m Image!"
    echo
    exit
fi

echo
