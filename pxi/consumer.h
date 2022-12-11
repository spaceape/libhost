#ifndef pxi_consumer_h
#define pxi_consumer_h
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
#include <pxi.h>
#include "producer.h"
#include <traits.h>
#include <log.h>

namespace pxi {

/* has_id_support
   detect if custom callable type supports unique identifiers, in order to enable cancellation
*/
template<typename Xt>
class has_id_support
{
  template<typename Ot>
  static  auto test(int) -> decltype(std::declval<Ot>().get_id(), std::false_type());

  template<typename Ot, typename Pt>
  static  auto test(...) -> std::false_type;

  public:
  static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), std::size_t>::value;
};

/* has_weight_support
   detect if custom callable type supports weight measurement, in order to enable more
   accurate scheduling
*/
template<typename Xt>
class has_weight_support
{
  template<typename Ot>
  static  auto test(int) -> decltype(std::declval<Ot>().get_weight(), std::false_type());

  template<typename Ot, typename Pt>
  static  auto test(...) -> std::false_type;

  public:
  static  constexpr bool value = std::is_same<decltype(test<Xt>(0)), float>::value;
};

template<typename Xt, typename Ct>
class consumer: public producer<Xt, Ct>
{
  private:
  using   base_type     = producer<Xt, Ct>;

  public:
  using   consumer_type = typename base_type::consumer_type;
  using   callable_type = typename base_type::callable_type;
  using   iterator      = typename base_type::iterator_type;

  public:
  inline  consumer(int reserve) noexcept:
          base_type(reserve) {
  }

          consumer(const consumer&) noexcept = delete;
          consumer(consumer&&) noexcept = delete;

  inline  ~consumer() {
  }

  template<typename... Args>
          bool schedule(Args&&... args) noexcept {
          bool l_result = false;
          if(static_cast<int>(base_type::m_task_list.size()) < base_type::m_capacity_max) {
              base_type::m_list_guard.lock();
              base_type::m_task_list.emplace_back(std::forward<Args>(args)...);
              base_type::m_load += 1.0f;
              l_result = true;
              base_type::m_list_guard.unlock();
          }
          return l_result;
  }

  inline  iterator begin() noexcept {
          return   base_type::m_task_list.begin();
  }

  inline  void     process() noexcept {
          auto  i_callable  = base_type::m_task_list.begin();
          while(i_callable != base_type::m_task_list.end()) {
              i_callable->operator()();
              i_callable++;
          }
          base_type::m_task_list.clear();
  }

  inline  void     process(float timeout) noexcept {          
          process(std::chrono::steady_clock::now(), timeout);
  }

  inline  void     process(const std::chrono::time_point<std::chrono::steady_clock>& time, float timeout) noexcept {          
          std::chrono::time_point<std::chrono::steady_clock> l_time_0 = time;
          std::chrono::time_point<std::chrono::steady_clock> l_time_1 = time;
          std::chrono::duration<float>                       l_elapse;
          if(timeout >= 0.0f) {
              auto  i_callable  = base_type::m_task_list.begin();
              while(i_callable != base_type::m_task_list.end()) {
                  i_callable->operator()();
                  l_time_1 = std::chrono::steady_clock::now();
                  l_elapse = std::chrono::duration<float>(l_time_1 - l_time_0);
                  if(l_elapse.count() >= timeout) {
                      break;
                  }
                  i_callable++;
              }
              if(i_callable < base_type::m_task_list.end()) {
                  base_type::m_task_list.erase(base_type::m_task_list.begin(), i_callable);
              } else
                  base_type::m_task_list.clear();
          }
  }

  inline  iterator end() noexcept {
          return   base_type::m_task_list.end();
  }
 
  inline  int      count() const noexcept {
          return   static_cast<int>(base_type::m_task_list.size());
  }

          consumer& operator=(const consumer&) noexcept = delete;
          consumer& operator=(consumer&&) noexcept = delete;
};

template<typename Xt>
class consumer<Xt, std::thread>: public producer<Xt, std::thread>
{
  private:
  using   base_type = producer<Xt, std::thread>;

  public:
  using   consumer_type = typename base_type::consumer_type;
  using   callable_type = typename base_type::callable_type;
  using   iterator      = typename base_type::iterator_type;

  private:
  std::thread  m_thread;
  int          m_live_index;
  float        m_wait_time;
  float        m_exit_time;
  bool         m_wake;
  bool         m_busy;

  private:
          void loop() noexcept {
          std::chrono::time_point<std::chrono::steady_clock> l_time_0 = std::chrono::steady_clock::now();
          std::chrono::time_point<std::chrono::steady_clock> l_time_1;
          std::chrono::duration<float>                       l_idle_time;

          std::chrono::duration<float>                       l_wait_time(m_wait_time);
          std::chrono::duration<float>                       l_exit_time(m_exit_time);

          std::unique_lock<std::mutex>                       l_list_guard(base_type::m_list_guard);

          m_wake       = true;
          m_busy       = true;
          m_live_index = 0;

          printdbg("--- queue %p loop enter", this);
          do {
              if(m_live_index < static_cast<int>(base_type::m_task_list.size())) {
                  // run through the list of tasks and execute them in the order they are stored
                  do {
                      if(m_wake) {
                          base_type::m_task_list[m_live_index++]();
                          base_type::m_load -= 1.0f;
                      } else
                          break;
                  }
                  while(m_live_index < static_cast<int>(base_type::m_task_list.size()));

                  // no longer busy
                  m_busy = false;

                  // discard tasks that were already executed in order to make room for potential new ones
                  if(m_live_index == static_cast<int>(base_type::m_task_list.size())) {
                      base_type::m_task_list.clear();
                      m_live_index = 0;
                  } else
                  if(m_live_index >= base_type::m_capacity_min) {
                      base_type::m_task_list.erase(
                          base_type::m_task_list.begin(),
                          base_type::m_task_list.begin() + m_live_index - 1
                      );
                      m_live_index = 0;
                  }
              }
              if(m_wake) {
                  // handle sleep and exit time
                  // if m_exit_time is 0.0f, exit the thread immediately;
                  // if m_exit_time is greater than 0.0f but not infinity, count the idle time and exit only if exceeded or equal;
                  // if m_exit_time is infinity don't count the time and exit only when m_wake is unset.
                  if(m_exit_time > 0.0f) {
                      if(base_type::m_list_fence.wait_for(l_list_guard, l_wait_time) == std::cv_status::no_timeout) {
                          l_time_0 = std::chrono::steady_clock::now();
                          l_time_1 = l_time_0;
                          m_busy   = m_wake;
                      } else
                      if(m_exit_time != std::numeric_limits<float>::infinity()) {
                          l_time_1    = std::chrono::steady_clock::now();
                          l_idle_time = std::chrono::duration<float>(l_time_1 - l_time_0);
                          if(l_idle_time < l_exit_time) {
                              m_busy = m_wake;
                          } else
                              m_busy = false;
                      } else
                          m_busy = m_wake;
                  }
              }
          }
          while(m_busy);
          m_wake = false;
          m_busy = false;
          printdbg("--- queue %p loop leave", this);
  }

  protected:
  inline  void resume_a() noexcept {
          join();
          m_wake = true;
          m_thread = std::thread(&consumer::loop, this);
          m_wake = m_thread.joinable();
  }

  inline  void resume() noexcept {
          if(base_type::m_list_guard.try_lock()) {
              if(m_wake == false) {
                  resume_a();
              }
              base_type::m_list_guard.unlock();
          }
  }
  
  inline  void suspend() noexcept {
          m_wake = false;
  }

  inline  void join() noexcept {
          if(m_thread.joinable()) {
              m_thread.join();
          }
  }

  public:
  static  constexpr float  default_wait_time = 0.1f;
  static  constexpr float  default_exit_time = 8.0f;

  public:
  inline  consumer(int reserve) noexcept:
          base_type(reserve),
          m_live_index(0),
          m_wait_time(default_wait_time),
          m_exit_time(default_exit_time),
          m_wake(false),
          m_busy(false) {
  }

  inline  consumer(int reserve, float wait_time, float exit_time, bool resume) noexcept:
          base_type(reserve),
          m_live_index(0),
          m_wait_time(wait_time),
          m_exit_time(exit_time),
          m_wake(false),
          m_busy(false) {
          if(resume) {
              resume_a();
          }
  }

          consumer(const consumer&) noexcept = delete;
          consumer(consumer&&) noexcept = delete;

  inline  ~consumer() {
          suspend();
          join();
  }

  template<typename... Args>
  inline  bool schedule(Args&&... args) noexcept {
          bool l_result = false;
          if(static_cast<int>(base_type::m_task_list.size()) < base_type::m_capacity_max) {
              base_type::m_list_guard.lock();
              base_type::m_task_list.emplace_back(std::forward<Args>(args)...);
              if(m_wake == true) {
                  if(m_busy == false) {
                      base_type::m_list_fence.notify_one();
                  }
              } else
              if(m_wake == false) {
                  resume_a();
              }
              l_result = true;
              base_type::m_load += 1.0f;
              base_type::m_list_guard.unlock();
          }
          return l_result;
  }

          consumer& operator=(const consumer&) noexcept = delete;
          consumer& operator=(consumer&&) noexcept = delete;
};

/*namespace pxi*/ }
#endif
