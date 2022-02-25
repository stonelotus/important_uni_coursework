# Tema 2 - Maze
Student: Rădoi Constantin-Iulian
Grupa/Serie: 331AAb 
An: 2021
Enunt: https://ocw.cs.pub.ro/courses/ac-is/teme/tema2
## Descriere generală
Tema conține un singur modul, modulul *maze.v*, descris in enunț. Modulul implementează algoritmul Wall Follower pentru Maze Solving (right-hand). Practic, ne asigurăm că mereu "playerul" ține *mâna dreaptă* pe zid. Acesta este implementat printr-un Automat cu 5 stări. Modulul rezolvă problema pentru o matrice bidimensională de dimensiune 64x64. Stările automatului sunt următoarele: 
1. entry_state
2. movement_state (MS)
3. position_write_state (PWS)
4. choose_direction_state (CDS)
5. finish_state

Algoritmul este implementat după Wikipedia și video-ul atașat la secțiunea de *Resurse folosite* de mai jos. Am încercat să îl fac pentru un număr de stări minime, împărțind totuși logica cât am putut. 
## Stările automatului
Automatul este conectat în felul următor: 
https://imgur.com/9nMRb57
### entry_state
Starea **entry_state** este starea inițială a automatului. În aceasta setăm o direcție, poziția actuală pe hartă (bazat pe *starting_row* și *starting_col*) și activăm *maze_oe* pentru a primi informații despre poziția row/col în următoarea stare - prin varaibila *maze_in*. De aici putem merge doar în starea *movement_state*. Default este pusă direcția *right* aleator. 

**Observație alegere direcții: Sunt luate după algoritmul Round-Robin.**

### movement_state - MS
În această stare se poate ajunge din *entry_state* și *CDS*. Această stare verifică dacă poziția dată în row/col este liberă - poziție ce semnifică "right-wall-ul viitor" - dacă este liberă, nu e ok deoarece mereu avem nevoie să stăm lipiți de un zid cu mâna dreaptă, așa că facem direcție++ (round-robin, sens orar) pentru a încerca altă dirceție și salvăm în new_row și new_col actualele row/col pentru a ne putea întoarce. Altfel, pozția viitoare e ok, așa că modificăm col și row pentru scriere. Dacă direcția ajunge vreodată mai mare de 4 (up), se va întoarce la 1 (right).
În această stare se face movement-ul propriu-zis al "playerului". Următoarea stare este *PWS*.

### position_write_state - PWS
În PWS se verifică și schimbă direcția/poziția în caz că ultimul row/col (poziția) nu este liber. Dacă nu este liber, revenim ân poziția veche prin row/col = new_row/new_col și schimbăm direcția opus. Apoi trecem la starea *CDS*.
Dacă poziția este liberă, verificăm dacă am ajuns la o ieșire. Dacă am ajuns la ieșire, trecem în state-ul *finish_state* și scriem pe poziția curentă cu ajutorul *maze_we*. Dacă nu am ajuns la ieșire, salvăm poziția curentă în new_row/new_col, activăm scrierea poziției inițiale în matrice (prin maze_we = 1) și trecem la starea *CDS*.

### choose_direction_state - CDS
În această stare, care este ultima din ciclu, aplicăm "if-ul" algoritmului de Wall-Followers - salăm poziția actuală și schimbăm row/col cu poziția "din dreapta direcției actuale" - ținem mâna dreaptă pe zid, pentru a ne asigura că mereu avem în dreapta noastră un zid și că stăm lipiți de el. Trimitem aceste coordonate în următoarea stare - MS, starea cu care am început ciclul - pentru a relua ciclul.

### finish_state
Această stare este "starea marcată". În ea edităm output-ul *done*, iar automatul nu va mai apela comanda ``` state <= next_state;```  


-------------------------------
Observații

- Stările și direcțiile posibile au fost scrise prin define, pentru o înțelegere și debugging mai ușoare  
- *maze_oe* activează input-ul din *maze_in* în starea imediat următoare a actualei stări 
- Variabilele reg *new_col* și *new_row* salvează *row* și *col* pentru a putea reveni la poziție ulterior în alte stări (în caz că nu suntem pe path-ul bun) - practic au rol de auxiliare
- Desenul atașat în acest readme are doar rolul de a demonstra ordinea state-urilor, nu de a fi o definiție pentru arborele de stări. (nu conține nimic pe linii)
- La prima iterație a algoritmului (după **singura** ieșire din *entry_state*), algoritmul v-a schimba direcția din *right* în *down* cu Round-Robin din cauză că prima poziție trebuia scrisă. Este un caz particular, care *nu* se repetă cu acest scop - următoarele schimbări de direcție din *MS* sunt strict din cauza state-ului *CDS*, ca urmare a verificării măinii drepte.

Resurse folosite:

- https://en.wikipedia.org/wiki/Maze-solving_algorithm#Wall_follower
- https://ocw.cs.pub.ro/courses/ac-is
- https://www.youtube.com/watch?v=vF6gO86RKp4
- https://en.wikipedia.org/wiki/Round-robin_scheduling