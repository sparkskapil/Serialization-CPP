#pragma once
#include <string>
#include <fstream>
#include <type_traits>
#include "ErrorCodes.h"

namespace Serialization {

	class Serialize {
	public:
		
		template<typename T>
		static ErrorCode Write(T, std::ofstream&); 
		
		template<>
		static ErrorCode Write(std::string String, std::ofstream&);

		static ErrorCode WriteObject(ISerializable &object, std::ofstream&);
	};

	template<typename T>
	inline ErrorCode Serialize::Write(T Value, std::ofstream &writer)
	{
		if(! writer.is_open())
			return ErrorCode::CannotOpenFile;

		if (!std::is_fundamental<T>::value)
			return ErrorCode::NotValidType;

		writer.write(reinterpret_cast<char*>(&Value),sizeof(T));
		return ErrorCode::NoError;
	}

	template<>
	inline ErrorCode Serialize::Write(std::string String, std::ofstream &writer)
	{
		if (!writer.is_open())
			return ErrorCode::CannotOpenFile;

		auto len = String.size();
		writer.write(reinterpret_cast<char*>(&len), sizeof(int));

		writer.write(String.c_str(),sizeof(char)*len);

		return ErrorCode::NoError;
	}

	inline ErrorCode Serialize::WriteObject(ISerializable & object, std::ofstream &writer)
	{
		if (!writer.is_open())
			return ErrorCode::CannotOpenFile;
		object.serialize(writer);
		return ErrorCode::NoError;
	}
}