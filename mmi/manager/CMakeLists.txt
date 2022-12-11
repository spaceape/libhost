set(NAME manager)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(MANAGER_SDK_DIR ${MMI_SDK_DIR}/${NAME})

set(inc
  heap.h map.h shared.h
)

if(SDK)
  file(MAKE_DIRECTORY ${MANAGER_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${MANAGER_SDK_DIR}
  )
endif(SDK)
