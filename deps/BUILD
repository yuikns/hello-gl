package(default_visibility = ["//visibility:public"])

# TODO(yu): other deps: https://github.com/nothings/stb

cc_library(
    name = "khrplatform",
    hdrs = [
      "KHR/khrplatform.h",
    ],
    includes = [
      "."
    ],
)


cc_library(
    name = "glad",
    srcs = [
      "glad.c",
    ],
    hdrs = [
      "glad/glad.h",
    ],
    deps = [
      ":khrplatform",
    ],
    includes = [
      "."
    ],
)

cc_library(
    name = "linmath",
    hdrs = [
      "linmath.h",
    ],
)

cc_library(
    name = "nuklear",
    hdrs = [
      "nuklear.h",
      "nuklear_glfw_gl2.h",
    ],
)

cc_library(
    name = "stb_image_write",
    hdrs = [
      "stb_image_write.h",
    ],
)

cc_library(
    name = "tinycthread",
    srcs = [
      "tinycthread.c",
    ],
    hdrs = [
      "tinycthread.h",
    ],
)


cc_library(
    name = "deps",
    deps = [
      ":khrplatform",
      ":glad",
      ":linmath",
      ":nuklear",
      ":stb_image_write",
      ":tinycthread",
    ],
)
