README

Stato del progetto: Completo.

SCELTE ARCHITETTURALI PRINCIPALI
- Classe Function: centralizza la gestione delle funzioni matematiche e dei relativi gradienti.
- Minimizzazione iterativa: il calcolo del minimo avviene tramite una funzione non ricorsiva.
- Nel main viene gestito tutta la parte di comunicazione con l'utente.

CARATTERISTICHE DEL PROGETTO
- Parsing e calcolo autonomo in Function: la classe interpreta una funzione a partire dalla sua rappresentazione testuale in 
  stringa e calcola autonomamente sia il valore che il gradiente.
- Classe Vector2: supporta le operazioni su punti bidimensionali e gradienti in modo chiaro e riutilizzabile.
- Costruzione dinamica nel main: dopo aver acquisito gli input dall'utente, il programma istanzia dinamicamente le 8 funzioni 
  richieste a partire dalle rispettive espressioni testuali.

DIVISIONE DEL LAVORO
- Alessio Pantarotto: main.cpp
- Simone Serafin: minimizer.h, vector2.h, minimizer.cpp, vector2.cpp
- Davide Trevisan: function.h, function.cpp

NOTE AGGIUNTIVE
- Stile e convenzioni C++: alcune scelte stilistiche o costruttive riflettono conoscenze pregresse di C++
- Complessità della classe Function: l'implementazione di Function è più ampia e articolata rispetto sia all'effettivo
  utilizzo nel resto del progetto sia ai requisiti minimi della consegna.
