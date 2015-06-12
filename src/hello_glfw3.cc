#include <cstdio>
#include <unistd.h>

#include <string>

#include <GLFW/glfw3.h>

void error_callback(int error, const char* description) {
    fprintf(stderr, "[GLFW] error message: %s\n", description);
}

std::string key_callback_mod_parse(int mods) {
    std::string m;
    m += mods & GLFW_MOD_SHIFT ? "[SHIFT]" : "_";
    m += mods & GLFW_MOD_CONTROL ? "[CTRL]" : "_";
    m += mods & GLFW_MOD_ALT ? "[ALT]" : "_";
    m += mods & GLFW_MOD_SUPER ? "[SUPER]" : "_";
    return m;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("[KEY]: %c %d %d %s %s --- %.3f\n", key, key, scancode,
           action == GLFW_PRESS ? "GLFW_PRESS" : (action == GLFW_RELEASE ? "GLFW_RELEASE" : "GLFW_REPEAT"),
           key_callback_mod_parse(mods).c_str(), glfwGetTime());
    fflush(NULL);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // http://www.glfw.org/docs/latest/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == 'V' && action == GLFW_PRESS) {
        int w;
        int h;
        glfwGetFramebufferSize(window, &w, &h);
        printf("glfwGetFramebufferSize: %d %d\n", w, h);
        fflush(NULL);
    }
    if (key == 'H' && action == GLFW_PRESS) {
        glfwHideWindow(window);
    } else if (key == 'S' && action == GLFW_PRESS) {
        glfwShowWindow(window);
    }
}

int main(int argc, char* argv[]) {
    printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        fprintf(stderr, "GLFW init failed\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello, OpenGL!", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    // http://www.glfw.org/docs/latest/group__context.html#ga1c04dc242268f827290fe40aa1c91157
    glfwMakeContextCurrent(window);

    // http://www.glfw.org/docs/latest/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
    glfwSwapInterval(1);

    // http://www.glfw.org/docs/latest/group__init.html#gaa5d796c3cf7c1a7f02f845486333fb5f
    glfwSetErrorCallback(error_callback);

    // http://www.glfw.org/docs/latest/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
    glfwSetKeyCallback(window, key_callback);

    // Loop until the user closes the window
    // http://www.glfw.org/docs/latest/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(1.f, 1.f, 0.f);
        glVertex2f(-width/3, -height/3);
        glVertex2f(width/3, height/3);
        glEnd();

        // Swap front and back buffers
        // http://www.glfw.org/docs/latest/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
        glfwSwapBuffers(window);

        // Poll for and process events
        // http://www.glfw.org/docs/latest/group__window.html#ga37bd57223967b4211d60ca1a0bf3c832
        glfwPollEvents();
    }
    printf("jump out of the loop\n");

    // http://www.glfw.org/docs/latest/group__window.html#gacdf43e51376051d2c091662e9fe3d7b2
    glfwDestroyWindow(window);

    // http://www.glfw.org/docs/latest/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
    glfwTerminate();
    return 0;
}
