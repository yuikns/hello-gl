MESSAGE(STATUS "[GLEW] configuration , ref: http://www.glfw.org/docs/latest/build.html#build_link_cmake_source ")
MESSAGE(STATUS "[GLEW] coding , ref: http://www.glfw.org/docs/latest/quick.html")

# look for FreeType
FIND_PACKAGE(Freetype REQUIRED)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIRS})

ADD_EXECUTABLE(hello_glfw3 WIN32 MACOSX_BUNDLE hello_glfw3.cc)
#ADD_EXECUTABLE(hello_glfw3 hello_glfw3.cc)

TARGET_LINK_LIBRARIES(hello_glfw3 ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(hello_glfw3 ${GLFW_LIBRARIES})
TARGET_LINK_LIBRARIES(hello_glfw3 glfw)
TARGET_LINK_LIBRARIES(hello_glfw3 ${FREETYPE_LIBRARIES})


#SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES OUTPUT_NAME "hello_glfw3")

if (APPLE)
    SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "hello_glfw3")
    SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
                          MACOSX_BUNDLE_INFO_PLIST "${GLFW_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in")
endif()


ADD_EXECUTABLE(hello_ft hello_ft.cc)
TARGET_LINK_LIBRARIES(hello_ft ${FREETYPE_LIBRARIES})
TARGET_LINK_LIBRARIES(hello_ft glfw)




