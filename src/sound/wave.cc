#include "sound/wave.h"

#include "exception.h"

#include <al.h>

namespace sound {
File *Wave::LoadFile(const std::string &filename) {
  // Local Declarations
  FILE *file = nullptr;
  try {
    file = fopen(filename.c_str(), "rb");
    if (file == nullptr) {
      throw engine::Exception("Failed to open file: " + filename);
    }
    file = fopen(filename.c_str(), "rb");
    if (file == nullptr) {
      throw engine::Exception(filename);
    }
    auto *audioFile = new File(filename);

    /*Load header*/
    loadHeader(file, *audioFile);
    /*Load whole data*/
    auto *buffer = new ALubyte[static_cast<size_t>(audioFile->DataSize)];
    loadData(file, buffer, audioFile->DataOffset, audioFile->DataSize);
    audioFile->Data = buffer;
    fclose(file);
    return audioFile;
  } catch (engine::Exception &ex) {
    // clean up memory if wave loading fails
    if (file != nullptr) {
      fclose(file);
    }
    throw ex;
  }
  return nullptr;
}

void Wave::loadHeader(FILE *wavFile, File &audioFile) {
  // Local Declarations
  WAVE_Format wave_format{};
  RIFF_Header riff_header{};
  WAVE_Data wave_data{};
  fpos_t dataOffset;

  ALenum format = AL_FORMAT_MONO8;
  ALsizei size = 0;
  ALsizei frequency = 0;

  if (wavFile == nullptr) {
    throw engine::Exception("Invalid FILE handle");
  }
  rewind(wavFile);
  // Read in the first chunk into the struct
  fread(&riff_header, sizeof(RIFF_Header), 1, wavFile);

  // check for RIFF and WAVE tag in memory
  if (riff_header.chunkID[0] != 'R' || riff_header.chunkID[1] != 'I' ||
      riff_header.chunkID[2] != 'F' || riff_header.chunkID[3] != 'F' ||
      (riff_header.format[0] != 'W' || riff_header.format[1] != 'A' ||
       riff_header.format[2] != 'V' || riff_header.format[3] != 'E')) {
    throw engine::Exception("Invalid RIFF or WAVE Header");
  }

  // Read in the 2nd chunk for the wave info
  fread(&wave_format, sizeof(WAVE_Format), 1, wavFile);
  // check for fmt tag in memory
  if (wave_format.subChunkID[0] != 'f' || wave_format.subChunkID[1] != 'm' ||
      wave_format.subChunkID[2] != 't' || wave_format.subChunkID[3] != ' ') {
    throw engine::Exception("Invalid Wave Format");
  }

  // check for extra parameters;
  if (wave_format.subChunkSize > 16) {
    fseek(wavFile, sizeof(short), SEEK_CUR);
  }

  // Read in the last byte of data before the sound file
  fread(&wave_data, sizeof(WAVE_Data), 1, wavFile);
  // check for data tag in memory
  if (wave_data.subChunkID[0] != 'd' || wave_data.subChunkID[1] != 'a' ||
      wave_data.subChunkID[2] != 't' || wave_data.subChunkID[3] != 'a') {
    throw engine::Exception("Invalid data header");
  }

  // Get position before data
  fgetpos(wavFile, &dataOffset);

  // Now we set the variables that we passed in with the
  // data from the structs
  size = wave_data.subChunk2Size;
  frequency = wave_format.dwSamplesPerSec;
  // The format is worked out by looking at the number of
  // channels and the bits per sample.
  if (wave_format.wChannels == 1) {
    if (wave_format.wBitsPerSample == 8) {
      format = AL_FORMAT_MONO8;
    } else if (wave_format.wBitsPerSample == 16) {
      format = AL_FORMAT_MONO16;
    }
  } else if (wave_format.wChannels == 2) {
    if (wave_format.wBitsPerSample == 8) {
      format = AL_FORMAT_STEREO8;
    } else if (wave_format.wBitsPerSample == 16) {
      format = AL_FORMAT_STEREO16;
    }
  }
  audioFile.Format = format;
  audioFile.Frequency = frequency;
  audioFile.DataSize = size;
  audioFile.DataOffset = dataOffset;
}

long Wave::loadData(FILE *file, ALubyte *buffer, const fpos_t offset,
                    const ALsizei size) {
  fsetpos(file, &offset);

  // Read in the sound data into the soundData variable
  if (fread(buffer, static_cast<size_t>(size), 1, file) == 0U) {
    if (feof(file) != 0) {
      return EOF;
    }
    throw engine::Exception("error loading WAVE data into struct!");
  }
  if (feof(file) != 0) {
    return EOF;
  }
  return ftell(file);
}
} // namespace sound
