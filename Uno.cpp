#include <iostream>
#include <cstdlib>
#include <ctime>

const int numberCards = 108;
const int Rows = 2;

void isNumberRight(int numberOfPlayers) {
    bool isNumberRight = false;

    while (!isNumberRight) {
        if (numberOfPlayers > 4 || numberOfPlayers < 2) {
            std::cout << "Invalid number" << std::endl
                << "Please enter number between 2 and 4" << std::endl;
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

int my_len(char deck[Rows][numberCards]) {
    int size = 0;

    for (int i = 0; i < numberCards; i++)
    {
        if (deck[0][i] == '\0') {
            break; 
        }
        size++;
    }
    return size;
}

void deleteTheLastCards(char deck[Rows][numberCards]) {
    for (size_t i = 0; i < my_len(deck); i++)
    {
        if (deck[1][i] == 'e' || deck[0][i] == 'e') {
            deck[0][i] = '\0';
            deck[1][i] = '\0';
        }
    }
}

void addPlayersFirstCards(char deck[Rows][numberCards], char player[Rows][numberCards], int playerNumber) {
    int playerNumberMinus7 = 7 + (playerNumber * 7);
    
    for (int i = 0; i < 7; i++) {
        int len = i + my_len(deck) - playerNumberMinus7;

        player[0][i] = deck[0][len];
        player[1][i] = deck[1][len];

        deck[0][len] = 'e';
        deck[1][len] = 'e';
    }
    player[0][7] = '\0';
    player[1][7] = '\0';
}

void drawCard(char deck[Rows][numberCards], char playerCards[Rows][numberCards]) {
    int deckLen = my_len(deck) - 1;
    int playerLen = my_len(playerCards);

    playerCards[0][playerLen] = deck[0][deckLen];
    playerCards[1][playerLen] = deck[1][deckLen];

    playerCards[0][playerLen + 1] = '\0';
    playerCards[1][playerLen + 1] = '\0';

    deck[0][deckLen] = 'e';
    deck[1][deckLen] = 'e';

    deleteTheLastCards(deck);
}

void printCards(char player[Rows][numberCards]) {
    for (int i = 0; i < my_len(player); i++) {
        std::cout << '[' << i << "] " << player[0][i] << player[1][i] << " ";
    }
    std::cout << "[d] draw" << std::endl;
}

void twoPlayerGame(char deck[Rows][numberCards]) {
    char playerOne[Rows][numberCards];
    char playerTwo[Rows][numberCards];

    addPlayersFirstCards(deck, playerOne, 0);
    addPlayersFirstCards(deck, playerTwo, 1); 

    deleteTheLastCards(deck);

    bool gameEnd = false;
    int playersTurn = 1;

    while (!gameEnd) {
        char playerAction = 'z';

        if (playersTurn == 1) {
            printCards(playerOne);
            std::cin >> playerAction;
            if (playerAction == 'd') {
                drawCard(deck, playerOne);
                playersTurn++;
            }
        } else if (playersTurn == 2) {
            printCards(playerTwo);
            std::cin >> playerAction;
            if (playerAction == 'd') {
                drawCard(deck, playerTwo);
                playersTurn--;
            }
        }
        
    }

}

void threePlayerGame(char deck[Rows][numberCards]) {
    char playerOne[Rows][numberCards];
    char playerTwo[Rows][numberCards];
    char playerThree[Rows][numberCards];


}

void fourPlayerGame(char deck[Rows][numberCards]) {
    char playerOne[Rows][numberCards];
    char playerTwo[Rows][numberCards];
    char playerThree[Rows][numberCards];
    char playerFour[Rows][numberCards];
    

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

    if (numberOfPlayers == 2) {
        twoPlayerGame(deck);
    }
    else if (numberOfPlayers == 3) {
        threePlayerGame(deck);
    }
    else if (numberOfPlayers == 4) {
        fourPlayerGame(deck);
    }

    return 0;
}
