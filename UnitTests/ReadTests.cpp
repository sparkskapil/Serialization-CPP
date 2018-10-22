#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Serialization/Serialization.h"
#include <fstream>

#include "MockClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Serialization;

namespace SerializationTest
{
	TEST_CLASS(ReadTests)
	{
		std::ifstream intReader;
		std::ifstream floatReader;
		std::ifstream stringReader;
		std::ifstream objectReader;

	public:
		TEST_METHOD_INITIALIZE(FileInitialize) 
		{								
			intReader.open("TestData\\IntFile.bin", std::ios::binary);			//int a = 20
			floatReader.open("TestData\\FloatFile.bin", std::ios::binary);		//float b = 3.14159
			stringReader.open("TestData\\StringFile.bin", std::ios::binary);	//string c = "Hello World!"	
			objectReader.open("TestData\\ObjectFile.bin", std::ios::binary);
		}

		TEST_METHOD_CLEANUP(FileCleanup) 
		{
			intReader.close();
			floatReader.close();
			stringReader.close();
			objectReader.close();
		}

		TEST_METHOD(ReadTest_Read_Int_From_File)
		{
			int a;
			ErrorCode error = Deserialize::Read(a, intReader);
			Assert::AreEqual((int)ErrorCode::NoError,(int)error);
			Assert::AreEqual(20, a);		
		}

		TEST_METHOD(ReadTest_Read_Float_From_File)
		{
			float a;
			ErrorCode error = Deserialize::Read(a, floatReader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			Assert::IsTrue(abs(3.14159f - a) < 0.0001);
		}

		TEST_METHOD(ReadTest_Read_String_From_File)
		{
			std::string a;
			ErrorCode error = Deserialize::Read(a, stringReader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			Assert::AreEqual("Hello World!", a.c_str());
		}

		TEST_METHOD(ReadTest_Read_Object_From_File)
		{
			MockClass mockClass;
			ErrorCode error = Deserialize::ReadObject(mockClass, objectReader);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
			MockClass baseLine("TestObjectRead", 20, 3.14159250f);
			Assert::IsTrue(mockClass == baseLine);
		}

	};
}