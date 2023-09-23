# Makefile

# ターゲットとソースファイルの定義
TARGET = ../Server
SRC = Server.c

# コンパイラとコンパイルオプションの設定
CC = gcc
CFLAGS = -pthread -lcjson

# デフォルトのターゲットとビルドルール
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)

# クリーンアップルール
clean:
	rm -f $(TARGET)
