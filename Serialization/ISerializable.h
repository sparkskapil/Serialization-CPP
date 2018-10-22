#pragma once
#include <fstream>


namespace Serialization {

	using std::ofstream;
	using std::ifstream;

	class ISerializable
	{
		//NOTE ORDER OF WRITING MUST BE SAME AS 
		//ORDER OF READING TO AVOID DATA CORRUPTION

	public:

		//Implement this interface in the class
		//Whose objects you need to write into the file
		//Write how you want your objects
		//To be Written into the file
		virtual void serialize(ofstream &writer) = 0;


		//Implement this interface in the class
		//Whose objects you need to read from the file
		//Write how you want your objects
		//To be read into the file
		virtual void deserialize(ifstream &reader) = 0;
	};
};