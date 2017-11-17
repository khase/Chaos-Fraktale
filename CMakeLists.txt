# add solutions
file(GLOB SOLUTIONS *.cpp)

foreach(solution ${SOLUTIONS})
    get_filename_component(name ${solution} NAME_WE)

    add_executable(${name} ${solution})
    target_link_libraries(${name} ${LIBRARIES_TO_LINK})
    target_include_directories(${name} PUBLIC ${DIRECTORIES_TO_INCLUDE})
endforeach()
