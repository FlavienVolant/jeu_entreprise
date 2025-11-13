# === Configuration ===
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/main.c src/entreprise.c src/fournisseurs.c
BIN = programme.exe

# === Compilation ===
all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# === Exécution ===
run: $(BIN)
	./$(BIN)

# === Nettoyage ===
clean:
	del /Q $(BIN) 2>nul || rm -f $(BIN)
