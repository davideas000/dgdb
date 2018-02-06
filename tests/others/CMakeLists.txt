add_executable( test_0 test_0.cpp )
target_link_libraries( test_0 ${local_libs})

if(WIN32)
    add_executable( test_win test_win.cpp )
    target_link_libraries( test_win ${local_libs})
endif()
