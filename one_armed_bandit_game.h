#include <string>

using namespace std;

const string BOLD = "\e[1m",
             NOBOLD = "\e[0m",
             ENDL = "\n";

struct WiningRows {
    bool rows[3] = {false, false, false};
    bool columns[3] = {false, false, false};
    bool up = false;
    bool down = false;
    int total = 0;
};;

int getDepositFromUser();
int getBetOrQuitFromUser(int&);
void rollField(char[3][3]);
WiningRows countWinningRows(char[3][3]);
int calculateWinings(int, int);
void displayField(char[3][3], WiningRows);
void displayAccount(int);
void displayWinnings(int, int);
void askQuestion(string, string&);
void askQuestion(string, int&);
void invalidInput(string = "please try again");