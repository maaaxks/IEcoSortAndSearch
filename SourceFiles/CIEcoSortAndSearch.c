/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CIEcoSortAndSearch_CAF05939
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CIEcoSortAndSearch_CAF05939
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
#include "CIEcoSortAndSearch.h"
/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IIEcoSortAndSearch interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CIEcoSortAndSearch_CAF05939_QueryInterface(/* in */ IIEcoSortAndSearchPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CIEcoSortAndSearch_CAF05939* pCMe = (CIEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IIEcoSortAndSearch) ) {
        *ppv = &pCMe->m_pVTblIIEcoSortAndSearch;
        pCMe->m_pVTblIIEcoSortAndSearch->AddRef((IIEcoSortAndSearch*)pCMe);
    }
	
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIIEcoSortAndSearch;
        pCMe->m_pVTblIIEcoSortAndSearch->AddRef((IIEcoSortAndSearch*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IIEcoSortAndSearch interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CIEcoSortAndSearch_CAF05939_AddRef(/* in */ IIEcoSortAndSearchPtr_t me) {
    CIEcoSortAndSearch_CAF05939* pCMe = (CIEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IIEcoSortAndSearch interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CIEcoSortAndSearch_CAF05939_Release(/* in */ IIEcoSortAndSearchPtr_t me) {
    CIEcoSortAndSearch_CAF05939* pCMe = (CIEcoSortAndSearch_CAF05939*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    --pCMe->m_cRef;
    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
		
        return 0;
    }
    return pCMe->m_cRef;
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
static int16_t ECOCALLMETHOD CIEcoSortAndSearch_CAF05939_Timsort(/* in */ IIEcoSortAndSearchPtr_t me,
                                                                 /* in */ voidptr_t base,
                                                                 /* in */ uint32_t num,
                                                                 /* in */ uint32_t size,
                                                                 /* in */ CompareFunc_t cmp) {
    CIEcoSortAndSearch_CAF05939* pCMe = (CIEcoSortAndSearch_CAF05939*)me;
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
 *   MyFunction Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */


/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCIEcoSortAndSearch_CAF05939(/*in*/ CIEcoSortAndSearch_CAF05939Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CIEcoSortAndSearch_CAF05939* pCMe = (CIEcoSortAndSearch_CAF05939*)me;
    IEcoInterfaceBus1* pIBus = 0;

    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;	

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

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
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }



    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
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
static int16_t ECOCALLMETHOD createCIEcoSortAndSearch_CAF05939(/* in */ CIEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }



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
static void ECOCALLMETHOD deleteCIEcoSortAndSearch_CAF05939(/* in */ CIEcoSortAndSearch_CAF05939Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IIEcoSortAndSearch Virtual Table */
IIEcoSortAndSearchVTbl g_xE6748509DDCC4A21BB65C0BF5878B5FBVTbl_CAF05939 = {
    CIEcoSortAndSearch_CAF05939_QueryInterface,
    CIEcoSortAndSearch_CAF05939_AddRef,
    CIEcoSortAndSearch_CAF05939_Release,
    CIEcoSortAndSearch_CAF05939_Timsort
};



/* Object Instance */
CIEcoSortAndSearch_CAF05939 g_xCIEcoSortAndSearch_CAF05939 = {
    &g_xE6748509DDCC4A21BB65C0BF5878B5FBVTbl_CAF05939,
   
    initCIEcoSortAndSearch_CAF05939,
    createCIEcoSortAndSearch_CAF05939,
    deleteCIEcoSortAndSearch_CAF05939,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0  /* m_Name */
};
