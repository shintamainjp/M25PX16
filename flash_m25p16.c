/**
 * @file flash_m25p16.c
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

#include "flash.h"
#include "m25p16.h"

/**
 * @brief Initialize the target flash.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_init(void)
{
  m25p16_init();
  return 0;
}

/**
 * @brief Flash information.
 *
 * @param p Flash information structure.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_info(flash_info_t *p)
{
  p->page_count = M25P16_PAGE_COUNT;
  p->page_bytes = M25P16_PAGE_BYTE_SIZE;
  p->sector_count = M25P16_SECTOR_COUNT;
  p->sector_bytes = M25P16_SECTOR_BYTE_SIZE;
  return 0;
}

/**
 * @brief Erase sector.
 *
 * @param sector The target sector number.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_sector_erase(unsigned int sector)
{
    uint8_t sreg = 0;

    m25p16_write_enable();
    m25p16_sector_erase(M25P16_SECTOR_BYTE_SIZE * sector);
    do {
      m25p16_read_status_register(&sreg);
    } while (M25P16_SREG_WRITE_IN_PROGRESS(sreg));
    m25p16_write_disable();

    return 0;
}

/**
 * @brief Write data to the target flash.
 *
 * @param page The target page number.
 * @param callback The callback function.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_page_write(unsigned int page, unsigned char *buf, unsigned int siz)
{
  unsigned int addr = M25P16_PAGE_BYTE_SIZE * page;

  if (M25P16_PAGE_BYTE_SIZE < siz) {
    return -1;
  }

  uint8_t sreg = 0;
  m25p16_write_enable();
  m25p16_page_program(addr, buf, siz);
  do {
    m25p16_read_status_register(&sreg);
  } while (M25P16_SREG_WRITE_IN_PROGRESS(sreg));
  m25p16_write_disable();

  return 0;
}

/**
 * @brief Read data from the target flash.
 *
 * @param page The target page number.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_page_read(unsigned int page, unsigned char *buf, unsigned int siz)
{
  unsigned int addr = M25P16_PAGE_BYTE_SIZE * page;

  if (M25P16_PAGE_BYTE_SIZE < siz) {
    return -1;
  }

  m25p16_read_data_bytes(addr, buf, siz);

  return 0;
}

