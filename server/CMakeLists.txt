target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    server_protocol.cpp
    PUBLIC
    # .h files
    server_protocol.h
    acceptor.h
    client_handler.h
    server_receiver.h
    server_sender.h
    server.h
    game_loop.h
    )
