#ifndef sys_config_h
#define sys_config_h
/** 
    Copyright (c) 2023, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include <sys.h>
#include <bcm2835.h>

/* hw_enable_uart0
*/
constexpr bool hw_enable_uart0 = false;

/* pin_uart0_*
   pin assignments for uart0
*/
constexpr unsigned int  pin_uart0_rx = pin_none;
constexpr unsigned int  pin_uart0_tx = pin_none;

/* hw_enable_uart1
*/
constexpr bool hw_enable_uart1 = false;

/* pin_uart1_*
   pin assignments for uart1
*/
constexpr unsigned int  pin_uart1_rx = pin_none;
constexpr unsigned int  pin_uart1_tx = pin_none;

/* hw_default_uart_baud
*/
constexpr int  hw_default_uart_baud = 115200;

/* hw_enable_spi0
*/
constexpr bool  hw_enable_spi0 = false;

/* pin_spi0_*
   pin assignments for spi0
*/
constexpr unsigned int  pin_spi0_mosi = RPI_V2_GPIO_P1_19;
constexpr unsigned int  pin_spi0_miso = RPI_V2_GPIO_P1_21;
constexpr unsigned int  pin_spi0_sck = RPI_V2_GPIO_P1_23;
constexpr unsigned int  pin_spi0_ce0 = RPI_V2_GPIO_P1_24;
constexpr unsigned int  pin_spi0_ce1 = RPI_V2_GPIO_P1_26;

/* hw_enable_spi1
*/
constexpr bool hw_enable_spi1 = false;

/* pin_spi1_*
   pin assignments for spi1
*/
constexpr unsigned int  pin_spi1_mosi = pin_none;
constexpr unsigned int  pin_spi1_miso = pin_none;
constexpr unsigned int  pin_spi1_sck = pin_none;
constexpr unsigned int  pin_spi1_ce0 = pin_none;
constexpr unsigned int  pin_spi1_ce1 = pin_none;

/* hw_default_spi
   whether or not to add an ADT entry for a default 'spi' device and which spi instance that should point to
*/
constexpr auto hw_default_spi_index = -1;

/* hw_default_spi_baud
*/
constexpr int  hw_default_spi_divider = 16;


/* hw_enable_lcd
*/
constexpr bool hw_enable_lcd = true;

constexpr int  hw_lcd_spi = 0;

constexpr std::uint16_t hw_lcd_sx  = 240;
constexpr std::uint16_t hw_lcd_sy  = 320;

/* pin_lcd_*
   default LCD pins
*/
constexpr unsigned int  pin_lcd_power = pin_none;
constexpr unsigned int  pin_lcd_cs = pin_none;
constexpr unsigned int  pin_lcd_dc = 5;
constexpr unsigned int  pin_lcd_reset = 6;

/* hw_enable_touch
   initialise touch screen support on the selected SPI interface
   requires: SPI
*/
constexpr bool hw_enable_touch = false;

/* pin_touch_*
   default touch pins (SPI0)
*/
constexpr unsigned int  pin_touch_cs = pin_none;
constexpr unsigned int  pin_touch_interrupt = pin_none;

/* pin_backlight_*
*/
constexpr bool hw_enable_backlight = false;
constexpr unsigned int  pin_backlight_ph0 = pin_none;

/* pin_debug
   debug pin
*/
constexpr unsigned int  pin_debug = pin_none;

#endif
