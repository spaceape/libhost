set(NAME dev)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(DEV_SDK_DIR ${SYS_SDK_DIR}/${NAME})

set(inc
  bcm2835.h
)

if(SDK)
  file(MAKE_DIRECTORY ${DEV_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${DEV_SDK_DIR}
  )
endif(SDK)
