bool aboutAnimation = true;
void displayAbout(int id) {
  if (aboutAnimation) {
    display.println("+----------+");
    display.println("+  0    _0 +");
    display.println("+-/----0---+");
    display.println(" 0      \\");
    display.println("  \\      *");
    display.println("   *");
  } else {
    display.println("+----------+");
    display.println("+ _0     0 +");
    display.println("+0------/--+");
    display.println("  \\    0");
    display.println("   *    \\");
    display.println("         *");
  }
  aboutAnimation = !aboutAnimation;
}
