#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

const int maxPlayers = 4;
const int numberCards = 108;
const int rows = 2;
const int maxNumbersForChoices = 20;
const int initialCardsCount = 7;
const int penaltyDrawTwo = 2;
const int penaltyDrawFour = 4;
const char redColor = 'R';
const char greenColor = 'G';
const char blueColor = 'B';
const char yellowColor = 'Y';

int myLenForCards(char cards[rows][numberCards]) {
    if (cards == nullptr) return 0;

    int size = 0;

    for (int i = 0; i < numberCards; i++) {
        if (cards[0][i] == '\0') {
            break;
        }
        size++;
    }
    return size;
}

int myLenForChoices(char* str) {
    if (str == nullptr) return 0;

    int size = 0;

    while (*str != '\0') {
        size++;
        str++;
    }
    return size;
}

int NumberOfPlayers() {
    char number[maxNumbersForChoices];
    bool isNumberRight = false;
    std::cout << "Please enter number of players: ";
    std::cin >> number;

    while (!isNumberRight) {
        if (*number > '4' || *number < '2' || myLenForChoices(number) > 1) {
            std::cout << "Invalid number" << std::endl
                << "Please enter number between 2 and 4: ";
            std::cin >> number;
        }
        else {
            isNumberRight = true;
        }
    }
    return *number - '0';
}

int choice() {
    char choice[maxNumbersForChoices];
    bool isNumberRight = false;
    std::cin >> choice;

    while (!isNumberRight) {
        if (*choice > '2' || *choice < '1' || myLenForChoices(choice) > 1) {
            std::cout << "Invalid number" << std::endl
                << "Please enter 1 or 2: ";
            std::cin >> choice;
        }
        else {
            isNumberRight = true;
        }
    }
    return *choice - '0';
}

char color() {
    char color[maxNumbersForChoices];
    bool isColorRight = false;
    std::cin >> color;

    while (!isColorRight) {
        if ((*color == redColor || *color == blueColor || *color == yellowColor || *color == greenColor) 
            && myLenForChoices(color) == 1) {
            isColorRight = true;
        }
        else {
            std::cout << "Invalid color" << std::endl
                << "Choose new color (R, G, B, Y): ";
            std::cin >> color;
        }
    }
    return *color;
}

int rightIndexAction(bool* saidUno) {
    if (saidUno == nullptr) return 0;

    char action[maxNumbersForChoices];
    bool isActionRight = false;
    bool noNumbers = false;
    std::cout << "Choose index of card you want to play: ";
    std::cin >> action;

    while (!isActionRight) {
        noNumbers = false;
        for (size_t i = 0; i < myLenForChoices(action); i++) {
            if (action[i] < '0' && action[i] > '9') {
                noNumbers = true;
            }
        }

        if (action[0] == '-' && action[1] == '1' && myLenForChoices(action) == 2) {
            return -1;
            isActionRight = true;
        } 
        else if (action[0] == 'u' && action[1] == 'n' && action[2] == 'o' && myLenForChoices(action) == 3) {
            *saidUno = true;
            isActionRight = true;
        }
        else if (noNumbers == false) {
            int result = 0;
            int multiplyNumber = 1;
            for (int i = myLenForChoices(action) - 1; i >= 0; i--) {
                result += (action[i] - '0') * multiplyNumber;
                multiplyNumber *= 10;
            }
            return result;
            isActionRight = true;
        }
        else {
            std::cout << "Invalid action" << std::endl
                << "Choose valid index: ";
            std::cin >> action;
        }
    }
}

void clear() {
    std::cout << "\x1B[2J\x1B[H";
}

void shuffleCard(char deck[rows][numberCards]) {
    if (deck == nullptr) return;

    int numberOfCards = myLenForCards(deck);

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

void deleteTheLastCards(char deck[rows][numberCards]) {
    if (deck == nullptr) return;

    for (size_t i = 0; i < myLenForCards(deck); i++)
    {
        if (deck[1][i] == 'e' || deck[0][i] == 'e') {
            deck[0][i] = '\0';
            deck[1][i] = '\0';
        }
    }
}

void writeArrayToFile(std::ofstream& file, char target[rows][numberCards]) {
    int len = myLenForCards(target);
    file << len << std::endl;
    for (int i = 0; i < len; i++) file << target[0][i];
    file << std::endl;
    for (int i = 0; i < len; i++) file << target[1][i];
    file << std::endl;
}

void saveGame(char deck[rows][numberCards], char players[maxPlayers][rows][numberCards],
    char usedDeck[rows][numberCards], int numberOfPlayers,
    int playersTurn, bool turnDirection, bool isDouble, bool isPlus4) {
    if (deck == nullptr || players == nullptr || usedDeck == nullptr) return;

    std::ofstream file("uno_save.txt");
    if (!file) {
        std::cout << "Something went wrong!" << std::endl;
        return;
    }

    file << numberOfPlayers << " " << playersTurn << " " 
        << turnDirection << " " << isDouble << " " << isPlus4 << std::endl;

    writeArrayToFile(file, deck);
    writeArrayToFile(file, usedDeck);
    for (int i = 0; i < numberOfPlayers; i++) {
        writeArrayToFile(file, players[i]);
    }

    file.close();
    std::cout << "The game is saved. You can close the game." << std::endl;
}

void readArrayFromFile(std::ifstream& file, char target[rows][numberCards]) {
    int len = 0;
    file >> len;
    for (int i = 0; i < len; i++) file >> target[0][i];
    target[0][len] = '\0';
    for (int i = 0; i < len; i++) file >> target[1][i];
    target[1][len] = '\0';
}

bool loadGame(char deck[rows][numberCards], char players[maxPlayers][rows][numberCards],
    char usedDeck[rows][numberCards], int& numberOfPlayers,
    int& playersTurn, bool& turnDirection, bool& isDouble, bool& isPlus4) {

    std::ifstream file("uno_save.txt");
    if (!file) return false;

    file >> numberOfPlayers >> playersTurn >> turnDirection >> isDouble >> isPlus4;

    readArrayFromFile(file, deck);
    readArrayFromFile(file, usedDeck);
    for (int i = 0; i < numberOfPlayers; i++) readArrayFromFile(file, players[i]);

    file.close();
    return true;
}

void addPlayersFirstCards(char deck[rows][numberCards], char player[rows][numberCards], int playerNumber) {
    if (deck == nullptr || player == nullptr) return;

    int firstNumberOfPlayerCards = initialCardsCount + (playerNumber * initialCardsCount);
    int len = numberCards - firstNumberOfPlayerCards;

    for (int i = 0; i < initialCardsCount; i++) {
        player[0][i] = deck[0][len];
        player[1][i] = deck[1][len];

        deck[0][len] = 'e';
        deck[1][len] = 'e';

        len++;
    }
    player[0][initialCardsCount] = '\0';
    player[1][initialCardsCount] = '\0';

    deleteTheLastCards(deck);
}

void addUsedDeckFirstCards(char deck[rows][numberCards], char usedDeck[rows][numberCards]) {
    if (deck == nullptr || usedDeck == nullptr) return;

    usedDeck[0][0] = deck[0][myLenForCards(deck) - 1];
    usedDeck[1][0] = deck[1][myLenForCards(deck) - 1];

    usedDeck[0][1] = '\0';
    usedDeck[1][1] = '\0';

    deck[0][myLenForCards(deck) - 1] = 'e';
    deck[1][myLenForCards(deck) - 1] = 'e';

    deleteTheLastCards(deck);
}

void drawCard(char deck[rows][numberCards], char playerCards[rows][numberCards]) {
    if (deck == nullptr || playerCards == nullptr) return;

    int deckLen = myLenForCards(deck) - 1;
    int playerLen = myLenForCards(playerCards);

    playerCards[0][playerLen] = deck[0][deckLen];
    playerCards[1][playerLen] = deck[1][deckLen];

    playerCards[0][playerLen + 1] = '\0';
    playerCards[1][playerLen + 1] = '\0';

    deck[0][deckLen] = 'e';
    deck[1][deckLen] = 'e';

    std::cout << "New card: " << playerCards[0][playerLen] << playerCards[1][playerLen] 
        << std::endl << std::endl;

    deleteTheLastCards(deck);
}

void printCards(char player[rows][numberCards], char usedDeck[rows][numberCards], int playersTurn, int choice) {
    if (player == nullptr || usedDeck == nullptr) return;

    clear();
    std::cout << "Current card: " << usedDeck[0][myLenForCards(usedDeck) - 1] 
        << usedDeck[1][myLenForCards(usedDeck) - 1] << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Player " << playersTurn << " - Your cards:" << std::endl;

    std::cout << "[-1] SAVE ";
    if (choice != 1) {
        std::cout << "[0] DRAW ";
    }
    for (int i = 0; i < myLenForCards(player); i++) {
        std::cout << '[' << i + 1 << "] " << player[0][i] << player[1][i] << " ";
    }
    if (myLenForCards(player) == 1) {
        std::cout << "[uno] Uno ";
    }

    std::cout << std::endl;
    std::cout << std::endl;
}

void removeCard(char usedDeck[rows][numberCards], char player[rows][numberCards], int playerAction) {
    if (player == nullptr || usedDeck == nullptr) return;

    int usedDeckLen = myLenForCards(usedDeck);
    int playerLen = myLenForCards(player);

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

bool isTheRightCard(char usedDeck[rows][numberCards], char player[rows][numberCards], int playerAction) {
    if (player == nullptr || usedDeck == nullptr) return false;

    if (usedDeck[0][myLenForCards(usedDeck) - 1] == player[0][playerAction - 1]
        || usedDeck[1][myLenForCards(usedDeck) - 1] == player[1][playerAction - 1]
        || player[0][playerAction - 1] == 'W'
        || usedDeck[0][myLenForCards(usedDeck) - 1] == 'W') {
        return true;
    }
    return false;
}

void checkForSpecialCard(char usedDeck[rows][numberCards], bool* turnDirection, 
    bool* isSkip, bool* isDouble1, bool* isReverse, bool* isPlus4) {
    if (turnDirection == nullptr || usedDeck == nullptr || isSkip == nullptr
        || isDouble1 == nullptr || isReverse == nullptr || isPlus4 == nullptr) return;

    int lastId = myLenForCards(usedDeck) - 1;

    if (usedDeck[1][lastId] == 'R') {
        *turnDirection = !(*turnDirection);
        *isReverse = true;
    }
    if (usedDeck[1][lastId] == 'S') {
        *isSkip = true;
    }
    if (usedDeck[1][lastId] == 'D') {
        *isDouble1 = false;
    }
    if (usedDeck[0][lastId] == 'W') {
        std::cout << "Wild card played! Choose new color (R, G, B, Y): ";
        char newColor = color();
        usedDeck[0][lastId] = newColor;

        if (usedDeck[1][lastId] == 'F') {
            *isPlus4 = false;
        }
    }
}

void isTheDeckEmpty(char deck[rows][numberCards], char usedDeck[rows][numberCards]) {
    if (deck == nullptr || usedDeck == nullptr) return;

    if (myLenForCards(deck) == 0) {
        std::cout << "No more cards in the deck. Shuffling used cards..." << std::endl;
        int len = myLenForCards(usedDeck);
        if (len <= 1) return;

        for (size_t i = 0; i < len - 1; i++) {
            if (usedDeck[1][i] == 'W' || usedDeck[1][i] == 'F') {
                deck[0][i] = 'W';
            }
            else {
                deck[0][i] = usedDeck[0][i];
            }
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
        shuffleCard(deck);
    }
}

void haveMatchCard(char player[rows][numberCards], char usedDeck[rows][numberCards], bool *foundMatch) {
    if (player == nullptr || usedDeck == nullptr || foundMatch == nullptr) return;

    for (size_t i = 0; i < myLenForCards(player); i++) {
        if (usedDeck[0][myLenForCards(usedDeck) - 1] == player[0][i]
            || usedDeck[1][myLenForCards(usedDeck) - 1] == player[1][i]
            || player[0][i] == 'W') {
            *foundMatch = true;
        }
    }
}

bool handlePenalties(char deck[rows][numberCards], char player[rows][numberCards], 
    char usedDeck[rows][numberCards], bool& isDouble, bool& isPlus4) {
    if (deck == nullptr || player == nullptr || usedDeck == nullptr) return false;

    if (!isPlus4) {
        std::cout << "Penalty! Drawing 4 cards..." << std::endl;
        for (int i = 0; i < penaltyDrawFour; i++) {
            if (myLenForCards(deck) == 0) continue;
            drawCard(deck, player);
            isTheDeckEmpty(deck, usedDeck);
        }
        isPlus4 = true;
        return true;
    }
    else if (!isDouble) {
        std::cout << "Penalty! Drawing 2 cards..." << std::endl;
        for (int i = 0; i < penaltyDrawTwo; i++) {
            if (myLenForCards(deck) == 0) continue;
            drawCard(deck, player);
            isTheDeckEmpty(deck, usedDeck);
        }
        isDouble = true;
        return true;
    }
    return false;
}

bool executePlayerTurn(char deck[rows][numberCards], char allPlayers[maxPlayers][rows][numberCards], int* playersTurn, 
    char usedDeck[rows][numberCards], bool* gameEnd, int numberOfPlayers, bool* turnDirection, bool* isDouble, 
    bool* isPlus4, bool& isSkip, bool& isReverse) {
    if (deck == nullptr || allPlayers == nullptr || playersTurn == nullptr ||
        usedDeck == nullptr || gameEnd == nullptr || turnDirection == nullptr ||
        isDouble == nullptr || isPlus4 == nullptr) {
        return false;
    }
    int continuePlaying = 0;
    bool rightAction = false;
    bool foundMatch = false;
    isSkip = false;
    isReverse = false;

    printCards(allPlayers[*playersTurn - 1], usedDeck, *playersTurn, continuePlaying);
    haveMatchCard(allPlayers[*playersTurn - 1], usedDeck, &foundMatch);
    isTheDeckEmpty(deck, usedDeck);

    if (foundMatch == false && usedDeck[0][myLenForCards(usedDeck) - 1] != 'W' && myLenForCards(deck) != 0) {
        std::cout << "No suitable cards. Automatically drawing..." << std::endl;
        drawCard(deck, allPlayers[*playersTurn - 1]);
        isTheDeckEmpty(deck, usedDeck);
        haveMatchCard(allPlayers[*playersTurn - 1], usedDeck, &foundMatch);

        if (foundMatch == false) return true;

        std::cout << "Do you want to play the card [1] Yes [2] No: ";
        continuePlaying = choice();
        
        if (continuePlaying == 2) return true;

        printCards(allPlayers[*playersTurn - 1], usedDeck, *playersTurn, continuePlaying);
    }
    bool saidUno = false;
    int playerAction = -2;

    while (!rightAction) {
        playerAction = rightIndexAction(&saidUno);

        if (playerAction == -1) {
            saveGame(deck, allPlayers, usedDeck, numberOfPlayers, *playersTurn, *turnDirection, *isDouble, *isPlus4);
            *gameEnd = true;
            return false;
        }

        if (playerAction == 0 && continuePlaying != 1 && myLenForCards(deck) != 0) {
            drawCard(deck, allPlayers[*playersTurn - 1]);
            return true;
        }

        int currentPlayerLen = myLenForCards(allPlayers[*playersTurn - 1]);
        if (playerAction >= 1 && playerAction <= currentPlayerLen && isTheRightCard(usedDeck, allPlayers[*playersTurn - 1], playerAction)) {
            if (currentPlayerLen == 1 && !saidUno) {
                std::cout << "You didn't say [uno]! Penalty card added." << std::endl;
                drawCard(deck, allPlayers[*playersTurn - 1]);
                return true;
            }
            else if (saidUno && currentPlayerLen == 1) {
				std::cout << "You said [uno]! \nNow just type 1 for the last card to be placed! \n \n";
                playerAction = rightIndexAction(&saidUno);
			}
            if (playerAction >= 1 && playerAction <= currentPlayerLen && isTheRightCard(usedDeck, allPlayers[*playersTurn - 1], playerAction)) {
                removeCard(usedDeck, allPlayers[*playersTurn - 1], playerAction);
                checkForSpecialCard(usedDeck, turnDirection, &isSkip, isDouble, &isReverse, isPlus4);
                rightAction = true;
            }
        }
    }
    return true;
}

void updateTurnOrder(int* playersTurn, int numberOfPlayers, bool turnDirection, bool isSkip, bool isReverse) {
    if (playersTurn == nullptr) return;

    if (isReverse && numberOfPlayers == 2) return;

    if (!turnDirection) {
        (*playersTurn)++;
    }
    else {
        (*playersTurn)--;
    }

    if (isSkip) {
        if (!turnDirection) {
            (*playersTurn)++;
        }
        else {
            (*playersTurn)--;
        }
    }

    if (*playersTurn <= 0) {
        *playersTurn += numberOfPlayers;
    }
    if (*playersTurn > numberOfPlayers) {
        *playersTurn -= numberOfPlayers;
    }
}

int lastPlayerTurn(int playersTurn, int numberOfPlayers, bool turnDirection) {
        if (!turnDirection) {
            (playersTurn)--;
        }
        else {
            (playersTurn)++;
        }

    if (playersTurn <= 0) {
        playersTurn += numberOfPlayers;
    }
    if (playersTurn > numberOfPlayers) {
        playersTurn -= numberOfPlayers;
    }
    return playersTurn;
}

void action(char deck[rows][numberCards], char allPlayers[maxPlayers][rows][numberCards], int* playersTurn,
    char usedDeck[rows][numberCards], bool* gameEnd, int numberOfPlayers, bool* turnDirection, bool* isDouble, bool* isPlus4) {
    if (deck == nullptr || allPlayers == nullptr || playersTurn == nullptr ||
        usedDeck == nullptr || gameEnd == nullptr || turnDirection == nullptr ||
        isDouble == nullptr || isPlus4 == nullptr) {
        return;
    }

    bool isSkip = false;
    bool isReverse = false;
    int rightPlayerTurn = *playersTurn - 1;
	int temporaryPlayersTurn = *playersTurn;
	int lastPlayerTurnIndex = lastPlayerTurn(temporaryPlayersTurn, numberOfPlayers, *turnDirection);

    if (!handlePenalties(deck, allPlayers[lastPlayerTurnIndex], usedDeck, *isDouble, *isPlus4)) {
        if (executePlayerTurn(deck, allPlayers, playersTurn, usedDeck, gameEnd, numberOfPlayers, turnDirection, isDouble, isPlus4, isSkip, isReverse)) {
            if (myLenForCards(allPlayers[rightPlayerTurn]) == 0) {
                clear();
                std::cout << "Player " << *playersTurn << " YOU WIN!" << std::endl;
                *gameEnd = true;
                return;
            }
            updateTurnOrder(playersTurn, numberOfPlayers, *turnDirection, isSkip, isReverse);
        }
    }
    else {
        updateTurnOrder(playersTurn, numberOfPlayers, *turnDirection, isSkip, isReverse);
    }
}

void playGame(char deck[rows][numberCards]) {
    if (deck == nullptr) return;

    char players[maxPlayers][rows][numberCards];
    char usedDeck[rows][numberCards];
    int numberOfPlayers = 0;
    bool gameEnd = false;
    int playersTurn = 1;
    bool turnDirection = false;
    bool isDouble = true;
    bool isPlus4 = true;

    std::cout << "1. New Game" << std::endl << "2. Load Game" << std::endl << "Choice: ";
    int choiceIfNewGame = choice();

    if (choiceIfNewGame == 2 && loadGame(deck, players, usedDeck, numberOfPlayers, playersTurn, turnDirection, isDouble, isPlus4)) {
        std::cout << "Game Loaded!" << std::endl;
    }
    else if (choiceIfNewGame == 1) {
        shuffleCard(deck);
        numberOfPlayers = NumberOfPlayers();

        for (int i = 0; i < numberOfPlayers; i++) {
            addPlayersFirstCards(deck, players[i], i);
        }
        addUsedDeckFirstCards(deck, usedDeck);
    }

    while (!gameEnd) {
        action(deck, players, &playersTurn, usedDeck, 
            &gameEnd, numberOfPlayers, &turnDirection, &isDouble, &isPlus4);
    }
}

int main() {
    srand(time(0));

    char deck[rows][numberCards] = {
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
            'W','W','W','W', 'F','F','F','F'
        }
    };

    std::cout << "-----Uno-----" << std::endl;
    playGame(deck);

    return 0;
}
