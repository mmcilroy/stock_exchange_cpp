add_executable( server server.cpp messages.cpp io.cpp event.cpp )
target_link_libraries( server boost_system pthread )

add_executable( client client.cpp messages.cpp event.cpp )
target_link_libraries( client boost_system pthread )
