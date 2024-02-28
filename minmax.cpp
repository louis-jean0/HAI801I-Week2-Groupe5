#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

// Fonction pour afficher le plateau de jeu
void afficherPlateau(const vector<vector<char>>& plateau) {
    for (const auto& ligne : plateau) {
        for (char caseJeu : ligne) {
            cout << (caseJeu == ' ' ? '_' : caseJeu) << ' '; // Afficher '_' pour les cases vides
        }
        cout << endl;
    }
}

// Fonction pour vérifier si un joueur a gagné
bool estGagnant(const vector<vector<char>>& plateau, char joueur) {
    // Vérifier les lignes et colonnes
    for (int i = 0; i < 3; ++i) {
        if ((plateau[i][0] == joueur && plateau[i][1] == joueur && plateau[i][2] == joueur) ||
            (plateau[0][i] == joueur && plateau[1][i] == joueur && plateau[2][i] == joueur)) {
            return true;
        }
    }

    // Vérifier les diagonales
    if ((plateau[0][0] == joueur && plateau[1][1] == joueur && plateau[2][2] == joueur) ||
        (plateau[0][2] == joueur && plateau[1][1] == joueur && plateau[2][0] == joueur)) {
        return true;
    }

    return false;
}

// Fonction pour vérifier si le plateau est complet (match nul)
bool estMatchNul(const vector<vector<char>>& plateau) {
    for (const auto& ligne : plateau) {
        for (char caseJeu : ligne) {
            if (caseJeu == '_') {
                return false; // Il y a encore des cases vides
            }
        }
    }
    return true; // Aucune case vide, match nul
}

// Fonction pour évaluer le plateau en utilisant l'algorithme Minimax
int evaluerPlateau(const vector<vector<char>>& plateau, char joueur) {
    if (estGagnant(plateau, joueur)) {
        return joueur == 'X' ? 10 : -10;
    } else if (estMatchNul(plateau)) {
        return 0;
    }
    return 0;
}

// Fonction Minimax pour choisir le meilleur mouvement
int minimax(vector<vector<char>>& plateau, char joueur, int profondeur, bool maximizer) {
    int score = evaluerPlateau(plateau, joueur);

    if (score == 10 || score == -10) {
        return score;
    }

    if (estMatchNul(plateau)) {
        return 0;
    }

    if (maximizer) {
        int meilleurScore = numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (plateau[i][j] == '_') {
                    plateau[i][j] = joueur;
                    meilleurScore = max(meilleurScore, minimax(plateau, joueur, profondeur + 1, !maximizer));
                    plateau[i][j] = '_';
                }
            }
        }
        return meilleurScore;
    } else {
        int meilleurScore = numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (plateau[i][j] == '_') {
                    plateau[i][j] = joueur;
                    meilleurScore = min(meilleurScore, minimax(plateau, joueur, profondeur + 1, !maximizer));
                    plateau[i][j] = '_';
                }
            }
        }
        return meilleurScore;
    }
}

// Fonction pour effectuer le mouvement optimal en utilisant l'algorithme Minimax
void jouerCoupOptimal(vector<vector<char>>& plateau, char joueur) {
    int meilleurScore = numeric_limits<int>::min();
    pair<int, int> meilleurCoup;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (plateau[i][j] == '_') {
                plateau[i][j] = joueur;
                int score = minimax(plateau, joueur, 0, false);
                plateau[i][j] = '_';

                if (score > meilleurScore) {
                    meilleurScore = score;
                    meilleurCoup = make_pair(i, j);
                }
            }
        }
    }

    plateau[meilleurCoup.first][meilleurCoup.second] = joueur;
}

int main() {
    ifstream fichier("dataset.txt");
    if (!fichier) {
        cerr << "Erreur lors de l'ouverture du fichier." << endl;
        return 1;
    }

    auto debutTotal = high_resolution_clock::now(); // Enregistrer le temps de début total

    char joueur;
    int cpt = 1;
    while (fichier.get(joueur)) {
        auto debutPartie = high_resolution_clock::now(); // Enregistrer le temps de début de la partie

        // Lire le plateau de jeu à partir du fichier
        vector<vector<char>> plateau(3, vector<char>(3, '_'));
        cout << "Partie " << cpt << ":" << endl;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                char caseJeu;
                fichier.get(caseJeu);
                plateau[i][j] = caseJeu;
            }
            cout << endl;
        }

        // Afficher le plateau initial
        cout << "Plateau initial de la partie " << cpt << endl;
        afficherPlateau(plateau);

        // Jouer la partie complète jusqu'à ce qu'elle soit terminée
        while (!estGagnant(plateau, 'X') && !estGagnant(plateau, 'O') && !estMatchNul(plateau)) {
            // Exemple : Jouer le coup optimal pour le joueur
            jouerCoupOptimal(plateau, 'X');

            // Afficher le plateau après le coup optimal
            cout << "Plateau après le coup optimal : " << endl;
            afficherPlateau(plateau);
        }

        // Vérifier le gagnant
        char gagnant = ' ';
        if (estGagnant(plateau, 'X')) {
            gagnant = 'X';
        } else if (estGagnant(plateau, 'O')) {
            gagnant = 'O';
        }

        cout << "Joueur gagnant : " << gagnant << endl;

        auto finPartie = high_resolution_clock::now(); // Enregistrer le temps de fin de la partie
        auto dureePartie = duration_cast<microseconds>(finPartie - debutPartie);

        cout << "Temps écoulé pour la partie " << cpt << " : " << dureePartie.count() << " microsecondes" << endl;
        cout << "-------------------------------------" << endl;
        cpt++;
    }

    auto finTotal = high_resolution_clock::now(); // Enregistrer le temps de fin total
    auto dureeTotal = duration_cast<seconds>(finTotal - debutTotal);

    cout << "Temps total écoulé : " << dureeTotal.count() << " secondes" << endl;

    fichier.close();

    return 0;
}
