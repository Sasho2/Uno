#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

const int maxPlayers = 4;
const int numberCards = 108;
const int Rows = 2;
const int maxNumbersForChoices = 20;
const int initialCardsCount = 7;
const int penaltyDrawTwo = 2;
const int penaltyDrawFour = 4;

int my_len(char cards[Rows][numberCards]) {
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

int Choice() {
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

char Color() {
    char color[maxNumbersForChoices];
    bool isColorRight = false;

    std::cin >> color;

    while (!isColorRight) {
        if ((*color == 'R' || *color == 'B' || *color == 'Y' || *color == 'G') && myLenForChoices(color) == 1) {
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

int Action() {
    char action[maxNumbersForChoices];
    bool isActionRight = false;
    bool noNumbers = false;
    std::cout << "Choose index of card you want to play: ";
    std::cin >> action;

    while (!isActionRight) {
        for (size_t i = 0; i < myLenForChoices(action); i++) {
            if (action[i] < '0' && action[i] > '9') {
                noNumbers = true;
            }
        }

        if (action[0] == '-' && action[1] == '1' && myLenForChoices(action) == 2) {
            return -1;
            isActionRight = true;
        } 
        else if (noNumbers == false) {
            int result = 0;
            int multiplyNumber = 1;
            for (int i = myLenForChoices(action) - 1; i >= 0; i--) {
                result = (action[i] - '0') * multiplyNumber;
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

void ShuffleCard(char deck[Rows][numberCards]) {
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

void writeArrayToFile(std::ofstream& file, char target[Rows][numberCards]) {
    int len = my_len(target);
    file << len << std::endl;
    for (int i = 0; i < len; i++) file << target[0][i];
    file << std::endl;
    for (int i = 0; i < len; i++) file << target[1][i];
    file << std::endl;
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

void readArrayFromFile(std::ifstream& file, char target[Rows][numberCards]) {
    int len = 0;
    file >> len;
    for (int i = 0; i < len; i++) file >> target[0][i];
    target[0][len] = '\0';
    for (int i = 0; i < len; i++) file >> target[1][i];
    target[1][len] = '\0';
}

bool loadGame(char deck[Rows][numberCards], char players[maxPlayers][Rows][numberCards],
    char usedDeck[Rows][numberCards], int& numberOfPlayers,
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

void addPlayersFirstCards(char deck[Rows][numberCards], char player[Rows][numberCards], int playerNumber) {
    if (deck == nullptr || player == nullptr) return;

    int playerNumberMinus7 = initialCardsCount + (playerNumber * initialCardsCount);

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

    std::cout << "New card: " << playerCards[0][playerLen] << playerCards[1][playerLen] 
        << std::endl << std::endl;

    deleteTheLastCards(deck);
}

void printCards(char player[Rows][numberCards], char usedDeck[Rows][numberCards], int playersTurn, int choice) {
    if (player == nullptr || usedDeck == nullptr) return;

    std::cout << "Current card: " << usedDeck[0][my_len(usedDeck) - 1] 
        << usedDeck[1][my_len(usedDeck) - 1] << " ";
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Player " << playersTurn << " - Your cards:" << std::endl;

    std::cout << "[-1] SAVE ";
    if (choice != 1) {
        std::cout << "[0] DRAW ";
    }
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
        

        std::cout << "Wild card played! Choose new color (R, G, B, Y): ";
        char newColor = Color();
        usedDeck[0][lastId] = newColor;

        if (usedDeck[1][lastId] == 'F') {
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
        ShuffleCard(deck);
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
    char usedDeck[Rows][numberCards], bool* gameEnd, int numberOfPlayers, 
    bool* turnDirection, bool* isDouble, bool* isPlus4) {
    if (deck == nullptr || usedDeck == nullptr || allPlayers == nullptr ||
        playersTurn == nullptr || gameEnd == nullptr || turnDirection == nullptr
        || isDouble == nullptr || isPlus4 == nullptr) return;

    int choice = 0;
    bool turnDirection1 = *turnDirection;
    bool rightAction = false;
    bool foundMatch = false;
    bool isSkip = true;
    bool isDouble1 = *isDouble;
    bool isPlus4_1 = *isPlus4;
    bool isReverse = false;

    printCards(allPlayers[*playersTurn - 1], usedDeck, *playersTurn, choice);
    haveMatchCard(allPlayers[*playersTurn - 1], usedDeck, &foundMatch);
    isTheDeckEmpty(deck, usedDeck);

    if (!isPlus4_1) {
        std::cout << "Penalty! Drawing 4 cards..." << std::endl;
        for (int i = 0; i < penaltyDrawFour; i++) {
            if (my_len(deck) == 0) continue;
            drawCard(deck, allPlayers[*playersTurn - 1]);
            isTheDeckEmpty(deck, usedDeck);
        }
        *isPlus4 = true;
        rightAction = true;
    } 
    else if (!isDouble1) {
        std::cout << "Penalty! Drawing 2 cards..." << std::endl;
        for (int i = 0; i < penaltyDrawTwo; i++) {
            if (my_len(deck) == 0) continue;
            drawCard(deck, allPlayers[*playersTurn - 1]);
            isTheDeckEmpty(deck, usedDeck);
        }
        *isDouble = true;
        rightAction = true;
    }
    else if (foundMatch == false && usedDeck[0][my_len(usedDeck) - 1] != 'W' && my_len(deck) != 0) {
        std::cout << "No suitable cards. Automatically drawing..." << std::endl;
        drawCard(deck, allPlayers[*playersTurn - 1]);
        isTheDeckEmpty(deck, usedDeck);
        haveMatchCard(allPlayers[*playersTurn - 1], usedDeck, &foundMatch);

        if (foundMatch == false) {
            rightAction = true;
        }
        else {
            std::cout << "Do you want to play the card [1] Yes [2] No" << std::endl;
            std::cout << "Type the index: ";
            choice = Choice();
            if (choice == 2) {
                rightAction = true;
            }
            else if (choice == 1) {
                printCards(allPlayers[*playersTurn - 1], usedDeck, *playersTurn, choice);
            }
        }
    }

    while (!rightAction) {
        int playerAction = Action();

        oneCardLeft(allPlayers[*playersTurn - 1], playerAction, usedDeck, &rightAction);

        if (playerAction == -1) {
            saveGame(deck, allPlayers, usedDeck, numberOfPlayers, *playersTurn, *turnDirection, *isDouble, *isPlus4);
            *gameEnd = true;
            break;
        }
        else if (playerAction == 0 && !rightAction && choice != 1 && my_len(deck) != 0) {
            drawCard(deck, allPlayers[*playersTurn - 1]);
            rightAction = true;
        }
        else if ((playerAction >= 1 && playerAction <= my_len(allPlayers[*playersTurn - 1])) 
            && !rightAction && isTheRightCard(usedDeck, allPlayers[*playersTurn - 1], playerAction)) {
            RemoveCard(usedDeck, allPlayers[*playersTurn - 1], playerAction);
            CheckForSpecialCard(usedDeck, &turnDirection1, &isSkip, &isDouble1, &isReverse, isPlus4);
            *isDouble = isDouble1;
            *turnDirection = turnDirection1;
            rightAction = true;
        }
    }

    if (my_len(allPlayers[*playersTurn - 1]) == 0) {
        if (!(*turnDirection)) {
            (*playersTurn)--;
        }
        else if (*turnDirection) {
            (*playersTurn)++;
        }
        if (*playersTurn <= 0) {
            *playersTurn += numberOfPlayers;
        }
        else if (*playersTurn >= (numberOfPlayers + 1)) {
            *playersTurn -= numberOfPlayers;
        }
        std::cout << "Player " << *playersTurn << " YOU WIN";
        *gameEnd = true;
    }

    if (!(isReverse && numberOfPlayers == 2)) {
        if (!(*turnDirection)) {
            (*playersTurn)++;
        }
        else if (*turnDirection) {
            (*playersTurn)--;
        }
    }
    
    if (!(*turnDirection) && !isSkip) {
        (*playersTurn)++;
    }
    else if (*turnDirection && !isSkip) {
        (*playersTurn)--;
    }

    if (*playersTurn <= 0) {
        *playersTurn += numberOfPlayers;
    }
    else if (*playersTurn >= (numberOfPlayers + 1)) {
        *playersTurn -= numberOfPlayers;
    }
}

void playGame(char deck[Rows][numberCards]) {
    if (deck == nullptr) return;

    char players[maxPlayers][Rows][numberCards];
    char usedDeck[Rows][numberCards];
    int numberOfPlayers = 0;
    bool gameEnd = false;
    int playersTurn = 1;
    bool turnDirection = false;
    bool isDouble = true;
    bool isPlus4 = true;

    std::cout << "1. New Game" << std::endl << "2. Load Game" << std::endl << "Choice: ";
    int choice = Choice();

    if (choice == 2 && loadGame(deck, players, usedDeck, numberOfPlayers, playersTurn, turnDirection, isDouble, isPlus4)) {
        std::cout << "Game Loaded!" << std::endl;
    }
    else if (choice == 1) {
        ShuffleCard(deck);

        std::cout << "Please enter number of players: ";
        numberOfPlayers = NumberOfPlayers();

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
            'W','W','W','W', 'F','F','F','F'
        }
    };

    std::cout << "-----Uno-----" << std::endl;
    playGame(deck);

    return 0;
}
