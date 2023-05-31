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

- `trie` - pentru a stoca cuvintele din dicționar și a facilita interogările

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

- `INSERT`: Se inserează un cuvânt în dicționar, parcurgându-se literă cu
literă, creându-se nodurile aferente dacă nu există, iar la nodul final se
incrementează numărul de apariții ale cuvântului (inițial 0). La întoarcerea din
inserție, se calculează pentru fiecare nod parcurs frecvența maximă și lungimea
minimă a cuvintelor care se ramifică din acel nod, pentru a facilita
interogările ulterioare.

- `LOAD`: Se inserează toate cuvintele dintr-un fișier dat (vezi `INSERT`).

- `REMOVE`: Se șterge un cuvânt din dicționar, parcurgându-se literă cu literă
și setându-se numărul de apariții ale cuvântului 0. La întoarcere, se
recalculează frecvența maximă și lungimea minimă a cuvintelor care se ramifică
din noduri și se șterg nodurile care nu mai conduc către niciun cuvânt.

- `AUTOCORRECT`: Se parcurge arborele până la nivelul corespunzător lungimii
cuvântului inițial, încercându-se toate combinațiile de litere care conduc la
cuvinte existente, în limita schimbărilor de litere disponibile. Se contorizează
numărul de litere schimbate. Când se ajunge la nivelul corespunzător, dacă
cuvântul curent este valid, se afișează. Modul de încercare a combinațiilor 
garantează că rezultatele vor fi în ordine lexicografică. În cazul în care 
niciun cuvânt nu este găsit, se afișează "No words found!".

- `AUTOCOMPLETE`:
- Se parcurge arborele până la nodul corespunzător prefixului dat. Există 4
moduri de autocomplete, dintre care `0` semnifică că se vor executa toate
celelalte 3:
  - `1`: Se afișează primul cuvânt (în ordine lexicografică). Pentru aceasta, se
  pleacă din prefix, se încearcă fiecare literă din alfabet și se avansează pe
  prima găsită (modul de construire a trie-ului garantează că toate literele
  disponibile duc la cuvinte existente). Se afișează primul cuvânt descoperit.
  - `2`: Se afișează cel mai scurt cuvânt. Pentru aceasta, se pleacă din prefix,
  și se urmăresc nodurile care duc la o lungime minimă cu 1 mai mică decât cele
  curente. Se afișează primul cuvânt descoperit.
  - `3`: Se afișează cel mai frecvent cuvânt. Pentru aceasta, se pleacă din
  prefix, se încearcă fiecare literă din alfabet și se urmăresc nodurile care
  duc la cuvinte cu aceeași frecvență maximă ca și cele curente. Se afișează
  primul cuvânt descoperit.
  - Pentru toate cele 3 moduri, dacă nu există niciun cuvânt care să
  îndeplinească condiția, se afișează "No words found!".
- Grație similarității modurilor de autocomplete, am implementat o singură
funcție care primește în plus ca parametri 2 funcții:
  - O funcție de stop, care spune când se oprește căutarea;
  - O funcție de avansare, care spune cum se alege nodul următor în parcurgere.

- `EXIT`: Se eliberează elementele stocate în trie și se termină execuția
programului.

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
