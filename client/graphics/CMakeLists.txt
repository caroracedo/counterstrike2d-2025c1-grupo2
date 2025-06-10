file(GLOB GRAPHIC_SOURCES *.cpp)
file(GLOB GRAPHIC_HEADERS *.h)

target_sources(taller_client
    PRIVATE
        ${GRAPHIC_SOURCES}
        ${GRAPHIC_HEADERS}
)
