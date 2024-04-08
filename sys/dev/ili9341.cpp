#include "ili9341.h"
#include <os.h>

#if defined(ILI9341) || defined(ILI9340)

#include "config.h"
#include <bcm2835.h>
#include <unistd.h>
#include <cstdint>
#include <utility>

void  ili9341_send_raw() noexcept
{
}

template<typename... Args>
void  ili9341_send_raw(std::uint8_t byte, Args&&... next) noexcept
{
      bcm2835_spi_transfer(byte);
      ili9341_send_raw(std::forward<Args&&>(next)...);
}

template<typename... Args>
void  ili9341_send_raw(std::uint16_t word, Args&&... next) noexcept
{
    std::uint8_t l_bytes[2];
    if constexpr (os::is_lsb) {
        l_bytes[0] = (word & 0xff00) >> 8;
        l_bytes[1] = (word & 0x00ff);
    } else
    if constexpr (os::is_msb) {
        l_bytes[0] = (word & 0x00ff);
        l_bytes[1] = (word & 0xff00) >> 8;
    }
    ili9341_send_raw(l_bytes[0], l_bytes[1]);
    ili9341_send_raw(std::forward<Args&&>(next)...);
}

void  ili9341_send_raw(std::uint8_t* data, std::size_t size) noexcept
{
      std::uint8_t* p_data_head = data;
      std::uint8_t* p_data_tail = p_data_head + size;
      while(p_data_head < p_data_tail) {
          bcm2835_spi_transfer(*p_data_head++);
      }
}

template<typename... Args>
void  ili9341_send_cmd(std::uint8_t cmd, Args&&... next) noexcept
{
    if constexpr (pin_lcd_cs != pin_none) {
        bcm2835_gpio_clr(pin_lcd_cs);
    }
    bcm2835_gpio_clr(pin_lcd_dc);
    bcm2835_spi_transfer(cmd);
    bcm2835_gpio_set(pin_lcd_dc);
    ili9341_send_raw(std::forward<Args&&>(next)...);
    if constexpr (pin_lcd_cs != pin_none) {
        bcm2835_gpio_set(pin_lcd_cs);
    }
}

void ili9341_initialise() noexcept
{
    if constexpr (pin_lcd_cs != pin_none) {
        bcm2835_gpio_fsel(pin_lcd_cs, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_set(pin_lcd_cs);
    }
    bcm2835_gpio_fsel(pin_lcd_reset, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(pin_lcd_reset);
    bcm2835_gpio_fsel(pin_lcd_dc, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(pin_lcd_dc);
    bcm2835_delay(128);
    ili9341_hard_reset();
  
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    __sync_synchronize();

    // initialise the display device
    ili9341_soft_reset();
    ili9341_send_cmd(0xef, (std::uint8_t)0x03, (std::uint8_t)0x80, (std::uint8_t)0x02);
    ili9341_send_cmd(0xcf, (std::uint8_t)0x00, (std::uint8_t)0xc1, (std::uint8_t)0x30);
    ili9341_send_cmd(0xed, (std::uint8_t)0x64, (std::uint8_t)0x03, (std::uint8_t)0x12, (std::uint8_t)0x81);
    ili9341_send_cmd(0xe8, (std::uint8_t)0x85, (std::uint8_t)0x00, (std::uint8_t)0x78);                                           // driver timing control A
    ili9341_send_cmd(0xcb, (std::uint8_t)0x39, (std::uint8_t)0x2c, (std::uint8_t)0x00, (std::uint8_t)0x34, (std::uint8_t)0x02);   // extended power control A
    ili9341_send_cmd(0xf7, (std::uint8_t)0x20);                                                                                   // pump ratio control
    ili9341_send_cmd(0xea, (std::uint8_t)0x00, (std::uint8_t)0x00);                                                               // driver timing control B

    ili9341_send_cmd(ili9341_pwctr1, (std::uint8_t)0x23);
    ili9341_send_cmd(ili9341_pwctr2, (std::uint8_t)0x10);
    ili9341_send_cmd(ili9341_vmctr1, (std::uint8_t)0x3e, (std::uint8_t)0x28);
    ili9341_send_cmd(ili9341_vmctr2, (std::uint8_t)0x86);
    ili9341_send_cmd(ili9341_madctl, (std::uint8_t)0x40);
    ili9341_send_cmd(ili9341_pixfmt, (std::uint8_t)0x55);                               // pixel format == 16 bit per colour
    // ili9341_send_cmd(ili9341_ifctl, (std::uint8_t)0x00, (std::uint8_t)0x10, (std::uint8_t)0x22);

    ili9341_send_cmd(ili9341_frmctr1, (std::uint8_t)0x00, (std::uint8_t)0x13);          // frame control: 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
    ili9341_send_cmd(ili9341_dfunctr, (std::uint8_t)0x08, (std::uint8_t)0x82, (std::uint8_t)0x27);
    ili9341_send_cmd(0xf2, (std::uint8_t)0x00);                                         // 3gamma function disable

    ili9341_send_cmd(ili9341_gammaset, (std::uint8_t)0x01);
    ili9341_send_cmd(ili9341_gmctrp1, (std::uint8_t)0x0f, (std::uint8_t)0x31, (std::uint8_t)0x2b, (std::uint8_t)0x0c, (std::uint8_t)0x0e, (std::uint8_t)0x08, (std::uint8_t)0x4e, (std::uint8_t)0xf1, (std::uint8_t)0x37, (std::uint8_t)0x07, (std::uint8_t)0x10, (std::uint8_t)0x03, (std::uint8_t)0x0e, (std::uint8_t)0x09, (std::uint8_t)0x00);
    ili9341_send_cmd(ili9341_gmctrn1, (std::uint8_t)0x00, (std::uint8_t)0x0e, (std::uint8_t)0x14, (std::uint8_t)0x03, (std::uint8_t)0x11, (std::uint8_t)0x07, (std::uint8_t)0x31, (std::uint8_t)0xc1, (std::uint8_t)0x48, (std::uint8_t)0x08, (std::uint8_t)0x0f, (std::uint8_t)0x0c, (std::uint8_t)0x31, (std::uint8_t)0x36, (std::uint8_t)0x0f);

    ili9341_pwon();

    __sync_synchronize();

    bcm2835_spi_setClockDivider(hw_default_spi_divider);
}

void ili9341_hard_reset() noexcept
{
    bcm2835_gpio_clr(pin_lcd_reset);
    bcm2835_delay(128);
    bcm2835_gpio_set(pin_lcd_reset);
    bcm2835_delay(128);
}

void ili9341_soft_reset() noexcept
{
    ili9341_send_cmd(ili9341_swrst);
}

void ili9341_pwon() noexcept
{
    ili9341_send_cmd(ili9341_slpout);
    ili9341_send_cmd(ili9341_dispon);
}

void ili9341_copy(const std::uint8_t*, int, int, int, int) noexcept
{
}

void ili9341_fill(const std::uint8_t* data, int px, int py, int sx, int sy) noexcept
{
    auto raw_rgb565 = reinterpret_cast<const std::uint16_t*>(data);
    // set row addresses
    ili9341_send_cmd(ili9341_caset, (std::uint16_t)px, (std::uint16_t)(px + sx - 1));
    // push display data for 
    for(int y = 0; y < sy; ++y) {
        ili9341_send_cmd(ili9341_paset, (std::uint16_t)(py + y), (std::uint16_t)(py + y));
        ili9341_send_cmd(ili9341_ramwr);
        for(int x = 0; x < sx; ++x) {
            ili9341_send_raw(*raw_rgb565++);
        }
    }
}

void ili9341_fill(std::uint32_t colour, int px, int py, int sx, int sy) noexcept
{
    std::uint16_t cc = colour & 0xffff;
    // set display address
    ili9341_send_cmd(ili9341_caset, (std::uint16_t)px, (std::uint16_t)(sx - 1));
    ili9341_send_cmd(ili9341_paset, (std::uint16_t)py, (std::uint16_t)(sy - 1));
    // push display data
    ili9341_send_cmd(ili9341_ramwr);
    for(int y = 0; y < sy; ++y) {
        for(int x = 0; x < sx; ++x) {
            ili9341_send_raw(cc);
        }
    }
}

void ili9341_clear(std::uint32_t colour) noexcept
{
    std::uint16_t sx = hw_lcd_sx;
    std::uint16_t sy = hw_lcd_sy;
    std::uint16_t cc = colour & 0xffff;
    ili9341_send_cmd(ili9341_caset, (std::uint16_t)0u, (std::uint16_t)(sx - 1));
    ili9341_send_cmd(ili9341_paset, (std::uint16_t)0u, (std::uint16_t)(sy - 1));
    ili9341_send_cmd(ili9341_ramwr);
    for(int y = 0; y < sy; ++y) {
        for(int x = 0; x < sx; ++x) {
            ili9341_send_raw(cc);
        }
    }
}

void ili9341_pwoff() noexcept
{
    ili9341_send_cmd(ili9341_dispoff);
}

void ili9341_dispose() noexcept
{
    ili9341_pwoff();
    bcm2835_spi_end();
}
#endif
