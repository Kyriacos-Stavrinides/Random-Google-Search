#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

std::vector<std::string> readDictionary(const std::string& dictionaryFile) {
    std::vector<std::string> words;
    std::ifstream file(dictionaryFile);

    if (file.is_open()) {
        std::string word;
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    }
    else {
        std::cerr << "Failed to open dictionary file: " << dictionaryFile << std::endl;
    }

    return words;
}

std::string generateRandomWord(const std::vector<std::string>& words) {
    int index = rand() % words.size();
    return words[index];
}

void performGoogleSearch(const std::string& searchQuery) {
    // Format the search query to replace spaces with plus signs
    std::string formattedQuery = searchQuery;
    std::replace(formattedQuery.begin(), formattedQuery.end(), ' ', '+');

    // Generate the search URL
    std::string searchURL = "https://www.google.com/search?q=" + formattedQuery;

    // Open the search URL in a web browser
    std::string openCommand;
#ifdef _WIN32
    openCommand = "start " + searchURL;
#elif __APPLE__
    openCommand = "open " + searchURL;
#elif __linux__
    openCommand = "xdg-open " + searchURL;
#else
    std::cerr << "Unsupported operating system" << std::endl;
    exit(1);
#endif

    system(openCommand.c_str());
}

int main() {
    srand(time(nullptr));

    // Read dictionary file
    std::vector<std::string> words = readDictionary("dictionary.txt");

    // Generate a random word
    std::string randomWord = generateRandomWord(words);

    // Perform the initial Google search
    performGoogleSearch(randomWord);

    // Generate a random time between 40 to 120 minutes
    int randomTime = (rand() % 81) + 40;  // Generates a random number between 40 and 120 (inclusive)

    // Convert minutes to seconds
    int remainingTime = randomTime * 60;

    // Display the remaining time every second
    while (remainingTime > 0) {
        std::cout << "Next search in " << remainingTime << " seconds. Press 's' to search now.\n";

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 's') {
                remainingTime = 0;
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        --remainingTime;
    }

    while (true) {
        // Generate a new random word
        randomWord = generateRandomWord(words);

        // Perform the Google search with the new random word
        performGoogleSearch(randomWord);

        // Generate a random time between 40 to 120 minutes
        randomTime = (rand() % 81) + 40;  // Generates a random number between 40 and 120 (inclusive)

        // Convert minutes to seconds
        remainingTime = randomTime * 60;

        // Display the remaining time every second
        while (remainingTime > 0) {
            std::cout << "Next search in " << remainingTime << " seconds. Press 's' to search now.\n";

            if (_kbhit()) {
                char ch = _getch();
                if (ch == 's') {
                    remainingTime = 0;
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
            --remainingTime;
        }
    }

    return 0;
}
