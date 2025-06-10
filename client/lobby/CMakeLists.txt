file(GLOB LOBBY_SOURCES *.cpp)
file(GLOB LOBBY_HEADERS *.h)

target_sources(taller_client
    PRIVATE
        ${LOBBY_SOURCES}
        ${LOBBY_HEADERS}
)
