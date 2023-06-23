def pkg(str; re; s):
  select(startswith(str)?) | sub(re; s);

.packages[].resolved |
  pkg("git"; ".+/(?<repo>[a-z-]+/[a-z-]+)[.]git#(?<hash>[0-9a-f]+)"; "- \(.repo)@\(.hash)"),
  pkg("https"; ".+/(?<name>[a-z-]+)-(?<version>[0-9.]+)[.]tgz"; "- [\(.name)@\(.version)](https://www.npmjs.com/package/\(.name)/v/\(.version))")
