/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IIEcoSortAndSearch
 * </summary>
 *
 * <description>
 *   This header describes the interface IIEcoSortAndSearch
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_IECOSORTANDSEARCH_H__
#define __I_IECOSORTANDSEARCH_H__

#include "IEcoBase1.h"

/* IIEcoSortAndSearch IID = {E6748509-DDCC-4A21-BB65-C0BF5878B5FB} */
#ifndef __IID_IIEcoSortAndSearch
static const UGUID IID_IIEcoSortAndSearch = {0x01, 0x10, {0xE6, 0x74, 0x85, 0x09, 0xDD, 0xCC, 0x4A, 0x21, 0xBB, 0x65, 0xC0, 0xBF, 0x58, 0x78, 0xB5, 0xFB}};
#endif /* __IID_IIEcoSortAndSearch */

typedef struct IIEcoSortAndSearch* IIEcoSortAndSearchPtr_t;
/*comare function like in qsort fo timsort*/
typedef int16_t (ECOCALLMETHOD *CompareFunc_t)(/* in */ voidptr_t a, /* in */ voidptr_t b);

typedef struct IIEcoSortAndSearchVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IIEcoSortAndSearchPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IIEcoSortAndSearchPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IIEcoSortAndSearchPtr_t me);

    /* IIEcoSortAndSearch */
    int16_t (ECOCALLMETHOD *Timsort)(/* in */ IIEcoSortAndSearchPtr_t me,
                                     /* in */ voidptr_t base,
                                     /* in */ uint32_t num,
                                     /* in */ uint32_t size,
                                     /* in */ CompareFunc_t cmp);
} IIEcoSortAndSearchVTbl, *IIEcoSortAndSearchVTblPtr_t;

interface IIEcoSortAndSearch {
    struct IIEcoSortAndSearchVTbl *pVTbl;
} IIEcoSortAndSearch;


#endif /* __I_IECOSORTANDSEARCH_H__ */

