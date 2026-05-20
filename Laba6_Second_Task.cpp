#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <random>

using namespace std;

// ======================== S-BOX (таблица замен) ========================
const unsigned char Sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Обратный S-BOX (для расшифрования)
const unsigned char InvSbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Константы раундов
const unsigned char Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// ======================== Вспомогательные функции ========================
void printHex(const unsigned char* data, size_t len, const string& label) {
    cout << label << ": ";
    for (size_t i = 0; i < len; i++) {
        cout << hex << setw(2) << setfill('0') << (int)data[i];
    }
    cout << dec << endl;
}

void printState(const unsigned char state[4][4], const string& label) {
    cout << label << ":\n";
    for (int i = 0; i < 4; i++) {
        cout << "  ";
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << (int)state[i][j] << " ";
        }
        cout << dec << endl;
    }
}

void printRoundKeys(const unsigned char roundKeys[11][16], int rounds) {
    cout << "\n--- РАУНДОВЫЕ КЛЮЧИ (Key Expansion) ---\n";
    for (int r = 0; r <= rounds; r++) {
        cout << "RoundKey[" << r << "]: ";
        for (int i = 0; i < 16; i++) {
            cout << hex << setw(2) << setfill('0') << (int)roundKeys[r][i];
        }
        cout << dec << endl;
    }
}

// ======================== AES Core Operations ========================
void subBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = Sbox[state[i][j]];
        }
    }
}

void invSubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = InvSbox[state[i][j]];
        }
    }
}

void shiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    // Строка 1: сдвиг влево на 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Строка 2: сдвиг влево на 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Строка 3: сдвиг влево на 3
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void invShiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    // Строка 1: сдвиг вправо на 1
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // Строка 2: сдвиг вправо на 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Строка 3: сдвиг вправо на 3
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

// Умножение в поле GF(2^8)
unsigned char galoisMult(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi_bit = a & 0x80;
        a <<= 1;
        if (hi_bit) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void mixColumns(unsigned char state[4][4]) {
    unsigned char result[4][4];
    for (int c = 0; c < 4; c++) {
        result[0][c] = galoisMult(0x02, state[0][c]) ^ galoisMult(0x03, state[1][c]) ^ state[2][c] ^ state[3][c];
        result[1][c] = state[0][c] ^ galoisMult(0x02, state[1][c]) ^ galoisMult(0x03, state[2][c]) ^ state[3][c];
        result[2][c] = state[0][c] ^ state[1][c] ^ galoisMult(0x02, state[2][c]) ^ galoisMult(0x03, state[3][c]);
        result[3][c] = galoisMult(0x03, state[0][c]) ^ state[1][c] ^ state[2][c] ^ galoisMult(0x02, state[3][c]);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = result[i][j];
        }
    }
}

void invMixColumns(unsigned char state[4][4]) {
    unsigned char result[4][4];
    for (int c = 0; c < 4; c++) {
        result[0][c] = galoisMult(0x0e, state[0][c]) ^ galoisMult(0x0b, state[1][c]) ^
                       galoisMult(0x0d, state[2][c]) ^ galoisMult(0x09, state[3][c]);
        result[1][c] = galoisMult(0x09, state[0][c]) ^ galoisMult(0x0e, state[1][c]) ^
                       galoisMult(0x0b, state[2][c]) ^ galoisMult(0x0d, state[3][c]);
        result[2][c] = galoisMult(0x0d, state[0][c]) ^ galoisMult(0x09, state[1][c]) ^
                       galoisMult(0x0e, state[2][c]) ^ galoisMult(0x0b, state[3][c]);
        result[3][c] = galoisMult(0x0b, state[0][c]) ^ galoisMult(0x0d, state[1][c]) ^
                       galoisMult(0x09, state[2][c]) ^ galoisMult(0x0e, state[3][c]);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = result[i][j];
        }
    }
}

void addRoundKey(unsigned char state[4][4], const unsigned char roundKey[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= roundKey[i * 4 + j];
        }
    }
}

// ======================== Key Expansion ========================
void keyExpansion(const unsigned char key[16], unsigned char roundKeys[11][16]) {
    // Первые 16 байт = исходный ключ
    for (int i = 0; i < 16; i++) {
        roundKeys[0][i] = key[i];
    }

    unsigned char temp[4];

    for (int round = 1; round <= 10; round++) {
        // RotWord
        for (int i = 0; i < 4; i++) {
            temp[i] = roundKeys[round - 1][(i + 1) * 4];
        }

        // SubWord
        for (int i = 0; i < 4; i++) {
            temp[i] = Sbox[temp[i]];
        }

        // Rcon - разные ключи
        temp[0] ^= Rcon[round - 1];

        // Первый столбец
        for (int i = 0; i < 4; i++) {
            roundKeys[round][i] = roundKeys[round - 1][i] ^ temp[i];
        }

        // Остальные столбцы
        for (int col = 1; col < 4; col++) {
            for (int i = 0; i < 4; i++) {
                roundKeys[round][col * 4 + i] = roundKeys[round - 1][col * 4 + i] ^ roundKeys[round][(col - 1) * 4 + i];
            }
        }
    }
}

// ======================== AES Шифрование блока ========================
void aesEncryptBlock(const unsigned char input[16], unsigned char output[16], const unsigned char roundKeys[11][16], bool verbose = true) {
    unsigned char state[4][4];

    // Преобразование в матрицу
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = input[i * 4 + j];
        }
    }

    if (verbose) printState(state, "Начальный State");

    // Начальный AddRoundKey
    addRoundKey(state, roundKeys[0]);
    if (verbose) printState(state, "После AddRoundKey[0]");

    // 9 раундов
    for (int round = 1; round <= 9; round++) {
        if (verbose) cout << "\n--- Раунд " << round << " ---\n";

        subBytes(state);
        if (verbose) printState(state, "После SubBytes");

        shiftRows(state);
        if (verbose) printState(state, "После ShiftRows");

        mixColumns(state);
        if (verbose) printState(state, "После MixColumns");

        addRoundKey(state, roundKeys[round]);
        if (verbose) printState(state, "После AddRoundKey");
    }

    // Финальный раунд (без MixColumns)
    if (verbose) cout << "\n--- Финальный раунд (10) ---\n";
    subBytes(state);
    if (verbose) printState(state, "После SubBytes");

    shiftRows(state);
    if (verbose) printState(state, "После ShiftRows");

    addRoundKey(state, roundKeys[10]);
    if (verbose) printState(state, "После AddRoundKey (финальный)");

    // Преобразование обратно в массив
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i * 4 + j] = state[i][j];
        }
    }
}

// ======================== AES Расшифрование блока ========================
void aesDecryptBlock(const unsigned char input[16], unsigned char output[16],
                     const unsigned char roundKeys[11][16], bool verbose = true) {
    unsigned char state[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = input[i * 4 + j];
        }
    }

    if (verbose) printState(state, "Начальный State (зашифрованный блок)");

    addRoundKey(state, roundKeys[10]);
    if (verbose) printState(state, "После AddRoundKey[10]");

    for (int round = 9; round >= 1; round--) {
        if (verbose) cout << "\n--- Обратный раунд " << round << " ---\n";

        invShiftRows(state);
        if (verbose) printState(state, "После InvShiftRows");

        invSubBytes(state);
        if (verbose) printState(state, "После InvSubBytes");

        addRoundKey(state, roundKeys[round]);
        if (verbose) printState(state, "После AddRoundKey");

        invMixColumns(state);
        if (verbose) printState(state, "После InvMixColumns");
    }

    // Финальный раунд
    if (verbose) cout << "\n--- Финальный обратный раунд ---\n";
    invShiftRows(state);
    if (verbose) printState(state, "После InvShiftRows");

    invSubBytes(state);
    if (verbose) printState(state, "После InvSubBytes");

    addRoundKey(state, roundKeys[0]);
    if (verbose) printState(state, "После AddRoundKey[0] (финальный)");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i * 4 + j] = state[i][j];
        }
    }
}

// ======================== CFB Режим ========================
vector<unsigned char> cfbEncrypt(const vector<unsigned char>& plaintext,
                                  const unsigned char roundKeys[11][16],
                                  const unsigned char iv[16]) {
    size_t blocks = (plaintext.size() + 15) / 16;
    vector<unsigned char> ciphertext(plaintext.size());
    unsigned char feedback[16];
    unsigned char encryptedFeedback[16];

    memcpy(feedback, iv, 16);

    for (size_t block = 0; block < blocks; block++) {
        bool showDetails = (block == 0);

        if (showDetails) {
            cout << "\n=== Шифрование блока " << block << " (CFB) ===\n";
            cout << "Feedback (IV для блока 0): ";
            printHex(feedback, 16, "");
        }

        aesEncryptBlock(feedback, encryptedFeedback, roundKeys, showDetails);

        if (showDetails) {
            cout << "Encrypted feedback: ";
            printHex(encryptedFeedback, 16, "");
        }

        size_t offset = block * 16;
        size_t remaining = plaintext.size() - offset;
        size_t blockSize = remaining < 16 ? remaining : 16;

        for (size_t i = 0; i < blockSize; i++) {
            ciphertext[offset + i] = plaintext[offset + i] ^ encryptedFeedback[i];
        }

        if (showDetails) {
            cout << "Ciphertext блока: ";
            printHex(&ciphertext[offset], blockSize, "");
        }

        if (blockSize == 16) {
            memcpy(feedback, &ciphertext[offset], 16);
        } else {
            memcpy(feedback, encryptedFeedback, 16);
            for (size_t i = 0; i < blockSize; i++) {
                feedback[i] = ciphertext[offset + i];
            }
        }
    }

    return ciphertext;
}

vector<unsigned char> cfbDecrypt(const vector<unsigned char>& ciphertext, const unsigned char roundKeys[11][16], const unsigned char iv[16]) {
    size_t blocks = (ciphertext.size() + 15) / 16;
    vector<unsigned char> plaintext(ciphertext.size());
    unsigned char feedback[16];
    unsigned char encryptedFeedback[16];

    memcpy(feedback, iv, 16);

    for (size_t block = 0; block < blocks; block++) {
        aesEncryptBlock(feedback, encryptedFeedback, roundKeys, false);

        size_t offset = block * 16;
        size_t remaining = ciphertext.size() - offset;
        size_t blockSize = remaining < 16 ? remaining : 16;

        for (size_t i = 0; i < blockSize; i++) {
            plaintext[offset + i] = ciphertext[offset + i] ^ encryptedFeedback[i];
        }

        // Обновляем feedback
        if (blockSize == 16) {
            memcpy(feedback, &ciphertext[offset], 16);
        } else {
            memcpy(feedback, encryptedFeedback, 16);
            for (size_t i = 0; i < blockSize; i++) {
                feedback[i] = ciphertext[offset + i];
            }
        }
    }

    return plaintext;
}

// ======================== PKCS7 Padding ========================
vector<unsigned char> addPadding(const vector<unsigned char>& data) {
    size_t paddingLen = 16 - (data.size() % 16);
    vector<unsigned char> padded = data;
    padded.insert(padded.end(), paddingLen, paddingLen);
    return padded;
}

vector<unsigned char> removePadding(const vector<unsigned char>& data) {
    if (data.empty()) return data;
    unsigned char paddingLen = data.back();
    if (paddingLen > 16 || paddingLen == 0) return data;
    return vector<unsigned char>(data.begin(), data.end() - paddingLen);
}

// ======================== Генерация случайных байт ========================
void generateRandomBytes(unsigned char* buffer, size_t size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; i++) {
        buffer[i] = dis(gen);
    }
}

// ======================== Чтение/запись файлов ========================
vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return {};
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return buffer;
    }
    return {};
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Ошибка: не удалось создать файл " << filename << endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    cout << "AES-128 CFB \n\n";

    // 1. Генерация ключа и вектора инициализации IV
    unsigned char key[16];
    unsigned char iv[16];

    generateRandomBytes(key, 16);
    generateRandomBytes(iv, 16);

    printHex(key, 16, "Сгенерированный ключ (128 bit)");
    printHex(iv, 16, "Вектор инициализации (IV)");

    // 2. Запись ключа в файл
    ofstream keyFile("aes_key.bin", ios::binary);
    keyFile.write(reinterpret_cast<char*>(key), 16);
    keyFile.close();
    cout << "\nКлюч сохранён в файл: aes_key.bin\n";

    // 3. Key Expansion
    unsigned char roundKeys[11][16];
    keyExpansion(key, roundKeys);
    printRoundKeys(roundKeys, 10);

    // 4. Чтение исходного файла
    cout << "\nЧтение файла input.txt...\n";
    vector<unsigned char> plainBytes = readFile("input.txt");
    if (plainBytes.empty()) {
        cerr << "Создайте файл input.txt с текстом для шифрования\n";
        return 1;
    }

    cout << "Исходный файл (" << plainBytes.size() << " байт)\n";

    // 5. Добавление padding
    vector<unsigned char> paddedPlain = addPadding(plainBytes);
    cout << "После PKCS7 padding (" << paddedPlain.size() << " байт)\n";

    // 6. Шифрование
    cout << "\n=== ШИФРОВАНИЕ ===\n";
    vector<unsigned char> ciphertext = cfbEncrypt(paddedPlain, roundKeys, iv);
    writeFile("encrypted.bin", ciphertext);
    cout << "Зашифрованные данные сохранены в файл: encrypted.bin (" << ciphertext.size() << " байт)\n";

    // 7. Расшифрование
    cout << "\n=== РАСШИФРОВАНИЕ ===\n";
    vector<unsigned char> decryptedPadded = cfbDecrypt(ciphertext, roundKeys, iv);
    vector<unsigned char> decrypted = removePadding(decryptedPadded);
    writeFile("decrypted.txt", decrypted);
    cout << "Расшифрованные данные сохранены в файл: decrypted.txt (" << decrypted.size() << " байт)\n";

    // 8. Проверка (сравнение исходного и расшифрованного файлов)
    cout << "\n=== ПРОВЕРКА ===\n";

    // Читаем decrypted.txt для сравнения
    vector<unsigned char> checkBytes = readFile("decrypted.txt");

    if (plainBytes.size() == checkBytes.size() &&
        memcmp(plainBytes.data(), checkBytes.data(), plainBytes.size()) == 0) {
        cout << "  УСПЕХ! Файл успешно зашифрован и расшифрован.\n";
        cout << "  Исходный: input.txt\n";
        cout << "  Зашифрованный: encrypted.bin\n";
        cout << "  Расшифрованный: decrypted.txt\n";
    } else {
        cout << "  ОШИБКА: Расшифрованный файл не совпадает с исходным.\n";
    }

    return 0;
}