set(NAME pxi)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(PXI_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  producer.h consumer.h queue.h
)

if(SDK)
  file(MAKE_DIRECTORY ${PXI_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${PXI_SDK_DIR}
  )
endif(SDK)
