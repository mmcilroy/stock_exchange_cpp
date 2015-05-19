add_executable( client client.cpp )
target_link_libraries( client boost_system pthread )

add_executable( server server.cpp )
target_link_libraries( server boost_system pthread )

add_executable( test_journal test_journal.cpp )
target_link_libraries( test_journal boost_system pthread )



#add_executable( test_events test_events.cpp )
#target_link_libraries( test_events boost_system pthread )

#add_executable( test_book test_book.cpp )
#target_link_libraries( test_book boost_system pthread )
