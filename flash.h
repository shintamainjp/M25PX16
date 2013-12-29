/**
 * @file flash.h
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

#ifndef FLASH_H
#define FLASH_H

/**
 * @brief Flash information.
 */
typedef struct {
  unsigned int page_count;
  unsigned int page_bytes;
  unsigned int sector_count;
  unsigned int sector_bytes;
} flash_info_t;

/**
 * @brief Initialize the target flash.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_init(void);

/**
 * @brief Flash information.
 *
 * @param p Flash information structure.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_info(flash_info_t *p);

/**
 * @brief Erase sector.
 *
 * @param sector The target sector number.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_sector_erase(unsigned int sector);

/**
 * @brief Write data to the target flash.
 *
 * @param page The target page number.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_page_write(unsigned int page, unsigned char *buf, unsigned int siz);

/**
 * @brief Read data from the target flash.
 *
 * @param page The target page number.
 *
 * @retval 0 Success.
 * @retval !0 Failure.
 */
int flash_page_read(unsigned int page, unsigned char *buf, unsigned int siz);

#endif

