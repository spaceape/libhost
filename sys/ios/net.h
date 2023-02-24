#ifdef LINUX
#ifndef sys_net_h
#define sys_net_h
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
#include <sys/socket.h>

/* net
   stream for unix and internet domain sockets
*/
class net: public fio
{
  public:
          net() noexcept;
          net(int, long int = SOCK_STREAM, const char* = nullptr, long int = 0) noexcept;
          net(int, long int = O_RDWR, long int = 0777) noexcept = delete;
          net(const char*, long int = O_RDWR, long int = 0777) noexcept = delete;
          net(const net&) noexcept = delete;
          net(net&&) noexcept;
  virtual ~net();

          bool open(int, long int, const char* = nullptr, long int = 0) noexcept;
        //bool bind(int, long int, long int = 0) noexcept;

  virtual bool is_seekable() const noexcept override;
  virtual bool is_readable() const noexcept override;
  virtual bool is_writable() const noexcept override;

          net& operator=(const net&) noexcept = delete;
          net& operator=(net&&) noexcept;
};
#endif
#endif
