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
#include "net.h"
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

      net::net() noexcept:
      fio()
{
}

      net::net(int domain, long int type, const char* address, long int port) noexcept:
      fio()
{
      if(address && address[0]) {
          open(domain, type, address, port);
      }
}

      net::net(net&& copy) noexcept:
      fio(std::move(copy))
{
}

      net::~net()
{
}

bool  net::open(int domain, long int type, const char* address, long int port) noexcept
{
      reset();
      if(address) {
          if(address[0]) {
              bool l_nb = type & O_NONBLOCK;
              if(domain == AF_UNIX) {
                  int         l_rc;
                  int         l_desc = ::socket(AF_UNIX, SOCK_STREAM | (type & O_NONBLOCK), 0);
                  sockaddr_un l_info;
                  if(l_desc >= 0) {
                      l_info.sun_family = AF_UNIX;
                      std::strncpy(
                          l_info.sun_path,
                          address,
                          sizeof(l_info.sun_path) - 1
                      );
                      if(port == M_BIND) {
                          l_rc = ::bind(l_desc, reinterpret_cast<sockaddr*>(std::addressof(l_info)), sizeof(sockaddr_un));
                          if((l_rc == 0) ||
                              ((l_nb == true) && (errno == EINPROGRESS))) {
                              m_desc = l_desc;
                              m_own  = true;
                              return true;
                          }
                      } else
                      if(port == M_CONNECT) {
                          l_rc = ::connect(l_desc, reinterpret_cast<sockaddr*>(std::addressof(l_info)), sizeof(sockaddr_un));
                          if((l_rc == 0) ||
                              ((l_nb == true) && (errno == EINPROGRESS))) {
                              m_desc = l_desc;
                              m_own  = true;
                              return true;
                          }
                      }
                      ::close(l_desc);
                  }
              } else
              if(domain == AF_INET) {
                  int         l_rc;
                  int         l_desc = ::socket(AF_INET, type, 0);
                  sockaddr_in l_info;
                  if(l_desc >= 0) {
                      l_info.sin_family = AF_INET;
                      l_info.sin_addr.s_addr = INADDR_ANY;
                      std::strncpy(
                          reinterpret_cast<char*>(std::addressof(l_info.sin_addr.s_addr)),
                          address,
                          sizeof(l_info.sin_addr.s_addr) - 1
                      );
                      l_info.sin_port = htons(port);
                      l_rc = ::connect(l_desc, reinterpret_cast<sockaddr*>(std::addressof(l_info)), sizeof(sockaddr_in));
                      if((l_rc == 0) ||
                          ((l_nb == true) && (errno == EINPROGRESS))) {
                          m_desc = l_desc;
                          m_own  = true;
                          return true;
                      }
                      ::close(l_desc);
                  }
              }
          }
      }
      return false;
}

// bool  net::bind(int domain, long int type, long int port) noexcept
// {
//       reset();
//       if(domain == AF_INET) {
//           int         l_desc = ::socket(AF_INET, type, 0);
//           sockaddr_in l_info;
//           if(l_desc >= 0) {
//               l_info.sin_family = AF_INET;
//               l_info.sin_addr.s_addr = INADDR_ANY;
//               l_info.sin_port = htons(port);
//               if(::bind(l_desc, reinterpret_cast<sockaddr*>(std::addressof(l_info)), sizeof(sockaddr_un)) == 0) {
//                   m_desc = l_desc;
//                   m_own  = true;
//                   return true;
//               }
//               ::close(l_desc);
//           }
//       }
//       return false;
// }

bool  net::is_seekable() const noexcept
{
      return false;
}

bool  net::is_readable() const noexcept
{
      return true;
}

bool  net::is_writable() const noexcept
{
      return true;
}

net&  net::operator=(net&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
