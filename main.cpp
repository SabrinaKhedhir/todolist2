#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <ctime>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

struct Attività {
    string descrizione;
    string priorità;
    string data_creazione;
    string data_attività;
    string ora_attività;

    Attività(string d, string p, string dataAtt, string oraAtt)
        : descrizione(std::move(d)), priorità(std::move(p)), data_attività(std::move(dataAtt)), ora_attività(std::move(oraAtt)) {
        time_t now = time(nullptr);
        data_creazione = string(ctime(&now));
        data_creazione.pop_back();
    }
};

int prioritàValore(const string& prio) {
    if (prio == "Alta") return 3;
    if (prio == "Media") return 2;
    return 1;
}

void ordinaPerPriorità(vector<Attività>& lista) {
    sort(lista.begin(), lista.end(), [](const Attività& a, const Attività& b) {
        return prioritàValore(a.priorità) > prioritàValore(b.priorità);
    });
}

void disegnoTitolo() {
    cout << CYAN << "\n╔══════════════════════════════╗\n";
    cout << "║        📘 TO-DO LIST         ║\n";
    cout << "╚══════════════════════════════╝" << RESET << endl;
}

void mostraMenu() {
    disegnoTitolo();
    cout << "1️⃣  Aggiungi attivita\n";
    cout << "2️⃣  Mostra attivita\n";
    cout << "3️⃣  Rimuovi attivita\n";
    cout << "4️⃣  Salva su file\n";
    cout << "5️⃣  Carica da file\n";
    cout << "6️⃣  Modifica attivita\n";
    cout << "7️⃣  Esci\n";
    cout << "➡️  Scegli un'opzione: ";
}

void aggiungiAttività(vector<Attività>& lista) {
    string desc, prio, dataAtt, oraAtt;
    cout << "\n✏️ Inserisci la descrizione: ";
    cin.ignore();
    getline(cin, desc);

    cout << "🎯 Inserisci la priorita (Alta / Media / Bassa): ";
    getline(cin, prio);

    while (prio != "Alta" && prio != "Media" && prio != "Bassa") {
        cout << RED << "❗ Priorita non valida. Riprova (Alta / Media / Bassa): " << RESET;
        getline(cin, prio);
    }

    cout << "📅 Inserisci la data dell'attivita (es. 2025-06-20): ";
    getline(cin, dataAtt);

    cout << "🕒 Inserisci l'ora dell'attivita (es. 14:30): ";
    getline(cin, oraAtt);

    lista.emplace_back(desc, prio, dataAtt, oraAtt);
    cout << GREEN << "✅ Attivita aggiunta con successo!" << RESET << endl;
}

void mostraAttività(vector<Attività>& lista) {
    if (lista.empty()) {
        cout << YELLOW << "📭 Nessuna attivita trovata." << RESET << endl;
        return;
    }

    ordinaPerPriorità(lista);
    cout << BOLD << "\n📋 Elenco Attivita Ordinato per Priorita:\n" << RESET;
    cout << "╔══════════════════════════════════════════════════════════════════════╗\n";
    for (size_t i = 0; i < lista.size(); ++i) {
        cout << i + 1 << ". " << lista[i].descrizione
             << " [" << lista[i].priorità << "]"
             << " 📅 " << lista[i].data_attività
             << " 🕒 " << lista[i].ora_attività
             << " (Creato il: " << lista[i].data_creazione << ")\n";
    }
    cout << "╚══════════════════════════════════════════════════════════════════════╝\n";
}

void rimuoviAttività(vector<Attività>& lista) {
    mostraAttività(lista);
    if (lista.empty()) return;

    int index;
    cout << "🗑️ Inserisci il numero dell'attivita da rimuovere: ";
    cin >> index;

    if (index < 1 || index > static_cast<int>(lista.size())) {
        cout << RED << "⚠️ Indice non valido." << RESET << endl;
    } else {
        lista.erase(lista.begin() + index - 1);
        cout << GREEN << "✅ Attivita rimossa correttamente." << RESET << endl;
    }
}

void modificaAttività(vector<Attività>& lista) {
    mostraAttività(lista);
    if (lista.empty()) return;

    int index;
    cout << "✏️ Inserisci il numero dell'attivita da modificare: ";
    cin >> index;
    cin.ignore();

    if (index < 1 || index > static_cast<int>(lista.size())) {
        cout << RED << "⚠️ Indice non valido." << RESET << endl;
        return;
    }

    Attività& att = lista[index - 1];
    cout << "📝 Nuova descrizione (attuale: " << att.descrizione << "): ";
    getline(cin, att.descrizione);

    cout << "🎯 Nuova priorita (attuale: " << att.priorità << "): ";
    getline(cin, att.priorità);
    while (att.priorità != "Alta" && att.priorità != "Media" && att.priorità != "Bassa") {
        cout << RED << "❗ Priorita non valida. Riprova (Alta / Media / Bassa): " << RESET;
        getline(cin, att.priorità);
    }

    cout << "📅 Nuova data attivita (attuale: " << att.data_attività << "): ";
    getline(cin, att.data_attività);

    cout << "🕒 Nuova ora attivita (attuale: " << att.ora_attività << "): ";
    getline(cin, att.ora_attività);

    cout << GREEN << "✅ Attivita modificata con successo!" << RESET << endl;
}

void salvaSuFile(const vector<Attività>& lista, const string& nome_file) {
    ofstream file(nome_file);
    if (!file) {
        cout << RED << "❌ Errore durante l'apertura del file." << RESET << endl;
        return;
    }

    for (const auto& att : lista) {
        file << att.descrizione << ";" << att.priorità << ";" << att.data_creazione
             << ";" << att.data_attività << ";" << att.ora_attività << "\n";
    }

    cout << GREEN << "💾 Attivita salvate su '" << nome_file << "' con successo." << RESET << endl;
}

void caricaDaFile(vector<Attività>& lista, const string& nome_file) {
    ifstream file(nome_file);
    if (!file) {
        cout << RED << "❌ File '" << nome_file << "' non trovato." << RESET << endl;
        return;
    }

    lista.clear();
    string linea;
    while (getline(file, linea)) {
        size_t primo = linea.find(';');
        size_t secondo = linea.find(';', primo + 1);
        size_t terzo = linea.find(';', secondo + 1);
        size_t quarto = linea.find(';', terzo + 1);

        if (primo != string::npos && secondo != string::npos && terzo != string::npos && quarto != string::npos) {
            string desc = linea.substr(0, primo);
            string prio = linea.substr(primo + 1, secondo - primo - 1);
            string creato = linea.substr(secondo + 1, terzo - secondo - 1);
            string dataAtt = linea.substr(terzo + 1, quarto - terzo - 1);
            string oraAtt = linea.substr(quarto + 1);

            Attività nuova(desc, prio, dataAtt, oraAtt);
            nuova.data_creazione = creato;
            lista.push_back(nuova);
        }
    }

    cout << GREEN << "📂 Attivita caricate da '" << nome_file << "' con successo." << RESET << endl;
}

int main() {
    vector<Attività> lista;
    int scelta;
    const string nome_file = "todo.txt";

    do {
        mostraMenu();
        cin >> scelta;

        switch (scelta) {
            case 1: aggiungiAttività(lista); break;
            case 2: mostraAttività(lista); break;
            case 3: rimuoviAttività(lista); break;
            case 4: salvaSuFile(lista, nome_file); break;
            case 5: caricaDaFile(lista, nome_file); break;
            case 6: modificaAttività(lista); break;
            case 7: cout << CYAN << "\n👋 Uscita dal programma. A presto!" << RESET << endl; break;
            default: cout << RED << "❗ Opzione non valida. Riprova." << RESET << endl;
        }
    } while (scelta != 7);

    return 0;
}

