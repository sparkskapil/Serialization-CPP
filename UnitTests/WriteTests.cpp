#include "stdafx.h"
#include "CppUnitTest.h"

#include "../Serialization/Serialization.h"
#include <fstream>

#include"MockClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Serialization;

namespace SerializationTest
{
	TEST_CLASS(WriteTests)
	{
		std::ofstream writer;
	public:
		TEST_METHOD_INITIALIZE(FileInitialize) 
		{
			writer.open("TestData\\TestWrite.bin",std::ios::binary);
		}
		TEST_METHOD_CLEANUP(FileCleanup) 
		{
			writer.close();
		}

		TEST_METHOD(WriteTest_Write_Integer_To_File)
		{
			int intValue = 30;
			ErrorCode error = Serialize::Write(intValue, writer);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
		}

		TEST_METHOD(WriteTest_Write_Float_To_File)
		{
			float floatValue = 3.14159f;
			ErrorCode error = Serialize::Write(floatValue, writer);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
		}

		TEST_METHOD(WriteTest_Write_String_To_File)
		{
			std::string String = "Hello World!";
			ErrorCode error = Serialize::Write(String, writer);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
		}

		TEST_METHOD(WriteTest_Write_Object_Of_Class)
		{
			MockClass mockClass("Hello", 20, 3.14159f);
			ErrorCode error = Serialize::WriteObject(mockClass,writer);
			Assert::AreEqual((int)ErrorCode::NoError, (int)error);
		}
	};
}