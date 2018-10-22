#include "stdafx.h"
#include"MockClass.h"

void MockClass::deserialize(ifstream& reader)
{
	Deserialize::Read(String, reader);
	Deserialize::Read(Integer, reader);
	Deserialize::Read(Float, reader);
}

bool MockClass::operator==(const MockClass &mockClass)
{
	if (String != mockClass.String)
		return false;
	if (Integer != mockClass.Integer)
		return false;
	if (Float != mockClass.Float)
		return false;
	return true;
}

MockClass::MockClass()
{
	String = "";
	Integer = 0;
	Float = 0.0f;
}

MockClass::MockClass(std::string Word, int intVal, float floatVal)
{
	String = Word;
	Integer = intVal;
	Float = floatVal;
}

void MockClass::serialize(ofstream& writer)
{
	Serialize::Write(String, writer);
	Serialize::Write(Integer, writer);
	Serialize::Write(Float, writer);
}