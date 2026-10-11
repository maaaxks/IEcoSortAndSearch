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
 *   This header describes the implementation of the CIEcoSortAndSearch_CAF05939 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_IECOSORTANDSEARCH_H__
#define __C_IECOSORTANDSEARCH_H__

#include "IIEcoSortAndSearch.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CIEcoSortAndSearch_CAF05939* CIEcoSortAndSearch_CAF05939Ptr_t;

typedef struct CIEcoSortAndSearch_CAF05939 {

    /* IIEcoSortAndSearch interface function table */
    IIEcoSortAndSearchVTbl* m_pVTblIIEcoSortAndSearch;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CIEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CIEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CIEcoSortAndSearch_CAF05939Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CIEcoSortAndSearch_CAF05939;

#endif /* __C_IECOSORTANDSEARCH_H__ */
