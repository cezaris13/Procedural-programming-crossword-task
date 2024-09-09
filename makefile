SHELL:=/bin/bash

file = main.c
executable = main
testDirectory= './TestCrosswords'
tests = $(shell ls $(testDirectory))
wordsFile = 'words.txt'
crosswordFile = 'crossword.txt'
defaultTest = 'test5'
build:
	gcc $(file) -o $(executable)

run:
	./$(executable) $(testDirectory)/$(defaultTest)/$(wordsFile) $(testDirectory)/$(defaultTest)/$(crosswordFile)

debug:
	./$(executable) $(testDirectory)/$(defaultTest)/$(wordsFile) $(testDirectory)/$(defaultTest)/$(crosswordFile) DEBUG

test:
	@for oneTest in $(tests) ; do \
		echo $$oneTest; \
	 	./$(executable) $(testDirectory)/$$oneTest/$(wordsFile) $(testDirectory)/$$oneTest/$(crosswordFile); \
	done