/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoLab2
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoLab2
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

#ifndef __I_ECOLAB2_H__
#define __I_ECOLAB2_H__

#include "IEcoBase1.h"

/* IEcoLab2 IID = {37E3CBC5-B953-434B-946C-86319278A5AB} */
#ifndef __IID_IEcoLab2
static const UGUID IID_IEcoLab2 = {0x01, 0x10, {0x37, 0xE3, 0xCB, 0xC5, 0xB9, 0x53, 0x43, 0x4B, 0x94, 0x6C, 0x86, 0x31, 0x92, 0x78, 0xA5, 0xAB}};
#endif /* __IID_IEcoLab2 */

typedef struct IEcoLab2* IEcoLab2Ptr_t;

typedef struct IEcoLab2VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab2Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab2Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab2Ptr_t me);

    /* IEcoLab2 */
    /* Вычисление a op b, где op - один из символов '+', '-', '*', '/' */
    int16_t (ECOCALLMETHOD *Calculate)(/* in */ IEcoLab2Ptr_t me, /* in */ char_t op, /* in */ int16_t a, /* in */ int16_t b, /* out */ int32_t* pResult);

} IEcoLab2VTbl, *IEcoLab2VTblPtr_t;

interface IEcoLab2 {
    struct IEcoLab2VTbl *pVTbl;
} IEcoLab2;


#endif /* __I_ECOLAB2_H__ */
