file(GLOB COMMON_SOURCES *.cpp)
file(GLOB COMMON_HEADERS *.h)

target_sources(taller_common
    PRIVATE
        ${COMMON_SOURCES}
    PUBLIC
        ${COMMON_HEADERS}
)
