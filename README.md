# IEcoSortAndSearch: Timsort + калькуляторы (включение/агрегирование)

Компонент Timsort для Eco (ACOM), C89, без libc.

## Сборка
1. Установить окружение Eco и задать `ECO_FRAMEWORK`, `ECO_TOOLCHAIN` (`source ~/Dev/activate_eco.sh`).
2. Скопировать в `DependenciesFiles` компоненты из `ecoPackage.json`
   (`cp -r $ECO_FRAMEWORK/<имя> DependenciesFiles/`).
3. `AssemblyFiles/Linux/gcc_v132/build.sh` - собирает калькуляторы A-E, Eco.Lab1, Eco.Lab2 и UnitTest.
4. Запуск: `BuildFiles/Linux/x86_64/StaticRelease/IEcoSortAndSearch`

---
