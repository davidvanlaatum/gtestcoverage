#!/usr/bin/env bash

shift

LOGFILE=${@: -1}
LOGFILE=$(basename $LOGFILE).iwyu.log
while [ $# -gt 0 ]; do
        case "$1" in
            -o) LOGFILE=$2; shift; shift ;;
            --) shift ; break ;;
            *) ARGS+=" $1"; shift ;;
        esac
done

@IWYU_PATH@ -Wno-gnu-include-next $ARGS -Xiwyu --max_line_length=160 -Xiwyu --mapping_file=@CMAKE_SOURCE_DIR@/iwyu.imp 2> $LOGFILE
EXIT=$?
if [ "$EXIT" -gt 2 ]
then
    cat $LOGFILE
fi

if [ "$EXIT" -eq 1 ] || grep -q "fatal error" $LOGFILE
then
    rm -f $LOGFILE
    exit 1
fi
