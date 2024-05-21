cmake_minimum_required(VERSION 3.10)
project(CSV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    CSV_NewGeneration/CSV.cpp
    CSV_NewGeneration/CSV_dyn.cpp
	CSV_NewGeneration/Evaluation.cpp
	CSV_NewGeneration/Evaluation_freq.cpp
	CSV_NewGeneration/Evaluation_xyz.cpp
	CSV_NewGeneration/Extraction.cpp
	CSV_NewGeneration/Global.cpp
	CSV_NewGeneration/Pattern.cpp
	CSV_NewGeneration/Set.cpp
	CSV_NewGeneration/Set_dyn.cpp
	CSV_NewGeneration/Set_xyz.cpp
	CSV_NewGeneration/UnitTest.cpp
	CSV_NewGeneration/Verification.cpp
)

set(HEADERS
	CSV_NewGeneration/Global.hpp
)

add_executable(CSV ${SOURCES} ${HEADERS})

