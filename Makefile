CC = gcc
LDFLAGS = -lgmp -lpthread # GMPとPthreadのリンクオプション

SRC = src/main.c       # ソースファイル
OBJ = $(SRC:.c=.o)     # オブジェクトファイル
TARGET = factorial     # 出力ファイル名

# ビルドルール
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# クリーンルール
clean:
	rm -f $(OBJ) $(TARGET)
