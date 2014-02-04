include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

set(TEST_SOURCES
  check_money.c
)

add_executable(check_money ${TEST_SOURCES})
target_link_libraries(check_money money ${CHECK_LIBRARIES})
