project(host)
set(NAME "host")
set(PREFIX)

add_definitions(
  -fPIC
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
  -DPIC
)

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
  sys/asio.cpp sys/ios/rio.cpp sys/ios/sio.cpp sys/ios/fio.cpp sys/ios/tty.cpp sys/ios/net.cpp sys/ios/bio.cpp sys/ios/pio.cpp
  sys/var.cpp sys/descriptor.cpp sys/ios.cpp sys/sys.cpp
  tmp.cpp
)

set(libs
  m dl rt pthread
)

add_subdirectory(dpu)
add_subdirectory(fpu)
add_subdirectory(gpu)
add_subdirectory(mmi)
add_subdirectory(pxi)
add_subdirectory(sys)

add_library(${NAME} STATIC ${srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(${NAME} ${libs})

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
