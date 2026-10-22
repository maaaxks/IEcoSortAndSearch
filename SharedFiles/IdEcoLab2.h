/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab2
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab2
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

#ifndef __ID_ECOLAB2_H__
#define __ID_ECOLAB2_H__

#include "IEcoBase1.h"
#include "IEcoLab2.h"

/* EcoLab2 CID = {3FDF228D-0701-437F-AF2A-696A86AB2353} */
#ifndef __CID_EcoLab2
static const UGUID CID_EcoLab2 = {0x01, 0x10, {0x3F, 0xDF, 0x22, 0x8D, 0x07, 0x01, 0x43, 0x7F, 0xAF, 0x2A, 0x69, 0x6A, 0x86, 0xAB, 0x23, 0x53}};
#endif /* __CID_EcoLab2 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_3FDF228D0701437FAF2A696A86AB2353;
#endif

#endif /* __ID_ECOLAB2_H__ */
