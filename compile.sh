CODE="main.c"
OUTPUT="wm-v4"
OUTPUT_DIR="build"

clear
arm-linux-gnueabi-gcc -static -march=armv6 -o $OUTPUT_DIR/$OUTPUT $CODE
echo [COMPILE.SH] "$OUTPUT" compiled for ARMv6 architecture if no compiler errors shown.
