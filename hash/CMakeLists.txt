set(NAME hash)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(HASH_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  fnv.h md5.h md5_digest.h
)

if(SDK)
  file(MAKE_DIRECTORY ${HASH_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${HASH_SDK_DIR}
  )
endif(SDK)
