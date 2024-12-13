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
    while(true) {

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
