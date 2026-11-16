# Java OOP Projects

Object-oriented programming projects in Java.

- **clinic-reservation-system/** — Clinic appointment system with a Swing GUI. Model: `CRS → Hospital → Section → Doctor → Schedule → Rendezvous`, plus `Patient` and `Appointment`. Duplicate IDs are rejected with custom exceptions (`IDException`, `DuplicateInfoException`).
- **tetris/** — Playable Tetris in Java Swing: random tetrominoes, rotation, line clearing, game over.

## Run

```bash
cd clinic-reservation-system
javac -d out $(find src -name '*.java' ! -name module-info.java)
java -cp out clinic.ClinicReservationSystemGUI

cd ../tetris
javac -d out $(find src -name '*.java' ! -name module-info.java)
java -cp out tetris.TetrisGame
```

Or open the folder as a project in IntelliJ IDEA / Eclipse.
