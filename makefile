SHELL:=/bin/bash

file = main.c
executable = main
testDirectory= './TestCrosswords'
tests = $(shell ls $(testDirectory))
wordsFile = 'words.txt'
crosswordFile = 'crossword.txt'
defaultTest = 'test9'
build:
	gcc $(file) -o $(executable)

run:
	./$(executable) $(testDirectory)/$(defaultTest)/$(wordsFile) $(testDirectory)/$(defaultTest)/$(crosswordFile)

debug:
	./$(executable) $(testDirectory)/$(defaultTest)/$(wordsFile) $(testDirectory)/$(defaultTest)/$(crosswordFile) DEBUG

test:
	# for oneTest in $(tests) ; do \
	#  	./$(executable) $(testDirectory)/$${oneTest}/$(wordsFile) $(testDirectory)/$${oneTest}/$(crosswordFile)
	# done
	./$(executable) $(testDirectory)/test1/$(wordsFile) $(testDirectory)/test1/$(crosswordFile)
	./$(executable) $(testDirectory)/test2/$(wordsFile) $(testDirectory)/test2/$(crosswordFile)
	./$(executable) $(testDirectory)/test3/$(wordsFile) $(testDirectory)/test3/$(crosswordFile)
	./$(executable) $(testDirectory)/test4/$(wordsFile) $(testDirectory)/test4/$(crosswordFile)
	./$(executable) $(testDirectory)/test5/$(wordsFile) $(testDirectory)/test5/$(crosswordFile)
	./$(executable) $(testDirectory)/test6/$(wordsFile) $(testDirectory)/test6/$(crosswordFile)
	./$(executable) $(testDirectory)/test7/$(wordsFile) $(testDirectory)/test7/$(crosswordFile)
	./$(executable) $(testDirectory)/test8/$(wordsFile) $(testDirectory)/test8/$(crosswordFile)
	./$(executable) $(testDirectory)/test9/$(wordsFile) $(testDirectory)/test9/$(crosswordFile)
