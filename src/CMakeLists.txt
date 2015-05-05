add_executable( server server.cpp io.cpp )
target_link_libraries( server boost_system pthread )

add_executable( client client.cpp )
target_link_libraries( client boost_system pthread )

add_executable( test test.cpp )
target_link_libraries( test boost_system pthread )
