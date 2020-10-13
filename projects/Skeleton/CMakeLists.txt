# --------------------------------------------------------------------------
# Skeleton project
# --------------------------------------------------------------------------

project(skeleton)
FILE(GLOB Skeleton_headers code/*.h)
FILE(GLOB Skeleton_sources code/*.cc code/*.cpp)
FILE(GLOB Footman code/Footman/*.*)
FILE(GLOB Shaders code/Shaders/*.*)

SET(files_Skeleton ${Skeleton_headers} ${Skeleton_sources})
SET(files_Footman ${Footman})
SET(files_Shaders ${Shaders})
SOURCE_GROUP("code" FILES ${files_Skeleton})
SOURCE_GROUP("footman" FILES ${files_Footman})
SOURCE_GROUP("shaders" FILES ${files_Shaders})

foreach(FILE ${Footman})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)

foreach(FILE ${Shaders})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)


ADD_EXECUTABLE(Skeleton ${files_Skeleton} ${files_Footman} ${files_Shaders})
TARGET_LINK_LIBRARIES(Skeleton core render)
ADD_DEPENDENCIES(Skeleton core render)