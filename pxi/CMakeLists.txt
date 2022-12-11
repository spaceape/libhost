set(NAME pxi)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(PXI_SRC_DIR ${HOST_SRC_DIR}/${NAME})

set(inc
  producer.h consumer.h queue.h
)

if(SDK)
  file(MAKE_DIRECTORY ${PXI_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${PXI_SRC_DIR}
  )
endif(SDK)
