void hotwire() {
  if (State >= 5) {
    int hot_on = 12 * 12 + 400;
    if (altitudeBMP < hot_on) {
      digitalWrite(9, HIGH);
    }
    else {
      digitalWrite(9, LOW);
    }
  }
}
