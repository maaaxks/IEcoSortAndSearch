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
 *   This source code describes the implementation of the interfaces for CEcoLab2_86AB2353
 *   Схема рис. 3:
 *     IEcoLab2            - собственный интерфейс;
 *     IEcoSortAndSearch  - агрегирование компонента Eco.Lab1;
 *     IEcoCalculatorY     - включение компонента E;
 *     IEcoCalculatorX     - включение компонента E (внутри E это агрегированный компонент B).
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
#include "CEcoLab2.h"
#include "ErrEcoLab2.h"
#include "IdEcoSortAndSearch.h"
#include "IdEcoCalculatorE.h"

/* Указатель на экземпляр по указателю на интерфейс, хранящийся в поле с номером index */
#define CEcoLab2_86AB2353_FromInterface(me, index) \
    ((CEcoLab2_86AB2353*)((char_t*)(me) - (index) * sizeof(voidptr_t)))

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   Общая реализация QueryInterface для всех интерфейсов компонента
 * </description>
 *
 */
static int16_t CEcoLab2_86AB2353_QueryInterfaceImpl(/* in */ CEcoLab2_86AB2353* pCMe, /* in */ const UGUID* riid, /* out */ void** ppv) {

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoLab2) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab2;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pIX != 0 ) {
        /* Включение: собственная таблица, методы которой вызывают компонент E */
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) && pCMe->m_pIY != 0 ) {
        /* Включение: собственная таблица, методы которой вызывают компонент E */
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSortAndSearch) && pCMe->m_pIUnkInnerLab1 != 0 ) {
        /* Агрегирование: указатель на интерфейс отдает сам внутренний компонент Eco.Lab1 */
        return pCMe->m_pIUnkInnerLab1->pVTbl->QueryInterface(pCMe->m_pIUnkInnerLab1, riid, ppv);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    ++pCMe->m_cRef;

    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Общая реализация Release для всех интерфейсов компонента
 * </description>
 *
 */
static uint32_t CEcoLab2_86AB2353_ReleaseImpl(/* in */ CEcoLab2_86AB2353* pCMe) {

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
 *   QueryInterface function for the IEcoLab2 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_QueryInterface(/* in */ IEcoLab2Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return CEcoLab2_86AB2353_QueryInterfaceImpl(CEcoLab2_86AB2353_FromInterface(me, 0), riid, ppv);
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoLab2 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_AddRef(/* in */ IEcoLab2Ptr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++CEcoLab2_86AB2353_FromInterface(me, 0)->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoLab2 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_Release(/* in */ IEcoLab2Ptr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return CEcoLab2_86AB2353_ReleaseImpl(CEcoLab2_86AB2353_FromInterface(me, 0));
}

/*
 *
 * <summary>
 *   Calculate Function
 * </summary>
 *
 * <description>
 *   Вычисление a op b через включенные интерфейсы IEcoCalculatorX и IEcoCalculatorY
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_Calculate(/* in */ IEcoLab2Ptr_t me, /* in */ char_t op, /* in */ int16_t a, /* in */ int16_t b, /* out */ int32_t* pResult) {
    CEcoLab2_86AB2353* pCMe = CEcoLab2_86AB2353_FromInterface(me, 0);

    /* Pointer Validation */
    if (me == 0 || pResult == 0) {
        return ERR_ECO_POINTER;
    }

    if (op == '+' || op == '-') {
        if (pCMe->m_pIX == 0) {
            return ERR_ECO_NOINTERFACE;
        }
        if (op == '+') {
            *pResult = pCMe->m_pIX->pVTbl->Addition(pCMe->m_pIX, a, b);
        } else {
            *pResult = pCMe->m_pIX->pVTbl->Subtraction(pCMe->m_pIX, a, b);
        }
    }
    else if (op == '*' || op == '/') {
        if (pCMe->m_pIY == 0) {
            return ERR_ECO_NOINTERFACE;
        }
        if (op == '*') {
            *pResult = pCMe->m_pIY->pVTbl->Multiplication(pCMe->m_pIY, a, b);
        } else {
            if (b == 0) {
                return ERR_ECOLAB2_DIVISION_BY_ZERO;
            }
            *pResult = pCMe->m_pIY->pVTbl->Division(pCMe->m_pIY, a, b);
        }
    }
    else {
        return ERR_ECOLAB2_UNKNOWN_OPERATION;
    }

    return ERR_ECO_SUCCESS;
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
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorX_QueryInterface(/* in */ IEcoCalculatorXPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return CEcoLab2_86AB2353_QueryInterfaceImpl(CEcoLab2_86AB2353_FromInterface(me, 1), riid, ppv);
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
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorX_AddRef(/* in */ IEcoCalculatorXPtr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++CEcoLab2_86AB2353_FromInterface(me, 1)->m_cRef;
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
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorX_Release(/* in */ IEcoCalculatorXPtr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return CEcoLab2_86AB2353_ReleaseImpl(CEcoLab2_86AB2353_FromInterface(me, 1));
}

/*
 *
 * <summary>
 *   Addition Function
 * </summary>
 *
 * <description>
 *   Включение: вызов метода компонента E
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoLab2_86AB2353_Addition(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_86AB2353* pCMe = CEcoLab2_86AB2353_FromInterface(me, 1);

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
 *   Включение: вызов метода компонента E
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_Subtraction(/* in */ IEcoCalculatorXPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_86AB2353* pCMe = CEcoLab2_86AB2353_FromInterface(me, 1);

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
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorY_QueryInterface(/* in */ IEcoCalculatorYPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    return CEcoLab2_86AB2353_QueryInterfaceImpl(CEcoLab2_86AB2353_FromInterface(me, 2), riid, ppv);
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
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorY_AddRef(/* in */ IEcoCalculatorYPtr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++CEcoLab2_86AB2353_FromInterface(me, 2)->m_cRef;
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
static uint32_t ECOCALLMETHOD CEcoLab2_86AB2353_IEcoCalculatorY_Release(/* in */ IEcoCalculatorYPtr_t me) {

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return CEcoLab2_86AB2353_ReleaseImpl(CEcoLab2_86AB2353_FromInterface(me, 2));
}

/*
 *
 * <summary>
 *   Multiplication Function
 * </summary>
 *
 * <description>
 *   Включение: вызов метода компонента E
 * </description>
 *
 */
static int32_t ECOCALLMETHOD CEcoLab2_86AB2353_Multiplication(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_86AB2353* pCMe = CEcoLab2_86AB2353_FromInterface(me, 2);

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
 *   Включение: внешний компонент проверяет аргументы до вызова компонента E
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab2_86AB2353_Division(/* in */ IEcoCalculatorYPtr_t me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab2_86AB2353* pCMe = CEcoLab2_86AB2353_FromInterface(me, 2);

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
static int16_t ECOCALLMETHOD initCEcoLab2_86AB2353(/*in*/ CEcoLab2_86AB2353Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoLab2_86AB2353* pCMe = (CEcoLab2_86AB2353*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    /* Управляющий IEcoUnknown для агрегируемого компонента - собственный IEcoLab2 */
    IEcoUnknown* pOuterUnknown = (IEcoUnknown*)&pCMe->m_pVTblIEcoLab2;
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

    /* Рис. 3: создание внутреннего компонента Eco.Lab1 с агрегированием */
    /* ВАЖНО: при агрегировании передается управляющий IEcoUnknown и запрашивается IID_IEcoUnknown */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoSortAndSearch, pOuterUnknown, &IID_IEcoUnknown, (void**) &pCMe->m_pIUnkInnerLab1);
    if (result != 0) {
        pCMe->m_pIUnkInnerLab1 = 0;
    }

    /* Рис. 3: включение компонента E */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
    if (result == 0 && pCMe->m_pIY != 0) {
        /* IEcoCalculatorX запрашивается у E: его отдает агрегированный внутри E компонент B */
        result = pCMe->m_pIY->pVTbl->QueryInterface(pCMe->m_pIY, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
        if (result != 0) {
            pCMe->m_pIX = 0;
        }
    }
    else {
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
static int16_t ECOCALLMETHOD createCEcoLab2_86AB2353(/* in */ CEcoLab2_86AB2353Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {

    /* Pointer Validation */
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_pIUnkInnerLab1 = 0;
    pCMe->m_pIY = 0;
    pCMe->m_pIX = 0;

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
static void ECOCALLMETHOD deleteCEcoLab2_86AB2353(/* in */ CEcoLab2_86AB2353Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIUnkInnerLab1 != 0 ) {
            /* Агрегированный компонент освобождается через его неделегирующий IEcoUnknown */
            pCMe->m_pIUnkInnerLab1->pVTbl->Release(pCMe->m_pIUnkInnerLab1);
            pCMe->m_pIUnkInnerLab1 = 0;
        }
        if ( pCMe->m_pIX != 0 ) {
            pCMe->m_pIX->pVTbl->Release(pCMe->m_pIX);
            pCMe->m_pIX = 0;
        }
        if ( pCMe->m_pIY != 0 ) {
            pCMe->m_pIY->pVTbl->Release(pCMe->m_pIY);
            pCMe->m_pIY = 0;
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

/* IEcoLab2 Virtual Table */
IEcoLab2VTbl g_x37E3CBC5B953434B946C86319278A5ABVTbl_86AB2353 = {
    CEcoLab2_86AB2353_QueryInterface,
    CEcoLab2_86AB2353_AddRef,
    CEcoLab2_86AB2353_Release,
    CEcoLab2_86AB2353_Calculate
};

/* IEcoCalculatorX Virtual Table */
IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTbl_86AB2353 = {
    CEcoLab2_86AB2353_IEcoCalculatorX_QueryInterface,
    CEcoLab2_86AB2353_IEcoCalculatorX_AddRef,
    CEcoLab2_86AB2353_IEcoCalculatorX_Release,
    CEcoLab2_86AB2353_Addition,
    CEcoLab2_86AB2353_Subtraction
};

/* IEcoCalculatorY Virtual Table */
IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTbl_86AB2353 = {
    CEcoLab2_86AB2353_IEcoCalculatorY_QueryInterface,
    CEcoLab2_86AB2353_IEcoCalculatorY_AddRef,
    CEcoLab2_86AB2353_IEcoCalculatorY_Release,
    CEcoLab2_86AB2353_Multiplication,
    CEcoLab2_86AB2353_Division
};


/* Object Instance */
CEcoLab2_86AB2353 g_xCEcoLab2_86AB2353 = {
    &g_x37E3CBC5B953434B946C86319278A5ABVTbl_86AB2353,
    &g_x9322111622484742AE0682819447843DVTbl_86AB2353,
    &g_xBD6414C29096423EA90C04D77AFD1CADVTbl_86AB2353,
    initCEcoLab2_86AB2353,
    createCEcoLab2_86AB2353,
    deleteCEcoLab2_86AB2353,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_pIUnkInnerLab1 */
    0, /* m_pIY */
    0  /* m_pIX */
};
