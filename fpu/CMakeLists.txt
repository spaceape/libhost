set(NAME fpu)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(FPU_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  soft.h
)

if(SDK)
  file(MAKE_DIRECTORY ${FPU_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${FPU_SDK_DIR}
  )
endif(SDK)
