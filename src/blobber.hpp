#pragma once

#include <cstring>
#include <fstream>
#include <string>
#include <vector>
namespace blob
{
	constexpr long headerFilenameSize = 64;
	struct BlobEntryHeader
	{
		char name[64];
		char extension[16];
		long datasize;
		long offset;
	};
	struct BlobHeader
	{
		long nElements;
		std::vector<std::string> elements;
	};
	class BlobReader
	{
	  private:
		long dataread()
		{
			long value = ((long*)data)[marker];
			marker += sizeof(long);
			return value;
		}
		void dataread(void* buffer, long size)
		{
			memcpy(buffer, &data[marker], size);
			marker += size;
		}
        bool init()
        {
			bh.nElements = dataread();
			for (int i = 0; i < bh.nElements; i++)
			{
				char entry[headerFilenameSize] = {};
				dataread(entry, headerFilenameSize);
				bh.elements.emplace_back(entry);
			}
			for (int i = 0; i < bh.nElements; i++)
			{
				BlobEntryHeader h;
				dataread(&h, sizeof(h));
				headers.emplace_back(h);
				marker += h.datasize;
			}
			return true;
        }

	  public:
		char* data;
		long marker = 0;
        bool shouldFree = false;
		BlobHeader bh;
		std::vector<BlobEntryHeader> headers;
		~BlobReader() { unload(); }
		bool readFile(const std::string& filepath)
		{
			std::ifstream ifile;
			ifile.open(filepath.c_str(), std::ios::binary | std::ios::ate);
			if (!ifile.is_open())
				return false;
			long fsize = ifile.tellg();
			data = (char*)calloc(ifile.tellg(), 1);
            shouldFree = true;
			ifile.seekg(0);
			ifile.read((char*)data, fsize);
			ifile.close();
            return init();
		}
        bool readMemory(char* blobPtr)
        {
            data = blobPtr;
            return init();
        }
		void unload()
		{
			if (shouldFree)
				free(data);
		}
		void* getAsset(const std::string& assetName, long& datasize)
		{
			for (BlobEntryHeader& h : headers)
				if (strcmp(h.name, assetName.c_str()) == 0)
				{
					datasize = h.datasize;
					return (void*)(&data[h.offset]);
				}
			return nullptr;
		}
	};
} // namespace blob
