#!/bin/bash
#------------------#
#СОЗДАНО НЕЙРОСЕТЬЮ!
#------------------#
# --- Конфигурация ---
BINARY_NAME="task254"
OUTPUT_DIR="test_output"
TEST_DATA_DIR="test_data"
EXPECTED_OUTPUT_DIR="expected_output"

# --- Цвета для вывода ---
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # Без цвета

# --- Функция очистки ---
cleanup() {
    echo "Очистка..."
    rm -rf "$OUTPUT_DIR" "$TEST_DATA_DIR"
    rm -f "$BINARY_NAME"
}

# Регистрация функции очистки при выходе
trap cleanup EXIT

# --- Компиляция кода C ---
echo "Компиляция $BINARY_NAME..."
gcc -o "$BINARY_NAME" task254.c -Wall -Wextra -g
if [ $? -ne 0 ]; then
    echo -e "${RED}Компиляция не удалась!${NC}"
    exit 1
fi
echo -e "${GREEN}Компиляция успешна!${NC}"

# --- Создание необходимых директорий ---
mkdir -p "$OUTPUT_DIR" "$TEST_DATA_DIR" "$EXPECTED_OUTPUT_DIR"

# --- Тестовый случай 1: Пустой файл ---
echo -n "Запуск тестового случая 1: Пустой файл... "
TEST_FILE="$TEST_DATA_DIR/empty.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_empty.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_empty.txt"

touch "$TEST_FILE"
echo "$TEST_FILE:empty_file" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 2: Одно положительное целое число ---
echo -n "Запуск тестового случая 2: Одно положительное целое число... "
TEST_FILE="$TEST_DATA_DIR/single_pos.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_single_pos.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_single_pos.txt"

# Запись целого числа 10 в файл
printf "\x0a\x00\x00\x00" > "$TEST_FILE" # Little-endian 10
echo "$TEST_FILE:1:10:10" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 3: Одно отрицательное целое число ---
echo -n "Запуск тестового случая 3: Одно отрицательное целое число... "
TEST_FILE="$TEST_DATA_DIR/single_neg.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_single_neg.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_single_neg.txt"

# Запись целого числа -5 в файл (дополнительный код, little-endian)
# -5 = FFFFFFFB (шестнадцатеричное)
printf "\xfb\xff\xff\xff" > "$TEST_FILE"
echo "$TEST_FILE:1:-5:-5" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 4: Несколько положительных целых чисел ---
echo -n "Запуск тестового случая 4: Несколько положительных целых чисел... "
TEST_FILE="$TEST_DATA_DIR/multi_pos.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_multi_pos.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_multi_pos.txt"

# Целые числа: 1, 5, 2, 8, 3
printf "\x01\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x08\x00\x00\x00\x03\x00\x00\x00" > "$TEST_FILE"
echo "$TEST_FILE:5:1:8" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 5: Несколько отрицательных целых чисел ---
echo -n "Запуск тестового случая 5: Несколько отрицательных целых чисел... "
TEST_FILE="$TEST_DATA_DIR/multi_neg.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_multi_neg.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_multi_neg.txt"

# Целые числа: -1, -5, -2, -8, -3
# -1 = FFFFFFFF, -5 = FFFFFFFB, -2 = FFFFFFFE, -8 = FFFFFF8, -3 = FFFFFFFD
printf "\xff\xff\xff\xff\xfb\xff\xff\xff\xfe\xff\xff\xff\xf8\xff\xff\xff\xfd\xff\xff\xff" > "$TEST_FILE"
echo "$TEST_FILE:5:-8:-1" > "$EXPECTED_OUTPUT"
./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 6: Смешанные положительные и отрицательные целые числа и ноль ---
echo -n "Запуск тестового случая 6: Смешанные целые числа (положительные, отрицательные, ноль)... "
TEST_FILE="$TEST_DATA_DIR/mixed.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_mixed.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_mixed.txt"

# Целые числа: 0, 10, -10, 5, -5
printf "\x00\x00\x00\x00\x0a\x00\x00\x00\xf6\xff\xff\xff\x05\x00\x00\x00\xfb\xff\xff\xff" > "$TEST_FILE"
echo "$TEST_FILE:5:-10:10" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 7: Размер файла не кратен sizeof(int) (частичное int в конце) ---
echo -n "Запуск тестового случая 7: Частичное целое число в конце... "
TEST_FILE="$TEST_DATA_DIR/partial_int.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_partial_int.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_partial_int.txt"

# Целые числа: 10, за которыми следуют 3 байта (неполное int)
printf "\x0a\x00\x00\x00\x01\x02\x03" > "$TEST_FILE"
echo "$TEST_FILE:1:10:10" > "$EXPECTED_OUTPUT" # Только первые 4 байта (10) должны быть разобраны

./"$BINARY_NAME" "$TEST_FILE" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 8: Несуществующий входной файл (должен быть пропущен) ---
echo -n "Запуск тестового случая 8: Несуществующий входной файл... "
NON_EXISTENT_FILE="$TEST_DATA_DIR/does_not_exist.bin"
TEST_FILE_2="$TEST_DATA_DIR/single_pos_2.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_non_existent.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_non_existent.txt"

printf "\x01\x00\x00\x00" > "$TEST_FILE_2" # Действительный файл, чтобы убедиться, что для него генерируется вывод

# Ожидаемый вывод должен содержать только анализ действительного файла
echo "$TEST_FILE_2:1:1:1" > "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$NON_EXISTENT_FILE" "$TEST_FILE_2" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

# --- Тестовый случай 9: Несколько входных файлов ---
echo -n "Запуск тестового случая 9: Несколько входных файлов... "
TEST_FILE_A="$TEST_DATA_DIR/file_A.bin"
TEST_FILE_B="$TEST_DATA_DIR/file_B.bin"
OUTPUT_FILE="$OUTPUT_DIR/output_multiple.txt"
EXPECTED_OUTPUT="$EXPECTED_OUTPUT_DIR/expected_multiple.txt"

# Файл A: 1, 2, 3 (мин 1, макс 3, кол-во 3)
printf "\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00" > "$TEST_FILE_A"
# Файл B: 100, 20 (мин 20, макс 100, кол-во 2)
printf "\x64\x00\x00\x00\x14\x00\x00\x00" > "$TEST_FILE_B"

echo "$TEST_FILE_A:3:1:3" > "$EXPECTED_OUTPUT"
echo "$TEST_FILE_B:2:20:100" >> "$EXPECTED_OUTPUT"

./"$BINARY_NAME" "$TEST_FILE_A" "$TEST_FILE_B" "$OUTPUT_FILE" > /dev/null 2>&1
if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT"; then
    echo -e "${GREEN}ПРОЙДЕНО${NC}"
else
    echo -e "${RED}ПРОВАЛЕНО${NC}"
    diff "$OUTPUT_FILE" "$EXPECTED_OUTPUT"
fi

echo ""
echo "Все тесты завершены."
