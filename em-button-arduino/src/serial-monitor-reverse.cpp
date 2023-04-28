/*void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readString();
    inputString.trim();

    String anagramString = "";
    int len = inputString.length();
    for (int i = 0; i < len; i++) {
      char c = inputString.charAt(len - 1 - i);
      anagramString += c;
    }

    Serial.println("Anagram: " + anagramString);
  }
}
*/