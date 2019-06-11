set(conf_file "checkmk.in" FILEPATH)
set(configure_input "Generated from ${conf_file} by configure.")
find_program(AWK_PATH awk)

configure_file(checkmk.in checkmk @ONLY)

file(COPY doc/checkmk.1 DESTINATION man/man1)

option(INSTALL_CHECKMK "Install checkmk" ON)
include(GNUInstallDirs)
if(INSTALL_CHECKMK)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/checkmk
    TYPE BIN
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )
  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man1
    TYPE MAN
  )
endif(INSTALL_CHECKMK)

