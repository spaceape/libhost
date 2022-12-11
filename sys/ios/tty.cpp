/** 
    Copyright (c) 2021, wicked systems
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
#include "tty.h"
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <unistd.h>

      tty::tty() noexcept:
      fio()
{
}

      tty::tty(int desc, long int mode, long int permissions) noexcept:
      fio(desc, mode | O_NOCTTY | O_SYNC, permissions)
{
}

      tty::tty(const char* name, long int mode, long int permissions) noexcept:
      fio(name, mode, permissions)
{
}

      tty::tty(const char* name, int baud, const char* format, long int mode, long int permissions) noexcept:
      fio(name, mode, permissions)
{
      if(m_desc) {
          if(set_attributes(baud, format)) {
              return;
          } else
              close(true);
      }
}

      tty::tty(const tty& copy) noexcept:
      fio(copy)
{
}

      tty::tty(tty&& copy) noexcept:
      fio(std::move(copy))
{
}

      tty::~tty()
{
}

bool  tty::open(const char* name, int baud, const char* format, long int mode, long int permissions) noexcept
{
      long int  l_mode = mode | O_NOCTTY | O_SYNC;
      bool      l_nblk = false;
      if(l_mode & O_NONBLOCK) {
          l_nblk = true;
          l_mode ^= O_NONBLOCK;
      }
      if(fio::open(name, l_mode, permissions)) {
          bool l_attr_success = set_attributes(baud, format);
          bool l_nblk_success = true;
          if(l_nblk) {
              l_nblk_success  = set_nonblocking();
          }
          return l_attr_success && l_nblk_success;
      }
      return false;
}

void  tty::close(bool reset) noexcept
{
      return fio::close(reset);
}

bool  tty::set_terminal_speed(struct termios* tty, int baud) noexcept
{
      switch(baud) {
          case 0:
              break;
          case 50:
              cfsetospeed(tty, B50);
              cfsetispeed(tty, B50);
              return true;
          case 75:
              cfsetospeed(tty, B75);
              cfsetispeed(tty, B75);
              return true;
          case 110:
              cfsetospeed(tty, B110);
              cfsetispeed(tty, B110);
              return true;
          case 134:
              cfsetospeed(tty, B134);
              cfsetispeed(tty, B134);
              return true;
          case 150:
              cfsetospeed(tty, B150);
              cfsetispeed(tty, B150);
              return true;
          case 200:
              cfsetospeed(tty, B200);
              cfsetispeed(tty, B200);
              return true;
          case 300:
              cfsetospeed(tty, B300);
              cfsetispeed(tty, B300);
              return true;
          case 600:
              cfsetospeed(tty, B600);
              cfsetispeed(tty, B600);
              return true;
          case 1200:
              cfsetospeed(tty, B1200);
              cfsetispeed(tty, B1200);
              return true;
          case 1800:
              cfsetospeed(tty, B1800);
              cfsetispeed(tty, B1800);
              return true;
          case 2400:
              cfsetospeed(tty, B2400);
              cfsetispeed(tty, B2400);
              return true;
          case 4800:
              cfsetospeed(tty, B4800);
              cfsetispeed(tty, B4800);
              return true;
          case 9600:
              cfsetospeed(tty, B9600);
              cfsetispeed(tty, B9600);
              return true;
          case 19200:
              cfsetospeed(tty, B19200);
              cfsetispeed(tty, B19200);
              return true;
          case 38400:
              cfsetospeed(tty, B38400);
              cfsetispeed(tty, B38400);
              return true;
          case 57600:
              cfsetospeed(tty, B57600);
              cfsetispeed(tty, B57600);
              return true;
          case 115200:
              cfsetospeed(tty, B115200);
              cfsetispeed(tty, B115200);
              return true;
          case 230400:
              cfsetospeed(tty, B230400);
              cfsetispeed(tty, B230400);
              return true;
          case 460800:
              cfsetospeed(tty, B460800);
              cfsetispeed(tty, B460800);
              return true;
          case 500000:
              cfsetospeed(tty, B500000);
              cfsetispeed(tty, B500000);
              return true;
          case 576000:
              cfsetospeed(tty, B576000);
              cfsetispeed(tty, B576000);
              return true;
          case 921600:
              cfsetospeed(tty, B921600);
              cfsetispeed(tty, B921600);
              return true;
          case 1000000:
              cfsetospeed(tty, B1000000);
              cfsetispeed(tty, B1000000);
              return true;
          case 1152000:
              cfsetospeed(tty, B1152000);
              cfsetispeed(tty, B1152000);
              return true;
          case 1500000:
              cfsetospeed(tty, B1500000);
              cfsetispeed(tty, B1500000);
              return true;
          case 2000000:
              cfsetospeed(tty, B2000000);
              cfsetispeed(tty, B2000000);
              return true;
          case 2500000:
              cfsetospeed(tty, B2500000);
              cfsetispeed(tty, B2500000);
              return true;
          case 3000000:
              cfsetospeed(tty, B3000000);
              cfsetispeed(tty, B3000000);
              return true;
          case 3500000:
              cfsetospeed(tty, B3500000);
              cfsetispeed(tty, B3500000);
              return true;
          case 4000000:
              cfsetospeed(tty, B4000000);
              cfsetispeed(tty, B4000000);
              return true;
          default:
              cfsetospeed(tty, B115200);
              cfsetispeed(tty, B115200);
              return true;
      }
      return false;
}

bool  tty::set_terminal_speed(struct termios* tty, const char* baud) noexcept
{
      return set_terminal_speed(tty, strtol(baud, nullptr, 10));
}

bool  tty::set_attributes(int baud, const char* format) noexcept
{
      int l_data_bits = 8;
      int l_parity = 'n';
      int l_stop_bits = 1;
      if(format) {
          if((format[0] >= '5') && (format[0] <= '8')) {
              l_data_bits = format[0] - '0';
              if((format[1] == 'n') || (format[1] == 'N') ||
                  (format[1] == 'e') || (format[1] == 'E') ||
                  (format[1] == 'o') || (format[1] == 'O')) {
                  l_parity = format[1];
                  if((format[2] >= '0') || (format[2] <= '1')) {
                      l_stop_bits = format[2] - '0';
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
          return set_attributes(baud, l_data_bits, l_parity, l_stop_bits);
      }
      return false;
}

bool  tty::set_attributes(int baud, int bits, int parity, int stop_bits) noexcept
{
      if(m_desc >= 0) {
          struct termios l_tty;

          if(baud == 0) {
              return false;
          }
          if((bits < 5) || (bits > 8)) {
              return false;
          }
          if((parity != 'n') && (parity != 'e') && (parity != 'o') &&
              (parity != 'N') && (parity != 'E') && (parity != 'O')) {
              return false;
          }
          if((stop_bits != 0) && (stop_bits != 1)) {
              return false;
          }

          if(tcgetattr(m_desc, std::addressof(l_tty)) == 0) {
              if(set_terminal_speed(std::addressof(l_tty), baud)) {
                  // disable IGNBRK for mismatched speed tests; otherwise receive break as NUL chars
                  l_tty.c_iflag &= ~IGNBRK;                         // disable break processing
                  l_tty.c_lflag = 0;                                // no signaling chars, no echo,
                  
                  // no canonical processing
                  l_tty.c_oflag = 0;                                // no remapping, no delays
                  l_tty.c_cc[VMIN]  = 0;                            // read doesn't block
                  l_tty.c_cc[VTIME] = 5;                            // 0.5 seconds read timeout

                  if(bits == 5) {
                      l_tty.c_cflag = l_tty.c_cflag & ~CSIZE;
                      l_tty.c_cflag = l_tty.c_cflag | CS5;
                  } else
                  if(bits == 6) {
                      l_tty.c_cflag = l_tty.c_cflag & ~CSIZE;
                      l_tty.c_cflag = l_tty.c_cflag | CS6;
                  }
                  if(bits == 7) {
                      l_tty.c_cflag = l_tty.c_cflag & ~CSIZE;
                      l_tty.c_cflag = l_tty.c_cflag | CS7;
                  }
                  if(bits == 8) {
                      l_tty.c_cflag = l_tty.c_cflag & ~CSIZE;
                      l_tty.c_cflag = l_tty.c_cflag | CS8;
                  }

                  if((parity == 'n') || (parity == 'N')) {
                      l_tty.c_cflag &= ~(PARENB | PARODD);    // shut off parity
                  } else
                  if((parity == 'e') || (parity == 'E')) {
                      l_tty.c_cflag &= ~(PARENB | PARODD);    // set even parity
                      l_tty.c_cflag |= PARENB;
                  } else
                  if((parity == 'o') || (parity == 'O')) {
                      l_tty.c_cflag &= ~(PARENB | PARODD);    // set odd parity
                      l_tty.c_cflag |= PARODD;
                  }

                  if(stop_bits == 0) {
                      l_tty.c_cflag &= ~CSTOPB;
                  } else
                  if(stop_bits == 1) {
                      l_tty.c_cflag |= CSTOPB;
                  }

                  l_tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // shut off xon/xoff ctrl
                  l_tty.c_iflag &= ~ICRNL;                          // prevent substituting \r to \n
                  l_tty.c_cflag |= (CLOCAL | CREAD);                // ignore modem controls,              
                  l_tty.c_cflag &= ~CRTSCTS;
                  
                  // set tty attributes
                  if(tcsetattr(m_desc, TCSANOW, std::addressof(l_tty)) == 0) {
                      return true;
                  } else
                      return false;
              } else
                  return false;
          } else
              return false;
      } else
          return false;
}

bool  tty::set_blocking(bool value) noexcept
{
      if(m_desc >= 0) {
          struct termios l_tty;
          std::memset(std::addressof(l_tty), 0, sizeof(struct termios));
          if(tcgetattr(m_desc, std::addressof(l_tty)) == 0) {
              if(value) {
                  l_tty.c_cc[VMIN] = 1;
              } else
                  l_tty.c_cc[VMIN] = 0;
              l_tty.c_cc[VTIME] = 5;
              if(tcsetattr(m_desc, TCSANOW, std::addressof(l_tty)) == 0) {
                  return true;
              }
          }
      }
      return false;
}

bool  tty::set_nonblocking() noexcept
{
      return set_blocking(false);
}

tty&  tty::operator=(const tty& rhs) noexcept
{
      assign(rhs);
      return *this;
}

tty&  tty::operator=(tty&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
