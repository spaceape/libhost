set(NAME sys)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(SYS_SRC_DIR ${HOST_SRC_DIR}/${NAME})

set(inc
  var.h fmt.h descriptor.h ios.h asio.h
)

if(SDK)
  file(MAKE_DIRECTORY ${SYS_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${SYS_SRC_DIR}
  )
endif(SDK)
