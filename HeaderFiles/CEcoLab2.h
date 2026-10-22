/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab2_86AB2353
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoLab2_86AB2353 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOLAB2_H__
#define __C_ECOLAB2_H__

#include "IEcoLab2.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoLab2_86AB2353* CEcoLab2_86AB2353Ptr_t;

/*
 * Порядок первых трех полей важен: по указателю на интерфейс вычисляется
 * указатель на экземпляр (смещение = номер поля * sizeof(voidptr_t))
 */
typedef struct CEcoLab2_86AB2353 {

    /* IEcoLab2 interface function table (он же IEcoUnknown компонента) */
    IEcoLab2VTbl* m_pVTblIEcoLab2;

    /* Таблица функций интерфейса IEcoCalculatorX (включение) */
    IEcoCalculatorXVTbl* m_pVTblIEcoCalculatorX;

    /* Таблица функций интерфейса IEcoCalculatorY (включение) */
    IEcoCalculatorYVTbl* m_pVTblIEcoCalculatorY;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoLab2_86AB2353Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoLab2_86AB2353Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoLab2_86AB2353Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* IEcoUnknown внутреннего компонента Eco.Lab1 (агрегирование IEcoSortAndSearch) */
    IEcoUnknown* m_pIUnkInnerLab1;

    /* Указатель на IEcoCalculatorY включаемого компонента E */
    IEcoCalculatorY* m_pIY;

    /* Указатель на IEcoCalculatorX компонента E (его отдает агрегированный в E компонент B) */
    IEcoCalculatorX* m_pIX;

} CEcoLab2_86AB2353;

#endif /* __C_ECOLAB2_H__ */
