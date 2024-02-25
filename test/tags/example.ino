void loop() {
//   ^^^^ @definition.function
    digitalWrite(LED_BUILTIN, HIGH);
//  ^^^^^^^^^^^^ @reference.call
    delay(1000);
//  ^^^^^ @reference.call
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
