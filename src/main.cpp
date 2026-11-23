#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include "../include/function.h"
#include "../include/minimizer.h"

using namespace std;

// Generazione randomica double
double getRandomDouble(double min, double max) {
    return min + (max - min) * (double)rand() / RAND_MAX;
}

int main() {
    srand(time(0));
    
    double T;
    int N;

    //Inserimento iniziale del gradiente
    cout << "Soglia sulla norma del gradiente T (T>0): ";
    cin >> T;
    while (T <= 0) {
        cout << "T deve essere maggiore di 0. Riprova: ";
        cin >> T;
    }
    
    //Inserimento iniziale del numero di iterazioni
    cout << "Numero massimo di iterazioni N (N>0): ";
    cin >> N;
    while (N <= 0) {
        cout << "N deve essere maggiore di 0. Riprova: ";
        cin >> N;
    }

    //Conversione di E e PI in stringa
    string str_e  = to_string(M_E);
    string str_pi = to_string(M_PI);

    //Definizione domini specificati
    Domain domain1 = Domain{Interval(-4,4)};
    Domain domain2 = Domain{Interval(-1.5,1.5)};
    Domain domain3 = Domain{Interval(-2,2)};

    //Creazione dell'array con le 8 funzioni richieste
    Function f[8] = {
        Function("((x^2) + (y^2)) / 2", domain1, 1.0/5),
        Function("((x^2) + (16 * (y^2))) / 2", domain1, 2.0/25),
        Function("(((x^2) - 1)^2) + (y^2)", domain2, 1.0/20),
        Function("((((x^2) - 1)^2) + (((x - 1)^2) / 5)) + (y^2)", domain2, 1.0/20),
        Function("(4 * ((sinh(x / 2))^2)) + (y^2)", domain3, 3.0/20),
        Function("log((1 + (x^2)) + (y^2))", domain1, 3.0/10),
        Function("1 - (" + str_e + " ^ (0 - ((x^2) + (y^2))))", domain2, 1.0/5),
        Function("(2 - cos(" + str_pi + " * x)) - cos(" + str_pi + " * y)", domain1, 1.0/10),
    };
    
    //Limiti dei domini per i punti casuali
    double limiti_dominio[8] = {4.0, 4.0, 1.5, 1.5, 2.0, 4.0, 1.5, 4.0};
    
    int scelta = 0;
    
    //Ciclo del menu principale
    while (true) {
        cout << "\nScegliere un'opzione per il test di convergenza: " << endl;
        cout << "1: Randomico dentro il dominio" << endl;
        cout << "2: Inserimento punto manuale" << endl;
        cout << "3: Cambia parametri (T e N)" << endl;
        cout << "4: Visualizza configurazione attuale" << endl;
        cout << "5: Uscita" << endl;
        cout << "Scelta: ";
        cin >> scelta;
        
        //Uscita
        if (scelta == 5) {
            cout << "Uscita dal programma." << endl;
            break;
        }
        
        //Controllo range inserito
        if (scelta < 1 || scelta > 5) {
            cout << "Opzione non valida. Riprova." << endl;
            continue;
        }
        
        //1: Genera un punto casuale nel dominio do ogni funzione ed esegue i test
        if (scelta == 1) {
            for (int i = 0; i < 8; i++) {
                double rx = getRandomDouble(-limiti_dominio[i], limiti_dominio[i]);
                double ry = getRandomDouble(-limiti_dominio[i], limiti_dominio[i]);
                Vector2 start_pos(rx, ry);

                cout << "\n--- Minimizzazione f[" << i << "] con p0 casuale " << start_pos << " ---" << endl;
                mimimizeFunction(f[i], N, T, start_pos);
            }
        }

        //2: Lettura valore manuale e test su tutte le funzioni
        else if (scelta == 2) {
            Vector2 start_pos;
            cout << "Inserisci punto di partenza (x y): ";
            cin >> start_pos.x >> start_pos.y;
            
            for (int i = 0; i < 8; i++) {
                cout << "\n--- Minimizzazione f[" << i << "] ---" << endl;
                mimimizeFunction(f[i], N, T, start_pos);
            }
        }
        
        //3: Cambio valori T e N
        else if (scelta == 3) {
            cout << "\nNuova soglia T (attuale = " << T << "): ";
            cin >> T;
            while (T <= 0) {
                cout << "T deve essere maggiore di 0. Riprova: ";
                cin >> T;
            }

            cout << "Nuovo max iterazioni N (attuale = " << N << "): ";
            cin >> N;
            while (N <= 0) {
                cout << "N deve essere maggiore di 0. Riprova: ";
                cin >> N;
            }
            cout << "Parametri aggiornati con successo!" << endl;
        }

        //Visualizzazione valori T e N
        else if (scelta == 4) {
            cout << "\nConfigurazione attuale: " << endl;
            cout << "Soglia gradiente T: " << T << endl;
            cout << "Max iterazioni N: " << N << endl;
        }
    }

    return 0;
}
