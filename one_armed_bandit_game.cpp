#include <iostream>
#include <string>
#include <ctime>
#include "enarmad_bandit.h"

using namespace std;

const string BOLD = "\e[1m",
             NOBOLD = "\e[0m",
             ENDL = "\n";

int main() {
    char game_field[3][3];
    int account = getDepositFromUser();
    displayAccount(account);

    // Main game loop 
    while (true) {
        int bet = getBetFromUser();
        rollField(game_field);
        WiningRows wining_rows = countWinningRows(game_field);
        int winings = calculateWinings(bet, wining_rows.total);
        account += winings;
        displayField(game_field, wining_rows);
        displayWinnings(winings, account);

        if (!askQuestion("Do you want to continue?"))
        {
            return 0;
        };
    }
}

int getDepositFromUser() {
    int choice;
    while (true) {
        askQuestion("How much money do you want to deposit?: " + ENDL + "  [1] 100kr" + ENDL + "  [2] 300kr" + ENDL + "  [3] 500kr", choice);

        // Translate input list number to deposit value
        switch (choice) {
        case 1: return 100;
        case 2: return 300;
        case 3: return 500;
        default:
            if (choice < 1) {
                invalidInput("number too low");
            }
            else if (choice > 3) {
                invalidInput("number too high");
            }
        }
    }
}

int getBetFromUser() {
    int bet;
    askQuestion("How much do you want to bet?", bet);
    return bet;
}

void rollField(char gf[3][3]) {
    srand(time(0));
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            switch (rand() % 3) {
                case 0:
                    gf[x][y] = 'A';
                    break;
                case 1:
                    gf[x][y] = 'O';
                    break;
                case 2:
                    gf[x][y] = 'X';
                    break;
            }
        }
        
    }
}

WiningRows countWinningRows(char gf[3][3]) {
    WiningRows wining_rows;
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (gf[i][0] == gf[i][1] && gf[i][0] == gf[i][2]) {
            wining_rows.rows[i] = true;
            wining_rows.total++;
        };
        // Check columns
        if (gf[0][i] == gf[1][i] && gf[0][i] == gf[2][i]) {
            wining_rows.columns[i] = true;
            wining_rows.total++;
        };
    }
    char center = gf[1][1];
    // Checks diagonal top-left to bottom-right (\)
    if (center == gf[0][0] && center == gf[2][2]) {
        wining_rows.down = true;
        wining_rows.total++;
    };
    // Checks diagonal bottom-left to top-right (/)
    if (center == gf[2][0] && center == gf[0][2]) {
        wining_rows.up = true;
        wining_rows.total++;
    };
    return wining_rows;
}

int calculateWinings(int bet, int wining_rows) {
    // Only the options up until five are checked because those are the only possible options unless the entire game field is the same symbol, which would be handeled  by the default case
    switch (wining_rows) {
    case 0: return -bet;
    case 1: return bet * 2;
    case 2: return bet * 3;
    case 3: return bet * 4;
    case 4: return bet * 5;
    case 5: return bet * 7;
    default: return bet * 10;
    }
}

void displayField(char gf[3][3], WiningRows wining_rows) {
    cout << " --- --- ---" << endl;
    string row_divider;
    string column_divider;
    for (int x = 0; x < 3; x++) {
        row_divider = " ";
        column_divider = " | ";
        if (wining_rows.rows[x])
            column_divider = "-|-";
    
        for (int y = 0; y < 3; y++) {
            if (wining_rows.columns[y] && x != 2) {
                row_divider += "-|-";
            } else {
                row_divider += "---";
            }

            if (wining_rows.down && ((y == 0 && x == 0) || (y == 1 && x == 1))) {
                row_divider += '\\';
            } else if (wining_rows.up && ((y == 1 && x == 0) || (y == 0 && x == 1))) {
                row_divider += '/';
            } else {
                row_divider += ' ';
            }

            if (y == 0) {
                cout << "| ";
            } else {
                cout << column_divider;
            }
            cout << gf[x][y];
        }
        cout << " |" << endl << row_divider << endl;
    }

    /*
      --- --- ---
     | O | O | X |
      --- ---/---
     | O | X | O |
      ---/--- ---
     | X + X + X |
      --- --- ---

      --- --- ---
     | X | O | O |
      -|-\--- ---
     | X | X | O |
      -|- ---\---
     | X | O | X |
      --- --- ---
    */
}

void displayAccount(int account) {
    cout << "Account " + BOLD << account << NOBOLD + "kr" << endl;
}

void displayWinnings(int winings, int account) {
    string won_lost = "won";
    if (winings < 0) {
        won_lost = "lost";
        // Invert the winings variable so that "-XXX" isn't shown to the user
        winings = -winings;
    }
    cout << "You " << BOLD + won_lost + NOBOLD + ' ' << winings << "kr" << endl;
    displayAccount(account);
}

void askQuestion(string question, string &output) {
    cout << question << endl << ": ";
    cin >> output;
}

void askQuestion(string question, int &output) {
    string answer;
    while (true) {
        askQuestion(question, answer);
        try {
            output = stoi(answer.c_str());
            break;
        } catch (...) {
            invalidInput("please input a number");
        }
    }
}

bool askQuestion(string question) {
    question += " [y/n]";
    string answer;
    while (true) {
        askQuestion(question, answer);
        if (answer.length() > 1) {
            invalidInput();
        }
        else if (answer[0] == 'n') {
            return false;
        }
        else if (answer[0] == 'y') {
            return true;
        } else {
            invalidInput();
        }
    }
}

void invalidInput(string message) {
    cout << "Invalid input, " << message << endl;
}
