file(GLOB PRE_LOBBY_SOURCES *.cpp *.ui)
file(GLOB PRE_LOBBY_HEADERS *.h)

target_sources(taller_client
    PRIVATE
        ${PRE_LOBBY_SOURCES}
        ${PRE_LOBBY_HEADERS}
)
