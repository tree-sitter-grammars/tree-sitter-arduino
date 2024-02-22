const CPP = require('tree-sitter-cpp/grammar');

module.exports = grammar(CPP, {
  name: 'arduino',

  rules: {
    primitive_type: (_, og) => choice(og, 'byte', 'word'),

    type_qualifier: (_, og) => choice(og, 'PROGMEM'),

    number_literal: (_, og) => choice(og, /B[01]{1,8}/),

    constructor_or_destructor_definition: $ => prec.left(seq(
      repeat($._constructor_specifiers),
      field('declarator', $.function_declarator),
      choice(
        seq(
          optional($.field_initializer_list),
          field('body', $.compound_statement),
          optional(';')
        ),
        alias($.constructor_try_statement, $.try_statement),
        $.default_method_clause,
        $.delete_method_clause,
      ),
    )),
  }
});
