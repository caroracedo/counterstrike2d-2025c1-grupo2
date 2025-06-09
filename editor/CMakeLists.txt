file(GLOB EDITOR_SOURCES *.cpp)
file(GLOB EDITOR_HEADERS *.h)

target_sources(taller_editor
    PRIVATE
        ${EDITOR_SOURCES}
        ${RESOURCES}
    PUBLIC
        ${EDITOR_HEADERS}
)
