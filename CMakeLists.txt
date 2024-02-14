project(host)
set(NAME "host")
set(PREFIX)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(HOST_SDK_DIR ${HOST_SDK_DIR}/${NAME})
set(HOST_CFG_DIR ${HOST_CFG_DIR})

set(inc
  global.h export.h error.h traits.h log.h dbg.h none.h dpu.h fpu.h gpu.h hash.h compare.h convert.h char.h tmp.h
  mmi.h pxi.h sys.h
)

set(srcs
  error.cpp log.cpp dbg.cpp dpu/${DPU}.cpp fpu/${FPU}.cpp gpu/${GPU}.cpp
  mmi/mmi.cpp
  sys/arg.cpp sys/argv.cpp
  sys/asio.cpp
  sys/ios/rio.cpp sys/ios/sio.cpp sys/ios/fio.cpp sys/ios/tty.cpp sys/ios/net.cpp sys/ios/bio.cpp sys/ios/pio.cpp
  sys/ios.cpp sys/sys.cpp
)

set(libs
  m
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)
add_subdirectory(mmi)
add_subdirectory(sys)
add_subdirectory(hash)

add_library(${NAME} STATIC ${srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(
  ${NAME}
  PUBLIC
    ${libs}
  PRIVATE
    pico_stdlib
)

if(SDK)
  file(MAKE_DIRECTORY ${HOST_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${HOST_SDK_DIR}
  )
endif(SDK)

install(
  TARGETS
    ${NAME}
  DESTINATION
    ${HOST_LIB_DIR}
)
