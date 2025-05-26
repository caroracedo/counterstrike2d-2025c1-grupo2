target_sources(taller_common
    PRIVATE
    # .cpp files
    liberror.cpp
    resolver.cpp 
    resolvererror.cpp 
    socket_manager.cpp 
    socket.cpp 
    
    PUBLIC
    # .h files
    action_DTO.h 
    constants.h
    game.h 
    liberror.h 
    main_menu_DTO.h 
    monitor_game.h 
    monitor_games.h 
    object_DTO.h
    object.h
    obstacle.h
    player.h
    queue.h 
    resolver.h 
    resolvererror.h 
    socket_manager.h 
    socket.h 
    thread.h 
    weapon.h
    )
