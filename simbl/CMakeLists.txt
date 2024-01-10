set( SIMBL_SOURCES
        CollisionData.cpp
        Collisions.cpp
        ConfigLoader.cpp
        Configuration.cpp
        EField.cpp
        Electrons.cpp
        Excitations.cpp
        ExcitedParticles.cpp
        Geometry.cpp
        MapReader.cpp
        Molecules.cpp
        Photons.cpp
        Simulation.cpp
        Transitions.cpp
        TupleReader.cpp
        Util.cpp
)

add_executable(simbl_debug Cli.cpp ${SIMBL_SOURCES})

add_executable(simbl Cli.cpp ${SIMBL_SOURCES})
target_compile_options(simbl PRIVATE "-O2")
target_compile_options(simbl PRIVATE "-march=native")  # compile with available vectorization

add_executable(townsend Townsend.cpp ${SIMBL_SOURCES})
target_compile_options(townsend PRIVATE "-O2")
target_compile_options(townsend PRIVATE "-march=native")  # compile with available vectorization

add_executable(moss MossValidation.cpp ${SIMBL_SOURCES})
target_compile_options(moss PRIVATE "-O2")
target_compile_options(moss PRIVATE "-march=native")  # compile with available vectorization
