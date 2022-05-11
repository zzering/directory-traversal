cc = gcc
TARGET_DFS =a.out
TARGET_BFS =b.out
DFS=recusive_DFS.c
BFS=non-recusive_BFS.c
out=*.out

$(TARGET_DFS): $(DFS) $(BFS)
	$(cc) $(DFS) -o $(TARGET_DFS)
	@echo "\nDFS Build completed successfully \n"
	$(cc) $(BFS) -o $(TARGET_BFS)
	@echo "\nBFS Build completed successfully \n"

clean:
	rm $(out)
	@echo "\nClean completed successfully"