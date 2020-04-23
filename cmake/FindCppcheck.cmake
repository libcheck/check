find_program(
  Cppcheck_EXECUTABLE
  NAMES cppcheck
  DOC "Cppcheck static analysis tool (http://cppcheck.sourceforge.net)"
)

if (Cppcheck_EXECUTABLE)
  execute_process(
    COMMAND ${Cppcheck_EXECUTABLE} --version
    OUTPUT_VARIABLE cppcheck_version_out
    RESULT_VARIABLE cppcheck_version_error
    ERROR_VARIABLE cppcheck_version_suppress
  )
  if (NOT cppcheck_version_error)
    string(REPLACE "\n" "" cppcheck_version_out "${cppcheck_version_out}")
    string(REGEX REPLACE "Cppcheck (.+)" "\\1" cppcheck_version "${cppcheck_version_out}")
  endif ()
endif ()

if (cppcheck_version)
  set(Cppcheck_FOUND 1 CACHE INTERNAL "Cppcheck version ${cppcheck_version} found")
  set(Cppcheck_VERSION "${cppcheck_version}" CACHE INTERNAL "Cppcheck version number")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Cppcheck
  REQUIRED_VARS Cppcheck_EXECUTABLE
  VERSION_VAR Cppcheck_VERSION
)

