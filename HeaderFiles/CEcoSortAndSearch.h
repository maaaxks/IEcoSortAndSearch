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
 *   This header describes the implementation of the CEcoSortAndSearch_CAF05939 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOSORTANDSEARCH_H__
#define __C_ECOSORTANDSEARCH_H__

#include "IEcoSortAndSearch.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoSortAndSearch_CAF05939* CEcoSortAndSearch_CAF05939Ptr_t;

/*
 * Порядок первых четырех полей важен: по указателю на интерфейс вычисляется
 * указатель на экземпляр (смещение = номер поля * sizeof(voidptr_t))
 */
typedef struct CEcoSortAndSearch_CAF05939 {

    /* IEcoSortAndSearch interface function table */
    IEcoSortAndSearchVTbl* m_pVTblIEcoSortAndSearch;

    /* Неделегирующий интерфейс IEcoUnknown (поддержка агрегирования) */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Таблица функций интерфейса IEcoCalculatorX (включение A или C) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* Таблица функций интерфейса IEcoCalculatorY (включение D, E или C) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoSortAndSearch_CAF05939Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoSortAndSearch_CAF05939Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Делегирующий IEcoUnknown: указывает на внешний (если компонент агрегирован) или на собственный неделегирующий IEcoUnknown */
    IEcoUnknown* m_pIUnkOuter;

    /* IEcoUnknown внутреннего компонента B (агрегирование IEcoCalculatorX) */
    IEcoUnknown* m_pIUnkInnerX;

    /* Указатель на IEcoCalculatorX включаемого компонента A (или C) */
    IEcoCalculatorX* m_pIX;

    /* Указатель на IEcoCalculatorY включаемого компонента D (или E, или C) */
    IEcoCalculatorY* m_pIY;

    /* Instance data */
    char_t* m_Name;

} CEcoSortAndSearch_CAF05939;

#endif /* __C_ECOSORTANDSEARCH_H__ */
