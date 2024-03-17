package tree_sitter_arduino_test

import (
	"testing"

	tree_sitter "github.com/smacker/go-tree-sitter"
	"github.com/tree-sitter/tree-sitter-arduino"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_arduino.Language())
	if language == nil {
		t.Errorf("Error loading Arduino grammar")
	}
}
