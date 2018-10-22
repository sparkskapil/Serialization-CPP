#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Serialization/Serialization.h"
#include <fstream>

#include"MockClass.h"
#include<vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Serialization;

namespace SerializationTest
{
	TEST_CLASS(STLVectorTests)
	{
		std::ofstream writer;
		std::ifstream reader;

		bool OpenFile(std::string filePath, bool toWrite, bool appendMode = false)
		{
			if (toWrite == true)
				if (!appendMode)
					writer.open(filePath, std::ios::binary);
				else
					writer.open(filePath, std::ios::binary | std::ios::app);
			else
				reader.open(filePath, std::ios::binary);

			return reader.is_open() || writer.is_open();
		}

		void CloseFile()
		{
			if (writer.is_open())
				writer.close();
			if (reader.is_open())
				reader.close();
		}
	public:
		
		TEST_METHOD(STLVectorTests_Write_Read_Integer_Vector_From_File) 
		{
			std::string filePath = "TestData\\StlVector_Int.bin";
			OpenFile(filePath,true);
			std::vector<int> Collection;
			for (int i = 0; i < 10; i++)
				Collection.push_back(i);
			ErrorCode error = STLSerialization::Vector::Serialize<int>::Write(Collection, writer);
			CloseFile();
			Assert::AreEqual((int)ErrorCode::NoError,(int)error);

			OpenFile(filePath,false);
			std::vector<int> ReadCollection;
			error = STLSerialization::Vector::Deserialize<int>::Read(ReadCollection,reader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			Assert::IsTrue(Collection == ReadCollection);
		}

		TEST_METHOD(STLVectorTests_Write_Read_Float_Vector_From_File)
		{
			std::string filePath = "TestData\\StlVector_Float.bin";
			OpenFile(filePath, true);
			std::vector<float> Collection;
			for (float i = 0; i < 5; i+=0.2f)
				Collection.push_back(i);
			ErrorCode error = STLSerialization::Vector::Serialize<float>::Write(Collection, writer);
			CloseFile();
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);

			OpenFile(filePath, false);
			std::vector<float> ReadCollection;
			error = STLSerialization::Vector::Deserialize<float>::Read(ReadCollection, reader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			Assert::IsTrue(Collection == ReadCollection);
		}

		TEST_METHOD(STLVectorTests_Write_Read_Char_Vector_From_File)
		{
			std::string filePath = "TestData\\StlVector_Char.bin";
			OpenFile(filePath, true);
			std::vector<char> Collection;
			for (int i = 65; i < 75; i++)
				Collection.push_back((char)i);
			ErrorCode error = STLSerialization::Vector::Serialize<char>::Write(Collection, writer);
			CloseFile();
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);

			OpenFile(filePath, false);
			std::vector<char> ReadCollection;
			error = STLSerialization::Vector::Deserialize<char>::Read(ReadCollection, reader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			Assert::IsTrue(ReadCollection == Collection);
		}

		TEST_METHOD(STLVectorTests_Write_Read_String_Vector_From_File)
		{
			std::string filePath = "TestData\\StlVector_String.bin";
			OpenFile(filePath, true);
			std::vector<std::string> Collection;
			Collection.push_back("Initiating");
			Collection.push_back("Integration");
			Collection.push_back("Tests");
			ErrorCode error = STLSerialization::Vector::Serialize<std::string>::Write(Collection, writer);
			CloseFile();
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);

			OpenFile(filePath, false);
			std::vector<std::string> ReadCollection;
			error = STLSerialization::Vector::Deserialize<std::string>::Read(ReadCollection, reader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			Assert::IsTrue(Collection == ReadCollection);
		}

		TEST_METHOD(STLVectorTests_Write_Read_Object_Vector_From_File)
		{
			MockClass object1("Initial Commit", 1, 1.0f);
			MockClass object2("Fixed Bugs", 2, 1.2f);
			MockClass object3("Beta Release", 3, 2.0f);

			std::vector<MockClass> collection;
			collection.push_back(object1);
			collection.push_back(object2);
			collection.push_back(object3);

			std::string filePath = "TestData\\ObjectVector.bin";
			OpenFile(filePath,true);
			ErrorCode error = STLSerialization::Vector::Serialize<ISerializable>::Write(collection,writer);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			std::vector<MockClass> ReadCollection;
			OpenFile(filePath, false);
			error = STLSerialization::Vector::Deserialize<ISerializable>::Read(ReadCollection, reader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			CloseFile();

			Assert::IsTrue(CompareObjectVectors(collection,ReadCollection));
		}

		private:
			template<typename T>
			bool CompareObjectVectors(std::vector<T> first, std::vector<T> second)
			{
				if (first.size() != second.size())
					return false;
				for (size_t i = 0; i < first.size(); i++)
					if (!(first[i] == second[i]))
						return false;

				return true;
			}
	};
}