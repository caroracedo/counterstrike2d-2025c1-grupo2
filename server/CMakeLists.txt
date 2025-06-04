file(GLOB SERVER_SOURCES *.cpp)
file(GLOB SERVER_HEADERS *.h)

target_sources(taller_server
    PRIVATE
        ${SERVER_SOURCES}
    PUBLIC
        ${SERVER_HEADERS}
)
