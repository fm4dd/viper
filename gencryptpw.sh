#!/bin/bash

## generate a test password for cracking with viper

## test for help
test "$1" == "-h" || test "$1" == "--help" || test "$1" == "?" && {
    echo -e "\n  usage: ${0//*\//} <key> <salt> [user] [algorithm] (-crypt, -1, -apr1)\n"
    exit 0
}

## check required input
test -n "$1" && test -n "$2" || {
    echo -e "Error: insufficient input, usage: ${0//*\//} <key> <salt> [user] [algorithm] (-crypt, -1, -apr1)\n"
    exit 1
}

## test openssl availability
osslbin="`which openssl`"

test -x "$osslbin" || {
    echo -e "\n  Error: required openssl executable NOT found,\n\nusage: ${0//*\//} <key> <salt> [user] [algorithm] (-crypt, -1, -apr1)\n"
    exit 1
}

## set key
key="$1"

## set user
test -n "$3" && unm="$3" || unm="$USER"

## set crypt algorithm (-crypt (default), -1, -apr1), and validate
alg="${4:--crypt}"
algok=-1
for i in -crypt -1 -apr1; do
    test "$alg" == "$i" && algok=0 && break
done

if test "$algok" -ne 0; then
    echo -e "\n  Error: invalid algorithm supplied '$alg', must be one of (-crypt, -1, -apr1)\n"
    exit 1
fi

## set salt, if alg = -crypt and salt > 2 || key > 8, truncate and warn
if test "$alg" == "-crypt"; then
    test "${#2}" -gt 2 && echo -e "\n  Warning: '$alg' only allows 2 char salt, truncating to '${2:0:2}'"
    salt="${2:0:2}"
    test "${#key}" -gt 8 && echo -e "\n  Warning: '$alg' only allows 8 char key, truncating to '${key:0:8}'"
    key="${key:0:8}"
else
    salt="$2"
fi

## set output filename (don't overwrite)
i=1
outfn="testviper_${i}.txt"
while test -e "testviper_${i}.txt"; do
    ((i++))
    outfn="testviper_${i}.txt"
done

## generate password (default md5 hash)
pass="`$osslbin passwd $alg -salt $salt $key`"

## print output
echo -e "\ncmd : $osslbin passwd $alg -salt $salt $key\n"
echo -e "key : $key\nsalt: $salt\nuser: $unm\ncrypt pw: $pass\n"
echo -e "$unm:$pass:::::\n"

## write output to testviper_#.txt
echo -e "$unm:$pass:::::" > "testviper_${i}.txt"
echo -e "user:pass string written to: './testviper_${i}.txt'\n"

exit 0
