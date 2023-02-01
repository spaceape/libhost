set(NAME sys)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(SYS_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  arg.h argv.h var.h fmt.h descriptor.h process.h ios.h asio.h
)

if(SDK)
  file(MAKE_DIRECTORY ${SYS_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${SYS_SDK_DIR}
  )
endif(SDK)
