#include <iostream>
#include <ctime>
#include "one_armed_bandit_game.h"

using namespace std;

int main() {
    // initialize the rand() function at the start of the program
    srand(time(0));
    char game_field[3][3];
    int account = getDepositFromUser();
    cout << "You have  " + BOLD << account << NOBOLD + "kr in your account" << endl;

    // Main game loop
    while (account > 0) {
        int bet = getBetFromUser(account);
        rollField(game_field);
        WinningRows winning_rows = countWinningRows(game_field);
        int winnings = calculateWinnings(bet, winning_rows.total);
        // Only add the winnings to account if you won something, the "loses" (negative numbers) will be displayed by displayWinnings()
        if (winnings > 0)
            account += winnings;
        displayField(game_field, winning_rows);
        displayWinnings(winnings);
        cout << "Account " + BOLD << account << NOBOLD + "kr" << endl;
    }
    cout << "You have gone bankrupt, please come back another time!" << endl;
    return 0;
}

int getDepositFromUser() {
    int choice;
    while (true) {
        askQuestion("How much money do you want to deposit?: " + ENDL + 
                    "  [1] 100kr" + ENDL + 
                    "  [2] 300kr" + ENDL + 
                    "  [3] 500kr", choice);

        // Translate input list number to deposit value
        switch (choice) {
        case 1: return 100;
        case 2: return 300;
        case 3: return 500;
        default:
            if (choice < 1) 
                invalidInput("number too low");
            else if (choice > 3) 
                invalidInput("number too high");
        }
    }
}

int getBetFromUser(int &account) {
    int bet;
    while (true) {
        askQuestion("How much do you want to bet? [number OR [q]uit]", bet);
        
        if (bet <= 0)
            invalidInput("you can't bet 0 or less");
        else if (bet > account)
            invalidInput("you only have " + to_string(account) + " in your account");
        else  {
            // If bet is smaller than account, remove bet from account to "spend" the money
            account -= bet;
            return bet;
        } 
    }
}

void rollField(char game_field[3][3]) {
    // Generate a random number for every index in game_field and choose one of three options based on the random number to become the value at that index
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            switch (rand() % 3) {
            case 0:
                game_field[x][y] = 'A';
                break;
            case 1:
                game_field[x][y] = 'O';
                break;
            case 2:
                game_field[x][y] = 'X';
                break;
            }
        }  
    }
}

WinningRows countWinningRows(char game_field[3][3]) {
    WinningRows winning_rows;
    // Only loops three times since the game_field is 3x3 in size, which means we can check one row and one column on every iteration, thus we only need three iterations
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (game_field[i][0] == game_field[i][1] && game_field[i][0] == game_field[i][2]) {
            winning_rows.rows[i] = true;
            winning_rows.total++;
        };
        // Check columns
        if (game_field[0][i] == game_field[1][i] && game_field[0][i] == game_field[2][i]) {
            winning_rows.columns[i] = true;
            winning_rows.total++;
        };
    }
    char center = game_field[1][1];
    // Checks diagonal top-left to bottom-right (\)
    if (center == game_field[0][0] && center == game_field[2][2]) {
        winning_rows.top_to_bottom = true;
        winning_rows.total++;
    };
    // Checks diagonal bottom-left to top-right (/)
    if (center == game_field[2][0] && center == game_field[0][2]) {
        winning_rows.bottom_to_top = true;
        winning_rows.total++;
    };
    return winning_rows;
}

int calculateWinnings(int bet, int winning_rows) {
    // Only the options up until six are checked because those are the only possible options unless the entire game field is the same symbol, which would be handled by the default case
    switch (winning_rows) {
    case 0: return -bet;
    case 1: return bet * 2;
    case 2: return bet * 3;
    case 3: return bet * 4;
    case 4: return bet * 5;
    case 5: return bet * 7;
    case 6: return bet * 8; // Not part of the specification, but 6 winning rows had to be handled
    default: return bet * 10;
    }
}

void displayField(char game_field[3][3], WinningRows winning_rows) {
    /*  Example outputs:
         --- --- ---
        | O | O | X |
         --- ---/---
        | O | X | O |
         ---/--- ---
        | X-|-X-|-X |
         --- --- ---

         --- --- ---
        | X | O | O |
         -|-\--- ---
        | X | X | O |
         -|- ---\---
        | X | O | X |
         --- --- ---     */
    string current_row;
    string row_divider;
    string column_divider;

    // An array for the spaces between the row_dividers. This is initialized outside the loop to minimize comparisons done within the big loop below
    // This array does not change the leftmost or bottommost layers in the displayed gamefield and needs to be 3x3 to simplify the big loop below, without comparisons
    char center_dividers[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '},
    };
    // If the is a winning diagonal, change the corresponding center_dividers to add the diagonals lines to indicate that
    if (winning_rows.top_to_bottom) {
        center_dividers[1][0] = '\\';
        center_dividers[2][1] = '\\';
        /*  Would add this to the center of the field
            "\" " "
            " " "\"     */
    }
    if (winning_rows.bottom_to_top) {
        center_dividers[1][1] = '/';
        center_dividers[2][0] = '/';
        /*  Would add this to the center of the field
            " " "/"
            "/" " "     */
    }

    for (int x = 0; x < 3; x++) {
        column_divider = " | ";
        // If this row is a win, change the divider to indicate that
        if (winning_rows.rows[x])
                column_divider = "-|-";
        current_row = "";
        // start the row_divider with a space to align it with the columns in the output
        row_divider = " ";
        // For every row, build the current_row and row_divider with the necessary connecting versions
        for (int y = 0; y < 3; y++)
        {
            // Connect vertically if the column is a win and it's not the top row_divider
            if (winning_rows.columns[y] && x > 0)
                row_divider += "-|-";
            else
                row_divider += "---";

            row_divider += center_dividers[x][y];

            if (y == 0)
                current_row += "| ";
            else
                current_row += column_divider;

            current_row += game_field[x][y];
        }
        // Print the row divider and then the current row on a new line
        cout << row_divider << endl
                << current_row + " |" << endl;
        }
        // Close the game field with a row divider
        cout << " --- --- --- " << endl;
    }

void displayWinnings(int winnings) {
    string won_lost = "won";
    if (winnings <= 0) {
        won_lost = "lost";
        // Invert the winnings variable so that "-XXX" isn't shown to the user
        winnings = -winnings;
    }
    cout << "You " << BOLD + won_lost + NOBOLD + ' ' << winnings << "kr" << endl;
}

void askQuestion(string question, string &output) {
    cout << question << endl << ": ";
    cin >> output;

    // If the user wants to quit, exit the application
    if (output == "quit" || output == "q")
        exit(0);
}

void askQuestion(string question, int &output) {
    string answer;
    while (true) {
        askQuestion(question, answer);
        try {
            output = stoi(answer.c_str());
            return;
        } catch (...) {
            invalidInput("please input a number");
        }
    }
}

void invalidInput(string message) {
    cout << "Invalid input, " << message << endl;
}