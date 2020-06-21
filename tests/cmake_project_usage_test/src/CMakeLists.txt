if($ENV{INCLUDE_CHECK_WITH} STREQUAL "FetchContent")
    include(FetchContent)
    FetchContent_Declare(check
        URL            "$ENV{INCLUDE_CHECK_FROM}"
        # URL file://${DIR}/check
    )
    if(CMAKE_VERSION VERSION_LESS "3.14")
        FetchContent_GetProperties(check)
        if(NOT check_POPULATED)
            FetchContent_Populate(check)
            add_subdirectory(${check_SOURCE_DIR} ${check_BINARY_DIR})
        endif()
    else()
        FetchContent_MakeAvailable(check)
    endif()
elseif($ENV{INCLUDE_CHECK_WITH} STREQUAL "find_package_config")
    set(Check_ROOT "" CACHE PATH "Check installation root dir")
    find_package(Check 0.13.0 REQUIRED CONFIG)
else()
    message(FATAL_ERROR "Variable INCLUDE_CHECK_WITH not properly set!")
endif()

include(CheckCCompilerFlag)
check_c_compiler_flag("-pthread" HAVE_PTHREAD)
if (HAVE_PTHREAD)
    add_definitions("-pthread")
    add_link_options("-pthread")
endif()

add_library(test_suite test_suite.c)
target_link_libraries(test_suite PUBLIC Check::check)

add_executable(tests.test tests.c)
target_link_libraries(tests.test test_suite)

