
#ifndef SOIL_SOUND_WAVE_H
#define SOIL_SOUND_WAVE_H
#include <cstdint>

#include "file.h"

namespace soil::sound {
    /**
     * Struct that holds the RIFF data of the Wave file.
     * The RIFF data is the metadata information that holds,
     * the ID, size and format of the wave file
     */
    struct RIFF_Header {
        char chunkID[4];
        uint32_t chunkSize; // size not including chunkSize or chunkID
        char format[4];
    };

    struct WAVE_Format {
        char subChunkID[4];
        uint32_t subChunkSize;
        uint16_t wFormatTag;
        uint16_t wChannels;
        uint32_t dwSamplesPerSec;
        uint32_t dwAvgBytesPerSec;
        uint16_t wBlockAlign;
        uint16_t wBitsPerSample;
    };

    /**
     * Struct to hold the data of the wave file
     */
    struct WAVE_Data {
        char subChunkID[4]; // should contain the word data
        uint32_t subChunk2Size; // Stores the size of the data block
    };

    class Wave {
    public:
        static File *LoadFile(const std::string &filename);

    private:
        /**
         * Loads data from Wave-File. The data of size is red from offset to
         * offset+size and stored in buffer.
         * @param file to read from
         * @param buffer to store in. Must be of size.
         * @param offset of file to be red from
         * @param size of data to be read.
         * @return the positive newOffset(should be offset + size) or EOF on file end.
         * A lesser value than EOF indicates error.
         */
        static long loadData(FILE *file, unsigned char *buffer, fpos_t offset, int size);

        /**
         * Reads in the header of wavFile and store the information in audioFile.
         * @param wavFile
         * @param audioFile
         */
        static void loadHeader(FILE *wavFile, File &audioFile);
    };
} // namespace soil::sound
#endif // SOIL_SOUND_WAVE_H
