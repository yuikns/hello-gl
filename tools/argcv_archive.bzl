_ARGCV_REVISION = "ARGCV_REVISION"

def _argcv_archive(ctx):
  git_commit = ctx.attr.git_commit
  sha256 = ctx.attr.sha256

  override_git_commit = ctx.os.environ.get(_ARGCV_REVISION)
  if override_git_commit:
    sha256 = ""
    git_commit = override_git_commit

  strip_prefix = "argcv-%s" % git_commit
  urls = [
      "https://github.com/yuikns/argcv/archive/%s.tar.gz" % git_commit,
  ]
  ctx.download_and_extract(
      urls,
      "",
      sha256,
      "",
      strip_prefix)

argcv_archive = repository_rule(
    implementation=_argcv_archive,
    attrs={
        "git_commit": attr.string(mandatory=True),
        "sha256": attr.string(mandatory=True),
    })
