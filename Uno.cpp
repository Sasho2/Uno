#include <iostream>
#include <cstdlib>
#include <ctime>

const int numberCards = 108;
const int Rows = 2;

void isNumberRight(int *numberOfPlayers) {
    bool isNumberRight = false;

    while (!isNumberRight) {
        if (*numberOfPlayers > 4 || *numberOfPlayers < 2) {
            std::cout << "Invalid number" << std::endl
                << "Please enter number between 2 and 4" << std::endl;
            std::cin >> *numberOfPlayers;
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

    deleteTheLastCards(deck);
}

void addUsedDeckFirstCards(char deck[Rows][numberCards], char usedDeck[Rows][numberCards]) {
    usedDeck[0][0] = deck[0][my_len(deck) - 1];
    usedDeck[1][0] = deck[1][my_len(deck) - 1];

    usedDeck[0][1] = '\0';
    usedDeck[1][1] = '\0';

    deck[0][my_len(deck) - 1] = 'e';
    deck[1][my_len(deck) - 1] = 'e';

    deleteTheLastCards(deck);
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

void printCards(char player[Rows][numberCards], char usedDeck[Rows][numberCards]) {
    std::cout << "Last card: " <<  usedDeck[0][my_len(usedDeck) - 1] << usedDeck[1][my_len(usedDeck) - 1] << " ";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "[0] draw ";
    for (int i = 0; i < my_len(player); i++) {
        std::cout << '[' << i + 1 << "] " << player[0][i] << player[1][i] << " ";
    }
    std::cout << std::endl;
}

void RemoveCard(char usedDeck[Rows][numberCards], char player[Rows][numberCards], int playerAction, bool *turnDirection) {
    int usedDeckLen = my_len(usedDeck);

    usedDeck[0][usedDeckLen] = player[0][playerAction - 1];
    usedDeck[1][usedDeckLen] = player[1][playerAction - 1];

    if (usedDeck[1][usedDeckLen] == 'R') {
        if (*turnDirection) {
            *turnDirection = false;
        }
        else if (!(*turnDirection)) {
            *turnDirection = true;
        }
    }

    usedDeck[0][usedDeckLen + 1] = '\0';
    usedDeck[1][usedDeckLen + 1] = '\0';
        
    for (size_t i = playerAction - 1; i < my_len(player); i++) {
        player[0][i] = player[0][i + 1];
        player[1][i] = player[1][i + 1];
    }
}

bool isTheRightCard(char usedDeck[Rows][numberCards], char player[Rows][numberCards], int playerAction) {
    if (usedDeck[0][my_len(usedDeck) - 1] == player[0][playerAction - 1] 
        || usedDeck[1][my_len(usedDeck) - 1] == player[1][playerAction - 1]
        || player[0][playerAction - 1] == 'W'
        || usedDeck[0][my_len(usedDeck) - 1] == 'W') {
        return true;
    }
    return false;
}

void oneCardLeft(char player[Rows][numberCards], int playerAction, char usedDeck[Rows][numberCards], bool *rightAction, bool turnDirection) {
    if (playerAction != 0 && my_len(player) == 1 && isTheRightCard(usedDeck, player, playerAction)) {
        char uno;
        std::cout << "Type [u] Uno" << std::endl;
        std::cin >> uno;

        if (uno == 'u') {
            RemoveCard(usedDeck, player, playerAction, &turnDirection);
            *rightAction = true;
        }
    }
}

void Action(char deck[Rows][numberCards], char player[Rows][numberCards], int playerAction, int *playersTurn, char usedDeck[Rows][numberCards], bool *gameEnd, int numberOfPlayers) {
    
    printCards(player, usedDeck);
    
    bool rightAction = false;
    bool turnDirection = false;

    while (!rightAction) {
        std::cin >> playerAction;

        oneCardLeft(player, playerAction, usedDeck, &rightAction, turnDirection);

        if (playerAction == 0 && !rightAction) {
            drawCard(deck, player);
            rightAction = true;
        }
        else if ((playerAction >= 1 && playerAction <= my_len(player)) && isTheRightCard(usedDeck, player, playerAction) && !rightAction) {
            RemoveCard(usedDeck, player, playerAction, &turnDirection);
            rightAction = true;
        }
        else {
            std::cout << "Wrong Action" << std::endl;
        }
    }
    system("cls");

    if (!turnDirection) {
        *playersTurn++;
    } else if (turnDirection) {
        *playersTurn--;
    }

    if (*playersTurn == 0) {
        *playersTurn = numberOfPlayers;
    }



    if (my_len(player) == 0) {
        std::cout << "Player "<< playersTurn << " YOU WIN";
        *gameEnd = true;
    }
}

void twoPlayerGame(char deck[Rows][numberCards]) {
    char playerOne[Rows][numberCards];
    char playerTwo[Rows][numberCards];

    int numberOfPlayers = 2;

    char usedDeck[Rows][numberCards];

    addPlayersFirstCards(deck, playerOne, 0);
    addPlayersFirstCards(deck, playerTwo, 1); 
    addUsedDeckFirstCards(deck, usedDeck);

    bool gameEnd = false;
    int playersTurn = 1;

    while (!gameEnd) {
        int playerAction = -2;

        if (playersTurn == 1 && !gameEnd) {
            Action(deck, playerOne, playerAction, &playersTurn, usedDeck, &gameEnd, numberOfPlayers);
            playersTurn++;
        } else if (playersTurn == 2 && !gameEnd) {
            Action(deck, playerTwo, playerAction, &playersTurn, usedDeck, &gameEnd, numberOfPlayers);
            playersTurn--;
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
    
    int numberOfPlayers = 0;
    std::cin >> numberOfPlayers;

    isNumberRight(&numberOfPlayers);

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
