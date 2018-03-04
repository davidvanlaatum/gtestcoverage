#!/usr/bin/env bash
shift

LOGFILE=${@: -1}
LOGFILE=$(basename $LOGFILE).clang_tidy.log
while [ $# -gt 0 ]; do
        case "$1" in
            -o) LOGFILE=$2; shift; shift ;;
            -c) INPUT=$2; shift; shift;;
            *) shift ;;
        esac
done

@CLANG_TIDY_PATH@ -p "@CMAKE_BINARY_DIR@" $INPUT &> $LOGFILE.tmp
EXIT=$?
if [ "$EXIT" -gt 0 ]
then
    echo $INPUT
    cat $LOGFILE.tmp
    rm $LOGFILE.tmp
else
    mv $LOGFILE{.tmp,}
fi
exit $EXIT
