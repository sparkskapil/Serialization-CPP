#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Serialization/Serialization.h"
#include <fstream>
#include"MockClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Serialization;

namespace SerializationTest
{
	TEST_CLASS(ReadWriteTests)
	{
		std::ofstream writer;
		std::ifstream reader;

		bool OpenFile(std::string filePath,bool toWrite, bool appendMode = false)
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
		
		TEST_METHOD(ReadWriteTest_Multiple_Int_Data)
		{
			std::string filePath = "TestData\\MultipleInt.bin";
			OpenFile(filePath, true);
			for (int i = 0; i < 10; i++)
				Serialize::Write(i,writer);
			CloseFile();

			OpenFile(filePath, false);
			int X;
			int i = 0;
			while (!reader.eof())
			{
				Deserialize::Read(X, reader);
				if (i != X)
					break;
				i++;
			}
			Assert::IsTrue(i>=10);
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_Float_Data)
		{
			std::string filePath = "TestData\\MultipleFloat.bin";
			OpenFile(filePath, true);
			for (float i = 0.0f; i < 1.5; i+=0.2f)
				Serialize::Write(i, writer);
			CloseFile();

			OpenFile(filePath, false);
			float X;
			float i = 0.0f;
			while (!reader.eof())
			{
				Deserialize::Read(X, reader);
				if (abs(i - X)>0.00001)
					break;
				i+=0.2f;
			}
			Assert::IsTrue(i >= 1.5);
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_String_Data)
		{
			std::string str1 = "Hello!";
			std::string str2 = "World!";
			std::string str3 = "Eureka!";
			std::string filePath = "TestData\\MultipleStrings.bin";
			OpenFile(filePath, true);
			Serialize::Write(str1, writer);
			Serialize::Write(str2, writer);
			Serialize::Write(str3, writer);
			Serialize::Write(filePath, writer);
			CloseFile();

			OpenFile(filePath,false);
			std::string readStr;
			Deserialize::Read(readStr,reader);
			Assert::AreEqual(str1.c_str(), readStr.c_str());
			Deserialize::Read(readStr, reader);
			Assert::AreEqual(str2.c_str(), readStr.c_str());
			Deserialize::Read(readStr, reader);
			Assert::AreEqual(str3.c_str(), readStr.c_str());
			Deserialize::Read(readStr, reader);
			Assert::AreEqual(filePath.c_str(), readStr.c_str());
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_Heterogeneous_Data)
		{
			int Integer = 10;
			float Float = 3.14159f;
			std::string Word = "Hello!!!";
			std::string filePath = "TestData\\HeterogeneousFile.bin";
			OpenFile(filePath,true);
			Serialize::Write(Integer, writer);
			Serialize::Write(Float, writer);
			Serialize::Write(Word, writer);
			Serialize::Write(Float, writer);
			Serialize::Write(Word, writer);
			Serialize::Write(Integer, writer);
			CloseFile();

			int readInt;
			float readFloat;
			std::string readWord;
			OpenFile(filePath, false);
			Deserialize::Read(readInt, reader);
			Assert::AreEqual(readInt,Integer);
			
			Deserialize::Read(readFloat, reader);
			Assert::AreEqual(readFloat, Float);
			
			Deserialize::Read(readWord, reader);
			Assert::AreEqual(readWord, Word);
			
			Deserialize::Read(readFloat, reader);
			Assert::AreEqual(readFloat, Float);
			
			Deserialize::Read(readWord, reader);
			Assert::AreEqual(readWord, Word);

			Deserialize::Read(readInt, reader);
			Assert::AreEqual(readInt, Integer);
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_Heterogeneous_Data_WithAppend)
		{
			int Integer = 10;
			float Float = 3.14159f;
			std::string Word = "Hello!!!";
			std::string filePath = "TestData\\HeterogeneousFile_WithAppend.bin";
			OpenFile(filePath, true);
			Serialize::Write(Integer, writer);
			CloseFile();
			OpenFile(filePath, true,true);
			Serialize::Write(Float, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::Write(Word, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::Write(Float, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::Write(Word, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::Write(Integer, writer);
			CloseFile();
			CloseFile();

			int readInt;
			float readFloat;
			std::string readWord;
			OpenFile(filePath, false);
			Deserialize::Read(readInt, reader);
			Assert::AreEqual(readInt, Integer);

			Deserialize::Read(readFloat, reader);
			Assert::AreEqual(readFloat, Float);

			Deserialize::Read(readWord, reader);
			Assert::AreEqual(readWord, Word);

			Deserialize::Read(readFloat, reader);
			Assert::AreEqual(readFloat, Float);

			Deserialize::Read(readWord, reader);
			Assert::AreEqual(readWord, Word);

			Deserialize::Read(readInt, reader);
			Assert::AreEqual(readInt, Integer);
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_Object_Data)
		{
			MockClass obj1("String1", 1, 1.1f);
			MockClass obj2("String2", 2, 2.2f);
			MockClass obj3("String3", 3, 3.3f);
			std::string filePath = "TestData\\MultipleObjectsFile.bin";
			OpenFile(filePath, true);
			Serialize::WriteObject(obj1, writer);
			Serialize::WriteObject(obj2, writer);
			Serialize::WriteObject(obj3, writer);
			CloseFile();

			OpenFile(filePath, false);
			MockClass readObj;
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj1);
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj2);
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj3);
			CloseFile();
		}

		TEST_METHOD(ReadWriteTest_Multiple_Object_Data_With_Append)
		{
			MockClass obj1("String1", 1, 1.1f);
			MockClass obj2("String2", 2, 2.2f);
			MockClass obj3("String3", 3, 3.3f);
			std::string filePath = "TestData\\MultipleObjectsFile_Appended.bin";
			OpenFile(filePath, true);
			Serialize::WriteObject(obj1, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::WriteObject(obj2, writer);
			CloseFile();
			OpenFile(filePath, true, true);
			Serialize::WriteObject(obj3, writer);
			CloseFile();

			OpenFile(filePath, false);
			MockClass readObj;
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj1);
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj2);
			Deserialize::ReadObject(readObj, reader);
			Assert::IsTrue(readObj == obj3);
			CloseFile();
		}
	};
}