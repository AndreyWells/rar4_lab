#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <windows.h>
#pragma pack(push,1)
using namespace std;
struct RARHeader {
    uint16_t header_crc;
    char header_type;
    uint16_t header_flags;
    uint16_t header_size;
};
struct FILEHeader {
    uint32_t pack_size;
    uint32_t unp_size;
    uint8_t host_os;
    uint32_t file_crc;
    uint32_t file_time;
    uint8_t unp_ver;
    uint8_t method;
    uint16_t name_size;
    uint32_t file_attr;
};
void ReadingFunc(vector<char>rar_data) {
    int offset = 7; // Сигнатура

    RARHeader* prar_header = reinterpret_cast<RARHeader*>(&rar_data[offset]); // Инициализируем указатель на структуру с первым заголовком рар4

    offset += prar_header->header_size; // Делаем смещение на длину заголовка

    while(true) {

        RARHeader *prar_header_2 = reinterpret_cast<RARHeader*>(&rar_data[offset]);// Инициализируем указатель на структуру с остальными заголовком рар4

        if (prar_header_2->header_type == 0x74) { // Ветвление по типу заголовка, чтобы выводило только файлы

        FILEHeader *pfile_header = reinterpret_cast<FILEHeader*>(&rar_data[offset+7]); // Инициализируем указатель на структуру файла по текущему смещению + 7 байт заголовка rar4

        cout << hex << "Pack size: " << int(pfile_header->pack_size) << endl;

        cout << hex << "Name size: " << int(pfile_header->name_size) << endl;

        uint32_t name_offset = offset + sizeof(struct FILEHeader) + sizeof(struct RARHeader); // Создаем переменную для смещения имени на текущее смещение + 25 первых байт заголовка файла + 7 байт заголовка rar4

        uint32_t name_size = pfile_header->name_size; // Создаем переменную для размера имени

        char* ConData = reinterpret_cast<char*>(&rar_data[name_offset]); //  Вычисляет адрес со смещением и приводит указатель на него к расширенной кодировке

        string ConUtf8 = string(ConData, name_size); // Считываем имя напрямую

        vector<wchar_t> ConUtf16(name_size); // Создаем вектор размером, как контент, и заполняем его 0

        MultiByteToWideChar(CP_UTF8, 0, ConUtf8.c_str(),name_size,ConUtf16.data(),ConUtf16.size()); // Переводим строку с кодировкой UTF-8, в кодировку UTF-16

        wstring Content = wstring(ConUtf16.begin(), ConUtf16.end());  // Преобразуем вектор в строку

        wcout << "Name of the file: " << Content << endl; // Вывод

        offset += int(prar_header_2->header_size); // Переходим на след.заголовок, добавляя размер заголовка rar4

        offset += int(pfile_header->pack_size); // Переходим на след.заголовок, добавляя размер упакованных файлов из структуры файла

        cout << endl;


} else {
cout << "End of the table of contents" << endl;
break;
}
}
}
#pragma pack(pop)
int main()
{
    ifstream rar_file("Example.rar",ios::binary); // Читаем файла

    if(rar_file.is_open()) { // Проверка открытия

        rar_file.seekg(0, ios::end); // Перемещаем указатель в конец

    int file_size = rar_file.tellg(); // Создаем переменную для полученной текущей позиции указателя, чтобы вычислить размер

    rar_file.seekg(0, ios::beg); // Перемещаем указатель в начало

    vector<char> rar_data(file_size,0); // Создаем вектор, с размером, который мы определили, и заполняем его 0

    rar_file.read(rar_data.data(), file_size); // Считываем значения в вектор

        ReadingFunc(rar_data); // Вызов функции
    }
    else {
        cout << "File isn't open" << endl;
    }
}
