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
    RARHeader* prar_header = reinterpret_cast<RARHeader*>(&rar_data[offset]);
    offset += prar_header->header_size;
    while(true) {
        RARHeader *prar_header_2 = reinterpret_cast<RARHeader*>(&rar_data[offset]);
        FILEHeader *pfile_header = reinterpret_cast<FILEHeader*>(&rar_data[offset+7]);
        cout << hex << "Pack size: " << int(pfile_header->pack_size) << endl;
        cout << hex << "Name size: " << int(pfile_header->name_size) << endl;
        uint32_t name_offset = offset + sizeof(struct FILEHeader) + sizeof(struct RARHeader);
        uint32_t name_size = pfile_header->name_size;
        char* ConData = reinterpret_cast<char*>(&rar_data[name_offset]); //  Вычисляет адрес со смещением и приводит указатель на него к расширенной кодировке
        string ConUtf8 = string(ConData, name_size); // Считываем имя напрямую
        vector<wchar_t> ConUtf16(name_size); // Создаем вектор размером, как контент, и заполняем его 0
        MultiByteToWideChar(CP_UTF8, 0, ConUtf8.c_str(),name_size,ConUtf16.data(),ConUtf16.size()); // Переводим строку с кодировкой UTF-8, в кодировку UTF-16
        wstring Content = wstring(ConUtf16.begin(), ConUtf16.end());  // Преобразуем вектор в строку
        wcout << "Name of the file: " << Content << endl; // Вывод
        offset += int(prar_header_2->header_size);
        offset += int(pfile_header->pack_size);
        cout << endl;
        cout << "offset: " << hex << offset << endl;
        cout <<"file offset: " << hex << name_offset << endl;
    }
}
#pragma pack(pop)
int main()
{
    ifstream rar_file("Example.rar",ios::binary);

    if(rar_file.is_open()) {
        rar_file.seekg(0, ios::end);

    int file_size = rar_file.tellg();
    rar_file.seekg(0, ios::beg);
    vector<char> rar_data(file_size,0);
    rar_file.read(rar_data.data(), file_size);
        ReadingFunc(rar_data);
    }
    else {
        cout << "File isn't open" << endl;
    }
}
