SHELL:=/bin/bash

file = main.c
executable = main
testDirectory= './TestCrosswords'
tests = $(shell ls $(testDirectory))
wordsFile = 'words.txt'
crosswordFile = 'crossword.txt'
defaultTest = 'test1'
build:
	gcc $(file) -o $(executable)

run:
	./$(executable) $(testDirectory)/$(defaultTest)/$(wordsFile) $(testDirectory)/$(defaultTest)/$(crosswordFile)

test:
	for oneTest in $(tests) ; do \
	 	./$(executable) $(testDirectory)/$${oneTest}/$(wordsFile) $(testDirectory)/$${oneTest}/$(crosswordFile)
	done