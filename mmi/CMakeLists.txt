set(NAME memory)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(MMI_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  metrics.h policy.h resource.h
  flat_list_traits.h flat_list.h
  flat_set_traits.h flat_set.h flat_map_traits.h flat_map.h hash_map.h
  linked_list_traits.h linked_list_base.h linked_list.h ordered_list.h
  pool_base.h pool.h page.h
  page.h
)

add_subdirectory(manager)

if(SDK)
  file(MAKE_DIRECTORY ${MMI_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${MMI_SDK_DIR}
  )
endif(SDK)
