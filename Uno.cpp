#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

const int maxPlayers = 4;
const int numberCards = 108;
const int Rows = 2;

void isNumberRight(int* numberOfPlayers) {
    if (numberOfPlayers == nullptr) return;

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

int my_len(char cards[Rows][numberCards]) {
    if (cards == nullptr) return 0;

    int size = 0;

    for (int i = 0; i < numberCards; i++)
    {
        if (cards[0][i] == '\0') {
            break;
        }
        size++;
    }
    return size;
}

void SuffleCard(char deck[Rows][numberCards]) {
    if (deck == nullptr) return;

    int numberOfCards = my_len(deck);

    for (int i = 0; i < numberOfCards; i++) {

        int j = rand() % numberOfCards;

        char tempColor = deck[0][i];
        deck[0][i] = deck[0][j];
        deck[0][j] = tempColor;

        char tempValue = deck[1][i];
        deck[1][i] = deck[1][j];
        deck[1][j] = tempValue;
    }
}

void deleteTheLastCards(char deck[Rows][numberCards]) {
    if (deck == nullptr) return;

    for (size_t i = 0; i < my_len(deck); i++)
    {
        if (deck[1][i] == 'e' || deck[0][i] == 'e') {
            deck[0][i] = '\0';
            deck[1][i] = '\0';
        }
    }
}

void saveGame(char deck[Rows][numberCards], char players[maxPlayers][Rows][numberCards],
    char usedDeck[Rows][numberCards], int numberOfPlayers,
    int playersTurn, bool turnDirection, bool isDouble, bool isPlus4) {
    if (deck == nullptr || players == nullptr || usedDeck == nullptr) return;

    std::ofstream file("uno_save.txt");
    if (!file) {
        std::cout << "Something went wrong!" << std::endl;
        return;
    }

    file << numberOfPlayers << " " << playersTurn << " " << turnDirection << " " << isDouble << " " << isPlus4 << std::endl;

    auto writeRow = [&](char target[Rows][numberCards]) {
        int len = my_len(target);
        file << len << std::endl;
        for (int i = 0; i < len; i++) file << target[0][i];
        file << std::endl;
        for (int i = 0; i < len; i++) file << target[1][i];
        file << std::endl;
        };

    writeRow(deck);
    writeRow(usedDeck);
    for (int i = 0; i < numberOfPlayers; i++) {
        writeRow(players[i]);
    }

    file.close();
    std::cout << "The game is saved. You can close the game." << std::endl;
}

bool loadGame(char deck[Rows][numberCards], char players[maxPlayers][Rows][numberCards],
    char usedDeck[Rows][numberCards], int& numberOfPlayers,
    int& playersTurn, bool& turnDirection, bool& isDouble, bool& isPlus4) {

    std::ifstream file("uno_save.txt");
    if (!file) return false;

    file >> numberOfPlayers >> playersTurn >> turnDirection >> isDouble >> isPlus4;

    auto readRow = [&](char target[Rows][numberCards]) {
        int len;
        file >> len;
        for (int i = 0; i < len; i++) file >> target[0][i];
        target[0][len] = '\0';
        for (int i = 0; i < len; i++) file >> target[1][i];
        target[1][len] = '\0';
        };

    readRow(deck);
    readRow(usedDeck);
    for (int i = 0; i < numberOfPlayers; i++) readRow(players[i]);

    file.close();
    return true;
}

void addPlayersFirstCards(char deck[Rows][numberCards], char player[Rows][numberCards], int playerNumber) {
    if (deck == nullptr || player == nullptr) return;


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
    if (deck == nullptr || usedDeck == nullptr) return;

    usedDeck[0][0] = deck[0][my_len(deck) - 1];
    usedDeck[1][0] = deck[1][my_len(deck) - 1];

    usedDeck[0][1] = '\0';
    usedDeck[1][1] = '\0';

    deck[0][my_len(deck) - 1] = 'e';
    deck[1][my_len(deck) - 1] = 'e';

    deleteTheLastCards(deck);
}

void drawCard(char deck[Rows][numberCards], char playerCards[Rows][numberCards]) {
    if (deck == nullptr || playerCards == nullptr) return;


    int deckLen = my_len(deck) - 1;
    int playerLen = my_len(playerCards);

    playerCards[0][playerLen] = deck[0][deckLen];
    playerCards[1][playerLen] = deck[1][deckLen];

    playerCards[0][playerLen + 1] = '\0';
    playerCards[1][playerLen + 1] = '\0';

    deck[0][deckLen] = 'e';
    deck[1][deckLen] = 'e';

    std::cout << "New card: " << playerCards[0][playerLen] << playerCards[1][playerLen] << std::endl << std::endl;

    deleteTheLastCards(deck);
}

void printCards(char player[Rows][numberCards], char usedDeck[Rows][numberCards], int playersTurn) {
    if (player == nullptr || usedDeck == nullptr) return;

    std::cout << "Current card: " << usedDeck[0][my_len(usedDeck) - 1] << usedDeck[1][my_len(usedDeck) - 1] << " ";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Player " << playersTurn << " - Your cards:" << std::endl;

    std::cout << "[-1] SAVE [0] DRAW ";
    for (int i = 0; i < my_len(player); i++) {
        std::cout << '[' << i + 1 << "] " << player[0][i] << player[1][i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void RemoveCard(char usedDeck[Rows][numberCards], char player[Rows][numberCards], int playerAction) {
    if (player == nullptr || usedDeck == nullptr) return;

    int usedDeckLen = my_len(usedDeck);
    int playerLen = my_len(player);

    usedDeck[0][usedDeckLen] = player[0][playerAction - 1];
    usedDeck[1][usedDeckLen] = player[1][playerAction - 1];
    usedDeck[0][usedDeckLen + 1] = '\0';
    usedDeck[1][usedDeckLen + 1] = '\0';

    for (size_t i = playerAction - 1; i < playerLen - 1; i++) {
        player[0][i] = player[0][i + 1];
        player[1][i] = player[1][i + 1];
    }
    player[0][playerLen - 1] = '\0';
    player[1][playerLen - 1] = '\0';
}

bool isTheRightCard(char usedDeck[Rows][numberCards], char player[Rows][numberCards], int playerAction) {
    if (player == nullptr || usedDeck == nullptr) return false;

    if (usedDeck[0][my_len(usedDeck) - 1] == player[0][playerAction - 1]
        || usedDeck[1][my_len(usedDeck) - 1] == player[1][playerAction - 1]
        || player[0][playerAction - 1] == 'W'
        || usedDeck[0][my_len(usedDeck) - 1] == 'W') {
        return true;
    }
    return false;
}

void oneCardLeft(char player[Rows][numberCards], int playerAction, 
    char usedDeck[Rows][numberCards], bool *rightAction) {
    if (player == nullptr || usedDeck == nullptr || rightAction == nullptr) return;

    if (playerAction != 0 && my_len(player) == 1 && isTheRightCard(usedDeck, player, playerAction)) {
        char uno;
        bool rightChar = false;

        while (!rightChar) {
            std::cout << "Type [u] Uno" << std::endl;
            std::cin >> uno;
            if (uno == 'u') {
                RemoveCard(usedDeck, player, playerAction);
                *rightAction = true;
                rightChar = true;
            }
        }
    }
}

void CheckForSpecialCard(char usedDeck[Rows][numberCards], bool* turnDirection, 
    bool* isSkip, bool* isDouble1, bool* isReverse, bool* isPlus4) {
    if (turnDirection == nullptr || usedDeck == nullptr || isSkip == nullptr
        || isDouble1 == nullptr || isReverse == nullptr || isPlus4 == nullptr) return;


    int lastId = my_len(usedDeck) - 1;

    if (usedDeck[1][lastId] == 'R') {
        *turnDirection = !(*turnDirection);
        *isReverse = true;
    }
    if (usedDeck[1][lastId] == 'S') {
        *isSkip = false;
    }
    if (usedDeck[1][lastId] == 'D') {
        *isDouble1 = false;
    }
    if (usedDeck[0][lastId] == 'W') {
        char newColor;
        std::cout << "Wild card played! Choose new color (R, G, B, Y): ";
        std::cin >> newColor;

        usedDeck[0][lastId] = newColor;

        if (usedDeck[1][lastId] == '4') {
            *isPlus4 = false;
        }
    }
}

void isTheDeckEmpty(char deck[Rows][numberCards], char usedDeck[Rows][numberCards]) {
    if (deck == nullptr || usedDeck == nullptr) return;

    if (my_len(deck) == 0) {
        std::cout << "No more cards in the deck. Shuffling used cards..." << std::endl;

        int len = my_len(usedDeck);
        if (len <= 1) return;

        for (size_t i = 0; i < len - 1; i++) {
            deck[0][i] = usedDeck[0][i];
            deck[1][i] = usedDeck[1][i];

            usedDeck[0][i] = 'e';
            usedDeck[1][i] = 'e';
        }

        deck[0][len - 1] = '\0';
        deck[1][len - 1] = '\0';

        char lastC = usedDeck[0][len - 1];
        char lastV = usedDeck[1][len - 1];

        usedDeck[0][len - 1] = 'e';
        usedDeck[1][len - 1] = 'e';

        usedDeck[0][0] = lastC;
        usedDeck[1][0] = lastV;

        usedDeck[0][1] = '\0';
        usedDeck[1][1] = '\0';

        deleteTheLastCards(deck);
        deleteTheLastCards(usedDeck);
        SuffleCard(deck);
    }
}

void haveMatchCard(char player[Rows][numberCards], char usedDeck[Rows][numberCards], bool *foundMatch) {
    if (player == nullptr || usedDeck == nullptr || foundMatch == nullptr) return;


    for (size_t i = 0; i < my_len(player); i++) {
            if (usedDeck[0][my_len(usedDeck) - 1] == player[0][i]
                || usedDeck[1][my_len(usedDeck) - 1] == player[1][i]
                || player[0][i] == 'W') {
                *foundMatch = true;
            }
        }
}

void Action(char deck[Rows][numberCards], char allPlayers[maxPlayers][Rows][numberCards], int* playersTurn,
    char usedDeck[Rows][numberCards], bool* gameEnd, int numberOfPlayers, bool* turnDirection, bool* isDouble, bool* isPlus4) {
    if (deck == nullptr || usedDeck == nullptr || allPlayers == nullptr ||
        playersTurn == nullptr || gameEnd == nullptr || turnDirection == nullptr
        || isDouble == nullptr || isPlus4 == nullptr) return;


    char (*player)[numberCards] = allPlayers[*playersTurn - 1];

    printCards(player, usedDeck, *playersTurn);
    
    int playerAction = -2;
    bool turnDirection1 = *turnDirection;
    bool rightAction = false;
    bool foundMatch = false;
    bool isSkip = true;
    bool isDouble1 = *isDouble;
    bool isPlus4_1 = *isPlus4;
    bool isReverse = false;

    haveMatchCard(player, usedDeck, &foundMatch);
    

    if (!isPlus4_1) {
        std::cout << "Penalty! Drawing 4 cards..." << std::endl;
        for (int i = 0; i < 4; i++) {
            drawCard(deck, player);
            isTheDeckEmpty(deck, usedDeck);
        }
        *isPlus4 = true;
        rightAction = true;
    } 
    else if (!isDouble1) {
        std::cout << "Penalty! Drawing 2 cards..." << std::endl;
        for (int i = 0; i < 2; i++) {
            drawCard(deck, player);
            isTheDeckEmpty(deck, usedDeck);
        }
        *isDouble = true;
        rightAction = true;
    }
    else if (foundMatch == false && usedDeck[0][my_len(usedDeck) - 1] != 'W') {
        std::cout << "No suitable cards. Automatically drawing..." << std::endl;
        drawCard(deck, player);
        isTheDeckEmpty(deck, usedDeck);
        rightAction = true;
    }

    while (!rightAction) {
        std::cout << "Choose index of card you want to play: ";
        std::cin >> playerAction;

        if (playerAction == -1) {
            saveGame(deck, allPlayers, usedDeck, numberOfPlayers, *playersTurn, *turnDirection, *isDouble, *isPlus4);
            exit(0);
        }

        oneCardLeft(player, playerAction, usedDeck, &rightAction);

        if (playerAction == 0 && !rightAction) {
            drawCard(deck, player);
            isTheDeckEmpty(deck, usedDeck);
            rightAction = true;
        }
        else if ((playerAction >= 1 && playerAction <= my_len(player)) && !rightAction  && isTheRightCard(usedDeck, player, playerAction)) {
            RemoveCard(usedDeck, player, playerAction);
            CheckForSpecialCard(usedDeck, &turnDirection1, &isSkip, &isDouble1, &isReverse, isPlus4);
            *isDouble = isDouble1;
            *turnDirection = turnDirection1;
            rightAction = true;
        }
    }
    if (!(isReverse && numberOfPlayers == 2)) {
        if (!(*turnDirection)) {
            (*playersTurn)++;
        }
        else if (*turnDirection) {
            (*playersTurn)--;
        }
    }
    if (*playersTurn <= 0) {
        *playersTurn = numberOfPlayers;
    }
    if (*playersTurn >= (numberOfPlayers + 1)) {
        *playersTurn = 1;
    }

    if (!(*turnDirection) && !isSkip) {
        (*playersTurn)++;
    }
    else if (*turnDirection && !isSkip) {
        (*playersTurn)--;
    }

    if (*playersTurn <= 0) {
        *playersTurn = numberOfPlayers;
    }
    if (*playersTurn >= (numberOfPlayers + 1)) {
        *playersTurn = 1;
    }

    if (my_len(player) == 0) {
        system("cls");

        if (!(*turnDirection)) {
            (*playersTurn)--;
        }
        else if (*turnDirection) {
            (*playersTurn)++;
        }

        if (*playersTurn == 0) {
            *playersTurn = numberOfPlayers;
        }
        if (*playersTurn == (numberOfPlayers + 1)) {
            *playersTurn = 1;
        }
        std::cout << "Player " << *playersTurn << " YOU WIN";
        *gameEnd = true;
    }
}

void playGame(char deck[Rows][numberCards]) {
    if (deck == nullptr) return;

    char players[maxPlayers][Rows][numberCards];
    char usedDeck[Rows][numberCards];
    
    int choice = 0; 
    int numberOfPlayers = 0;
    bool gameEnd = false;
    int playersTurn = 1;
    bool turnDirection = false;
    bool isDouble = true;
    bool isPlus4 = true;

    std::cout << "1. New Game" << std::endl << "2. Load Game" << std::endl << "Choice: ";
    std::cin >> choice;

    if (choice == 2 && loadGame(deck, players, usedDeck, numberOfPlayers, playersTurn, turnDirection, isDouble, isPlus4)) {
        std::cout << "Game Loaded!" << std::endl;
    }
    else {
        SuffleCard(deck);

        std::cout << "Please enter number of players: ";
        std::cin >> numberOfPlayers;
        isNumberRight(&numberOfPlayers);

        for (int i = 0; i < numberOfPlayers; i++) {
            addPlayersFirstCards(deck, players[i], i);
        }
        addUsedDeckFirstCards(deck, usedDeck);
    }

    while (!gameEnd) {
        Action(deck, players, &playersTurn,
            usedDeck, &gameEnd, numberOfPlayers, &turnDirection, &isDouble, &isPlus4);
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

    std::cout << "-----Uno-----" << std::endl;
    playGame(deck);

    return 0;
}
