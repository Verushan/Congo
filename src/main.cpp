#include "Agent.h"

#include <iostream>

using namespace std;

int main() {
    MyTimer timer;
    auto *myAgent = new Agent(Board::STARTING_FEN);

    int timeLimit;
    string line, instruction, fen, strTime, strMove;
    stringstream ss;
    getline(cin, line);

    while (line != "END") {
        ss.clear();
        ss << line;

        getline(ss, instruction, ' ');

        if (instruction == "position") {
            getline(ss, fen);
            myAgent->updateBoard(fen);
        } else if (instruction == "go") {
            getline(ss, strTime);
            timeLimit = stoi(strTime) * 1000;
            Move move = myAgent->getMove(timeLimit);
            cout << Board::moveToString(move) << "\n";
            myAgent->makeMove(move);
        } else if (instruction == "moves") {
            getline(ss, strMove);
            Move move(strMove);
            myAgent->makeMove(move);
        }

        getline(cin, line);
    }
}
