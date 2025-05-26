target_sources(taller_server
    PRIVATE
    # .cpp files
    main.cpp
    server_protocol.cpp
    PUBLIC
    # .h files
    acceptor.h
    client_handler.h
    game_loop.h
    monitor_client_send_queues.h
    server_protocol.h
    server_receiver.h
    server_sender.h
    server.h
    )
