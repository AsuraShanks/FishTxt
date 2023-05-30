#pragma once

struct EncodingUnit {
   int _codePage = 0;
   const char *_aliasList = nullptr;
};

class EncodingMapper {
public:
	static EncodingMapper& getInstance() {
		static  EncodingMapper  instance;
		return instance;
	}
    
	int getEncodingFromString(const char * encodingAlias) const;

private:
	EncodingMapper() = default;
	~EncodingMapper() = default;
};

