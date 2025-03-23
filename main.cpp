#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>

using namespace std;

const int BOARD_SIZE = 8;

// Classe représentant une pièce
class Piece {
public:
    char type;
    char couleur;

    Piece(char t, char c) : type(t), couleur(c) {}
    virtual ~Piece() {}
    virtual bool mouvementValide(int x1, int y1, int x2, int y2, vector<vector<Piece*>>& board) = 0;
    virtual char getChar() { return type; }
};

// Classes spécifiques pour chaque pièce
class Pion : public Piece {
public:
    Pion(char c) : Piece((c == 'B') ? 'P' : 'p', c) {}
    bool mouvementValide(int x1, int y1, int x2, int y2, vector<vector<Piece*>>& board) override {
        int direction = (couleur == 'B') ? -1 : 1;
        if (y1 == y2 && x2 == x1 + direction && board[x2][y2] == nullptr) return true;
        if (y1 == y2 && x2 == x1 + 2 * direction && ((x1 == 6 && couleur == 'B') || (x1 == 1 && couleur == 'N')) && board[x1 + direction][y1] == nullptr && board[x2][y2] == nullptr) return true;
        if (abs(y2 - y1) == 1 && x2 == x1 + direction && board[x2][y2] != nullptr && board[x2][y2]->couleur != couleur) return true;
        return false;
    }
};

class Tour : public Piece {
public:
    Tour(char c) : Piece((c == 'B') ? 'R' : 'r', c) {}
    bool mouvementValide(int x1, int y1, int x2, int y2, vector<vector<Piece*>>& board) override {
        if (x1 == x2 || y1 == y2) {
            int dx = (x2 - x1) == 0 ? 0 : (x2 - x1) / abs(x2 - x1);
            int dy = (y2 - y1) == 0 ? 0 : (y2 - y1) / abs(y2 - y1);
            for (int i = 1; i < max(abs(x2 - x1), abs(y2 - y1)); ++i) {
                if (board[x1 + i * dx][y1 + i * dy] != nullptr) return false;
            }
            return true;
        }
        return false;
    }
};

// Autres classes (Cavalier, Fou, Dame, Roi) suivent le même principe...

// Classe représentant l'échiquier
class Echiquier {
public:
    vector<vector<Piece*>> board;

    Echiquier() {
        board.resize(BOARD_SIZE, vector<Piece*>(BOARD_SIZE, nullptr));
        initialiser();
    }

    ~Echiquier() {
        for (auto& ligne : board) {
            for (auto& piece : ligne) {
                delete piece;
            }
        }
    }

    void initialiser() {
        board[1] = vector<Piece*>(BOARD_SIZE, new Pion('N'));
        board[6] = vector<Piece*>(BOARD_SIZE, new Pion('B'));
        board[0][0] = board[0][7] = new Tour('N');
        board[7][0] = board[7][7] = new Tour('B');
        // Ajouter autres pièces ici...
    }

    void afficher() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != nullptr) cout << board[i][j]->getChar() << " ";
                else cout << ". ";
            }
            cout << endl;
        }
    }

    bool deplacerPiece(int x1, int y1, int x2, int y2, char joueur) {
        if (board[x1][y1] == nullptr || board[x1][y1]->couleur != joueur) return false;
        if (board[x1][y1]->mouvementValide(x1, y1, x2, y2, board)) {
            delete board[x2][y2];
            board[x2][y2] = board[x1][y1];
            board[x1][y1] = nullptr;
            return true;
        }
        return false;
    }

    bool estEchec(char joueur) {
        // Vérifier si le roi du joueur est menacé
        return false;
    }

    bool estEchecEtMat(char joueur) {
        // Vérifier s'il y a un coup possible pour sortir de l'échec
        return false;
    }
};

int main() {
    Echiquier e;
    string input;
    char joueur = 'B';

    while (true) {
        e.afficher();
        cout << "Joueur " << ((joueur == 'B') ? "Blanc" : "Noir") << ", entrez votre coup (ex: e2 e4): ";
        getline(cin, input);
        if (input.size() < 5) {
            cout << "Format invalide. Essayez encore.\n";
            continue;
        }
        int x1 = 8 - (input[1] - '0');
        int y1 = input[0] - 'a';
        int x2 = 8 - (input[4] - '0');
        int y2 = input[3] - 'a';

        if (e.deplacerPiece(x1, y1, x2, y2, joueur)) {
            if (e.estEchecEtMat((joueur == 'B') ? 'N' : 'B')) {
                cout << "Échec et mat! Le joueur " << ((joueur == 'B') ? "Blanc" : "Noir") << " gagne!\n";
                break;
            }
            joueur = (joueur == 'B') ? 'N' : 'B';
        } else {
            cout << "Coup invalide. Essayez encore.\n";
        }
    }
    return 0;
}
