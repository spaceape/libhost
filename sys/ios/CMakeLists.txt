set(NAME ios)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(IOS_SDK_DIR ${SYS_SDK_DIR}/${NAME})

set(inc
  rio.h sio.h fio.h tty.h net.h bio.h pio.h
)

if(SDK)
  file(MAKE_DIRECTORY ${SYS_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${IOS_SDK_DIR}
  )
endif(SDK)
