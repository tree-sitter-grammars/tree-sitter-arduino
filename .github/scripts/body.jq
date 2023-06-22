.packages[].resolved |
  (select(startswith("git")?) | sub(".+/(?<repo>[a-z-]+/[a-z-]+)[.]git#(?<hash>[0-9a-f]+)"; "\(.repo)@\(.hash)")),
  (select(startswith("https")?) | sub(".+/(?<name>[a-z-]+)-(?<version>[0-9.]+)[.]tgz"; "[\(.name)@\(.version)](https://www.npmjs.com/package/\(.name)/v/\(.version))"))
