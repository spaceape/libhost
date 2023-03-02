set(NAME adt)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(ADT_SRC_DIR ${SYS_SRC_DIR}/${NAME})

set(inc
)

if(SDK)
  file(MAKE_DIRECTORY ${ADT_SRC_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${ADT_SRC_DIR}
  )
endif(SDK)
