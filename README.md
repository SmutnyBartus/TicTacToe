# Raylib Tic-Tac-Toe with MiniMax AI

Implementacja klasycznej gry **Kółko i Krzyżyk** w języku **C** z wykorzystaniem biblioteki graficznej **Raylib**. Projekt zawiera interaktywne menu główne, ekrany końcowe oraz niepokonanego przeciwnika komputerowego sterowanego algorytmem sztucznej inteligencji.

## 🚀 Funkcje projektu

* **Tryb PvE (z komputerem):** Walka z niepokonanym algorytmem **MiniMax**, który analizuje stan planszy w czasie rzeczywistym i wybiera optymalną ścieżkę (z uwzględnieniem najszybszej wygranej/najdłuższego oporu).
* **Tryb PvP (lokalny):** Możliwość gry dla dwóch graczy na jednym ekranie.
* **Interaktywne UI:** Menu główne z obsługą myszy (efekt podświetlenia przycisków, dynamiczna zmiana kursora).
* **Wykrywanie końca gry:** Automatyczne sprawdzanie warunków wygranej/remisu i graficzne rysowanie linii zwycięstwa.
  
## 🛠️ Uruchomienie (Linux/GCC)

```bash
gcc main.c -o tictactoe -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./tictactoe
```

[Watch preview](https://github.com/SmutnyBartus/TicTacToe/raw/main/TicTacTOe.mp4)
