#ifndef SOIL_SOUND_FILE_H
#define SOIL_SOUND_FILE_H
#include <string>

namespace soil::sound {
    struct File final {
        explicit File(std::string name, int format, int dataSize, int frequency, fpos_t dataOffset,
                      unsigned char *Data);

        explicit File(std::string name);

        ~File();

        std::string Name;
        int Format;
        int Frequency;
        int DataSize;
        fpos_t DataOffset;
        unsigned char *Data;
    };
} // namespace soil::sound
#endif // SOIL_SOUND_FILE_H
