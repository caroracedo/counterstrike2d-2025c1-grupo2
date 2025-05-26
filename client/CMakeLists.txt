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
    game_view.cpp
    input_handler.h
    legs_view.h
    animation.h
    game_camera.cpp
    event_handler.h
    mock_handler.h
    update_handler.h
    )
