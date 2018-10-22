#pragma once
#include <string>
#include <fstream>
#include <type_traits>
#include "ErrorCodes.h"

namespace Serialization {

	class Deserialize {
	public:

		template<typename T>
		static ErrorCode Read(T&, std::ifstream&);

		template<>
		static ErrorCode Read(std::string&, std::ifstream&);

		static ErrorCode ReadObject(ISerializable&, std::ifstream&);
	};

	template<typename T>
	inline ErrorCode Deserialize::Read(T& Value, std::ifstream &reader)
	{
		if (!reader.is_open())
			return ErrorCode::CannotOpenFile;

		if (!std::is_fundamental<T>::value)
			return ErrorCode::NotValidType;
		reader.read(reinterpret_cast<char*>(&Value), sizeof(T));

		return NoError;
	}

	template<>
	inline ErrorCode Deserialize::Read(std::string &String, std::ifstream &reader)
	{
		if (!reader.is_open())
			return ErrorCode::CannotOpenFile;

		int len;
		reader.read(reinterpret_cast<char*>(&len), sizeof(int));

		char *buffer = new char[len];
		reader.read(buffer, sizeof(char)*len);
		buffer[len] = '\0';

		String.assign(buffer);

		return ErrorCode::NoError;
	}

	inline ErrorCode Deserialize::ReadObject(ISerializable& object, std::ifstream& reader)
	{
		if (!reader.is_open())
			return ErrorCode::CannotOpenFile;
		object.deserialize(reader);
		return ErrorCode::NoError;
	}
}