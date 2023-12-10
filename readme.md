# Wymagania

- **Obowiązkowe** (grafika i logika prostego ruchu, max. 6 pkt)
  - [x] Podstawowy interfejs użytkownika (1 pkt)
  - [x] Wizualizacja pozycji w grze backgammon (1 pkt)
  - [x] Zapisanie stanu gry do pliku i wczytanie stanu gry z pliku (2 pkt)
  - [x] Zagranie kolejnego ruchu rozgrywki z wczytanego stanu gry (2 pkt)
- **Nieobowiązkowe** (symulacja i logika rozgrywki, max. 9 pkt)
  - [x] Realizacja reguły bicia pionka przeciwnika (1 pkt)
  - [x] Realizacja reguły zdejmowania maksymalnej liczby pionków (1 pkt)
  - [x] Symulacja rozgrywki od dowolnej pozycji do końca gry (2 pkt)
  - [x] Zapis gry w postaci kolejnych ruchów od początku do końca gry (1 pkt)
  - [x] Wizualizacja przebiegu rozgrywki na podstawie wczytanego zapisu (2 pkt)
  - [x] Pełny interfejs użytkownika (1 pkt)
  - [x] Ranking identyfikowanych nazwami (min. 3) graczy (Hall of Fame) (1 pkt)
- **Dodatkowe** (logika gracza komputerowego, max. 3 pkt)
  - [x] Własna propozycja logiki gracza komputerowego
  - [x] Porównanie logiki graczy komputerowych
  - [x] Sprawozdanie z wynikami

# Sprawozdanie

Algorytm gracza komputerowego jest specjalnie napisany tak, aby popełniał błędy, żebym miał szansę wygrać. Pomimo, że nie jest to algorytm optymalny, to i tak często ze mną wygrywa.

Priorytety gracza komputerowego (computer_turn):
1. Zdejmowanie pionków z bandy (computer_handle_bar)
2. Zbijanie pionków gracza (computer_handle_violence)
3. Zdejmowanie pionków z planszy (computer_handle_home)
4. Normalne poruszanie się (computer_handle_move)
5. Jeśli nie ma żadnego ruchu, to pasuje (break)

Co można poprawić:
- Zmienić zasady gry, żeby zdejmowanie pionków z bandy, zbijanie pionków gracza i zdejmowanie pionków z planszy nie było wymagane
- Dodać poziomy trudności (im niższy poziom, tym większa szansa na błąd gracza komputerowego)
- Zaimplementować algorytm polecany przez wikipedię