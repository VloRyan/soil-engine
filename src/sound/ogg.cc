#include "sound/ogg.h"

#include <cstring>
#include <fstream>
#include <iostream>
namespace soil::sound {

OggFile::OggFile(const std::string& filename, const InfoType& info)
    : File(filename, info) {}

long OggFile::Cursor::Read(char* buffer, const long bufferSize) {
  auto sizeRead = 0L;
  while (sizeRead < bufferSize) {
    const auto result =
        ov_read(&OggVorbisFile, &buffer[sizeRead], bufferSize - sizeRead, 0,
                2 /*16 bit */, 1, nullptr);
    if (result == 0) {
      if (!Loop) {
        break;
      }
      Rewind();
    } else if (result < 0) {
      std::string message;
      switch (result) {
        case OV_HOLE:
          message =
              "indicates there was an interruption in the data. (one of: "
              "garbage between pages, loss of sync followed by recapture, or a "
              "corrupt page)";
          break;
        case OV_EBADLINK:
          message =
              "indicates that an invalid stream section was supplied to "
              "libvorbisfile, or the requested link is corrupt.";
          break;
        case OV_EINVAL:
          message =
              "indicates the initial file headers couldn't be read or are "
              "corrupt, or that the initial open call for vf failed.";
          break;
        default:
          message = "Unknown error(" + std::to_string(result) + ")";
      }
      throw std::runtime_error(message);
    }
    sizeRead += result;
  }
  return sizeRead;
}

void OggFile::Cursor::Rewind() {
  const auto rawResult = ov_raw_seek(&OggVorbisFile, 0);
  if (rawResult != 0) {
    std::string message;
    switch (rawResult) {
      case OV_ENOSEEK:
        message = "Bitstream is not seekable";
        break;
      case OV_EINVAL:
        message =
            "Invalid argument value; possibly called with an OggVorbis_File "
            "structure that isn't open. ";
        break;
      case OV_EREAD:
        message = "A read from media returned an error";
        break;
      case OV_EFAULT:
        message =
            "Internal logic fault; indicates a bug or heap/stack corruption";
        break;
      case OV_EBADLINK:
        message =
            "Invalid stream section supplied to libvorbisfile, or the "
            "requested "
            "link is corrupt";
        break;
      default:
        message = "Unknown error(" + std::to_string(rawResult) + ")";
    }
    throw std::runtime_error(message);
  }
}
long OggFile::Cursor::FileSize() const { return File->Size; }
OggFile::Cursor::Cursor(OggFile* file) : File(file), OggVorbisFile() {}
void OggFile::Cursor::SetLoop(const bool loop) { Loop = loop; }
OggFile::Cursor* OggFile::NewCursor() {
  auto* cursor = new Cursor(this);

  ov_callbacks oggCallbacks;
  oggCallbacks.read_func = read_ogg_callback;
  oggCallbacks.close_func = nullptr;
  oggCallbacks.seek_func = seek_ogg_callback;
  oggCallbacks.tell_func = tell_ogg_callback;

  if (const auto ret = ov_open_callbacks(cursor, &cursor->OggVorbisFile,
                                         nullptr, -1, oggCallbacks);
      ret < 0) {
    std::string message;
    switch (ret) {
      case OV_EREAD:
        message = "A read from media returned an error";
        break;
      case OV_ENOTVORBIS:
        message = "Bitstream does not contain any Vorbis data";
        break;
      case OV_EVERSION:
        message = "Vorbis version mismatch";
        break;
      case OV_EBADHEADER:
        message = "Invalid Vorbis bitstream header";
        break;
      case OV_EFAULT:
        message =
            "Internal logic fault; indicates a bug or heap/stack corruption";
        break;
      default:
        message = "Could not open OggVorbisFile: " + std::to_string(ret);
    }
    throw std::runtime_error(message);
  }

  return cursor;
}

OggFile* OggFile::Load(const std::string& fileName) {
  InfoType info;
  int dataSize;
  loadInfo(fileName, dataSize, info);

  auto* file = new OggFile(fileName, info);
  file->Size = dataSize;
  return file;
}

bool OggFile::loadInfo(const std::string& filename, int& dataSize,
                       InfoType& info) {
  std::ifstream file;
  file.open(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "ERROR: couldn't open file" << std::endl;
    return false;
  }

  file.seekg(0, std::ios_base::beg);
  file.ignore(std::numeric_limits<std::streamsize>::max());
  dataSize = file.gcount();
  file.clear();
  file.seekg(0, std::ios_base::beg);

  OggVorbis_File oggVorbisFile;
  if (auto ret = ov_fopen(filename.c_str(), &oggVorbisFile); ret != 0) {
    std::string message;
    switch (ret) {
      case OV_EREAD:
        message = "A read from media returned an error";
        break;
      case OV_ENOTVORBIS:
        message = "Bitstream does not contain any Vorbis data";
        break;
      case OV_EVERSION:
        message = "Vorbis version mismatch";
        break;
      case OV_EBADHEADER:
        message = "Invalid Vorbis bitstream header";
        break;
      case OV_EFAULT:
        message =
            "Internal logic fault; indicates a bug or heap/stack corruption";
        break;
      default:
        message = "Could not open OggVorbisFile: " + std::to_string(ret);
    }
    throw std::runtime_error(message);
  }

  const auto* vorbisInfo = ov_info(&oggVorbisFile, -1);

  info.Samplerate = vorbisInfo->rate;

  ov_raw_seek(
      &oggVorbisFile,
      0);  // @see
           // https://stackoverflow.com/questions/8653670/vorbis-finding-decompressed-size-of-file
  auto pcmSize = ov_pcm_total(&oggVorbisFile, -1);
  pcmSize *= 2;  // 16bit
  pcmSize *= vorbisInfo->channels;

  info.DataSize = pcmSize;

  if (vorbisInfo->channels == 1)
    info.Format = BufferFormatType::Mono16;
  else if (vorbisInfo->channels == 2)
    info.Format = BufferFormatType::Stereo16;
  else {
    throw std::runtime_error("ERROR: unrecognised ogg format: " +
                             std::to_string(vorbisInfo->channels) +
                             " channels");
  }

  if (const auto ret = ov_clear(&oggVorbisFile); ret != 0) {
    throw std::runtime_error("ERROR: ov_clear() returned " +
                             std::to_string(ret));
  }
  return true;
}

std::size_t read_ogg_callback(void* destination, const std::size_t size1,
                              const std::size_t size2, void* fileHandle) {
  auto* cursor = static_cast<OggFile::Cursor*>(fileHandle);

  auto length = size1 * size2;

  if (cursor->Consumed + length > cursor->FileSize()) {
    length = cursor->FileSize() - cursor->Consumed;
  }
  std::ifstream file;
  file.open(cursor->File->Name(), std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR: couldn't open file");
  }

  auto* moreData = new char[length];

  file.clear();
  file.seekg(cursor->Consumed);
  if (!file.read(&moreData[0], length)) {
    if (file.eof()) {
      file.clear();  // just clear the error, we will resolve it later
    } else if (file.fail()) {
      std::cerr << "ERROR: OGG stream has fail bit set " << cursor->File->Name()
                << std::endl;
      file.clear();
      return 0;
    } else if (file.bad()) {
      perror(("ERROR: OGG stream has bad bit set " + cursor->File->Name())
                 .c_str());
      file.clear();
      return 0;
    }
  }
  cursor->Consumed += length;

  std::memcpy(destination, &moreData[0], length);

  delete[] moreData;

  file.close();

  return length;
}

std::int32_t seek_ogg_callback(void* fileHandle, const ogg_int64_t to,
                               const std::int32_t type) {
  auto* cursor = static_cast<OggFile::Cursor*>(fileHandle);

  if (type == SEEK_CUR) {
    cursor->Consumed += to;
  } else if (type == SEEK_END) {
    cursor->Consumed = cursor->FileSize() - to;
  } else if (type == SEEK_SET) {
    cursor->Consumed = to;
  } else
    return -1;  // what are you trying to do vorbis?

  if (cursor->Consumed < 0) {
    cursor->Consumed = 0;
    return -1;
  }
  if (cursor->Consumed > cursor->FileSize()) {
    cursor->Consumed = cursor->FileSize();
    return -1;
  }

  return 0;
}

long int tell_ogg_callback(void* fileHandle) {
  const auto* cursor = static_cast<OggFile::Cursor*>(fileHandle);
  return cursor->Consumed;
}
}  // namespace soil::sound
