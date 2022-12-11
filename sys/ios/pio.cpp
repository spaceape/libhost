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
#include "pio.h"
#include "sio.h"
#include <cstring>

      constexpr char s_ident_0[] = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      constexpr char s_ident_1[] = "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

      pio::pio() noexcept:
      ios(),
      m_io(nullptr),
      m_space{SPC, TAB},
      m_quote{'"', '"'},
      m_cache(EOF),
      m_depth(0)
{
}

      pio::pio(sys::ios* io) noexcept:
      ios(),
      m_io(io),
      m_space{SPC, TAB},
      m_quote{'"', '"'},
      m_cache(EOF),
      m_depth(0)
{
}

      pio::pio(sio& io) noexcept:
      pio(std::addressof(io))
{
}

      pio::pio(const pio& copy) noexcept:
      ios(copy),
      m_io(copy.m_io),
      m_space{SPC, TAB},
      m_quote{'"', '"'},
      m_cache(copy.m_cache),
      m_depth(copy.m_depth)
{
}

      pio::pio(pio&& copy) noexcept:
      ios(std::move(copy)),
      m_io(copy.m_io),
      m_space{SPC, TAB},
      m_quote{'"', '"'},
      m_cache(copy.m_cache),
      m_depth(copy.m_depth)
{
      copy.release();
}

      pio::~pio()
{
      reset(false);
}

int   pio::get_char() noexcept
{
      int  l_result;
      if(peek(l_result)) {
          m_cache = EOF;
      }
      return l_result;
}

unsigned int pio::get_byte() noexcept
{
      return get_char();
}

int   pio::seek(int offset, int whence) noexcept
{
      int l_offset_0;
      int l_offset_1;
      if(m_io->is_seekable()) {
          l_offset_0 = m_io->seek(0, SEEK_CUR);
          l_offset_1 = m_io->seek(offset, whence);
          if(l_offset_1 != l_offset_0) {
              m_cache = EOF;
          }
          return l_offset_1;

      }
      return -1;
}

int   pio::read(int count) noexcept
{
      if(count) {
          if(m_cache != EOF) {
              return m_io->read(count - 1) + 1;
          }
          return m_io->read(count);
      }
      return 0;
}

int   pio::read(int count, char* memory) noexcept
{
      if(count) {
          if(m_cache != EOF) {
              memory[0] = m_cache;
              m_cache = EOF;
              if(count > 1) {
                  return m_io->read(count - 1, memory + 1) + 1;
              }
              return 1;
          }
          return m_io->read(count);
      }
      return 0;
}

/* peek()
    get the value stored in the character buffer
*/
bool  pio::peek() noexcept
{
      if(__builtin_expect(m_cache == EOF, false)) {
          m_cache = m_io->get_char();
      }
      if(__builtin_expect(m_cache != EOF, true)) {
          return  true;
      }
      return false;
}

/* peek()
    get the value stored in the character buffer
*/
bool  pio::peek(int& value) noexcept
{
      bool l_result = peek();
      value = m_cache;
      return l_result;
}

/* skip()
    extract the next value from the stream and store it into the character buffer
*/
void  pio::skip() noexcept
{
      get_char();
}

/* skip()
    skip and increment the counter
*/
void  pio::skip(int& counter) noexcept
{
      skip();
      counter++;
}

/* get_any()
*/
int   pio::get_any(int& value) noexcept
{
      if(peek(value)) {
          skip();
          return 1;
      }
      return 0;
}

/* get_space()
    skip over the spaces in the stream
*/
int   pio::get_space() noexcept
{
      int  l_char;
      int  l_count = 0;
      while(peek(l_char)) {
          if((l_char == SPC) || (l_char == TAB)) {
              skip(l_count);
          } else
              break;
      }
      return l_count;
}

/* get_keyword()
   read a string terminated by spaces
*/
int   pio::get_keyword(const char* imm) noexcept
{
      int  l_char;
      int  l_offset;
      bool l_pass  = false;
      int  l_count = 0;
      if(imm) {
          save(l_offset);
          while(peek(l_char)) {
              if(l_char != imm[l_count]) {
                  if(l_count) {
                      l_pass = imm[l_count] <= SPC;
                  }
                  break;
              } else
              if(l_char == imm[l_count]) {
                  skip(l_count);
              }
          }
          if(l_pass) {
              return l_count;
          }
          restore(l_offset);
      }
      return 0;
}

/* get_symbol()
   read a character symbol
*/
int   pio::get_symbol(char imm) noexcept
{
      int l_char;
      int l_offset;
      int l_imm = imm;
      if(imm) {
          save(l_offset);
          peek(l_char);
          if(l_char == l_imm) {
              skip();
              return 1;
          }
          restore(l_offset);
      }
      return 0;
}

/* get_symbol()
   read a string terminated by any character not belonging to it
*/
int   pio::get_symbol(const char* imm) noexcept
{
      int  l_char;
      int  l_offset;
      bool l_pass  = true;
      int  l_count = 0;
      if(imm) {
          save(l_offset);
          while(peek(l_char)) {
              if(imm[l_count]) {
                  if(l_char != imm[l_count]) {
                      break;
                  } else
                  if(l_char == imm[l_count]) {
                      skip(l_count);
                  }
              } else
                  break;
          }
          if(l_pass) {
              return l_count;
          }
          restore(l_offset);
      }
      return 0;
}

/* get_ident()
   read idetifier into the buffer pointed to by <ident>, with a max length of <length>
*/
int   pio::get_ident(char* ident, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      if(length > 0) {
          if(peek(l_char)) {
              if(std::strchr(s_ident_0, l_char)) {
                  ident[l_count] = l_char;
                  skip(l_count);
              } else
                  return 0;
          } else
              return 0;
          while(peek(l_char)) {
              if(std::strchr(s_ident_1, l_char) == nullptr) {
                  if(l_count >= length) {
                      ident[0] = 0;
                      return 0;
                  }
                  ident[l_count] = 0;
                  return l_count;
              }
              if(l_count < length) {
                  if(l_count < length - 1) {
                      ident[l_count] = l_char;
                  } else
                      ident[l_count] = 0;
              }
              skip(l_count);
          }
      }
      return 0;
}

/* get_word()
   read string in between spaces into the buffer pointed to by <word>, with a max length of <length>
*/
int   pio::get_word(char* word, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      if(length > 0) {
          while(peek(l_char)) {
              if((l_char == m_space[0]) || (l_char == m_space[1])) {
                  if(l_count >= length) {
                      word[0] = 0;
                      return 0;
                  }
                  word[l_count] = 0;
                  return l_count;
              }
              if(l_count < length) {
                  if(l_count < length - 1) {
                      word[l_count] = l_char;
                  } else
                      word[l_count] = 0;
              }
              skip(l_count);
          }
      }
      return 0;
}

/* get_text()
   read string in between quotes into the buffer pointed to by <text>, with a max length of <length>
*/
int   pio::get_text(char* text, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_state = 1;
      int  l_offset;
      if(length > 0) {
          save(l_offset);
          while(l_state && peek(l_char)) {
              // state 1: expect opening quote
              if(l_state == 1) {
                  if(l_char == m_quote[0]) {
                      l_state = 2;
                  } else
                      break;
              } else
              // state 2: expect regular character or closing quote
              if(l_state == 2) {
                  if(l_char == m_quote[1]) {
                      if(l_count < length) {
                          text[l_count] = 0;
                      } else
                      if(l_count >= length) {
                          text[0] = 0;
                      }
                      l_state = 0;
                  } else
                  if((l_char >= SPC) &&
                      (l_char <= ASCII_MAX - 1)) {
                      if(l_count < length) {
                          text[l_count] = l_char;
                      }
                      ++l_count;
                  }
              }
              skip();
          }
          if(l_state == 0) {
              return l_count;
          }
          restore(l_offset);
      }
      return 0;
}

/* get_bin_value()
    read the digits of a binary number and save the conversion result into <value>
*/
int   pio::get_bin_value(std::int32_t& value, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      while(peek(l_char)) {
          if(l_count < length) {
              if(l_count <= static_cast<int>(sizeof(value)) * 8) {
                  if((l_char >= '0') && (l_char <= '1')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 2) {
                          return 0;
                      }
                      l_value *= 2;
                      l_value += l_char - '0';
                      l_count++;
                      skip();
                  } else
                      break;
              } else
                  return 0;
          } else
              return 0;
      }
      value = l_value;
      return  l_count;
}

/* get_dec_value()
    read the digits of a decimal number and save the conversion result into <value>
*/
int   pio::get_dec_value(std::int32_t& value) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      int  l_digit;
      while(peek(l_char)) {
          if((l_char >= '0') && (l_char <= '9')) {
              if(l_value < std::numeric_limits<std::int32_t>::max() / 10) {
                  l_digit = l_char - '0';
                  if(l_digit < std::numeric_limits<std::int32_t>::max() / 10 - l_value) {
                      l_value *= 10;
                      l_value += l_digit;
                  } else
                      return 0;
              } else
                  return 0;
              l_count++;
              skip();
          } else
              break;
      }
      value = l_value;
      return  l_count;
}

/* get_hex_value()
    read the digits of a hex number and save the conversion result into <value>
*/
int   pio::get_hex_value(std::int32_t& value, int length) noexcept
{
      int  l_char;
      int  l_count = 0;
      int  l_value = 0;
      while(peek(l_char)) {
          if(l_count < length) {
              if(l_count <= static_cast<int>(sizeof(value)) * 2) {
                  if((l_char >= '0') && (l_char <= '9')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += l_char - '0';
                      l_count++;
                      skip();
                  } else
                  if((l_char >= 'A') && (l_char <= 'F')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += 10 + l_char - 'A';
                      l_count++;
                      skip();
                  } else
                  if((l_char >= 'a') && (l_char <= 'f')) {
                      if(l_value > std::numeric_limits<std::int32_t>::max() / 16) {
                          return 0;
                      }
                      l_value *= 16;
                      l_value += 10 + l_char - 'a';
                      l_count++;
                      skip();
                  } else
                      break;
              } else
                  return 0;
          } else
              return 0;
      }
      value = l_value;
      return  l_count;
}

int   pio::get_int(std::int32_t& value) noexcept
{
      int  l_char;
      int  l_length;
      if(peek(l_char)) {
          if(l_char == '+') {
              skip();
              l_length = get_dec_value(value);
              if(l_length) {
                  return l_length + 1;
              }
          } else
          if(l_char == '-') {
              skip();
              l_length = get_dec_value(value);
              if(l_length) {
                  value *= -1;
                  return l_length + 1;
              }
          } else
          if(l_char == '0') {
              skip();
              if(peek(l_char)) {
                  if((l_char == 'b') || (l_char == 'B')) {
                      skip();
                      return get_bin_value(value);
                  } else
                  if((l_char == 'x') || (l_char == 'X')) {
                      skip();
                      return get_hex_value(value);
                  }
                  value = 0;
                  return 1;
              }
          } else
          if((l_char >= '1') && (l_char <= '9')) {
              return get_dec_value(value);
          }
      }
      value = 0;
      return 0;
}

int   pio::get_float(float&) noexcept
{
      return 0;
}

/* get_line()
   skip over everything until the end of the line
*/
int   pio::get_line() noexcept
{
      int  l_char;
      int  l_count = 0;
      while(peek(l_char)) {
          if(l_char != EOL) {
              if(l_char == RET) {
                  skip();
              } else
              if(l_char == EOL) {
                  skip();
                  break;
              } else
                  skip(l_count);
          } else
          if(l_char >= EOS) {
              skip();
              break;
          } else
              break;
      }
      return l_count;
}

/* get_line()
   copy line into the buffer
*/
int   pio::get_line(char* line, int length) noexcept
{
      int  l_char;
      int  l_count;
      if(length > 0) {
          l_count = 0;
          while(peek(l_char)) {
              if(l_char >= SPC) {
                  if(l_count < length) {
                      if(l_count < length - 1) {
                          line[l_count] = l_char;
                      } else
                          line[l_count] = 0;
                  }
                  skip(l_count);
              } else
              if(l_char >= EOS) {
                  skip();
                  break;
              } else
                  break;
          }
          return l_count;
      }
      return 0;
}

/* get_eol()
   skip whitespaces until finding en EOL;
   return false if anything else is encountered along the way
*/
bool  pio::get_eol() noexcept
{
      int  l_char;
      int  l_offset = save();
      while(peek(l_char)) {
          if(l_char == EOL) {
              skip();
              return true;
          } else
          if((l_char == RET) ||
              (l_char == SPC) ||
              (l_char == TAB)) {
              skip();
          } else
          if(l_char != EOS) {
              restore(l_offset);
              return false;
          } else
              return true;
      }
      return true;
}

int   pio::put_char(char value) noexcept
{
      return m_io->put_char(value);
}

int   pio::put_byte(unsigned char value) noexcept
{
      return m_io->put_char(value);
}

int   pio::write(int size, const char* data) noexcept
{
      return m_io->write(size, data);
}

int   pio::get_size() noexcept
{
      return m_io->get_size();
}

bool  pio::is_seekable() const noexcept
{
      return m_io->is_seekable();
}

bool  pio::is_readable() const noexcept
{
      return m_io->is_readable();
}

bool  pio::is_writable() const noexcept
{
      return m_io->is_writable();
}

void  pio::set_space_charset(char c0, char c1) noexcept
{
      m_space[0] = c0;
      m_space[1] = c1;
}

void  pio::set_quote_charset(char c0, char c1) noexcept
{
      m_quote[0] = c0;
      m_quote[1] = c1;
}

void  pio::clear() noexcept
{
      m_cache = EOF;
}

void  pio::reset(bool clear) noexcept
{
      if(clear) {
          release();
      }
}

void  pio::release() noexcept
{
      m_io   = nullptr;
      m_cache = EOF;
}

      pio::operator bool() const noexcept
{
      return m_io;
}

pio&  pio::operator=(const pio& rhs) noexcept
{
      m_io = rhs.m_io;
      m_cache = rhs.m_cache;
      return *this;
}

pio&  pio::operator=(pio&& rhs) noexcept
{
      m_io = rhs.m_io;
      m_cache = rhs.m_cache;
      rhs.release();
      return *this;
}
