#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

//  ANSI Colors
const string GREEN  = "\033[42m\033[30m";  // green background
const string YELLOW = "\033[43m\033[30m";  // yellow background
const string GRAY   = "\033[100m\033[37m"; // gray background
const string BOLD   = "\033[1m";
const string RESET  = "\033[0m";

//  Constants
const int WORD_LENGTH = 5;
const int MAX_GUESSES = 6;
const int WORD_COUNT  = 110;
const int ALPHABET    = 26;

// Word Bank
string wordBank[WORD_COUNT] = {
    "APPLE", "BRAIN", "CHESS", "DREAM", "EARTH",
    "FLUTE", "GRAPE", "HOUSE", "IMAGE", "JUICE",
    "KNIFE", "LIGHT", "MOUSE", "NIGHT", "OCEAN",
    "PARTY", "QUEEN", "RIVER", "SMILE", "TABLE",
    "UNITY", "VALUE", "WASTE", "YOUTH", "ZEBRA",
    "ALERT", "BAKER", "CANDY", "DANCE", "EARLY",
    "FAITH", "GIANT", "HAPPY", "INDEX", "JUMBO",
    "KNACK", "LAYER", "MIGHT", "NURSE", "OPERA",
    "PRESS", "QUIET", "ROUND", "STEAM", "TRACE",
    "UNDER", "VIVID", "WHALE", "EXTRA", "YIELD",
    "ZESTY", "ANGEL", "BLINK", "CLOWN", "DRIFT",
    "ELITE", "FABLE", "GHOST", "HONEY", "IDEAL",
    "JAZZY", "KNOCK", "LEGAL", "MAGIC", "NAVAL",
    "OASIS", "PIOUS", "QUEST", "ROYAL", "SOLAR",
    "TIDAL", "USAGE", "VOTER", "WEDGE", "XENON",
    "YACHT", "ZINGY", "ALBUM", "BOOST", "CRISP",
    "DELTA", "EPOCH", "FLEET", "GRADE", "HAVEN",
    "IRONY", "JOINT", "KINDS", "LLAMA", "MONEY",
    "NERVE", "ORDER", "POLAR", "QUICK", "REALM",
    "SCOPE", "THRUM", "TANGO", "ULTRA", "VENOM",
    "WRECK", "YEAST", "AZURE", "BRAVE", "CYCLE",
    "DRAMA", "ELDER", "FORGE", "GLADE", "HOTEL"
};

//  Game State (2D arrays)
char gridLetters [MAX_GUESSES][WORD_LENGTH];
char gridFeedback[MAX_GUESSES][WORD_LENGTH];
char usedLetters [ALPHABET]; // best result seen per letter A-Z

//  Function: Selecting random word from the bank 
string selectWord() {
    srand((unsigned)time(0)); // RNG using system time
    return wordBank[rand() % WORD_COUNT];
}

//  Function: Validating the input (length and letters only)
bool isValid(const string& guess) {
    if ((int)guess.length() != WORD_LENGTH) return false;
    for (int i = 0; i < WORD_LENGTH; i++)
        if (!isalpha(guess[i])) return false;
    return true;
}

// Function: Compute Green, Yellow, and gray feedback
void computeFeedback(const string& guess, const string& secret, int row) {
    bool secretUsed[WORD_LENGTH] = {false};
    bool guessUsed [WORD_LENGTH] = {false};

    for (int i = 0; i < WORD_LENGTH; i++) {
        gridLetters[row][i] = guess[i];
        if (guess[i] == secret[i]) {
            gridFeedback[row][i] = 'G';
            secretUsed[i] = guessUsed[i] = true;
        }
    }
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guessUsed[i]) continue;
        bool found = false;
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!secretUsed[j] && guess[i] == secret[j]) {
                gridFeedback[row][i] = 'Y';
                secretUsed[j] = true;
                found = true;
                break;
            }
        }
        if (!found) gridFeedback[row][i] = 'X';
    }
}

// Function: Updating used-letters (G > Y > X, no downgrade)
void updateUsedLetters(int row) {
    for (int i = 0; i < WORD_LENGTH; i++) {
        int  idx     = gridLetters[row][i] - 'A';
        char current = usedLetters[idx];
        char newMark = gridFeedback[row][i];

        if (current == 'G') continue;
        if (current == 'Y' && newMark != 'G') continue;
        usedLetters[idx] = newMark;
    }
}

// Function: sorting used letters for display
void getSortedUsed(char result[], int& count) {
    count = 0;
    for (int i = 0; i < ALPHABET; i++)
        if (usedLetters[i] != ' ')
            result[count++] = 'A' + i;
}

//  Function: Print the full 6-row grid with colors
void printGrid(int attempts) {
    cout << "\n  +-------+-------+-------+-------+-------+\n";
    for (int r = 0; r < MAX_GUESSES; r++) {
        cout << "  |";
        for (int c = 0; c < WORD_LENGTH; c++) {
            if (r < attempts) {
                char f = gridFeedback[r][c];
                if      (f == 'G') cout << GREEN  << "   " << gridLetters[r][c] << "   " << RESET << "|";
                else if (f == 'Y') cout << YELLOW << "   " << gridLetters[r][c] << "   " << RESET << "|";
                else               cout << GRAY   << "   " << gridLetters[r][c] << "   " << RESET << "|";
            } else {
                cout << "       |";
            }
        }
        cout << "\n  +-------+-------+-------+-------+-------+\n";
    }
}

// Function: Print used letters with color coding
void printUsedLetters() {
    char sorted[ALPHABET];
    int  count = 0;
    getSortedUsed(sorted, count);

    cout << "\n  Letters tried: ";
    if (count == 0) {
        cout << "none yet";
    } else {
        for (int i = 0; i < count; i++) {
            int  idx = sorted[i] - 'A';
            char mark = usedLetters[idx];
            if      (mark == 'G') cout << GREEN  << " " << sorted[i] << " " << RESET << " ";
            else if (mark == 'Y') cout << YELLOW << " " << sorted[i] << " " << RESET << " ";
            else                  cout << GRAY   << " " << sorted[i] << " " << RESET << " ";
        }
    }
    cout << "\n";
}

// Function: Print header and legend
void printHeader() {
    cout << BOLD << "\n  ======== C++ WORDLE ========\n" << RESET;
    cout << "  " << GREEN  << "  G  " << RESET << " Correct position\n";
    cout << "  " << YELLOW << "  Y  " << RESET << " Right letter, wrong spot\n";
    cout << "  " << GRAY   << "  X  " << RESET << " Not in the word\n";
}

// Function: Refresh display with grid and used letters 
void refreshDisplay(int attempts) {
    cout << string(40, '\n');
    printHeader();
    printGrid(attempts);
    printUsedLetters();
}

//  Main Function
int main() {
    string secretWord = selectWord(); //selecting a randow word from the bank using the random function
    string guess;
    int attempts = 0;

    char* letterPtr = usedLetters;
    for (int i = 0; i < ALPHABET; i++){
        *letterPtr = ' ';
        letterPtr++;
    }

    refreshDisplay(attempts);

    while (attempts < MAX_GUESSES) {
        cout << "\n  Guess " << (attempts + 1) << "/" << MAX_GUESSES << ": ";
        cin >> guess;

        for (int i = 0; i < (int)guess.length(); i++)
            guess[i] = toupper(guess[i]);

        if (!isValid(guess)) {
            cout << "  !! Must be exactly 5 letters, no numbers or spaces.\n";
            continue;
        }

        computeFeedback(guess, secretWord, attempts);
        updateUsedLetters(attempts);
        attempts++;
        refreshDisplay(attempts);

        if (guess == secretWord) {
            cout << GREEN << BOLD
                 << "  >> Got it in " << attempts
                 << (attempts == 1 ? " guess!" : " guesses! " )
                 << RESET <<" \n" ;
            return 0;
        }
    }

    cout << GRAY << "  >> Game over! The word was: " << secretWord << RESET << "\n" ;
    return 0;
}