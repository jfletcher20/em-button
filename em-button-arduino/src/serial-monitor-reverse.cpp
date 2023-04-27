/*void loop() {
  if (emb.serial.available() > 0) {
    String inputString = emb.serial.readString();
    inputString.trim();

    String anagramString = "";
    int len = inputString.length();
    for (int i = 0; i < len; i++) {
      char c = inputString.charAt(len - 1 - i);
      anagramString += c;
    }

    emb.serial.println("Anagram: " + anagramString);
  }
}
*/