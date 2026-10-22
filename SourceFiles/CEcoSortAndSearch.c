/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoSortAndSearch_CAF05939
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoSortAndSearch_CAF05939
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */


#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoSortAndSearch.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/* Указатель на экземпляр по указателю на интерфейс, хранящийся в поле с номером index */
#define CEcoSortAndSearch_CAF05939_FromInterface(me, index) \
    ((CEcoSortAndSearch_CAF05939*)((char_t*)(me) - (index) * sizeof(voidptr_t)))

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoSortAndSearch interface
 *   Делегирует вызов управляющему IEcoUnknown (внешнему компоненту, если агрегирован)
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_QueryInterface(/* in */ IEcoSortAndSearchPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSortAndSearch_CAF05939* pCMe = (CEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoSortAndSearch interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_AddRef(/* in */ IEcoSortAndSearchPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = (CEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoSortAndSearch interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Release(/* in */ IEcoSortAndSearchPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = (CEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   NondelegatingQueryInterface Function
 * </summary>
 *
 * <description>
 *   Неделегирующий QueryInterface: единственное место, где компонент решает,
 *   какие интерфейсы он отдает и каким способом (собственный, включение, агрегирование)
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_NondelegatingQueryInterface(/* in */ IEcoUnknownPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 1);

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoSortAndSearch) ) {
        *ppv = &pCMe->m_pVTblIEcoSortAndSearch;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        /* Для IEcoUnknown возвращается неделегирующий интерфейс - он задает идентичность компонента */
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pIUnkInnerX != 0 ) {
        /* Агрегирование: указатель на интерфейс отдает сам внутренний компонент B */
        return pCMe->m_pIUnkInnerX->pVTbl->QueryInterface(pCMe->m_pIUnkInnerX, riid, ppv);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pIX != 0 ) {
        /* Включение: собственная таблица, методы которой вызывают компонент A (C) */
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) && pCMe->m_pIY != 0 ) {
        /* Включение: собственная таблица, методы которой вызывают компонент D (E, C) */
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)*ppv);

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   NondelegatingAddRef Function
 * </summary>
 *
 * <description>
 *   NondelegatingAddRef function
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_NondelegatingAddRef(/* in */ IEcoUnknownPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 1);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   NondelegatingRelease Function
 * </summary>
 *
 * <description>
 *   NondelegatingRelease function
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_NondelegatingRelease(/* in */ IEcoUnknownPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 1);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    --pCMe->m_cRef;

    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        /* Защита от повторного входа при освобождении внутренних компонентов */
        pCMe->m_cRef = 1;
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoCalculatorX interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorX_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 2);

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoCalculatorX interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorX_AddRef(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 2);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoCalculatorX interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorX_Release(/* in */ IEcoCalculatorXPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 2);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Addition Function
 * </summary>
 *
 * <description>
 *   Включение: вызов метода включаемого компонента A (C)
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 2);

    /* Pointer Validation */
    if (me == 0 || pCMe->m_pIX == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIX->pVTbl->Addition(pCMe->m_pIX, a, b);
}

/*
 *
 * <summary>
 *   Subtraction Function
 * </summary>
 *
 * <description>
 *   Включение: вызов метода включаемого компонента A (C)
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 2);

    /* Pointer Validation */
    if (me == 0 || pCMe->m_pIX == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIX->pVTbl->Subtraction(pCMe->m_pIX, a, b);
}

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoCalculatorY interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorY_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 3);

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoCalculatorY interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorY_AddRef(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 3);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoCalculatorY interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_IEcoCalculatorY_Release(/* in */ IEcoCalculatorYPtr_t me) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 3);

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
}

/*
 *
 * <summary>
 *   Multiplication Function
 * </summary>
 *
 * <description>
 *   Включение: вызов метода включаемого компонента D (E, C)
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 3);

    /* Pointer Validation */
    if (me == 0 || pCMe->m_pIY == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    return pCMe->m_pIY->pVTbl->Multiplication(pCMe->m_pIY, a, b);
}

/*
 *
 * <summary>
 *   Division Function
 * </summary>
 *
 * <description>
 *   Включение: внешний компонент проверяет аргументы до вызова включаемого компонента D (E, C)
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoSortAndSearch_CAF05939* pCMe = CEcoSortAndSearch_CAF05939_FromInterface(me, 3);

    /* Pointer Validation */
    if (me == 0 || pCMe->m_pIY == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Деление на ноль: интерфейс не имеет кода ошибки, поэтому -1 */
    if (b == 0) {
        return -1;
    }

    return pCMe->m_pIY->pVTbl->Division(pCMe->m_pIY, a, b);
}

/*timsort */
#define TS_MIN_MERGE 64
#define TS_MAX_RUNS  96
#define TS_ERR_NOMEM ((int16_t)-3)

typedef struct TsCtx {
    char_t*       base;
    uint32_t      size;
    CompareFunc_t cmp;
    char_t*       tmp;
    uint32_t      runBase[TS_MAX_RUNS];
    uint32_t      runLen[TS_MAX_RUNS];
    uint32_t      nRuns;
} TsCtx;

#define TS_ELEM(c, i) ((c)->base + (uint32_t)(i) * (c)->size)

/* copy without overlappping regions(no memcpy) */
static void ts_copy(char_t* dst, const char_t* src, uint32_t n) {
    uint32_t k;
    for (k = 0; k < n; k++) {
        dst[k] = src[k];
    }
}

/*copy with possible overlapping regions (no memmove) */
static void ts_move(char_t* dst, char_t* src, uint32_t n) {
    uint32_t k;
    if (dst < src) {
        for (k = 0; k < n; k++) {
            dst[k] = src[k];
        }
    } else if (dst > src) {
        for (k = n; k > 0; k--) {
            dst[k - 1] = src[k - 1];
        }
    }
}
/* memmove analog for reverse descenfing runs*/
static void ts_swap(char_t* a, char_t* b, uint32_t n) {
    uint32_t k;
    char_t t;
    for (k = 0; k < n; k++) {
        t = a[k];
        a[k] = b[k];
        b[k] = t;
    }
}
/*reverse descending run*/
static void ts_reverse(TsCtx* c, uint32_t lo, uint32_t n) {
    uint32_t i = 0;
    uint32_t j = n - 1;
    while (i < j) {
        ts_swap(TS_ELEM(c, lo + i), TS_ELEM(c, lo + j), c->size);
        i++;
        j--;
    }
}
/* minrun caluculate for balanced merge*/
static uint32_t ts_minrun(uint32_t n) {
    uint32_t r = 0;
    while (n >= TS_MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

/* len of the natural run starting at lo, strictly descending run is reversed */
static uint32_t ts_count_run(TsCtx* c, uint32_t lo, uint32_t n) {
    uint32_t i = 2;
    if (n == 1) {
        return 1;
    }
    if (c->cmp(TS_ELEM(c, lo + 1), TS_ELEM(c, lo)) < 0) {
        /*stirctly descending*/
        while (i < n && c->cmp(TS_ELEM(c, lo + i), TS_ELEM(c, lo + i - 1)) < 0) {
            i++;
        }
        ts_reverse(c, lo, i);
    } else {
        /* non descending*/
        while (i < n && c->cmp(TS_ELEM(c, lo + i), TS_ELEM(c, lo + i - 1)) >= 0) {
            i++;
        }
    }
    return i;
}

/*binary insertion sort, [lo, lo+start) is already sorted
   temporary element is stored at the beginning of c->tmp*/
static void ts_binary_insertion(TsCtx* c, uint32_t lo, uint32_t n, uint32_t start) {
    uint32_t i, left, right, mid;
    for (i = start; i < n; i++) {
        ts_copy(c->tmp, TS_ELEM(c, lo + i), c->size);
        left = 0;
        right = i;
        while (left < right) {
            mid = (left + right) / 2;
            if (c->cmp(c->tmp, TS_ELEM(c, lo + mid)) < 0) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        ts_move(TS_ELEM(c, lo + left + 1), TS_ELEM(c, lo + left), (i - left) * c->size);
        ts_copy(TS_ELEM(c, lo + left), c->tmp, c->size);
    }
}

/*merge runs [lo, lo+la) and [lo+la, lo+la+lb) through the tmp buffer*/
static void ts_merge(TsCtx* c, uint32_t lo, uint32_t la, uint32_t lb) {
    uint32_t ia, ib, k;
    if (la == 0 || lb == 0) {
        return;
    }
    if (la <= lb) {
        /*copy left run into tmp, merge forward*/
        ts_copy(c->tmp, TS_ELEM(c, lo), la * c->size);
        ia = 0;
        ib = la;
        k = 0;
        while (ia < la && ib < la + lb) {
            if (c->cmp(TS_ELEM(c, lo + ib), c->tmp + ia * c->size) < 0) {
                ts_copy(TS_ELEM(c, lo + k), TS_ELEM(c, lo + ib), c->size);
                ib++;
            } else {
                ts_copy(TS_ELEM(c, lo + k), c->tmp + ia * c->size, c->size);
                ia++;
            }
            k++;
        }
        while (ia < la) {
            ts_copy(TS_ELEM(c, lo + k), c->tmp + ia * c->size, c->size);
            ia++;
            k++;
        }
    } else {
        /*copy right run into tmp, merge backward*/
        ts_copy(c->tmp, TS_ELEM(c, lo + la), lb * c->size);
        ia = la;
        ib = lb;
        k = la + lb;
        while (ia > 0 && ib > 0) {
            k--;
            if (c->cmp(c->tmp + (ib - 1) * c->size, TS_ELEM(c, lo + ia - 1)) >= 0) {
                ts_copy(TS_ELEM(c, lo + k), c->tmp + (ib - 1) * c->size, c->size);
                ib--;
            } else {
                ts_copy(TS_ELEM(c, lo + k), TS_ELEM(c, lo + ia - 1), c->size);
                ia--;
            }
        }
        while (ib > 0) {
            k--;
            ib--;
            ts_copy(TS_ELEM(c, lo + k), c->tmp + ib * c->size, c->size);
        }
    }
}
/*merge stack*/
static void ts_merge_at(TsCtx* c, uint32_t i) {
    uint32_t lo = c->runBase[i];
    uint32_t la = c->runLen[i];
    uint32_t lb = c->runLen[i + 1];

    c->runLen[i] = la + lb;
    if (i + 3 == c->nRuns) {
        c->runBase[i + 1] = c->runBase[i + 2];
        c->runLen[i + 1] = c->runLen[i + 2];
    }
    c->nRuns--;
    ts_merge(c, lo, la, lb);
}

/*run in stack invariants (a > b + c, b > c)*/
static void ts_collapse(TsCtx* c) {
    uint32_t n;
    while (c->nRuns > 1) {
        n = c->nRuns - 2;
        if ((n > 0 && c->runLen[n - 1] <= c->runLen[n] + c->runLen[n + 1]) ||
            (n > 1 && c->runLen[n - 2] <= c->runLen[n - 1] + c->runLen[n])) {
            if (c->runLen[n - 1] < c->runLen[n + 1]) {
                n--;
            }
            ts_merge_at(c, n);
        } else if (c->runLen[n] <= c->runLen[n + 1]) {
            ts_merge_at(c, n);
        } else {
            break;
        }
    }
}

static void ts_force_collapse(TsCtx* c) {
    uint32_t n;
    while (c->nRuns > 1) {
        n = c->nRuns - 2;
        if (n > 0 && c->runLen[n - 1] < c->runLen[n + 1]) {
            n--;
        }
        ts_merge_at(c, n);
    }
}

/*
 *
 * <summary>
 *   Timsort Function
 * </summary>
 *
 * <description>
 *   Устойчивая сортировка, прототип как у qsort (base, num, size, cmp).
 *   Временный буфер выделяется один раз через аллокатор Eco.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoSortAndSearch_CAF05939_Timsort(/* in */ IEcoSortAndSearchPtr_t me,
                                                                 /* in */ voidptr_t base,
                                                                 /* in */ uint32_t num,
                                                                 /* in */ uint32_t size,
                                                                 /* in */ CompareFunc_t cmp) {
    CEcoSortAndSearch_CAF05939* pCMe = (CEcoSortAndSearch_CAF05939*)me;
    TsCtx ctx;
    uint32_t remaining, lo, minrun, runLen, force, tmpBytes;

    if (me == 0 || base == 0 || cmp == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }
    if (num < 2) {
        return ERR_ECO_SUCCESS;
    }
    if ((num / 2 + 1) > (0xFFFFFFFFu / size)) {
        return TS_ERR_NOMEM;
    }

    /*buffer: the shorter of the merged runs is <= num/2, plus one spare element*/
    tmpBytes = (num / 2 + 1) * size;
    ctx.tmp = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, tmpBytes);
    if (ctx.tmp == 0) {
        return TS_ERR_NOMEM;
    }
    ctx.base = (char_t*)base;
    ctx.size = size;
    ctx.cmp = cmp;
    ctx.nRuns = 0;

    minrun = ts_minrun(num);
    remaining = num;
    lo = 0;
    while (remaining > 0) {
        runLen = ts_count_run(&ctx, lo, remaining);
        if (runLen < minrun) {
            force = (remaining < minrun) ? remaining : minrun;
            ts_binary_insertion(&ctx, lo, force, runLen);
            runLen = force;
        }
        ctx.runBase[ctx.nRuns] = lo;
        ctx.runLen[ctx.nRuns] = runLen;
        ctx.nRuns++;
        ts_collapse(&ctx);
        lo += runLen;
        remaining -= runLen;
    }
    ts_force_collapse(&ctx);

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, ctx.tmp);
    return ERR_ECO_SUCCESS;
}


/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 *   Здесь компонент получает внутренние компоненты калькулятора:
 *     IEcoCalculatorX - агрегирование B (рис. 2), если B нет - включение A (рис. 1);
 *     IEcoCalculatorY - включение D, если D нет - E, если нет и E - C (рис. 1).
 *   Калькуляторы необязательны: без них компонент работает как сортировщик.
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoSortAndSearch_CAF05939(/*in*/ CEcoSortAndSearch_CAF05939Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoSortAndSearch_CAF05939* pCMe = (CEcoSortAndSearch_CAF05939*)me;
    IEcoInterfaceBus1* pIBus = 0;

    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Pointer Validation */
    if (me == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Storing the pointer to the system interface (own reference, released in Delete) */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pCMe->m_pISys);
    if (result != 0 || pCMe->m_pISys == 0) {
        pCMe->m_pISys = 0;
        return ERR_ECO_NOSYSTEM;
    }

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        return ERR_ECO_NOBUS;
    }

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        pCMe->m_pIMem = 0;
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* IEcoCalculatorX, рис. 2: создание внутреннего компонента B с агрегированием */
    /* ВАЖНО: при агрегировании передается управляющий IEcoUnknown и запрашивается IID_IEcoUnknown */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pCMe->m_pIUnkOuter, &IID_IEcoUnknown, (void**) &pCMe->m_pIUnkInnerX);
    if (result != 0 || pCMe->m_pIUnkInnerX == 0) {
        pCMe->m_pIUnkInnerX = 0;
        /* Рис. 1: B недоступен - включение компонента A */
        /* Под CID A может быть установлена новая версия компонента - C */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
        if (result != 0) {
            pCMe->m_pIX = 0;
        }
    }

    /* IEcoCalculatorY, рис. 1: включение компонента D */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
    if (result != 0 || pCMe->m_pIY == 0) {
        /* D недоступен - включение компонента E */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
    }
    if (result != 0 || pCMe->m_pIY == 0) {
        /* E недоступен - включение компонента C (версия A, дополнительно реализующая IEcoCalculatorY) */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
    }
    if (result != 0) {
        pCMe->m_pIY = 0;
    }

    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoSortAndSearch_CAF05939(/* in */ CEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }

    /* If not aggregating, use the nondelegating IEcoUnknown interface */
    /* ВАЖНО: ссылка на внешний компонент не увеличивается, иначе возникнет циклическая ссылка */
    if (pIUnkOuter != 0) {
        pCMe->m_pIUnkOuter = pIUnkOuter;
    } else {
        pCMe->m_pIUnkOuter = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
    }

    pCMe->m_pIUnkInnerX = 0;
    pCMe->m_pIX = 0;
    pCMe->m_pIY = 0;

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Delete Function
 * </summary>
 *
 * <description>
 *   Instance freeing function
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoSortAndSearch_CAF05939(/* in */ CEcoSortAndSearch_CAF05939Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIUnkInnerX != 0 ) {
            /* Агрегированный компонент освобождается через его неделегирующий IEcoUnknown */
            pCMe->m_pIUnkInnerX->pVTbl->Release(pCMe->m_pIUnkInnerX);
            pCMe->m_pIUnkInnerX = 0;
        }
        if ( pCMe->m_pIX != 0 ) {
            pCMe->m_pIX->pVTbl->Release(pCMe->m_pIX);
            pCMe->m_pIX = 0;
        }
        if ( pCMe->m_pIY != 0 ) {
            pCMe->m_pIY->pVTbl->Release(pCMe->m_pIY);
            pCMe->m_pIY = 0;
        }
        if ( pIMem != 0 && pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        if ( pIMem != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe);
            pIMem->pVTbl->Release(pIMem);
        }
    }
}

/* IEcoSortAndSearch Virtual Table */
IEcoSortAndSearchVTbl g_xE6748509DDCC4A21BB65C0BF5878B5FBVTbl_CAF05939 = {
    CEcoSortAndSearch_CAF05939_QueryInterface,
    CEcoSortAndSearch_CAF05939_AddRef,
    CEcoSortAndSearch_CAF05939_Release,
    CEcoSortAndSearch_CAF05939_Timsort
};

/* IEcoNondelegatingUnknown Virtual Table */
IEcoUnknownVTbl g_x000000000000000000000000000000AAVTblUnk_CAF05939 = {
    CEcoSortAndSearch_CAF05939_NondelegatingQueryInterface,
    CEcoSortAndSearch_CAF05939_NondelegatingAddRef,
    CEcoSortAndSearch_CAF05939_NondelegatingRelease
};

/* IEcoCalculatorX Virtual Table */
IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTbl_CAF05939 = {
    CEcoSortAndSearch_CAF05939_IEcoCalculatorX_QueryInterface,
    CEcoSortAndSearch_CAF05939_IEcoCalculatorX_AddRef,
    CEcoSortAndSearch_CAF05939_IEcoCalculatorX_Release,
    CEcoSortAndSearch_CAF05939_Addition,
    CEcoSortAndSearch_CAF05939_Subtraction
};

/* IEcoCalculatorY Virtual Table */
IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTbl_CAF05939 = {
    CEcoSortAndSearch_CAF05939_IEcoCalculatorY_QueryInterface,
    CEcoSortAndSearch_CAF05939_IEcoCalculatorY_AddRef,
    CEcoSortAndSearch_CAF05939_IEcoCalculatorY_Release,
    CEcoSortAndSearch_CAF05939_Multiplication,
    CEcoSortAndSearch_CAF05939_Division
};


/* Object Instance */
CEcoSortAndSearch_CAF05939 g_xCEcoSortAndSearch_CAF05939 = {
    &g_xE6748509DDCC4A21BB65C0BF5878B5FBVTbl_CAF05939,
    &g_x000000000000000000000000000000AAVTblUnk_CAF05939,
    &g_x9322111622484742AE0682819447843DVTbl_CAF05939,
    &g_xBD6414C29096423EA90C04D77AFD1CADVTbl_CAF05939,
    initCEcoSortAndSearch_CAF05939,
    createCEcoSortAndSearch_CAF05939,
    deleteCEcoSortAndSearch_CAF05939,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_pIUnkOuter */
    0, /* m_pIUnkInnerX */
    0, /* m_pIX */
    0, /* m_pIY */
    0  /* m_Name */
};
