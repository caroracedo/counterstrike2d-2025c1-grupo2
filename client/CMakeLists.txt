file(GLOB CLIENT_SOURCES *.cpp)
file(GLOB CLIENT_HEADERS *.h)

target_sources(taller_client
    PRIVATE
        ${CLIENT_SOURCES}
    PUBLIC
        ${CLIENT_SOURCES}
)

add_subdirectory(prelobby)

add_subdirectory(lobby)

add_subdirectory(graphics)
