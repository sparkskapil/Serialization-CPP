#pragma once
#include <vector>
#include "ISerializable.h"
#include "ErrorCodes.h"
#include <fstream>

namespace Serialization
{
	namespace STLSerialization
	{
		namespace Vector
		{
			template<typename T>
			class Deserialize {
			public:
				static ErrorCode Read(std::vector<T> &collection, std::ifstream &reader)
				{
					if (!reader.is_open())
						return ErrorCode::CannotOpenFile;

					if (!std::is_fundamental<T>::value && !std::is_same<std::string,T>::value)
						return ErrorCode::NotValidType;

					ErrorCode error = ErrorCode::NoError;

					int size;
					reader.read(reinterpret_cast<char*>(&size), sizeof(int));
					while (size>0)
					{
						T item;
						error = Serialization::Deserialize::Read(item, reader);
						if (error != ErrorCode::NoError)
							break;
						collection.push_back(item);
						size--;
					}
					return error;
				}
			};

			template<>
			class Deserialize<ISerializable> {
			public:
				template<typename T>
				static ErrorCode Read(std::vector<T> &collection, std::ifstream &reader)
				{
					if (!reader.is_open())
						return ErrorCode::CannotOpenFile;

					if (!std::is_base_of<ISerializable,T>::value)
						return ErrorCode::NotValidType;

					ErrorCode error = ErrorCode::NoError;

					int size;
					reader.read(reinterpret_cast<char*>(&size), sizeof(int));
					while (size > 0)
					{
						T item;
						error = Serialization::Deserialize::ReadObject(item, reader);
						if (error != ErrorCode::NoError)
							break;
						collection.push_back(item);
						size--;
					}
					return error;
				
				}
			};
		}
	}
}