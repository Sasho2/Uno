#include <iostream>
#include <cstdlib>
#include <ctime>

const int numberCards = 108;
const int Rows = 2;

void isNumberRight(int numberOfPlayers) {
    bool isNumberRight = false;

        while (!isNumberRight) {
            if (numberOfPlayers > 4 || numberOfPlayers < 2) {
                std::cout << "Invalid number" << std::endl << "Please enter number between 2 and 4" << std::endl;
                std::cin >> numberOfPlayers;
            }
            else {
                isNumberRight = true;
            }
        }
}


void SuffleCard(char deck[Rows][numberCards]) {

    for (int i = 0; i < numberCards; i++) {

        int j = rand() % numberCards;

        char tempColor = deck[0][i];
        deck[0][i] = deck[0][j];
        deck[0][j] = tempColor;

        char tempValue = deck[1][i];
        deck[1][i] = deck[1][j];
        deck[1][j] = tempValue;
    }
}


int main() {

    srand(time(0));

    char deck[Rows][numberCards] = {
     {
            'R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R',
            'G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G','G',
            'B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B',
            'Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y',
            'W','W','W','W', 'W','W','W','W'
        },
        {
            '0','1','2','3','4','5','6','7','8','9','1','2','3','4','5','6','7','8','9','S','S','R','R','D','D',
            '0','1','2','3','4','5','6','7','8','9','1','2','3','4','5','6','7','8','9','S','S','R','R','D','D',
            '0','1','2','3','4','5','6','7','8','9','1','2','3','4','5','6','7','8','9','S','S','R','R','D','D',
            '0','1','2','3','4','5','6','7','8','9','1','2','3','4','5','6','7','8','9','S','S','R','R','D','D',
            'W','W','W','W', '4','4','4','4'
        }
    };

    SuffleCard(deck);
    
    int numberOfPlayers;
    std::cin >> numberOfPlayers;

    isNumberRight(numberOfPlayers);
    

    char playerOne[Rows][numberCards];
    char playerTwo[Rows][numberCards];
    char playerThree[Rows][numberCards];
    char playerFour[Rows][numberCards];





    for (int i = 0; i < numberCards; i++) {
        std::cout << "Karta " << i << ": " << deck[0][i] << " " << deck[1][i] << std::endl;
    }

    return 0;
}
