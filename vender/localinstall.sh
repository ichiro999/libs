#!/bin/bash

DEPLOY_ROOT_DIR=`pwd`
TEMP_DIR="tempdir"
INSTALL_DIR=`pwd`"/../local"

gputs()
{
    printf "\033[01;32m$1\033[00m\n"
}

cputs()
{
    printf "\033[01;36;40m$1\033[00m\n"
}

warn_puts()
{
    printf "\033[01;33;41m$1\033[00m\n"
}

run_cmd()
{
    printf "\033[01;32m$1\033[00m\n"
    printf "\033[01;30;40m"
    $1
    if [ $? != 0 ]
    then
        printf "\033[01;33;41mFailed: $1\033[00m\n"
        exit 1
    fi
    printf "\033[00m"
}

print_help()
{
    echo "Usage:"
    echo "  Must be ran under rock/vendor"
}

if [ -e $DEPLOY_ROOT_DIR/localinstall.sh ]
then
    if [ -e $TEMP_DIR ] && [ -e $INSTALL_DIR ]
    then
        cputs "Third-party softwares are already installed"
        exit 0
    fi
    gputs "Ready to install"
    run_cmd "mkdir $TEMP_DIR"
    run_cmd "mkdir $INSTALL_DIR"
    run_cmd "tar xfvj CUnit-2.1.tar.bz2 -C $TEMP_DIR"
    run_cmd "cd $TEMP_DIR/CUnit-2.1-2"
    run_cmd "./configure --prefix=$INSTALL_DIR --disable-shared"
    run_cmd "make"
    run_cmd "make install"
else
    print_help
fi
