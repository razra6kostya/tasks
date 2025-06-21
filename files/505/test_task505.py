#------------------#
#СОЗДАНО НЕЙРОСЕТЬЮ!
#------------------#
import os
import subprocess
import tempfile
import shutil

# --- Конфигурация ---
PROGRAM_PATH = "./task505"
HEXDUMP_PATH = "hexdump" 
BLOCK_SIZE = 4096

# --- Вспомогательные функции ---

def run_program(filename, start_pos, length, byte_value):
    """Запускает вашу программу с заданными параметрами."""
    byte_str = str(byte_value) # Передаем число, ваша программа должна его распарсить
    command = [
        PROGRAM_PATH,
        filename,
        str(start_pos),
        str(length),
        byte_str
    ]
    print(f"Running command: {' '.join(command)}")
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Program exited with error code {result.returncode}")
        print(f"Stdout:\n{result.stdout}")
        print(f"Stderr:\n{result.stderr}")
    return result

def get_file_content(filepath):
    """Возвращает содержимое файла в виде байтов."""
    try:
        with open(filepath, 'rb') as f:
            return f.read()
    except FileNotFoundError:
        return b'' # Если файл не существует, вернуть пустые байты

def create_initial_file(filepath, size, fill_byte=0x00):
    """Создает файл заданного размера, заполненный указанным байтом."""
    with open(filepath, 'wb') as f:
        f.write(bytes([fill_byte]) * size)

def generate_expected_content(initial_content, start_pos, length, fill_byte):
    """Генерирует ожидаемое содержимое файла после модификации."""
    initial_len = len(initial_content)
    
    # Всегда начинаем с bytearray для возможности модификации
    # Это ключевое изменение: превращаем initial_content в изменяемый bytearray
    expanded_content = bytearray(initial_content)

    # Если начальная позиция больше текущего размера файла, дополняем нулями
    if start_pos > initial_len:
        # Расширяем до start_pos, заполняя нулями
        expanded_content.extend(bytes([0x00]) * (start_pos - initial_len))

    end_pos = start_pos + length
    
    # Расширяем файл, если конечная позиция выходит за его текущие пределы
    if end_pos > len(expanded_content):
        expanded_content.extend(bytes([0x00]) * (end_pos - len(expanded_content)))

    # Заполняем нужный отрезок
    # Этот цикл также требует, чтобы expanded_content был bytearray
    for i in range(length):
        expanded_content[start_pos + i] = fill_byte

    return bytes(expanded_content) # В конце возвращаем обратно в bytes

def compare_files(actual_filepath, expected_bytes, test_name=""):
    """Сравнивает содержимое файла с ожидаемыми байтами."""
    actual_content = get_file_content(actual_filepath)
    
    if actual_content == expected_bytes:
        print(f"[SUCCESS] {test_name}: File content matches expected.")
        return True
    else:
        print(f"[FAIL] {test_name}: File content MISMATCH!")
        print("\n--- Actual File (hexdump) ---")
        subprocess.run([HEXDUMP_PATH, '-C', actual_filepath])
        
        print("\n--- Expected Content (hexdump) ---")
        # Создаем временный файл для hexdump ожидаемого контента
        with tempfile.NamedTemporaryFile(mode='wb', delete=False) as tmp_expected_file:
            tmp_expected_file.write(expected_bytes)
            tmp_expected_filepath = tmp_expected_file.name
        subprocess.run([HEXDUMP_PATH, '-C', tmp_expected_filepath])
        os.remove(tmp_expected_filepath) # Удаляем временный файл

        print(f"\nActual length: {len(actual_content)}, Expected length: {len(expected_bytes)}")
        
        # Попытка найти первое расхождение для отладки
        min_len = min(len(actual_content), len(expected_bytes))
        for i in range(min_len):
            if actual_content[i] != expected_bytes[i]:
                print(f"First mismatch at byte {i}: Actual={hex(actual_content[i])}, Expected={hex(expected_bytes[i])}")
                break
        else:
            if len(actual_content) != len(expected_bytes):
                print(f"Contents match up to {min_len} bytes, but lengths differ.")

        return False

# --- Тестовые сценарии ---
def run_test(name, initial_file_size, initial_fill_byte, start_pos, length, fill_byte):
    """Запускает один тестовый сценарий."""
    print(f"\n--- Running Test: {name} ---")

    # Создаем временную директорию для тестов
    with tempfile.TemporaryDirectory() as tmpdir:
        test_filepath = os.path.join(tmpdir, "test_file.bin")

        # 1. Создаем начальный файл
        create_initial_file(test_filepath, initial_file_size, initial_fill_byte)
        initial_content = get_file_content(test_filepath)

        # 2. Генерируем ожидаемое содержимое
        expected_content = generate_expected_content(initial_content, start_pos, length, fill_byte)

        # 3. Запускаем тестируемую программу
        program_result = run_program(test_filepath, start_pos, length, fill_byte)

        # 4. Проверяем результат
        success = compare_files(test_filepath, expected_content, name)

        # Важно: если программа вернула ошибку, но файл совпадает (что странно), или наоборот
        if program_result.returncode != 0 and success:
            print(f"[WARNING] Program returned non-zero exit code ({program_result.returncode}), but file content is correct.")
        elif program_result.returncode == 0 and not success:
            print(f"[ERROR] Program returned zero exit code (success), but file content is INCORRECT.")

        return success

def main():
    # Проверка наличия программы
    if not os.path.exists(PROGRAM_PATH):
        print(f"Error: Program '{PROGRAM_PATH}' not found. Please compile your solution and ensure PROGRAM_PATH is correct.")
        exit(1)
    if not os.path.isfile(PROGRAM_PATH) or not os.access(PROGRAM_PATH, os.X_OK):
        print(f"Error: '{PROGRAM_PATH}' is not an executable file.")
        exit(1)

    # Проверка наличия hexdump
    if shutil.which(HEXDUMP_PATH) is None:
        print(f"Error: '{HEXDUMP_PATH}' not found. Please install it or provide the full path.")
        exit(1)

    total_tests = 0
    failed_tests = 0

    # Тест 1: Базовый случай - запись в середину существующего файла, длина < 4096
    total_tests += 1
    if not run_test("Basic_Small_Overwrite", 100, 0xAA, 10, 20, 0xFF):
        failed_tests += 1

    # Тест 2: Запись с начала файла, частичная перезапись
    total_tests += 1
    if not run_test("Start_Partial_Overwrite", 50, 0xAA, 0, 15, 0xCC):
        failed_tests += 1

    # Тест 3: Полная перезапись файла
    total_tests += 1
    if not run_test("Full_Overwrite", 30, 0xAA, 0, 30, 0xBB):
        failed_tests += 1

    # Тест 4: Расширение файла (запись за пределы текущего конца)
    total_tests += 1
    if not run_test("Extend_File", 10, 0xAA, 10, 20, 0xEE):
        failed_tests += 1

    # Тест 5: Расширение файла с "дыркой" (начальная позиция сильно больше текущего размера)
    total_tests += 1
    if not run_test("Extend_File_With_Hole", 10, 0xAA, 50, 20, 0xDD):
        failed_tests += 1

    # Тест 6: Запись ровно одного блока (4096 байт)
    total_tests += 1
    if not run_test("Exact_Block_Size", BLOCK_SIZE * 2, 0x00, BLOCK_SIZE // 2, BLOCK_SIZE, 0x11):
        failed_tests += 1

    # Тест 7: Запись нескольких полных блоков
    total_tests += 1
    if not run_test("Multiple_Full_Blocks", BLOCK_SIZE * 5, 0x00, 0, BLOCK_SIZE * 3, 0x22):
        failed_tests += 1

    # Тест 8: Запись нескольких блоков + остаток (например, 2.5 блока)
    total_tests += 1
    if not run_test("Multiple_Blocks_With_Remainder", BLOCK_SIZE * 4, 0x00, 0, BLOCK_SIZE * 2 + 100, 0x33):
        failed_tests += 1

    # Тест 9: Запись, которая начинается в середине блока и заканчивается в следующем блоке с остатком
    total_tests += 1
    if not run_test("Mid_Block_Start_End_Remainder", BLOCK_SIZE * 3, 0x00, BLOCK_SIZE // 2, BLOCK_SIZE + 50, 0x44):
        failed_tests += 1

    # Тест 10: Запись нулевой длины (программа не должна ничего менять)
    total_tests += 1
    if not run_test("Zero_Length_Write", 100, 0xAA, 50, 0, 0xFE):
        failed_tests += 1
# Тест 11: Запись очень большого размера (например, 10 блоков)
    total_tests += 1
    if not run_test("Very_Large_Write", BLOCK_SIZE * 2, 0x00, 0, BLOCK_SIZE * 10, 0x55):
        failed_tests += 1

    print(f"\n--- Test Summary ---")
    print(f"Total tests run: {total_tests}")
    print(f"Tests passed: {total_tests - failed_tests}")
    print(f"Tests failed: {failed_tests}")

    if failed_tests == 0:
        print("All tests passed successfully!")
        exit(0)
    else:
        print("Some tests failed. Please check the output above.")
        exit(1)

if __name__ == "__main__":
    main()
