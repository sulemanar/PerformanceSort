cmake_minimum_required(VERSION 3.5)
project(PerformanceSort)

find_program(
	CLANG_TIDY_EXE
	NAMES "clang-tidy"
	DOC "Path to clang-tidy executable"
)
if(NOT CLANG_TIDY_EXE)
	message(STATUS "clang-tidy not found.")
else()
	message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
	set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

add_executable(sort_analysis sort_analysis.cpp)
set_target_properties(sort_analysis PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(sort_test sort_test.cpp)
set_target_properties(sort_test PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

enable_testing()
add_test(NAME "test_performance_sort" COMMAND ./sort_test performance_sort)
add_test(NAME "test_heapsort" COMMAND ./sort_test heapsort)
