#!/bin/sh
# Сборка в порядке зависимостей:
#   калькуляторы A-E -> Eco.Lab1 (IEcoSortAndSearch) -> Eco.Lab2 -> UnitTest
# Параметры make (TARGET, DEBUG, ARCH) передаются всем шагам: ./build.sh DEBUG=1
# Очистка: ./build.sh clean
# В окружении activate_eco.sh make - это alias, скрипту он не виден, поэтому
# путь к make можно передать через переменную: MAKE="..." ./build.sh
set -e
cd "$(dirname "$0")"

if [ -z "$MAKE" ]; then
    if [ -n "$ECO_TOOLCHAIN" ]; then
        GCC_DIR="$ECO_TOOLCHAIN/eco-x86_64-linux-gnu-gcc-13.2.0"
        MAKE="$GCC_DIR/lib/ld-linux-x86-64.so.2 --library-path $GCC_DIR/lib:$GCC_DIR/lib64 $ECO_TOOLCHAIN/eco-gnu-make-4.4.1/bin/make"
    else
        MAKE=make
    fi
fi
DEPS=../../../DependenciesFiles

for c in A B C D E; do
    $MAKE -C "$DEPS/Eco.Calculator$c/AssemblyFiles/Linux/gcc_v132" "$@"
done
$MAKE -f Makefile "$@"
$MAKE -f MakefileLab2 "$@"
# MakefileExe не отслеживает изменения библиотек - тест всегда пересобирается
$MAKE -f MakefileExe "$@" clean
$MAKE -f MakefileExe "$@"
