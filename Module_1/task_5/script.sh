#!/bin/bash

STR="$1"

if [ -z "$STR" ]; then
    echo "Ошибка! не задан параметр командной строки."
else
    pkill $STR
fi
