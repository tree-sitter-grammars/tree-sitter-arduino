package tree_sitter_arduino_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_arduino "github.com/tree-sitter-grammars/tree-sitter-arduino/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_arduino.Language())
	if language == nil {
		t.Errorf("Error loading Arduino grammar")
	}
}
