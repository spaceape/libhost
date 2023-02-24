#ifdef  LINUX
#ifndef sys_tty_h
#define sys_tty_h
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
#include "fio.h"
#include <termios.h>

/* tty
   stream for tty devices
*/
class tty: public fio
{
  protected:
          bool    set_terminal_speed(struct termios*, int) noexcept;
          bool    set_terminal_speed(struct termios*, const char*) noexcept;

  public:
          tty() noexcept;
          tty(int, long int = O_RDWR, long int = 0777) noexcept;
          tty(const char*, long int = O_RDWR, long int = 0777) noexcept;
          tty(const char*, int, const char*, long int = O_RDWR, long int = 0777) noexcept;
          tty(const tty&) noexcept;
          tty(tty&&) noexcept;
  virtual ~tty();

          bool open(const char*, int, const char*, long int, long int) noexcept;

          bool set_attributes(int, const char*) noexcept;
          bool set_attributes(int, int, int, int) noexcept;
          bool set_blocking(bool = true) noexcept;
          bool set_nonblocking() noexcept;

          void close(bool = true) noexcept;

          tty& operator=(const tty&) noexcept;
          tty& operator=(tty&&) noexcept;
};
#endif
#endif