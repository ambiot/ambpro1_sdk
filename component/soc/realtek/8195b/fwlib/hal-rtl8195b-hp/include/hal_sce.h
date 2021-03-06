/**************************************************************************//**
 * @file     hal_sce.h
 * @brief    This file implements the entry functions of the HAL API for SCE
 *           ROM functions.
 *
 * @version  V1.00
 * @date     2017-07-31
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef _HAL_SCE_H_
#define _HAL_SCE_H_
#include "cmsis.h"

/** @addtogroup hs_hal_sce
  * @{
  */

#ifdef  __cplusplus
 extern "C"
 {
#endif

#if (CHIP_VER == CHIP_A_CUT) && (defined(CONFIG_BUILD_RAM))
hal_status_t hal_sce_flash_remap_rtl8195bhp_patch (uint32_t phy_addr, uint32_t vir_addr, uint32_t map_size,
                                             sce_page_size_t page_size_sel, uint8_t secure_en,
                                             sce_block_size_t block_size, uint8_t key_id);
#endif

/**
  * @brief The global variable for the SCE HAL common resource management.
  */
extern const hal_sce_func_stubs_t hal_sce_stubs;

#if !defined(CONFIG_BUILD_NONSECURE)

/**
 *  @brief Writes a SCE register with a given address offset.
 *         The SCE registers writing is via indirect access method.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *              has 2 SCE hardwares. One works with the flash memory, the other one works
 *              with external SRAM(PSRAM) or DRAM(LPDDR).
 *                - SCE_TYPE_FLASH: flash memory.
 *                - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: LPDDR DRAM/Pseudo SRAM.
 *  @param[in]  addr  the SCE register address.
 *  @param[in]  value  the value to be written to the given SCE register.
 *
 *  @returns    void
 */
__STATIC_INLINE
void hal_sce_write_reg (uint8_t memory_type, uint32_t addr, uint32_t value)
{
    hal_sce_stubs.hal_sce_write_reg (memory_type, addr, value);
}

/**
 *  @brief Reads a given SCE register. The SCE registers reading is via indirect access method.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *              has 2 SCE hardwares. One works with the flash memory, the other one works
 *              with external SRAM(PSRAM) or DRAM(LPDDR). This argument is used to specify the SCE
 *              hardware.
 *                - SCE_TYPE_FLASH: flash memory.
 *                - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: LPDDR DRAM/Pseudo SRAM.
 *  @param[in]  addr  the SCE register address to read.
 *
 *  @returns    the read register value.
 */
__STATIC_INLINE
uint32_t hal_sce_read_reg (uint8_t memory_type, uint32_t addr)
{
    return hal_sce_stubs.hal_sce_read_reg (memory_type, addr);
}


/**
 *  @brief Allocates a free SCE section. A SCE section is used to configure a contiguous of memory region
 *         for the data encryption or decryption on the fly, or to do the flash memory re-mapping.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *
 *  @returns    The index of the allocated section. If this value > SCE_MAX_SECTION_ID then it means the section
 *              allocation is failed (no free section).
 */
__STATIC_INLINE
uint8_t hal_sce_comm_alloc_section (sce_mem_type_t memory_type)
{
    return hal_sce_stubs.hal_sce_comm_alloc_section (memory_type);
}

/**
 *  @brief Frees a SCE section.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *  @param[in]  sec_id The index of the section to be freed.
 *
 *  @returns    void.
 */
__STATIC_INLINE
void hal_sce_comm_free_section (sce_mem_type_t memory_type, uint8_t sec_id)
{
    hal_sce_stubs.hal_sce_comm_free_section (memory_type, sec_id);
}

/**
 *  @brief Checks if the specified key of the SCE hardware is configured or not.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *  @param[in]  pair_id The key index. A SCE hardware support up to 2 pairs of key and IV.
 *                            - 0: key pair 0.
 *                            - 1: key pair 1.
 *
 *  @return     true: The specified key is configured and valid.
 *  @return     false: The specified key is not valid.
 */
__STATIC_INLINE
BOOLEAN hal_sce_comm_key_valid (sce_mem_type_t memory_type, uint8_t pair_id)
{
    return hal_sce_stubs.hal_sce_comm_key_valid (memory_type, pair_id);
}

/**
 *  @brief Configure the SCE setting and enables the SCE memory access path.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *  @param[in]  page_size The page size be used to define a memory section for the SCE hardware operation.
 *  @param[in]  block_size The memory block size for the SCE to do the data encryption or decryption.
 *  @param[in]  rw_mode Supported memory access mode for the SCE to access the memory.
 *
 *  @return     HAL_ERR_PARA:  Input memory type is invalid.
 *  @return     HAL_OK:  The SCE configuration is OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_enable (sce_mem_type_t memory_type, sce_page_size_t page_size, sce_block_size_t block_size,
                             sce_mode_select_t rw_mode)
{
    return hal_sce_stubs.hal_sce_enable (memory_type, page_size, block_size, rw_mode);
}

/**
 *  @brief Disable the SCE memory access path(memory access bypass the SCE).
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @return     HAL_ERR_PARA:  Input memory type is invalid.
 *  @return     HAL_OK:  SCE memory access path disable OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_disable (sce_mem_type_t memory_type)
{
    return hal_sce_stubs.hal_sce_disable (memory_type);
}

/**
 *  @brief Modify the SCE configuration.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *  @param[in]  page_size The page size be used to define a memory section for the SCE hardware operation.
 *  @param[in]  block_size The memory block size for the SCE to do the data encryption or decryption.
 *  @param[in]  rw_mode Supported memory access mode for the SCE to access the memory.
 *
 *  @return     HAL_ERR_PARA:  Input memory type or the memory access type is invalid.
 *  @return     HAL_OK:  The SCE configuration is OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_cfg (sce_mem_type_t memory_type, sce_page_size_t page_size,
                          sce_block_size_t block_size, sce_mode_select_t rw_mode)
{
    return hal_sce_stubs.hal_sce_cfg (memory_type, page_size, block_size, rw_mode);
}

/**
 *  @brief Setups the kay data of the specified SCE key pair. A key pair includes a 16 bytes
 *         of key data and a 16 bytes of initial value.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  pair_id The index of the key pair. Since a SCE has 2 key pairs, so the valid value
 *                      is 0 and 1.
 *  @param[in]  key The 16 bytes key data.
 *
 *  @return     void.
 */
__STATIC_INLINE
void hal_sce_set_key (sce_mem_type_t memory_type, uint8_t pair_id, uint8_t *key)
{
    hal_sce_stubs.hal_sce_set_key (memory_type, pair_id, key);
}

/**
 *  @brief Setups the IV(initial value) data of the specified SCE key pair. A key pair includes a 16 bytes
 *         of key data and a 16 bytes of initial value.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  pair_id The index of the key pair. Since a SCE has 2 key pairs, so the valid value
 *                      is 0 and 1.
 *  @param[in]  iv The 16 bytes IV data. This buffer must be a 4-bytes aligned buffer.
 *
 *  @return     void.
 */
__STATIC_INLINE
void hal_sce_set_iv (sce_mem_type_t memory_type, uint8_t pair_id, uint8_t *iv)
{
    hal_sce_stubs.hal_sce_set_iv (memory_type, pair_id, iv);
}

/**
 *  @brief Querys the SCE key pair index by the given key data and IV data.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  key The 16 bytes key data.
 *  @param[in]  iv The 16 bytes IV data.
 *
 *  @return     The key pair index. If no key data and IV data matched key pair is found return 0xFF.
 */
__STATIC_INLINE
uint8_t hal_sce_key_pair_search (sce_mem_type_t memory_type, uint8_t *key, uint8_t *iv)
{
    return hal_sce_stubs.hal_sce_key_pair_search (memory_type, key, iv);
}

/**
 *  @brief Setups the kay data and the IV data of the specified SCE key pair.
 *         A key pair includes a 16 bytes of key data and a 16 bytes of initial value.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  pair_id The index of the key pair. Since a SCE has 2 key pairs, so the valid value
 *                      is 0 and 1.
 *  @param[in]  key The 16 bytes key data.
 *  @param[in]  iv The 16 bytes IV data. This buffer must be a 4-bytes aligned buffer.
 *
 *  @return     void.
 */
__STATIC_INLINE
void hal_sce_set_key_pair (sce_mem_type_t memory_type, uint8_t pair_id, uint8_t *key, uint8_t *iv)
{
    hal_sce_stubs.hal_sce_set_key_pair (memory_type, pair_id, key, iv);
}

/**
 *  @brief Reads the current kay data and IV data of the specified SCE key pair.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  pair_id The index of the key pair. Since a SCE has 2 key pairs, so the valid value
 *                      is 0 and 1.
 *  @param[out]  key The buffer for the 16 bytes key data.
 *  @param[out]  iv The buffer for the 16 bytes IV data. This buffer must be a 4-bytes aligned buffer.
 *
 *  @return     void.
 */
__STATIC_INLINE
void hal_sce_read_key_pair (sce_mem_type_t memory_type, uint8_t pair_id, uint8_t *key, uint8_t *iv)
{
    hal_sce_stubs.hal_sce_read_key_pair (memory_type, pair_id, key, iv);
}

/**
 *  @brief Configure a SCE section for memory data encryption/decryption or remapping.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  sec_id The index of the SCE section to be configured. Since a SCE hardware support up to 8
 *                     sections so the valid value is 0 ~ 7.
 *  @param[in]  pair_id The index of the key pair to be used for the memory data encryption/decryption.
 *  @param[in]  start_address The start address of the section. This address must be aligned to a page boundary.
 *                            Ex. the page size is configured as 16K bytes then this address must be aligned
 *                            to a 16K address.
 *  @param[in]  end_address The end address of the section.
 *  @param[in]  secure_en Controls the data encryption/decryption enable on the memory of this section.
 *                          - 0: Data encryption/decryption is disabled.
 *                          - 1: Data encryption/decryption is enabled.
 *
 *  @return     HAL_ERR_PARA:  One or more input argument is invalid.
 *  @return     HAL_OK:  SCE section configure OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_set_section (sce_mem_type_t memory_type, uint8_t sec_id, uint8_t pair_id,
                                  uint32_t start_address, uint32_t end_address, uint8_t secure_en)
{
    return hal_sce_stubs.hal_sce_set_section (memory_type, sec_id, pair_id, start_address, end_address, secure_en);
}

/**
 *  @brief Enables the SCE hardware function. It enable the clock for the SCE hardware and make
 *         the memory access path through the SCE hardware.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @return     HAL_ERR_PARA:  Input memory type is invalid.
 *  @return     HAL_OK:  SCE enable OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_func_enable (sce_mem_type_t memory_type)
{
    return hal_sce_stubs.hal_sce_func_enable (memory_type);
}

/**
 *  @brief Disable the SCE hardware function. It disable the clock for the SCE hardware and make
 *         the memory access path bypass the SCE hardware.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @return     HAL_ERR_PARA:  Input memory type is invalid.
 *  @return     HAL_OK:  SCE disable OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_func_disable (sce_mem_type_t memory_type)
{
    return hal_sce_stubs.hal_sce_func_disable (memory_type);
}

/**
 *  @brief Enable a SCE section memory remapping. The SCE section should be configured before calling
 *         of this function. This memory remapping should be applied to the flash memory only.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  sec_id The index of the SCE section for the memory remapping. The valid value is 0 ~ 7.
 *  @param[in]  remap_addr The virtual address of the memory remapping. The SCE section start address will
 *                         be mapped to this address. This address must be aligned to the configured page boundary.
 *
 *  @return     HAL_ERR_PARA:  Incorrect memory type or invalid section index.
 *  @return     HAL_OK:  Memory remapping enable OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_remap_enable (sce_mem_type_t memory_type, uint8_t sec_id, uint32_t remap_addr)
{
    return hal_sce_stubs.hal_sce_remap_enable (memory_type, sec_id, remap_addr);
}

/**
 *  @brief Disable the specified SCE section. The memory data encryption/decryption or remapping which is configured
 *         by this section is disabled too.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  sec_id The index of the SCE section to be disabled. The valid value is 0 ~ 7.
 *
 *  @return     void.
 */
__STATIC_INLINE
void hal_sce_section_disable (sce_mem_type_t memory_type, uint8_t sec_id)
{
    hal_sce_stubs.hal_sce_section_disable (memory_type, sec_id);
}

/**
 *  @brief Configure the SCE hardware to implements a flash memory remapping. The memory data
 *         decryption can be applied to the same flash memory range optionally.
 *
 *  @param[in]  phy_addr The physical address of the flash memory to do the remapping. This address must align
 *                       to the configured page boundary. Ex. if the page size is configured as 16K byes then this
 *                       address must align to 16K boundary address.
 *
 *  @param[in]  vir_addr The virtual address for the remapping. The physical address will be mapped to this address.
 *                       This address must align to a page boundary address.
 *  @param[in]  map_size The memory size will be applied to the remapping.
 *  @param[in]  page_size_sel Selects the page size.
 *                              - 0: 16K bytes.
 *                              - 1: 32K bytes.
 *                              - 2: 64K bytes.
 *  @param[in]  secure_en Controls the memory data decryption enable.
 *                          - 0: Data decryption function is off.
 *                          - 1: Data decryption function is on.
 *  @param[in]  block_size Selects the page size for the data decryption.
 *                         The data in the same page will be decrypted with the same key.
 *                          - 0: 32 bytes.
 *                          - 1: 64 bytes.
 *                          - 2: 128 bytes.
 *                          - 3: 256 bytes.
 *  @param[in]  key_id The index of the key pair to be used for the flash memory data decryption.
 *                     If the data decryption is disabled, this key idex is ignored. The valid value is
 *                     0 or 1. The specified key pair should be configured before the calling of this function.
 *
 *  @return     HAL_ERR_PARA:  One or more input argument is invalid.
 *  @return     HAL_NO_RESOURCE: Not enougth of SEC sections to do this remapping.
 *  @return     HAL_OK:  Flash memory remapping is OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_flash_remap (uint32_t phy_addr, uint32_t vir_addr, uint32_t map_size,
                                 sce_page_size_t page_size_sel, uint8_t secure_en,
                                 sce_block_size_t block_size, uint8_t key_id)
{
#if (CHIP_VER == CHIP_A_CUT) && (defined(CONFIG_BUILD_RAM))
    return hal_sce_flash_remap_rtl8195bhp_patch (phy_addr, vir_addr, map_size, page_size_sel,
                                                    secure_en, block_size, key_id);
#else
    return hal_sce_stubs.hal_sce_flash_remap (phy_addr, vir_addr, map_size, page_size_sel,
                                                    secure_en, block_size, key_id);
#endif
}

/**
 *  @brief Configure the SCE hardware for a range of memory data decryption/encryption.
 *
 *  @param[in]  memory_type The memory type for this SCE HW is working with. This platform
 *                          has 2 SCE hardwares. One works with the flash memory, the other one works
 *                          with external SRAM(PSRAM) or DRAM(LPDDR).
 *                            - SCE_TYPE_FLASH: the SCE for the flash memory.
 *                            - SCE_TYPE_LPDDR/SCE_TYPE_PSRAM: the SCE for the external RAM.
 *                          This value is used to specify the SCE hardware.
 *
 *  @param[in]  start_addr The start address of the memory to do the data encryption/decryption.
 *                         This address must align to the configured page boundary. Ex. if the page size is
 *                         configured as 16K byes then this address must align to 16K boundary address.
 *
 *  @param[in]  mem_size The memory size will be applied to the data encryption/decryption.
 *  @param[in]  key_id The index of the key pair to be used for the memory data encryption/decryption.
 *                     The valid value is 0 or 1. The specified key pair should be configured before
 *                     the calling of this function.
 *
 *  @return     HAL_ERR_PARA:  One or more input argument is invalid.
 *  @return     HAL_NO_RESOURCE: Not enougth of SEC sections to do this remapping.
 *  @return     HAL_OK:  Flash memory remapping is OK.
 */
__STATIC_INLINE
hal_status_t hal_sce_set_mem_crypto (sce_mem_type_t memory_type, uint32_t start_addr,
                                     uint32_t mem_size, uint8_t key_id)
{
    return hal_sce_stubs.hal_sce_set_mem_crypto (memory_type, start_addr, mem_size, key_id);
}

__STATIC_INLINE
void hal_sce_reg_dump (uint32_t type)
{
    hal_sce_stubs.hal_sce_reg_dump (type);
}

#endif  // #if defined(CONFIG_BUILD_SECURE)

#ifdef  __cplusplus
 }
#endif


#endif  // end of "#define _HAL_SCE_H_"
/** @} */ /* End of group hs_hal_sce */

