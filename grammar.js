const CPP = require('tree-sitter-cpp/grammar')

module.exports = grammar(CPP, {
  name: 'arduino',

  rules: {
    primitive_type: (_, og) => choice(og, 'byte', 'word'),

    type_qualifier: (_, og) => choice(og, 'PROGMEM'),

    number_literal: (_, og) => choice(og, /B[01]{1,8}/)
  }
});
