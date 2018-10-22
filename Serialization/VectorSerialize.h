#pragma once
#include <vector>
#include "ISerializable.h"
#include "ErrorCodes.h"

namespace Serialization 
{
	namespace STLSerialization 
	{
		namespace Vector
		{
			template<typename T>
			class Serialize {
			public:
				static ErrorCode Write(std::vector<T> &collection,std::ofstream &writer)
				{
					if (!writer.is_open())
						return ErrorCode::CannotOpenFile;

					if (!std::is_fundamental<T>::value && !std::is_same<std::string,T>::value)
						return ErrorCode::NotValidType;

					ErrorCode error = ErrorCode::NoError;

					auto size = collection.size();
					writer.write(reinterpret_cast<char*>(&size), sizeof(int));
					for (auto item : collection)
					{
						error = Serialization::Serialize::Write(item, writer);
						if (error != ErrorCode::NoError)
							break;
					}
					return error;
				}
			};

			template<>
			class Serialize<ISerializable> {
			public:
				template<typename T>
				static ErrorCode Write(std::vector<T> &collection, std::ofstream &writer) 
				{
					if (!writer.is_open())
						return ErrorCode::CannotOpenFile;

					if (!std::is_base_of<ISerializable,T>::value)
						return ErrorCode::NotValidType;

					ErrorCode error = ErrorCode::NoError;

					auto size = collection.size();
					writer.write(reinterpret_cast<char*>(&size), sizeof(int));
					for (auto item : collection)
					{
						error = Serialization::Serialize::WriteObject(item, writer);
						if (error != ErrorCode::NoError)
							break;
					}
					return error;				
				}
			};
		}
	}
}