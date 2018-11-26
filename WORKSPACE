workspace(name = "hello_gl")

load("//tools:argcv_archive.bzl", "argcv_archive")

argcv_archive(
  name = "com_github_argcv_argcv",
  sha256 = "40a22d11711dfc8f92d29308fab6cc981a5b4e2c74e02301e8e7a8dd98eb46f7",
  git_commit = "19a94bc980cea73a3a2f0c9b2b118c6446382ef6",
)

load("@com_github_argcv_argcv//tools:version.bzl", "check_version")

check_version("0.4.3")

load("@com_github_argcv_argcv//tools:argcv_deps.bzl", "argcv_deps")
argcv_deps("com_github_argcv_argcv")


bind(
  name = "glfw3",
  actual = "@local_glfw3//:lib",
)

bind(
  name = "freetype2",
  actual = "@local_freetype2//:lib",
)
