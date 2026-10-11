# IEcoSortAndSearch: Timsort

Компонент Timsort для Eco (ACOM), C89, без libc.

## Сборка
1. Установить окружение Eco и задать `ECO_FRAMEWORK`.
2. Скопировать в `DependenciesFiles` компоненты из `ecoPackage.json`
   (`cp -r $ECO_FRAMEWORK/<имя> DependenciesFiles/`).
3. `cd AssemblyFiles/Linux/gcc_v132 && make && make -f MakefileExe`
4. Запуск: `../../../BuildFiles/Linux/x86_64/StaticRelease/IEcoSortAndSearch`
