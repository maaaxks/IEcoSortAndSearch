/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoSortAndSearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Задание 2: включение/агрегирование компонентов калькулятора (EcoCalculatorTests.c) */
extern int RunCalculatorTests(IEcoInterfaceBus1* pIBus, IEcoUnknown* pISystem, int* pChecks);

typedef struct TestItem {
    int key;
    int id;
} TestItem;

static uint32_t g_seed = 12345;

static uint32_t nextRand(void) {
    uint32_t hi, lo;
    g_seed = g_seed * 1103515245u + 12345u;
    hi = (g_seed >> 16) & 0x7FFF;
    g_seed = g_seed * 1103515245u + 12345u;
    lo = (g_seed >> 16) & 0x7FFF;
    return (hi << 15) | lo;
}

static int g_checks = 0;
static int g_errors = 0;
static int16_t g_firstFail = 0;

static void check(int cond, int16_t code, const char* msg, int p1, int p2) {
    g_checks++;
    if (!cond) {
        g_errors++;
        if (g_firstFail == 0) {
            g_firstFail = code;
        }
        fprintf(stderr, "FAIL [%d]: %s (%d, %d)\n", (int)code, msg, p1, p2);
    }
}

static int16_t ECOCALLMETHOD cmpInt(voidptr_t a, voidptr_t b) {
    int x = *(int*)a;
    int y = *(int*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int16_t ECOCALLMETHOD cmpItem(voidptr_t a, voidptr_t b) {
    int x = ((TestItem*)a)->key;
    int y = ((TestItem*)b)->key;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int16_t ECOCALLMETHOD cmpDouble(voidptr_t a, voidptr_t b) {
    double x = *(double*)a;
    double y = *(double*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int16_t ECOCALLMETHOD cmpStr(voidptr_t a, voidptr_t b) {
    const char* x = *(char**)a;
    const char* y = *(char**)b;
    while (*x != 0 && *x == *y) {
        x++;
        y++;
    }
    if ((unsigned char)*x < (unsigned char)*y) return -1;
    if ((unsigned char)*x > (unsigned char)*y) return 1;
    return 0;
}

/*mode: 0- random, 1- sorted, 2- revers, 3- few unique, 4- almost sorted */
static void fillInt(int* a, int n, int mode) {
    int i, j, t;
    for (i = 0; i < n; i++) {
        if (mode == 0)      a[i] = (int)nextRand() - 16000;
        else if (mode == 1) a[i] = i;
        else if (mode == 2) a[i] = n - i;
        else if (mode == 3) a[i] = (int)(nextRand() % 10);
        else                a[i] = i;
    }
    if (mode == 4) {
        for (i = 0; i < n / 20 + 1 && n > 1; i++) {
            j = (int)(nextRand() % (uint32_t)n);
            t = a[j];
            a[j] = a[0];
            a[0] = t;
        }
    }
}

/*0= success, else- some errors*/
static int16_t runIntCase(IEcoSortAndSearch* pISort, IEcoMemoryAllocator1* pIMem, int n, int mode) {
    int* a;
    int i;
    uint32_t sumBefore = 0, sumAfter = 0;
    int16_t r;

    a = (int*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)(n + 1) * sizeof(int));
    if (a == 0) {
        return 4;
    }
    fillInt(a, n, mode);
    for (i = 0; i < n; i++) sumBefore += (uint32_t)a[i] * 2654435761u + 1;

    r = pISort->pVTbl->Timsort(pISort, a, (uint32_t)n, sizeof(int), cmpInt);
    if (r != 0) {
        pIMem->pVTbl->Free(pIMem, a);
        return 1;
    }
    for (i = 0; i < n; i++) sumAfter += (uint32_t)a[i] * 2654435761u + 1;
    for (i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            pIMem->pVTbl->Free(pIMem, a);
            return 2;
        }
    }
    pIMem->pVTbl->Free(pIMem, a);
    if (sumBefore != sumAfter) {
        return 3;
    }
    return 0;
}

/*stability: equal keys keep their original id order*/
static int16_t runStableCase(IEcoSortAndSearch* pISort, IEcoMemoryAllocator1* pIMem, int n) {
    TestItem* a;
    int i;
    int16_t r;

    a = (TestItem*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)n * sizeof(TestItem));
    if (a == 0) {
        return 4;
    }
    for (i = 0; i < n; i++) {
        a[i].key = (int)(nextRand() % 20);
        a[i].id = i;
    }
    r = pISort->pVTbl->Timsort(pISort, a, (uint32_t)n, sizeof(TestItem), cmpItem);
    if (r != 0) {
        pIMem->pVTbl->Free(pIMem, a);
        return 1;
    }
    for (i = 1; i < n; i++) {
        if (a[i - 1].key > a[i].key ||
            (a[i - 1].key == a[i].key && a[i - 1].id > a[i].id)) {
            pIMem->pVTbl->Free(pIMem, a);
            return 5;
        }
    }
    pIMem->pVTbl->Free(pIMem, a);
    return 0;
}


/* double, 0 = success; 1 = error; 2 = not sorted; 3 = changed elements; 4 = no space */
static int16_t runDoubleCase(IEcoSortAndSearch* pISort, IEcoMemoryAllocator1* pIMem, int n) {
    double* a;
    double sumBefore = 0.0, sumAfter = 0.0;
    int i;
    int16_t r;

    a = (double*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)n * sizeof(double));
    if (a == 0) {
        return 4;
    }
    for (i = 0; i < n; i++) {
        a[i] = (double)((int)(nextRand() % 2000001u) - 1000000) / 8.0;
        sumBefore += a[i];
    }
    r = pISort->pVTbl->Timsort(pISort, a, (uint32_t)n, sizeof(double), cmpDouble);
    if (r != 0) {
        pIMem->pVTbl->Free(pIMem, a);
        return 1;
    }
    for (i = 0; i < n; i++) {
        sumAfter += a[i];
    }
    for (i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            pIMem->pVTbl->Free(pIMem, a);
            return 2;
        }
    }
    pIMem->pVTbl->Free(pIMem, a);
    if (sumBefore != sumAfter) {
        return 3;
    }
    return 0;
}

/*char*/
static int16_t runStringCase(IEcoSortAndSearch* pISort, IEcoMemoryAllocator1* pIMem, int n) {
    char* pool;
    char** arr;
    char* p;
    unsigned long sumBefore = 0, sumAfter = 0;
    int i, k, len;
    int16_t r;

    pool = (char*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)n * 8u);
    arr = (char**)pIMem->pVTbl->Alloc(pIMem, (uint32_t)n * sizeof(char*));
    if (pool == 0 || arr == 0) {
        if (pool != 0) pIMem->pVTbl->Free(pIMem, pool);
        if (arr != 0)  pIMem->pVTbl->Free(pIMem, arr);
        return 4;
    }
    for (i = 0; i < n; i++) {
        p = pool + (uint32_t)i * 8u;
        len = 1 + (int)(nextRand() % 6u);
        for (k = 0; k < len; k++) {
            p[k] = (char)('a' + (int)(nextRand() % 4u));
        }
        p[len] = 0;
        arr[i] = p;
        sumBefore += (unsigned long)p;
    }
    r = pISort->pVTbl->Timsort(pISort, arr, (uint32_t)n, sizeof(char*), cmpStr);
    if (r != 0) {
        pIMem->pVTbl->Free(pIMem, arr);
        pIMem->pVTbl->Free(pIMem, pool);
        return 1;
    }
    for (i = 0; i < n; i++) {
        sumAfter += (unsigned long)arr[i];
    }
    for (i = 1; i < n; i++) {
        if (cmpStr(&arr[i - 1], &arr[i]) > 0) {
            pIMem->pVTbl->Free(pIMem, arr);
            pIMem->pVTbl->Free(pIMem, pool);
            return 2;
        }
    }
    pIMem->pVTbl->Free(pIMem, arr);
    pIMem->pVTbl->Free(pIMem, pool);
    if (sumBefore != sumAfter) {
        return 3;
    }
    return 0;
}

/*Error cases*/
static void runErrorCases(IEcoSortAndSearch* pISort) {
    int a[3];
    int16_t r;

    a[0] = 3; a[1] = 1; a[2] = 2;

    r = pISort->pVTbl->Timsort(pISort, 0, 3, sizeof(int), cmpInt);
    check(r != 0, 401, "base == NULL", (int)r, 0);

    r = pISort->pVTbl->Timsort(pISort, a, 3, 0, cmpInt);
    check(r != 0, 402, "size == 0", (int)r, 0);

    r = pISort->pVTbl->Timsort(pISort, a, 3, sizeof(int), 0);
    check(r != 0, 403, "cmp == NULL", (int)r, 0);

    r = pISort->pVTbl->Timsort(0, a, 3, sizeof(int), cmpInt);
    check(r != 0, 404, "me == NULL", (int)r, 0);

    check(a[0] == 3 && a[1] == 1 && a[2] == 2, 405, "data changed after error calls", 0, 0);

    r = pISort->pVTbl->Timsort(pISort, a, 0, sizeof(int), cmpInt);
    check(r == 0 && a[0] == 3 && a[1] == 1 && a[2] == 2, 406, "num == 0", (int)r, 0);

    r = pISort->pVTbl->Timsort(pISort, a, 1, sizeof(int), cmpInt);
    check(r == 0 && a[0] == 3 && a[1] == 1 && a[2] == 2, 407, "num == 1", (int)r, 0);
}

static unsigned long g_cmpCount = 0;

static int16_t ECOCALLMETHOD cmpIntCnt(voidptr_t a, voidptr_t b) {
    int x = *(int*)a;
    int y = *(int*)b;
    g_cmpCount++;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int cmpIntQ(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    g_cmpCount++;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/*time my timsort vs qsort*/
static void runBenchmark(IEcoSortAndSearch* pISort, IEcoMemoryAllocator1* pIMem) {
    static const char* modeName[5] = {"random", "sorted", "reversed", "few_unique", "nearly_sorted"};
    int sizes[5];
    int s, m, i, rep, reps, n, ok;
    int *src, *a, *b;
    clock_t t0, t1, tsTicks, qTicks;
    unsigned long tsCmp = 0, qCmp = 0;
    double tsMs, qMs;

    sizes[0] = 1000;   sizes[1] = 10000;  sizes[2] = 100000;
    sizes[3] = 500000; sizes[4] = 1000000;

    src = (int*)pIMem->pVTbl->Alloc(pIMem, 1000000u * sizeof(int));
    a   = (int*)pIMem->pVTbl->Alloc(pIMem, 1000000u * sizeof(int));
    b   = (int*)pIMem->pVTbl->Alloc(pIMem, 1000000u * sizeof(int));
    if (src == 0 || a == 0 || b == 0) {
        printf("bench: no memory\n");
        goto Done;
    }

    printf("n;data;timsort_ms;qsort_ms;qsort/timsort;timsort_cmp;qsort_cmp;match\n");
    for (s = 0; s < 5; s++) {
        n = sizes[s];
        reps = 4000000 / n;
        if (reps < 1) reps = 1;
        for (m = 0; m < 5; m++) {
            g_seed = 12345;
            fillInt(src, n, m);
            tsTicks = 0;
            qTicks = 0;
            for (rep = 0; rep < reps; rep++) {
                for (i = 0; i < n; i++) { a[i] = src[i]; b[i] = src[i]; }

                g_cmpCount = 0;
                t0 = clock();
                pISort->pVTbl->Timsort(pISort, a, (uint32_t)n, sizeof(int), cmpIntCnt);
                t1 = clock();
                tsTicks += t1 - t0;
                tsCmp = g_cmpCount;

                g_cmpCount = 0;
                t0 = clock();
                qsort(b, (size_t)n, sizeof(int), cmpIntQ);
                t1 = clock();
                qTicks += t1 - t0;
                qCmp = g_cmpCount;
            }
            ok = 1;
            for (i = 0; i < n; i++) {
                if (a[i] != b[i]) { ok = 0; break; }
            }
            tsMs = 1000.0 * (double)tsTicks / (double)CLOCKS_PER_SEC / (double)reps;
            qMs  = 1000.0 * (double)qTicks  / (double)CLOCKS_PER_SEC / (double)reps;
            printf("%d;%s;%.3f;%.3f;%.2f;%lu;%lu;%s\n", n, modeName[m], tsMs, qMs,
                   (tsMs > 0.0) ? (qMs / tsMs) : 0.0, tsCmp, qCmp, ok ? "OK" : "FAIL");
        }
    }
Done:
    if (src != 0) pIMem->pVTbl->Free(pIMem, src);
    if (a != 0)   pIMem->pVTbl->Free(pIMem, a);
    if (b != 0)   pIMem->pVTbl->Free(pIMem, b);
}


int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    int16_t r;
    int s, m;
    int calcChecks = 0;
    int calcErrors = 0;
    int sizes[15];
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    IEcoSortAndSearch* pISort = 0;

    sizes[0] = 0;    sizes[1] = 1;    sizes[2] = 2;     sizes[3] = 3;
    sizes[4] = 10;   sizes[5] = 31;   sizes[6] = 32;    sizes[7] = 33;
    sizes[8] = 63;   sizes[9] = 64;   sizes[10] = 65;   sizes[11] = 100;
    sizes[12] = 1000; sizes[13] = 10000; sizes[14] = 1000000;

    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }
#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoSortAndSearch, (IEcoUnknown*)GetIEcoComponentFactoryPtr_591FC2D6783F43F0B5B565CCCAF05939);
    if (result != 0) {
        goto Release;
    }
#endif
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0 || pIMem == 0) {
        goto Release;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoSortAndSearch, 0, &IID_IEcoSortAndSearch, (void**) &pISort);
    if (result != 0 || pISort == 0) {
        goto Release;
    }


    /* int */
    for (s = 0; s < 15; s++) {
        for (m = 0; m < 5; m++) {
            r = runIntCase(pISort, pIMem, sizes[s], m);
            check(r == 0, (int16_t)(100 + r), "int sort", sizes[s], m);
        }
    }

    r = runStableCase(pISort, pIMem, 5000);
    check(r == 0, (int16_t)(200 + r), "stability (struct)", 5000, 0);

    /* double */
    r = runDoubleCase(pISort, pIMem, 1000);
    check(r == 0, (int16_t)(300 + r), "double sort", 1000, 0);
    r = runDoubleCase(pISort, pIMem, 100000);
    check(r == 0, (int16_t)(300 + r), "double sort", 100000, 0);

    /*char**/
    r = runStringCase(pISort, pIMem, 1);
    check(r == 0, (int16_t)(500 + r), "string sort", 1, 0);
    r = runStringCase(pISort, pIMem, 2);
    check(r == 0, (int16_t)(500 + r), "string sort", 2, 0);
    r = runStringCase(pISort, pIMem, 5000);
    check(r == 0, (int16_t)(500 + r), "string sort", 5000, 0);

    runErrorCases(pISort);

    /* Задание 2 */
    calcErrors = RunCalculatorTests(pIBus, (IEcoUnknown*)pISys, &calcChecks);
    g_checks += calcChecks;
    g_errors += calcErrors;
    if (calcErrors != 0 && g_firstFail == 0) {
        g_firstFail = 600;
    }

    fprintf(stderr, "Checks: %d, errors: %d\n", g_checks, g_errors);
    result = (g_errors == 0) ? 0 : g_firstFail;

    if (g_errors == 0) {
        runBenchmark(pISort, pIMem);
    }

Release:
    if (pISort != 0) pISort->pVTbl->Release(pISort);
    if (pIMem != 0)  pIMem->pVTbl->Release(pIMem);
    if (pIBus != 0)  pIBus->pVTbl->Release(pIBus);
    if (pISys != 0)  pISys->pVTbl->Release(pISys);
    return result;
}