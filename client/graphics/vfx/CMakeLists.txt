file(GLOB VFX_SOURCES *.cpp)
file(GLOB VFX_HEADERS *.h)

target_sources(taller_client
    PRIVATE
        ${VFX_SOURCES}
        ${VFX_HEADERS}
)
