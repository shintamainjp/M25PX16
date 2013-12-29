/**
 * @file m25p16.h
 * @author Shinichiro Nakamura
 */

/*
 * ===============================================================
 *  BlueBoot - Blackfin SPI Flash Writer & Blackfin Boot Loader
 * ===============================================================
 * Copyright (c) 2013 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#ifndef M25P16_H
#define M25P16_H

#include <stdint.h>

#define M25P16_PAGE_COUNT       (8192)
#define M25P16_PAGE_BYTE_SIZE   (256)
#define M25P16_SECTOR_COUNT     (32)
#define M25P16_SECTOR_BYTE_SIZE (65536)

typedef struct {
  uint8_t manufacturer;
  uint8_t memory_type;
  uint8_t memory_capacity;
  uint8_t cfd_length;
  uint8_t cfd_content[16];
} m25p16_identification_t;

/**
 * @brief Write Protect.
 * @details
 * The status register write disable (SRWD) bit is operated in conjunction with the write protect (W#/VPP) signal.
 * When the SRWD bit is set to 1 and W#/VPP is driven LOW, the device is put in the hardware protected mode.
 * In the hardware protected mode, the non-volatile bits of the status register (SRWD, and the block protect bits)
 * become readonly bits and the WRITE STATUS REGISTER command is no longer accepted for execution.
 */
#define M25P16_SREG_WRITE_PROTECT(SREG)         ((SREG) & (1 << 7))

/**
 * @brief Block Protect 2.
 * @details
 * The block protect bits are non-volatile.
 * They define the size of the area to be software protected against PROGRAM and ERASE commands.
 * The block protect bits are written with the WRITE STATUS REGISTER command.
 * When one or more of the block protect bits is set to 1, the relevant memory area,
 * as defined in the Protected Area Sizes table, becomes protected against PAGE PROGRAM and SECTOR ERASE commands.
 * The block protect bits can be written provided that the HARDWARE PROTECTED mode has not been set.
 * The BULK ERASE command is executed only if all block protect bits are 0.
 */
#define M25P16_SREG_BLOCK_PROTECT_BP2(SREG)     ((SREG) & (1 << 4))

/**
 * @brief Block Protect 1.
 * @details
 * The block protect bits are non-volatile.
 * They define the size of the area to be software protected against PROGRAM and ERASE commands.
 * The block protect bits are written with the WRITE STATUS REGISTER command.
 * When one or more of the block protect bits is set to 1, the relevant memory area,
 * as defined in the Protected Area Sizes table, becomes protected against PAGE PROGRAM and SECTOR ERASE commands.
 * The block protect bits can be written provided that the HARDWARE PROTECTED mode has not been set.
 * The BULK ERASE command is executed only if all block protect bits are 0.
 */
#define M25P16_SREG_BLOCK_PROTECT_BP1(SREG)     ((SREG) & (1 << 3))

/**
 * @brief Block Protect 0.
 * @details
 * The block protect bits are non-volatile.
 * They define the size of the area to be software protected against PROGRAM and ERASE commands.
 * The block protect bits are written with the WRITE STATUS REGISTER command.
 * When one or more of the block protect bits is set to 1, the relevant memory area,
 * as defined in the Protected Area Sizes table, becomes protected against PAGE PROGRAM and SECTOR ERASE commands.
 * The block protect bits can be written provided that the HARDWARE PROTECTED mode has not been set.
 * The BULK ERASE command is executed only if all block protect bits are 0.
 */
#define M25P16_SREG_BLOCK_PROTECT_BP0(SREG)     ((SREG) & (1 << 2))

/**
 * @brief Write Enable Latch.
 * @details
 * The write enable latch (WEL) bit indicates the status of the internal write enable latch.
 * When the WEL bit is set to 1, the internal write enable latch is set; when the WEL bit is
 * set to 0, the internal write enable latch is reset and no WRITE STATUS REGISTER, PROGRAM, or ERASE command is accepted.
 */
#define M25P16_SREG_WRITE_ENABLE_LATCH(SREG)    ((SREG) & (1 << 1))

/**
 * @brief Write In Progress Flag.
 * @details
 * The write in progress (WIP) bit indicates whether the memory is busy with a WRITE
 * STATUS REGISTER cycle, a PROGRAM cycle, or an ERASE cycle. When the WIP bit is set
 * to 1, a cycle is in progress; when the WIP bit is set to 0, a cycle is not in progress.
 */
#define M25P16_SREG_WRITE_IN_PROGRESS(SREG)     ((SREG) & (1 << 0))

void m25p16_init(void);
void m25p16_write_enable(void);
void m25p16_write_disable(void);
void m25p16_read_identification(m25p16_identification_t *p);
void m25p16_read_status_register(uint8_t *sreg);
void m25p16_write_status_register(uint8_t sreg);
void m25p16_read_data_bytes(uint32_t addr, uint8_t *buf, uint32_t siz);
void m25p16_page_program(uint32_t addr, uint8_t *buf, uint32_t siz);
void m25p16_sector_erase(uint32_t addr);
void m25p16_bulk_erase(void);
void m25p16_deep_power_down(void);
void m25p16_release_from_deep_power_down(void);

#endif

