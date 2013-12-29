/**
 * @file m25px16.c
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

#include "m25px16.h"

#define SPI_INIT()      /* Your codes */
#define SPI_ASSERT()    /* Your codes */
#define SPI_TRANSMIT(D) /* Your codes */
#define SPI_DEASSERT()  /* Your codes */

#define CMD_WRITE_ENABLE                    (0x06)
#define CMD_WRITE_DISABLE                   (0x04)
#define CMD_READ_IDENTIFICATION             (0x9F)
#define CMD_READ_STATUS_REGISTER            (0x05)
#define CMD_WRITE_STATUS_REGISTER           (0x01)
#define CMD_WRITE_LOCK_REGISTER             (0xE5)
#define CMD_READ_LOCK_REGISTER              (0xE8)
#define CMD_READ_DATA_BYTES                 (0x03)
#define CMD_PAGE_PROGRAM                    (0x02)
#define CMD_SECTOR_ERASE                    (0xD8)
#define CMD_BULK_ERASE                      (0xC7)
#define CMD_DEEP_POWER_DOWN                 (0xB9)
#define CMD_RELEASE_FROM_DEEP_POWER_DOWN    (0xAB)

void m25px16_init(void)
{
  SPI_INIT();
}

/**
 * @brief Write Enable.
 * @details
 * The Write Enable (WREN) instruction sets the Write Enable Latch (WEL) bit.
 * The Write Enable Latch (WEL) bit must be set prior to every Page Program (PP), Dual Input
 * Fast Program (DIFP), Program OTP (POTP), Write to Lock Register (WRLR), Subsector
 * Erase (SSE), Sector Erase (SE), Bulk Erase (BE) and Write Status Register (WRSR)
 * instruction.
 * The Write Enable (WREN) instruction is entered by driving Chip Select (S) Low, sending the
 * instruction code, and then driving Chip Select (S) High.
 */
void m25px16_write_enable(void)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_WRITE_ENABLE);
  SPI_DEASSERT();
}

/**
 * @brief Write Disable.
 * @details
 * The Write Disable (WRDI) instruction resets the Write Enable Latch (WEL) bit.
 * The Write Disable (WRDI) instruction is entered by driving Chip Select (S) Low, sending the
 * instruction code, and then driving Chip Select (S) High.
 * The Write Enable Latch (WEL) bit is reset under the following conditions:
 * - Power-up
 * - Write Disable (WRDI) instruction completion
 * - Write Status Register (WRSR) instruction completion
 * - Write lo Lock Register (WRLR) instruction completion
 * - Page Program (PP) instruction completion
 * - Dual Input Fast Program (DIFP) instruction completion
 * - Program OTP (POTP) instruction completion
 * - Subsector Erase (SSE) instruction completion
 * - Sector Erase (SE) instruction completion
 * - Bulk Erase (BE) instruction completion
 */
void m25px16_write_disable(void)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_WRITE_DISABLE);
  SPI_DEASSERT();
}

/**
 * @brief Read Identification.
 * @details
 * The Read Identification (RDID) instruction allows to read the device identification data:
 * - Manufacturer identification (1 byte)
 * - Device identification (2 bytes)
 * - A Unique ID code (UID) (17 bytes, of which 16 available upon customer request).
 * The manufacturer identification is assigned by JEDEC, and has the value 20h. The device
 * identification is assigned by the device manufacturer, and indicates the memory type in the
 * first byte (71h), and the memory capacity of the device in the second byte (15h). The UID
 * contains the length of the following data in the first byte (set to 10h) and 16 bytes of the
 * optional Customized Factory Data (CFD) content. The CFD bytes are read-only and can be
 * programmed with customers data upon their demand. If the customers do not make
 * requests, the devices are shipped with all the CFD bytes programmed to zero (00h).
 */
void m25px16_read_identification(m25px16_identification_t *p)
{
  int i;
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_READ_IDENTIFICATION);
  p->manufacturer = SPI_TRANSMIT(0);
  p->memory_type = SPI_TRANSMIT(0);
  p->memory_capacity = SPI_TRANSMIT(0);
  p->cfd_length = SPI_TRANSMIT(0);
  for (i = 0; i < p->cfd_length; i++) {
    p->cfd_content[i] = SPI_TRANSMIT(0);
  }
  SPI_DEASSERT();
}

/**
 * @brief Read Status Register.
 * @details
 * The Read Status Register (RDSR) instruction allows the Status Register to be read.
 * The Status Register may be read at any time, even while a Program, Erase or Write Status
 * Register cycle is in progress. When one of these cycles is in progress, it is recommended to
 * check the Write In Progress (WIP) bit before sending a new instruction to the device.
 * It is also possible to read the Status Register continuously.
 */
void m25px16_read_status_register(uint8_t *sreg)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_READ_STATUS_REGISTER);
  *sreg = SPI_TRANSMIT(0);
  SPI_DEASSERT();
}

/**
 * @brief Write Status Register.
 * @details
 * The Write Status Register (WRSR) instruction allows new values to be written to the Status
 * Register. Before it can be accepted, a Write Enable (WREN) instruction must previously
 * have been executed. After the Write Enable (WREN) instruction has been decoded and
 * executed, the device sets the Write Enable Latch (WEL).
 * The Write Status Register (WRSR) instruction is entered by driving Chip Select (S) Low,
 * followed by the instruction code and the data byte on Serial Data input (DQ0).
 * The Write Status Register (WRSR) instruction has no effect on b6, b1 and b0 of the Status
 * Register. b6 is always read as ‘0’.
 * Chip Select (S) must be driven High after the eighth bit of the data byte has been latched in.
 * If not, the Write Status Register (WRSR) instruction is not executed. As soon as Chip Select
 * (S) is driven High, the self-timed Write Status Register cycle (whose duration is tW) is
 * initiated. While the Write Status Register cycle is in progress, the Status Register may still
 * be read to check the value of the Write In Progress (WIP) bit. The Write In Progress (WIP)
 * bit is 1 during the self-timed Write Status Register cycle, and is 0 when it is completed.
 * When the cycle is completed, the Write Enable Latch (WEL) is reset.
 * The Write Status Register (WRSR) instruction allows the user to change the values of the
 * Block Protect (BP2, BP1, BP0) bits, to define the size of the area that is to be treated as
 * read-only. The Write Status Register (WRSR) instruction also allows
 * the user to set and reset the Status Register Write Disable (SRWD) bit in accordance with
 * the Write Protect (W/VPP) signal. The Status Register Write Disable (SRWD) bit and Write
 * Protect (W/VPP) signal allow the device to be put in the hardware protected mode (HPM).
 * The Write Status Register (WRSR) instruction is not executed once the hardware protected
 * mode (HPM) is entered.
 */
void m25px16_write_status_register(uint8_t sreg)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_WRITE_STATUS_REGISTER);
  SPI_TRANSMIT(sreg);
  SPI_DEASSERT();
}

/**
 * @brief Write to Lock Register.
 * @details
 * The Write to Lock Register (WRLR) instruction allows bits to be changed in the Lock
 * Registers. Before it can be accepted, a Write Enable (WREN) instruction must previously
 * have been executed. After the Write Enable (WREN) instruction has been decoded, the
 * device sets the Write Enable Latch (WEL).
 * The Write to Lock Register (WRLR) instruction is entered by driving Chip Select (S) Low,
 * followed by the instruction code, three address bytes (pointing to any address in the
 * targeted sector and one data byte on Serial Data input (DQ0).
 * Chip Select (S) must be driven High after the eighth bit of the data byte
 * has been latched in, otherwise the Write to Lock Register (WRLR) instruction is not
 * executed.
 * Lock Register bits are volatile, and therefore do not require time to be written. When the
 * Write to Lock Register (WRLR) instruction has been successfully executed, the Write
 * Enable Latch (WEL) bit is reset after a delay time less than tSHSL minimum value.
 * Any Write to Lock Register (WRLR) instruction, while an Erase, Program or Write cycle is in
 * progress, is rejected without having any effects on the cycle that is in progress.
 */
void m25px16_write_lock_register(uint32_t addr, uint8_t lock_register)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_WRITE_LOCK_REGISTER);
  SPI_TRANSMIT(addr >> 16);
  SPI_TRANSMIT(addr >>  8);
  SPI_TRANSMIT(addr >>  0);
  SPI_TRANSMIT(lock_register);
  SPI_DEASSERT();
}

/**
 * @brief Read Lock Register.
 * @details
 * The device is first selected by driving Chip Select (S) Low. The instruction code for the Read
 * Lock Register (RDLR) instruction is followed by a 3-byte address (A23-A0) pointing to any
 * location inside the concerned sector. Each address bit is latched-in during the rising edge of
 * Serial Clock (C). Then the value of the Lock Register is shifted out on Serial Data output
 * (DQ1), each bit being shifted out, at a maximum frequency fC, during the falling edge of
 * Serial Clock (C).
 * The Read Lock Register (RDLR) instruction is terminated by driving Chip Select (S) High at
 * any time during data output.
 * Any Read Lock Register (RDLR) instruction, while an Erase, Program or Write cycle is in
 * progress, is rejected without having any effects on the cycle that is in progress.
 */
void m25px16_read_lock_register(uint32_t addr, uint8_t *lock_register)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_READ_LOCK_REGISTER);
  SPI_TRANSMIT(addr >> 16);
  SPI_TRANSMIT(addr >>  8);
  SPI_TRANSMIT(addr >>  0);
  *lock_register = SPI_TRANSMIT(0);
  SPI_DEASSERT();
}

/**
 * @brief Read Data Bytes.
 * @details
 * The device is first selected by driving Chip Select (S) Low. The instruction code for the Read
 * Data Bytes (READ) instruction is followed by a 3-byte address (A23-A0), each bit being
 * latched-in during the rising edge of Serial Clock (C). Then the memory contents, at that
 * address, is shifted out on Serial Data output (DQ1), each bit being shifted out, at a
 * maximum frequency fR, during the falling edge of Serial Clock (C).
 * The first byte addressed can be at any location. The address is automatically incremented
 * to the next higher address after each byte of data is shifted out. The whole memory can,
 * therefore, be read with a single Read Data Bytes (READ) instruction. When the highest
 * address is reached, the address counter rolls over to 000000h, allowing the read sequence
 * to be continued indefinitely.
 * The Read Data Bytes (READ) instruction is terminated by driving Chip Select (S) High. Chip
 * Select (S) can be driven High at any time during data output. Any Read Data Bytes (READ)
 * instruction, while an Erase, Program or Write cycle is in progress, is rejected without having
 * any effects on the cycle that is in progress.
 */
void m25px16_read_data_bytes(uint32_t addr, uint8_t *buf, uint32_t siz)
{
  uint32_t i;
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_READ_DATA_BYTES);
  SPI_TRANSMIT(addr >> 16);
  SPI_TRANSMIT(addr >>  8);
  SPI_TRANSMIT(addr >>  0);
  for (i = 0; i < siz; i++) {
    buf[i] = SPI_TRANSMIT(0);
  }
  SPI_DEASSERT();
}

/**
 * @brief Page Program.
 * @details
 * The Page Program (PP) instruction allows bytes to be programmed in the memory
 * (changing bits from 1 to 0). Before it can be accepted, a Write Enable (WREN) instruction
 * must previously have been executed. After the Write Enable (WREN) instruction has been
 * decoded, the device sets the Write Enable Latch (WEL).
 * The Page Program (PP) instruction is entered by driving Chip Select (S) Low, followed by
 * the instruction code, three address bytes and at least one data byte on Serial Data input
 * (DQ0). If the 8 least significant address bits (A7-A0) are not all zero, all transmitted data that
 * goes beyond the end of the current page are programmed from the start address of the
 * same page (from the address whose 8 least significant bits (A7-A0) are all zero). Chip
 * Select (S) must be driven Low for the entire duration of the sequence.
 * If more than 256 bytes are sent to the device, previously latched data are discarded and the
 * last 256 data bytes are guaranteed to be programmed correctly within the same page. If less
 * than 256 data bytes are sent to device, they are correctly programmed at the requested
 * addresses without having any effects on the other bytes of the same page.
 * For optimized timings, it is recommended to use the Page Program (PP) instruction to
 * program all consecutive targeted bytes in a single sequence versus using several Page
 * Program (PP) sequences with each containing only a few bytes.
 * Chip Select (S) must be driven High after the eighth bit of the last data byte has been
 * latched in, otherwise the Page Program (PP) instruction is not executed.
 * As soon as Chip Select (S) is driven High, the self-timed Page Program cycle (whose
 * duration is tPP) is initiated. While the Page Program cycle is in progress, the Status Register
 * may be read to check the value of the Write In Progress (WIP) bit. The Write In Progress
 * (WIP) bit is 1 during the self-timed Page Program cycle, and is 0 when it is completed. At
 * some unspecified time before the cycle is completed, the Write Enable Latch (WEL) bit is
 * reset.
 * A Page Program (PP) instruction applied to a page which is protected by the Block Protect
 * (BP2, BP1, BP0) bits is not executed.
 */
void m25px16_page_program(uint32_t addr, uint8_t *buf, uint32_t siz)
{
  uint32_t i;
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_PAGE_PROGRAM);
  SPI_TRANSMIT(addr >> 16);
  SPI_TRANSMIT(addr >>  8);
  SPI_TRANSMIT(addr >>  0);
  for (i = 0; i < siz; i++) {
    SPI_TRANSMIT(buf[i]);
  }
  SPI_DEASSERT();
}

/**
 * @brief Sector Erase.
 * @details
 * The Sector Erase (SE) instruction sets to 1 (FFh) all bits inside the chosen sector. Before it
 * can be accepted, a Write Enable (WREN) instruction must previously have been executed.
 * After the Write Enable (WREN) instruction has been decoded, the device sets the Write
 * Enable Latch (WEL).
 * The Sector Erase (SE) instruction is entered by driving Chip Select (S) Low, followed by the
 * instruction code, and three address bytes on Serial Data input (DQ0). Any address inside
 * the Sector is a valid address for the Sector Erase (SE) instruction. Chip Select
 * (S) must be driven Low for the entire duration of the sequence.
 * Chip Select (S) must be driven High after the eighth bit of the last address byte has been
 * latched in, otherwise the Sector Erase (SE) instruction is not executed. As soon as Chip
 * Select (S) is driven High, the self-timed Sector Erase cycle (whose duration is tSE) is
 * initiated. While the Sector Erase cycle is in progress, the Status Register may be read to
 * check the value of the Write In Progress (WIP) bit. The Write In Progress (WIP) bit is 1
 * during the self-timed Sector Erase cycle, and is 0 when it is completed. At some unspecified
 * time before the cycle is completed, the Write Enable Latch (WEL) bit is reset.
 * A Sector Erase (SE) instruction applied to a page which is protected by the Block Protect
 * (BP2, BP1, BP0) bits is not executed.
 */
void m25px16_sector_erase(uint32_t addr)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_SECTOR_ERASE);
  SPI_TRANSMIT(addr >> 16);
  SPI_TRANSMIT(addr >>  8);
  SPI_TRANSMIT(addr >>  0);
  SPI_DEASSERT();
}

/**
 * @brief Bulk Erase.
 * @details
 * The Bulk Erase (BE) instruction sets all bits to 1 (FFh). Before it can be accepted, a Write
 * Enable (WREN) instruction must previously have been executed. After the Write Enable
 * (WREN) instruction has been decoded, the device sets the Write Enable Latch (WEL).
 * The Bulk Erase (BE) instruction is entered by driving Chip Select (S) Low, followed by the
 * instruction code on Serial Data input (DQ0). Chip Select (S) must be driven Low for the
 * entire duration of the sequence.
 * Chip Select (S) must be driven High after the eighth bit of the instruction code has been
 * latched in, otherwise the Bulk Erase instruction is not executed. As soon as Chip Select (S)
 * is driven High, the self-timed Bulk Erase cycle (whose duration is tBE) is initiated. While the
 * Bulk Erase cycle is in progress, the Status Register may be read to check the value of the
 * Write In Progress (WIP) bit. The Write In Progress (WIP) bit is 1 during the self-timed Bulk
 * Erase cycle, and is 0 when it is completed. At some unspecified time before the cycle is
 * completed, the Write Enable Latch (WEL) bit is reset.
 * The Bulk Erase (BE) instruction is executed only if all Block Protect (BP2, BP1, BP0) bits
 * are 0. The Bulk Erase (BE) instruction is ignored if one, or more, sectors are protected.
 */
void m25px16_bulk_erase(void)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_BULK_ERASE);
  SPI_DEASSERT();
}

/**
 * @brief Deep Power Down.
 * @details
 * Executing the Deep Power-down (DP) instruction is the only way to put the device in the
 * lowest consumption mode (the Deep Power-down mode). It can also be used as a software
 * protection mechanism, while the device is not in active use, as in this mode, the device
 * ignores all Write, Program and Erase instructions.
 * Driving Chip Select (S) High deselects the device, and puts the device in the Standby Power
 * mode (if there is no internal cycle currently in progress). But this mode is not the Deep
 * Power-down mode. The Deep Power-down mode can only be entered by executing the
 * Deep Power-down (DP) instruction, subsequently reducing the standby current (from ICC1 to
 * ICC2).
 * To take the device out of Deep Power-down mode, the Release from Deep Power-down
 * (RDP) instruction must be issued. No other instruction must be issued while the device is in
 * Deep Power-down mode.
 * The Deep Power-down mode automatically stops at power-down, and the device always
 * powers up in the Standby Power mode.
 * The Deep Power-down (DP) instruction is entered by driving Chip Select (S) Low, followed
 * by the instruction code on Serial Data input (DQ0). Chip Select (S) must be driven Low for
 * the entire duration of the sequence.
 * Chip Select (S) must be driven High after the eighth bit of the instruction code has been
 * latched in, otherwise the Deep Power-down (DP) instruction is not executed. As soon as
 * Chip Select (S) is driven High, it requires a delay of tDP before the supply current is reduced
 * to ICC2 and the Deep Power-down mode is entered.
 * Any Deep Power-down (DP) instruction, while an Erase, Program or Write cycle is in
 * progress, is rejected without having any effects on the cycle that is in progress.
 */
void m25px16_deep_power_down(void)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_DEEP_POWER_DOWN);
  SPI_DEASSERT();
}

/**
 * @brief Release From Deep Power Down.
 * @details
 * Once the device has entered the Deep Power-down mode, all instructions are ignored
 * except the Release from Deep Power-down (RDP) instruction. Executing this instruction
 * takes the device out of the Deep Power-down mode.
 * The Release from Deep Power-down (RDP) instruction is entered by driving Chip Select (S)
 * Low, followed by the instruction code on Serial Data input (DQ0). Chip Select (S) must be
 * driven Low for the entire duration of the sequence.
 * The Release from Deep Power-down (RDP) instruction is terminated by driving Chip Select
 * (S) High. Sending additional clock cycles on Serial Clock (C), while Chip Select (S) is driven
 * Low, cause the instruction to be rejected, and not executed.
 * After Chip Select (S) has been driven High, followed by a delay, tRDP, the device is put in the
 * Standby mode. Chip Select (S) must remain High at least until this period is over. The
 * device waits to be selected, so that it can receive, decode and execute instructions.
 * Any Release from Deep Power-down (RDP) instruction, while an Erase, Program or Write
 * cycle is in progress, is rejected without having any effects on the cycle that is in progress.
 */
void m25px16_release_from_deep_power_down(void)
{
  SPI_ASSERT();
  SPI_TRANSMIT(CMD_RELEASE_FROM_DEEP_POWER_DOWN);
  SPI_DEASSERT();
}

