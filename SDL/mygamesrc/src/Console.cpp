// Console.cpp
#include "Console.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <SDL2/SDL.h> // �K�[?�@��

extern bool running;
extern bool gameStarted;
extern std::string playerID;
extern std::map<std::string, int> playerScores;
extern SDL_Renderer* renderer; // �K�[?�@��
extern SDL_Window* window; // �K�[?�@��

void addPlayer() {
    std::string id;
    int score = 0;
    std::cout << "Enter Player ID: ";
    std::cin >> id;
    playerID = id; // ?�m playerID
    playerScores[id] = score;
    std::ofstream outfile("players.txt", std::ios::app);
    outfile << id << " " << score << std::endl;
    outfile.close();
}

void deletePlayer() {
    std::string id;
    std::cout << "Delete Player ID: ";
    std::cin >> id;
    playerScores.erase(id);
    std::ofstream outfile("players.txt");
    for (auto& it : playerScores) {
        outfile << it.first << " " << it.second << std::endl;
    }
    outfile.close();
}

void searchPlayer() {
    std::string id;
    std::cout << "Search Player ID: ";
    std::cin >> id;
    auto it = playerScores.find(id);
    if (it != playerScores.end()) {
        std::cout << "ID: " << it->first << ", Score: " << it->second << std::endl;
    } else {
        std::cout << "Player ID not found.\n";
    }
}

void printScoreRanking() {
    std::vector<std::pair<std::string, int>> ranking(playerScores.begin(), playerScores.end());
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });
    std::cout << "Score Ranking:\n";
    for (const auto& it : ranking) {
        std::cout << "ID: " << it.first << ", Score: " << it.second << std::endl;
    }
}

void loadPlayers() {
    std::ifstream infile("players.txt");
    std::string id;
    int score;
    while (infile >> id >> score) {
        playerScores[id] = score;
    }
    infile.close();
}

void savePlayers() {
    std::ofstream outfile("players.txt");
    for (const auto& it : playerScores) {
        outfile << it.first << " " << it.second << std::endl;
    }
    outfile.close();
}

void createConsole() {
    AllocConsole();
    FILE* file;
    freopen("CONOUT$", "w", stdout); // �ϥ� freopen ���N freopen_s
    freopen("CONIN$", "r", stdin);

    std::cout << "Player Management System\n";
    std::cout << "1. Enter Player ID\n";
    std::cout << "2. Delete Player ID\n";
    std::cout << "3. Search Player ID\n";
    std::cout << "4. Print Score Ranking\n";
    std::cout << "5. Start the Game\n";
    std::cout << "6. Exit\n";
    std::cout << "Please choose an operation: ";
}

void handleConsoleInput() {
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            addPlayer();
            break;
        case 2:
            deletePlayer();
            break;
        case 3:
            searchPlayer();
            break;
        case 4:
            printScoreRanking();
            break;
        case 5:
            if (playerID.empty()) {
                std::cout << "Please enter a Player ID before starting the game.\n";
            } else {
                gameStarted = true;
                startGame(); // ?�� startGame ��?
                std::cout << "Game started!\n";
            }
            break;
        case 6:
            running = false;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
    std::cout << "Please choose an operation: ";
}

void startGame() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL ��l�ƥ�?! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    window = SDL_CreateWindow("212410012", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "���f?�إ�?! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "��V��?�إ�?! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
}
