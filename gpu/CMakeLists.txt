set(NAME gpu)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(GPU_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  gl.h
)

if(SDK)
  file(MAKE_DIRECTORY ${GPU_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GPU_SDK_DIR}
  )
endif(SDK)
