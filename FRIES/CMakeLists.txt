
list(APPEND FRIES_FILELIST
        compress_utils.cpp
        det_store.c
        fci_utils.c
        Hamiltonians/heat_bathPP.cpp
        Hamiltonians/hub_holstein.cpp
        Hamiltonians/near_uniform.cpp
        Hamiltonians/molecule.cpp
        io_utils.cpp
        vec_utils.cpp
        math_utils.c)

add_library(fries SHARED ${FRIES_FILELIST})

set_target_properties(fries
  PROPERTIES
PUBLIC_HEADER "compress_utils.hpp;det_store.h;fci_utils.h;io_utils.hpp;math_utils.h;vec_utils.hpp;hh_vec.hpp;det_hash.hpp;ndarr.hpp")

add_subdirectory(Ext_Libs)
add_subdirectory(Hamiltonians)

target_link_libraries(fries PUBLIC m)

find_package(MPI REQUIRED)
target_include_directories(fries PUBLIC ${MPI_INCLUDE_PATH})
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "CLANG")
    target_link_libraries(fries PUBLIC MPI::MPI_CXX)
else()
    target_link_libraries(fries PUBLIC ${MPI_LIBRARIES})
endif()



install(
    TARGETS
        fries
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
        COMPONENT bin
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        COMPONENT lib
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/FRIES
        COMPONENT dev
)
