# Tema 3 - SDA

de Alexandru Sima (312CA)

--------------------------------------------------------------------------------

## Magic Keyboard

### Structura proiectului

- `mk` - entry point-ul programului și loop-ul de citire a comenzilor
- `mk_functions` - interogări în dicționar (autocorrect, autocomplete)
- `trie` - implementarea structurii unui trie
- `trie_node` - implementarea structurii interne dintr-un trie
- `utils` - funcții ajutătoare

### Structuri de date folosite

### Comenzi

- `INSERT <word>` - Se inserează un cuvânt în dicționar.
- `LOAD <file>` - Se încarcă cuvintele din fișierul dat ca argument.
- `REMOVE <word>` - Se șterge un cuvânt din dicționar.
- `AUTOCORRECT <word> <max_letters>` - Se caută în dicționar cuvintele de
aceeași lungime cu `word`, care diferă prin cel mult `max_letters` litere și se
afișează în ordine lexicografică. Se afișează "No words found!" dacă nu există
niciun cuvânt care să îndeplinească condiția.
- `AUTOCOMPLETE <prefix> <mode>` - Se completează prefixul cu cuvinte din
dicționar, în funcție de `mode`. Se afișează "No words found!" dacă nu există
niciun cuvânt care să înceapă cu `prefix`.
- `EXIT` - Se golește dicționarul și se termină execuția programului.

### Funcționalitate

--------------------------------------------------------------------------------

## KNN

### Structura proiectului

- `kNN` - entry point-ul programului și loop-ul de citire a comenzilor
- `kNN_functions` - interogări asupra datelor (range search și nearest
neighbour)
- `array` - implementarea structurii unui array list
- `bst` - implementarea structurii unui arbore binar de căutare k-ar
- `point` - implementarea structurii unui punct în spațiul k-dimensional
- `utils` - funcții ajutătoare

### Structuri de date folosite

- `array list` - pentru a stoca punctele găsite într-un query
- `bst k-ar` - pentru a stoca punctele din spațiul k-dimensional

### Comenzi

- `LOAD <file>` - încarcă datele din fișierul dat ca argument
- `NN <x1> <x2> ... <xk>` - găsește cei mai apropiați vecini ai unui punct dat
ca argument
- `RS <x1_start> <x1_end> <x2_start> <x2_end> ... <xk_start> <xk_end)` - găsește
toate punctele dintr-un anumit interval dat ca argument
- `EXIT` - termină execuția programului, eliberând memoria alocată

### Funcționalitate

- `LOAD`: Programul încarcă dintr-un fișier (primit ca argument al
comenzii `LOAD`) punctele din spațiul k-dimensional și le stochează într-un
arbore binar de căutare k-ar, cu proprietatea că ordinea stabilită între
nodurile de pe nivelul `i` este dată de coordonata `i % k` a punctelor.

- `NN`:
  - Pentru a determina cei mai apropiați vecini ai unui punct dat ca argument,
  se parcurge recursiv arborele, căutându-se frunza cea mai apropiată de punctul
  dat, folosindu-se ordinea stabilită de proprietatea arborelui.
  - La întoarcerea din recursivitate, se calculează distanța euclidiană între
  punctul curent și cel țintă, stabilindu-se dacă trebuie inserat în array-ul
  rezultat.
  - Dacă distanța minimă până în acest moment este mai mare decât distanța de la
  punctul curent la planul de separație determinat de coordonata `i % k` a
  părintelui, există posibilitatea să se afle un punct mai apropiat în
  subarborele frate, așa că se caută și în acela, inserându-se în array-ul
  rezultat dacă este cazul.

- `RS`:
  - Pentru a determina punctele dintr-un anumit interval dat ca argument, se
  parcurge recursiv arborele, căutându-se nodurile care se află în intervalul
  dat. Pentru eficientizarea căutării, căutarea nu se mai face pe subarborii ai
  căror dimensiune de separație (`i % k`) nu se află în interval, deoarece
  niciun punct din acei subarbori nu poate fi în interval.

- `EXIT`: Se eliberează elementele stocate în arbore și se termină execuția
programului.
