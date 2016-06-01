MESSAGE(STATUS "[GLEW] configuration , ref: http://www.glfw.org/docs/latest/build.html#build_link_cmake_source ")
MESSAGE(STATUS "[GLEW] coding , ref: http://www.glfw.org/docs/latest/quick.html")

ADD_EXECUTABLE(hello_glfw3 WIN32 MACOSX_BUNDLE hello_glfw3.cc)
#ADD_EXECUTABLE(hello_glfw3 hello_glfw3.cc)

TARGET_LINK_LIBRARIES(hello_glfw3 ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(hello_glfw3 ${GLFW_LIBRARIES})
TARGET_LINK_LIBRARIES(hello_glfw3 glfw)


#SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES OUTPUT_NAME "hello_glfw3")

if (APPLE)
    SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "hello_glfw3")
    SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
                          MACOSX_BUNDLE_INFO_PLIST "${GLFW_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in")
endif()
