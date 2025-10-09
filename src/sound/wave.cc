#include "sound/wave.h"

#include <al.h>

#include <fstream>
#include <stdexcept>

#include "base.h"

namespace soil::sound {

WaveFile::WaveFile(const std::string &name, const InfoType &info,
                   const long dataOffset)
    : File(name, info), dataOffset_(dataOffset) {}

WaveFile *WaveFile::Load(const std::string &file) {
  FILE *f = nullptr;
  InfoType info;
  long bytesRead;
  try {
    f = fopen(file.c_str(), "rb");
    if (f == nullptr) {
      throw std::runtime_error("Failed to open file: " + file);
    }
    f = fopen(file.c_str(), "rb");
    if (f == nullptr) {
      throw std::runtime_error(file);
    }
    loadHeader(f, info);
    bytesRead = ftell(f);
    fclose(f);
  } catch ([[maybe_unused]] std::runtime_error &ex) {
    // clean up memory if wave loading fails
    if (f != nullptr) {
      fclose(f);
    }
    throw;
  }

  return new WaveFile(file, info, bytesRead);
}
WaveFile::Cursor *WaveFile::NewCursor() const { return new Cursor(this); }

void WaveFile::loadHeader(FILE *wavFile, InfoType &info) {
  WAVE_Format wave_format{};
  RIFF_Header riff_header{};
  WAVE_Data wave_data{};

  ALsizei size = 0;
  ALsizei samplerate = 0;

  if (wavFile == nullptr) {
    throw std::runtime_error("Invalid FILE handle");
  }
  rewind(wavFile);
  // Read in the first chunk into the struct
  fread(&riff_header, sizeof(RIFF_Header), 1, wavFile);

  // check for RIFF and WAVE tag in memory
  if (riff_header.chunkID[0] != 'R' || riff_header.chunkID[1] != 'I' ||
      riff_header.chunkID[2] != 'F' || riff_header.chunkID[3] != 'F' ||
      riff_header.format[0] != 'W' || riff_header.format[1] != 'A' ||
      riff_header.format[2] != 'V' || riff_header.format[3] != 'E') {
    throw std::runtime_error("Invalid RIFF or WAVE Header");
  }

  // Read in the 2nd chunk for the wave info
  fread(&wave_format, sizeof(WAVE_Format), 1, wavFile);
  // check for fmt tag in memory
  if (wave_format.subChunkID[0] != 'f' || wave_format.subChunkID[1] != 'm' ||
      wave_format.subChunkID[2] != 't' || wave_format.subChunkID[3] != ' ') {
    throw std::runtime_error("Invalid Wave Format");
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
    throw std::runtime_error("Invalid data header");
  }

  // Now we set the variables that we passed in with the
  // data from the structs
  size = static_cast<int>(wave_data.subChunk2Size);
  samplerate = static_cast<int>(wave_format.dwSamplesPerSec);
  // The format is worked out by looking at the number of
  // channels and the bits per sample.
  auto format = BufferFormatType::Mono8;
  if (wave_format.wChannels == 1) {
    if (wave_format.wBitsPerSample == 8) {
      format = BufferFormatType::Mono8;
    } else if (wave_format.wBitsPerSample == 16) {
      format = BufferFormatType::Mono16;
    }
  } else if (wave_format.wChannels == 2) {
    if (wave_format.wBitsPerSample == 8) {
      format = BufferFormatType::Stereo8;
    } else if (wave_format.wBitsPerSample == 16) {
      format = BufferFormatType::Stereo16;
    }
  }
  info.Format = format;
  info.Samplerate = samplerate;
  info.DataSize = size;
}

WaveFile::Cursor::Cursor(const WaveFile *file) : File(file) {}

long WaveFile::Cursor::Read(char *buffer, const long bufferSize) {
  std::ifstream file;
  try {
    file.open(File->Name().c_str(), std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file: " + File->Name());
    }
    if (!file.seekg(File->dataOffset_ + Offset)) {
      throw std::runtime_error("Failed to load data of wav file: " +
                               File->Name());
    }
    auto sizeRead = 0L;
    while (sizeRead != bufferSize) {
      const auto toRead = bufferSize - sizeRead;
      file.read(&buffer[sizeRead], toRead);
      sizeRead += file.gcount();
      if (!file) {
        if (file.eof()) {
          file.clear();
          if (!Loop) {
            break;
          }
          Rewind();
          if (!file.seekg(File->dataOffset_)) {
            throw std::runtime_error("Failed to load data of wav file: " +
                                     File->Name());
          }
        } else {
          file.close();
          throw std::runtime_error("Failed to read file: " + File->Name());
        }
      }
    }
    Offset = file.tellg() - File->dataOffset_;
    file.close();
    return sizeRead;
  } catch ([[maybe_unused]] std::runtime_error &ex) {
    file.close();
    throw;
  }
}

void WaveFile::Cursor::Rewind() { Offset = 0; }
void WaveFile::Cursor::SetLoop(const bool loop) { Loop = loop; }
}  // namespace soil::sound
