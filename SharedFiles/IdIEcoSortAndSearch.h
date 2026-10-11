/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdIEcoSortAndSearch
 * </summary>
 *
 * <description>
 *   This header describes the interface IdIEcoSortAndSearch
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

#ifndef __ID_IECOSORTANDSEARCH_H__
#define __ID_IECOSORTANDSEARCH_H__

#include "IEcoBase1.h"
#include "IIEcoSortAndSearch.h"

/* IEcoSortAndSearch CID = {591FC2D6-783F-43F0-B5B5-65CCCAF05939} */
#ifndef __CID_IEcoSortAndSearch
static const UGUID CID_IEcoSortAndSearch = {0x01, 0x10, {0x59, 0x1F, 0xC2, 0xD6, 0x78, 0x3F, 0x43, 0xF0, 0xB5, 0xB5, 0x65, 0xCC, 0xCA, 0xF0, 0x59, 0x39}};
#endif /* __CID_IEcoSortAndSearch */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_591FC2D6783F43F0B5B565CCCAF05939;
#endif

#endif /* __ID_IECOSORTANDSEARCH_H__ */

