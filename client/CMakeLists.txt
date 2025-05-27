target_sources(taller_client
    PRIVATE
    # .cpp files
    main.cpp
    client_protocol.cpp
    PUBLIC
    # .h files
    client_protocol.h
    client.h
    client_sender.h
    client_receiver.h
    event_handler.h
    # input_handler.h
    mock_handler.h
    update_handler.h
    )
