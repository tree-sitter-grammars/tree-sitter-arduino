import XCTest
import SwiftTreeSitter
import TreeSitterArduino

final class TreeSitterArduinoTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_arduino())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Arduino grammar")
    }
}
