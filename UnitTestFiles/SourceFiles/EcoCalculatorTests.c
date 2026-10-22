/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoCalculatorTests
 * </summary>
 *
 * <description>
 *   Задание 2: демонстрация включения и агрегирования компонентов калькулятора
 *   A, B, C, D, E в компонентах Eco.Lab1 (IEcoSortAndSearch) и Eco.Lab2.
 *   Перед каждым сценарием на шине регистрируется свой набор калькуляторов,
 *   поэтому выполняется каждая ветка выбора внутреннего компонента.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoMemoryManager1.h"
#include "IdEcoSortAndSearch.h"
#include "IdEcoLab2.h"
#include "ErrEcoLab2.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include <stdio.h>

/* Наборы калькуляторов на шине; C - новая версия A, регистрируется под тем же CID вместо A */
#define CALC_A 0x01
#define CALC_B 0x02
#define CALC_C 0x04
#define CALC_D 0x08
#define CALC_E 0x10

/* Способ, которым компонент отдает интерфейс */
#define MECH_NONE        0
#define MECH_OWN         1  /* собственная таблица компонента (включение) */
#define MECH_AGGREGATION 2  /* указатель внутреннего компонента (агрегирование) */

#define MAX_IFACES 5

static int g_calcChecks = 0;
static int g_calcErrors = 0;

#ifdef ECO_LIB

static void expect(int cond, const char* what) {
    g_calcChecks++;
    if (!cond) {
        g_calcErrors++;
        printf("    FAIL: %s\n", what);
    }
}

/* Менеджер памяти: число занятых блоков до создания и после освобождения компонента должно совпадать */
static IEcoMemoryManager1* g_pIMemMgr = 0;

static uint32_t usedBlocks(void) {
    ECOMEMORYMANAGER1STATUS status;

    if (g_pIMemMgr == 0 || !g_pIMemMgr->pVTbl->get_Status(g_pIMemMgr, &status)) {
        return 0;
    }
    return status.usedBlocks;
}

static void checkNoLeaks(uint32_t blocksBefore) {
    uint32_t blocksAfter = usedBlocks();

    printf("    Блоков памяти до создания / после Release: %u / %u\n", (unsigned)blocksBefore, (unsigned)blocksAfter);
    expect(blocksAfter == blocksBefore, "вся память компонента и внутренних компонентов освобождена");
}

/*
 * Установка/удаление калькуляторов между сценариями.
 *
 * UnRegisterComponent шины Eco.InterfaceBus1 повреждает список компонентов: после
 * удаления одного компонента следующие вызовы UnRegisterComponent падают (проверено
 * отдельно, порядок удаления не помогает). Поэтому каждый CID регистрируется один раз
 * с подменной фабрикой, а сценарий только переключает реальную фабрику, к которой она
 * обращается. "Не установленный" компонент возвращает ERR_ECO_COMPONENT_NOTFOUND -
 * для запрашивающего компонента это то же, что отсутствие компонента в системе.
 */
typedef struct CalcSlot {
    IEcoComponentFactoryVTbl* pVTbl;
    IEcoComponentFactory* pTarget;
} CalcSlot;

static int16_t ECOCALLMETHOD CalcSlot_QueryInterface(/* in */ IEcoComponentFactory* me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    if ( IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoComponentFactory) ) {
        *ppv = me;
        return ERR_ECO_SUCCESS;
    }
    *ppv = 0;
    return ERR_ECO_NOINTERFACE;
}

/* Подменные фабрики - статические объекты, счетчик ссылок не нужен */
static uint32_t ECOCALLMETHOD CalcSlot_AddRef(/* in */ IEcoComponentFactory* me) {
    return 1;
}

static uint32_t ECOCALLMETHOD CalcSlot_Release(/* in */ IEcoComponentFactory* me) {
    return 1;
}

static int16_t ECOCALLMETHOD CalcSlot_Alloc(/* in */ IEcoComponentFactory* me, /* in */ IEcoUnknown* pISystem, /* in */ IEcoUnknown* pIUnknownOuter, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    IEcoComponentFactory* pTarget = ((CalcSlot*)me)->pTarget;

    if (pTarget == 0) {
        return ERR_ECO_COMPONENT_NOTFOUND;
    }
    return pTarget->pVTbl->Alloc(pTarget, pISystem, pIUnknownOuter, riid, ppv);
}

static int16_t ECOCALLMETHOD CalcSlot_Init(/* in */ IEcoComponentFactory* me, /* in */ IEcoUnknown* pISystem, /* in */ voidptr_t pv) {
    IEcoComponentFactory* pTarget = ((CalcSlot*)me)->pTarget;

    if (pTarget == 0) {
        return ERR_ECO_COMPONENT_NOTFOUND;
    }
    return pTarget->pVTbl->Init(pTarget, pISystem, pv);
}

static char_t* ECOCALLMETHOD CalcSlot_get_Name(/* in */ IEcoComponentFactory* me) {
    IEcoComponentFactory* pTarget = ((CalcSlot*)me)->pTarget;
    return (pTarget == 0) ? (char_t*)"-" : pTarget->pVTbl->get_Name(pTarget);
}

static char_t* ECOCALLMETHOD CalcSlot_get_Version(/* in */ IEcoComponentFactory* me) {
    IEcoComponentFactory* pTarget = ((CalcSlot*)me)->pTarget;
    return (pTarget == 0) ? (char_t*)"-" : pTarget->pVTbl->get_Version(pTarget);
}

static char_t* ECOCALLMETHOD CalcSlot_get_Manufacturer(/* in */ IEcoComponentFactory* me) {
    IEcoComponentFactory* pTarget = ((CalcSlot*)me)->pTarget;
    return (pTarget == 0) ? (char_t*)"-" : pTarget->pVTbl->get_Manufacturer(pTarget);
}

static IEcoComponentFactoryVTbl g_xCalcSlotVTbl = {
    CalcSlot_QueryInterface,
    CalcSlot_AddRef,
    CalcSlot_Release,
    CalcSlot_Alloc,
    CalcSlot_Init,
    CalcSlot_get_Name,
    CalcSlot_get_Version,
    CalcSlot_get_Manufacturer
};

/* Слоты по CID: A (в нем A или C - у них один CID), B, D, E */
static CalcSlot g_slotA = {&g_xCalcSlotVTbl, 0};
static CalcSlot g_slotB = {&g_xCalcSlotVTbl, 0};
static CalcSlot g_slotD = {&g_xCalcSlotVTbl, 0};
static CalcSlot g_slotE = {&g_xCalcSlotVTbl, 0};

static int16_t registerCalculatorSlots(IEcoInterfaceBus1* pIBus) {
    int16_t result = 0;

    result |= pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)&g_slotA);
    result |= pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)&g_slotB);
    result |= pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)&g_slotD);
    result |= pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)&g_slotE);
    return result;
}

/* Установка выбранного набора калькуляторов (CALC_C ставится вместо A под CID A) */
static void setCalculators(int mask) {
    g_slotA.pTarget = 0;
    if (mask & CALC_A) {
        g_slotA.pTarget = GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E;
    }
    else if (mask & CALC_C) {
        g_slotA.pTarget = GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E_C;
    }
    g_slotB.pTarget = (mask & CALC_B) ? GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565 : 0;
    g_slotD.pTarget = (mask & CALC_D) ? GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61 : 0;
    g_slotE.pTarget = (mask & CALC_E) ? GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0 : 0;
}

static void printCalculators(void) {
    IEcoComponentFactory* pA = g_slotA.pTarget;
    printf("    Установлены: CID A -> %s %s; B -> %s; D -> %s; E -> %s\n",
           pA ? pA->pVTbl->get_Name(pA) : "-", pA ? pA->pVTbl->get_Version(pA) : "",
           g_slotB.pTarget ? "Eco.CalculatorB" : "-",
           g_slotD.pTarget ? "Eco.CalculatorD" : "-",
           g_slotE.pTarget ? "Eco.CalculatorE" : "-");
}

static const char* mechName(int mech) {
    if (mech == MECH_OWN)         return "включение (таблица внешнего компонента)";
    if (mech == MECH_AGGREGATION) return "агрегирование (указатель внутреннего компонента)";
    return "нет интерфейса";
}

/*
 * Определение способа по адресу: собственные таблицы внешнего компонента лежат в начале
 * его экземпляра, в полях 0..slots-1 (см. HeaderFiles). Указатель, полученный
 * агрегированием, принадлежит экземпляру внутреннего компонента.
 */
static int mechOf(void* pOuterFirst, void* pIface, int slots) {
    int i;

    if (pIface == 0) {
        return MECH_NONE;
    }
    for (i = 0; i < slots; i++) {
        if ((char*)pIface == (char*)pOuterFirst + i * sizeof(voidptr_t)) {
            return MECH_OWN;
        }
    }
    return MECH_AGGREGATION;
}

/*
 * Свойства QueryInterface для набора интерфейсов компонента:
 *  - через любой указатель запрашивается любой другой интерфейс (таблица ниже);
 *  - повторный запрос возвращает тот же указатель (симметричность, транзитивность);
 *  - IEcoUnknown через любой указатель один и тот же (идентичность компонента).
 */
static void checkQueryInterface(IEcoUnknown* pIUnk, const UGUID** iids, const char** names, int count) {
    IEcoUnknown* ptrs[MAX_IFACES];
    IEcoUnknown* pIdentity = 0;
    IEcoUnknown* pQ = 0;
    int16_t r;
    int i, j, k;
    char msg[256];

    for (j = 0; j < count; j++) {
        ptrs[j] = 0;
        r = pIUnk->pVTbl->QueryInterface(pIUnk, iids[j], (void**)&ptrs[j]);
        if (r != 0) {
            ptrs[j] = 0;
        }
    }
    pIUnk->pVTbl->QueryInterface(pIUnk, &IID_IEcoUnknown, (void**)&pIdentity);

    printf("    QueryInterface: строка - имеющийся указатель, столбец - запрошенный интерфейс\n");
    printf("    %-20s", "");
    for (j = 0; j < count; j++) {
        printf(" %-19s", names[j]);
    }
    printf("\n");
    for (i = 0; i < count; i++) {
        if (ptrs[i] == 0) {
            continue;
        }
        printf("    %-20s", names[i]);
        for (j = 0; j < count; j++) {
            pQ = 0;
            r = ptrs[i]->pVTbl->QueryInterface(ptrs[i], iids[j], (void**)&pQ);
            printf(" %-19s", r == 0 ? "+" : "-");
            k = (r == 0) == (ptrs[j] != 0);
            sprintf(msg, "QueryInterface %s -> %s: результат не совпадает с запросом через IEcoUnknown", names[i], names[j]);
            expect(k, msg);
            if (r == 0) {
                sprintf(msg, "QueryInterface %s -> %s: другой указатель", names[i], names[j]);
                expect(pQ == ptrs[j], msg);
                pQ->pVTbl->Release(pQ);
            }
        }
        printf("\n");

        pQ = 0;
        r = ptrs[i]->pVTbl->QueryInterface(ptrs[i], &IID_IEcoUnknown, (void**)&pQ);
        sprintf(msg, "IEcoUnknown через %s не совпадает с IEcoUnknown компонента", names[i]);
        expect(r == 0 && pQ == pIdentity, msg);
        if (r == 0) {
            pQ->pVTbl->Release(pQ);
        }
    }
    printf("    IEcoUnknown через любой указатель: %p (идентичность компонента)\n", (void*)pIdentity);

    for (j = 0; j < count; j++) {
        if (ptrs[j] != 0) {
            ptrs[j]->pVTbl->Release(ptrs[j]);
        }
    }
    if (pIdentity != 0) {
        pIdentity->pVTbl->Release(pIdentity);
    }
}

static int16_t ECOCALLMETHOD cmpInt16(voidptr_t a, voidptr_t b) {
    int16_t x = *(int16_t*)a;
    int16_t y = *(int16_t*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Проверка, что сортировка доступна через полученный указатель */
static void checkSort(IEcoSortAndSearch* pISort) {
    int16_t arr[6];
    int16_t r;

    arr[0] = 5; arr[1] = -3; arr[2] = 9; arr[3] = 0; arr[4] = 5; arr[5] = -7;
    r = pISort->pVTbl->Timsort(pISort, arr, 6, sizeof(int16_t), cmpInt16);
    printf("    Timsort({5, -3, 9, 0, 5, -7}) = {%d, %d, %d, %d, %d, %d}\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
    expect(r == 0 && arr[0] == -7 && arr[1] == -3 && arr[2] == 0 && arr[3] == 5 && arr[4] == 5 && arr[5] == 9, "Timsort");
}

static void checkX(IEcoCalculatorX* pIX) {
    int32_t c;
    int16_t d;

    c = pIX->pVTbl->Addition(pIX, 3, 5);
    printf("    Addition(3, 5) = %d\n", (int)c);
    expect(c == 8, "Addition(3, 5)");
    d = pIX->pVTbl->Subtraction(pIX, 3, 5);
    printf("    Subtraction(3, 5) = %d\n", (int)d);
    expect(d == -2, "Subtraction(3, 5)");
    c = pIX->pVTbl->Addition(pIX, 32767, 32767);
    printf("    Addition(32767, 32767) = %d\n", (int)c);
    expect(c == 65534, "Addition(32767, 32767)");
}

static void checkY(IEcoCalculatorY* pIY) {
    int32_t c;
    int16_t d;

    c = pIY->pVTbl->Multiplication(pIY, 3, 5);
    printf("    Multiplication(3, 5) = %d\n", (int)c);
    expect(c == 15, "Multiplication(3, 5)");
    c = pIY->pVTbl->Multiplication(pIY, -300, 300);
    printf("    Multiplication(-300, 300) = %d\n", (int)c);
    expect(c == -90000, "Multiplication(-300, 300)");
    d = pIY->pVTbl->Division(pIY, 15, 4);
    printf("    Division(15, 4) = %d\n", (int)d);
    expect(d == 3, "Division(15, 4)");
    d = pIY->pVTbl->Division(pIY, 1, 0);
    printf("    Division(1, 0) = %d (деление на ноль)\n", (int)d);
    expect(d == -1, "Division(1, 0)");
}

/*
 * Сценарий для Eco.Lab1: какие калькуляторы зарегистрированы и каким способом
 * Eco.Lab1 должен отдать IEcoCalculatorX и IEcoCalculatorY.
 */
static void scenarioLab1(IEcoInterfaceBus1* pIBus, const char* title, int mask, int expectMechX, int expectMechY) {
    static const UGUID* iids[4] = {&IID_IEcoUnknown, &IID_IEcoSortAndSearch, &IID_IEcoCalculatorX, &IID_IEcoCalculatorY};
    static const char* names[4] = {"IEcoUnknown", "IEcoSortAndSearch", "IEcoCalculatorX", "IEcoCalculatorY"};
    /* Указатель на тестируемый интерфейс */
    IEcoSortAndSearch* pIEcoLab1 = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    IEcoSortAndSearch* pISortFromX = 0;
    int16_t result;
    uint32_t cRef;
    uint32_t blocks;
    int mech;

    printf("\n=== %s\n", title);
    setCalculators(mask);
    printCalculators();
    blocks = usedBlocks();

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoSortAndSearch, 0, &IID_IEcoSortAndSearch, (void**) &pIEcoLab1);
    expect(result == 0 && pIEcoLab1 != 0, "создание Eco.Lab1");
    if (result != 0 || pIEcoLab1 == 0) {
        return;
    }

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**) &pIX);
    if (result != 0) {
        pIX = 0;
    }
    mech = mechOf(pIEcoLab1, pIX, 4);
    printf("    IEcoCalculatorX: %s\n", mechName(mech));
    expect(mech == expectMechX, "способ получения IEcoCalculatorX");
    if (pIX != 0) {
        checkX(pIX);
        /* Обратный запрос через интерфейс внутреннего компонента */
        result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoSortAndSearch, (void**) &pISortFromX);
        expect(result == 0 && pISortFromX == pIEcoLab1, "IEcoSortAndSearch через IEcoCalculatorX");
        if (result == 0) {
            checkSort(pISortFromX);
            pISortFromX->pVTbl->Release(pISortFromX);
        }
    }

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**) &pIY);
    if (result != 0) {
        pIY = 0;
    }
    mech = mechOf(pIEcoLab1, pIY, 4);
    printf("    IEcoCalculatorY: %s\n", mechName(mech));
    expect(mech == expectMechY, "способ получения IEcoCalculatorY");
    if (pIY != 0) {
        checkY(pIY);
    }

    checkQueryInterface((IEcoUnknown*)pIEcoLab1, iids, names, 4);

    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }
    cRef = pIEcoLab1->pVTbl->Release(pIEcoLab1);
    expect(cRef == 0, "счетчик ссылок Eco.Lab1 после последнего Release");
    checkNoLeaks(blocks);
}

/* Внутренний компонент сам по себе: какие интерфейсы он отдает */
static void scenarioComponent(IEcoInterfaceBus1* pIBus, const char* title, const UGUID* cid, int hasX, int hasY) {
    static const UGUID* iids[3] = {&IID_IEcoUnknown, &IID_IEcoCalculatorX, &IID_IEcoCalculatorY};
    static const char* names[3] = {"IEcoUnknown", "IEcoCalculatorX", "IEcoCalculatorY"};
    IEcoUnknown* pIUnk = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int16_t result;
    uint32_t blocks = usedBlocks();

    printf("\n=== %s\n", title);
    result = pIBus->pVTbl->QueryComponent(pIBus, cid, 0, &IID_IEcoUnknown, (void**) &pIUnk);
    expect(result == 0 && pIUnk != 0, "создание компонента");
    if (result != 0 || pIUnk == 0) {
        return;
    }
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &IID_IEcoCalculatorX, (void**) &pIX);
    expect((result == 0) == hasX, "наличие IEcoCalculatorX");
    if (result == 0) {
        checkX(pIX);
        pIX->pVTbl->Release(pIX);
    }
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &IID_IEcoCalculatorY, (void**) &pIY);
    expect((result == 0) == hasY, "наличие IEcoCalculatorY");
    if (result == 0) {
        checkY(pIY);
        pIY->pVTbl->Release(pIY);
    }
    checkQueryInterface(pIUnk, iids, names, 3);
    expect(pIUnk->pVTbl->Release(pIUnk) == 0, "счетчик ссылок после последнего Release");
    checkNoLeaks(blocks);
}

/* Рис. 3: Eco.Lab2 агрегирует Eco.Lab1 и включает E (внутри E агрегирован B) */
static void scenarioLab2(IEcoInterfaceBus1* pIBus, const char* title, int mask, int hasCalc) {
    static const UGUID* iids[5] = {&IID_IEcoUnknown, &IID_IEcoLab2, &IID_IEcoSortAndSearch, &IID_IEcoCalculatorX, &IID_IEcoCalculatorY};
    static const char* names[5] = {"IEcoUnknown", "IEcoLab2", "IEcoSortAndSearch", "IEcoCalculatorX", "IEcoCalculatorY"};
    IEcoLab2* pIEcoLab2 = 0;
    IEcoLab2* pILab2FromSort = 0;
    IEcoSortAndSearch* pISort = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int32_t value = 0;
    int16_t result;
    uint32_t cRef;
    uint32_t blocks;
    int mech;

    printf("\n=== %s\n", title);
    setCalculators(mask);
    printCalculators();
    blocks = usedBlocks();

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab2, 0, &IID_IEcoLab2, (void**) &pIEcoLab2);
    expect(result == 0 && pIEcoLab2 != 0, "создание Eco.Lab2");
    if (result != 0 || pIEcoLab2 == 0) {
        return;
    }

    /* Собственный интерфейс IEcoLab2 */
    result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '+', 100, 23, &value);
    if (result == 0) {
        printf("    Calculate('+', 100, 23) = %d\n", (int)value);
    } else {
        printf("    Calculate('+', 100, 23): код %d (ERR_ECO_NOINTERFACE = %d)\n", (int)result, (int)ERR_ECO_NOINTERFACE);
    }
    expect(hasCalc ? (result == 0 && value == 123) : result == ERR_ECO_NOINTERFACE, "Calculate +");
    if (hasCalc) {
        result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '-', 100, 23, &value);
        printf("    Calculate('-', 100, 23) = %d\n", (int)value);
        expect(result == 0 && value == 77, "Calculate -");
        result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '*', 100, 23, &value);
        printf("    Calculate('*', 100, 23) = %d\n", (int)value);
        expect(result == 0 && value == 2300, "Calculate *");
        result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '/', 100, 23, &value);
        printf("    Calculate('/', 100, 23) = %d\n", (int)value);
        expect(result == 0 && value == 4, "Calculate /");
        result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '/', 100, 0, &value);
        printf("    Calculate('/', 100, 0): код %d (ERR_ECOLAB2_DIVISION_BY_ZERO)\n", (int)result);
        expect(result == ERR_ECOLAB2_DIVISION_BY_ZERO, "Calculate / 0");
    }
    result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '%', 100, 23, &value);
    printf("    Calculate('%%', 100, 23): код %d (ERR_ECOLAB2_UNKNOWN_OPERATION)\n", (int)result);
    expect(result == ERR_ECOLAB2_UNKNOWN_OPERATION, "Calculate %");
    result = pIEcoLab2->pVTbl->Calculate(pIEcoLab2, '+', 1, 2, 0);
    expect(result == ERR_ECO_POINTER, "Calculate с pResult == 0");

    /* IEcoSortAndSearch - агрегирование Eco.Lab1 */
    result = pIEcoLab2->pVTbl->QueryInterface(pIEcoLab2, &IID_IEcoSortAndSearch, (void**) &pISort);
    if (result != 0) {
        pISort = 0;
    }
    mech = mechOf(pIEcoLab2, pISort, 3);
    printf("    IEcoSortAndSearch: %s\n", mechName(mech));
    expect(mech == MECH_AGGREGATION, "способ получения IEcoSortAndSearch");
    if (pISort != 0) {
        checkSort(pISort);
        /* Eco.Lab1 не реализует IEcoLab2: запрос успешен только благодаря делегированию внешнему компоненту */
        result = pISort->pVTbl->QueryInterface(pISort, &IID_IEcoLab2, (void**) &pILab2FromSort);
        printf("    IEcoSortAndSearch -> IEcoLab2: %s\n", result == 0 ? "получен (делегирование внешнему Eco.Lab2)" : "нет");
        expect(result == 0 && pILab2FromSort == pIEcoLab2, "IEcoLab2 через агрегированный IEcoSortAndSearch");
        if (result == 0) {
            pILab2FromSort->pVTbl->Release(pILab2FromSort);
        }
    }

    /* IEcoCalculatorX / IEcoCalculatorY - включение E */
    result = pIEcoLab2->pVTbl->QueryInterface(pIEcoLab2, &IID_IEcoCalculatorX, (void**) &pIX);
    if (result != 0) {
        pIX = 0;
    }
    mech = mechOf(pIEcoLab2, pIX, 3);
    printf("    IEcoCalculatorX: %s\n", mechName(mech));
    expect(mech == (hasCalc ? MECH_OWN : MECH_NONE), "способ получения IEcoCalculatorX");
    if (pIX != 0) {
        checkX(pIX);
    }
    result = pIEcoLab2->pVTbl->QueryInterface(pIEcoLab2, &IID_IEcoCalculatorY, (void**) &pIY);
    if (result != 0) {
        pIY = 0;
    }
    mech = mechOf(pIEcoLab2, pIY, 3);
    printf("    IEcoCalculatorY: %s\n", mechName(mech));
    expect(mech == (hasCalc ? MECH_OWN : MECH_NONE), "способ получения IEcoCalculatorY");
    if (pIY != 0) {
        checkY(pIY);
    }

    checkQueryInterface((IEcoUnknown*)pIEcoLab2, iids, names, 5);

    if (pISort != 0) {
        pISort->pVTbl->Release(pISort);
    }
    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }
    cRef = pIEcoLab2->pVTbl->Release(pIEcoLab2);
    expect(cRef == 0, "счетчик ссылок Eco.Lab2 после последнего Release");
    checkNoLeaks(blocks);
}

/*
 * Недопустимое агрегирование должно отклоняться. Код ERR_ECO_NOAGGREGATION проверяется
 * у фабрики напрямую: шина любой отказ фабрики возвращает как ERR_ECO_COMPONENT_NOTFOUND
 */
static void checkAggregationRefused(IEcoInterfaceBus1* pIBus, const char* name, const UGUID* cid, IEcoComponentFactory* pIFactory,
                                    IEcoUnknown* pISystem, const UGUID* riid, const char* riidName) {
    IEcoUnknown* pIUnk = 0;
    int16_t result;
    char msg[256];

    result = pIFactory->pVTbl->Alloc(pIFactory, pISystem, pISystem, riid, (void**) &pIUnk);
    printf("    %s, внешний компонент + %s: фабрика вернула %d (ERR_ECO_NOAGGREGATION = %d)\n", name, riidName, (int)result, (int)ERR_ECO_NOAGGREGATION);
    sprintf(msg, "%s: фабрика должна отказать в агрегировании с %s", name, riidName);
    expect(result == ERR_ECO_NOAGGREGATION && pIUnk == 0, msg);

    pIUnk = 0;
    result = pIBus->pVTbl->QueryComponent(pIBus, cid, pISystem, riid, (void**) &pIUnk);
    printf("    %s, тот же запрос через шину: код %d\n", name, (int)result);
    sprintf(msg, "%s: шина должна отказать в агрегировании с %s", name, riidName);
    expect(result != 0 && pIUnk == 0, msg);
}

static void scenarioAggregationRules(IEcoInterfaceBus1* pIBus, IEcoUnknown* pISystem) {
    printf("\n=== Правила агрегирования\n");
    /* Eco.Lab1 агрегируется только через IID_IEcoUnknown */
    checkAggregationRefused(pIBus, "Eco.Lab1", &CID_EcoSortAndSearch, GetIEcoComponentFactoryPtr_591FC2D6783F43F0B5B565CCCAF05939,
                            pISystem, &IID_IEcoSortAndSearch, "IID_IEcoSortAndSearch");
    /* Eco.Lab2 - внешний компонент, агрегирование не поддерживает */
    checkAggregationRefused(pIBus, "Eco.Lab2", &CID_EcoLab2, GetIEcoComponentFactoryPtr_3FDF228D0701437FAF2A696A86AB2353,
                            pISystem, &IID_IEcoUnknown, "IID_IEcoUnknown");
}

#endif /* ECO_LIB */

/*
 * Запуск демонстрации. показывает количество ошибок, количество проверок - в pChecks.
 */
int RunCalculatorTests(IEcoInterfaceBus1* pIBus, IEcoUnknown* pISystem, int* pChecks) {
    g_calcChecks = 0;
    g_calcErrors = 0;

    printf("\nЛаба 2\n");

#ifdef ECO_LIB
    expect(pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab2, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3FDF228D0701437FAF2A696A86AB2353) == 0, "регистрация Eco.Lab2");
    expect(registerCalculatorSlots(pIBus) == 0, "регистрация калькуляторов A-E");
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryManager1, (void**) &g_pIMemMgr);
    expect(g_pIMemMgr != 0, "менеджер памяти для проверки утечек");

    /* Внутренние компоненты по отдельности */
    setCalculators(CALC_A | CALC_B | CALC_D | CALC_E);
    scenarioComponent(pIBus, "Компонент A: только IEcoCalculatorX", &CID_EcoCalculatorA, 1, 0);
    scenarioComponent(pIBus, "Компонент B: IEcoCalculatorX, поддерживает агрегирование", &CID_EcoCalculatorB, 1, 0);
    scenarioComponent(pIBus, "Компонент D: IEcoCalculatorY + IEcoCalculatorX включением B", &CID_EcoCalculatorD, 1, 1);
    scenarioComponent(pIBus, "Компонент E: IEcoCalculatorY + IEcoCalculatorX агрегированием B", &CID_EcoCalculatorE, 1, 1);
    setCalculators(CALC_C);
    scenarioComponent(pIBus, "Компонент C (версия 2 компонента A, тот же CID): IEcoCalculatorX + IEcoCalculatorY", &CID_EcoCalculatorC, 1, 1);

    /* Eco.Lab1 */
    scenarioLab1(pIBus, "Eco.Lab1: X - агрегирование B, Y - включение D (внутри D включен B)",
                 CALC_A | CALC_B | CALC_D | CALC_E, MECH_AGGREGATION, MECH_OWN);
    scenarioLab1(pIBus, "Eco.Lab1: B нет - X включением A, Y - включение D (внутри D включен A)",
                 CALC_A | CALC_D | CALC_E, MECH_OWN, MECH_OWN);
    scenarioLab1(pIBus, "Eco.Lab1: D нет - Y включением E (внутри E агрегирован B)",
                 CALC_A | CALC_B | CALC_E, MECH_AGGREGATION, MECH_OWN);
    scenarioLab1(pIBus, "Eco.Lab1: вместо A установлен C (тот же CID) - X и Y включением C",
                 CALC_C, MECH_OWN, MECH_OWN);
    scenarioLab1(pIBus, "Eco.Lab1: только A - IEcoCalculatorY недоступен",
                 CALC_A, MECH_OWN, MECH_NONE);
    scenarioLab1(pIBus, "Eco.Lab1: калькуляторов нет - остается только сортировка",
                 0, MECH_NONE, MECH_NONE);

    /* Eco.Lab2 */
    scenarioLab2(pIBus, "Eco.Lab2: IEcoLab2 + агрегирование Eco.Lab1 + включение E (внутри E агрегирован B)",
                 CALC_A | CALC_B | CALC_D | CALC_E, 1);
    scenarioLab2(pIBus, "Eco.Lab2: E нет - калькулятор недоступен, сортировка через Eco.Lab1 остается",
                 CALC_A | CALC_B | CALC_D, 0);

    scenarioAggregationRules(pIBus, pISystem);

    setCalculators(CALC_A | CALC_B | CALC_D | CALC_E);
    if (g_pIMemMgr != 0) {
        g_pIMemMgr->pVTbl->Release(g_pIMemMgr);
        g_pIMemMgr = 0;
    }
#else
    printf("Сценарии регистрируют компоненты на шине и требуют статической сборки (ECO_LIB)\n");
#endif

    printf("\nЛаба 2: проверок %d, ошибок %d\n", g_calcChecks, g_calcErrors);
    if (pChecks != 0) {
        *pChecks = g_calcChecks;
    }
    return g_calcErrors;
}
