set(conf_file "checkmk.in" FILEPATH)
set(configure_input "Generated from ${conf_file} by configure.")
find_program(AWK_PATH awk)

configure_file(checkmk.in checkmk @ONLY)

file(COPY doc/checkmk.1
  DESTINATION man/man1
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

option(INSTALL_CHECKMK "Install checkmk" ON)
if(INSTALL_CHECKMK)
  # install(TARGETS checkmk RUNTIME DESTINATION bin)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/man1
      TYPE MAN)
endif(INSTALL_CHECKMK)

