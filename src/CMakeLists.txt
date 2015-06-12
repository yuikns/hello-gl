
IF(USE_OPENGL_GLUT)
    ADD_EXECUTABLE(hello_glut hello_glut.cc)

    TARGET_LINK_LIBRARIES(hello_glut ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

    SET_TARGET_PROPERTIES(hello_glut PROPERTIES OUTPUT_NAME "hello_glut")
ENDIF()

IF(USE_OPENGL_GLFW)
    MESSAGE(STATUS "[GLEW] configuration , ref: http://www.glfw.org/docs/latest/build.html#build_link_cmake_source ")
    MESSAGE(STATUS "[GLEW] coding , ref: http://www.glfw.org/docs/latest/quick.html")

    ADD_EXECUTABLE(hello_glfw3 hello_glfw3.cc)

    TARGET_LINK_LIBRARIES(hello_glfw3 ${OPENGL_LIBRARIES})
    TARGET_LINK_LIBRARIES(hello_glfw3 ${GLFW_LIBRARIES})
    #TARGET_LINK_LIBRARIES(hello_glfw3 ${GLFW_STATIC_LIBRARIES})

    SET_TARGET_PROPERTIES(hello_glfw3 PROPERTIES OUTPUT_NAME "hello_glfw3")
ENDIF()

