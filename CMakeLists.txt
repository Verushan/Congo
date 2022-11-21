cmake_minimum_required(VERSION 3.23)
project(Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Code
        include/Board.h
        include/Move.h
        include/MoveGenerator.h
        include/Piece.h
        include/LionMoveGenerator.h
        include/ZebraMoveGenerator.h
        include/GiraffeMoveGenerator.h
        include/PawnMoveGenerator.h
        include/SuperPawnMoveGenerator.h
        include/Evaluator.h
        include/MaterialEvaluator.h
        include/AdvancedEvaluator.h
        include/Agent.h
        include/MyTimer.h
        src/Evaluator.cpp
        src/MaterialEvaluator.cpp
        src/AdvancedEvaluator.cpp
        src/ZebraMoveGenerator.cpp
        src/Board.cpp
        src/Agent.cpp
        src/main.cpp
        src/LionMoveGenerator.cpp
        src/GiraffeMoveGenerator.cpp
        src/MoveGenerator.cpp
        src/PawnMoveGenerator.cpp
        src/SuperPawnMoveGenerator.cpp
        src/MyTimer.cpp
        )
