.packages | to_entries | del(.[0])[] |
  "- [`\(.key[13:])@\(.value.version)`]" +
  "(https://www.npmjs.com/package/\(.key[13:])/v/\(.value.version))"
