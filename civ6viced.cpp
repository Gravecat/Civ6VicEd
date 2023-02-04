/* civ6viced.cpp -- Edits Civilization VI saved games to enable or disable victory conditions.
   Based on hex editing work by Tahiaji: https://gaming.stackexchange.com/questions/356803/can-i-change-victory-condition-mid-game

   MIT License

   Copyright (c) 2023 Raine Simmons.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

uint32_t find_sequence(std::vector<uint8_t> &data, std::vector<uint8_t> seq)
{
	for (unsigned int i = 0; i < data.size() - seq.size(); i++)
	{
		if (data.at(i) != seq.at(0)) continue;
		bool valid_sequence = true;
		for (unsigned int j = 0; j < seq.size(); j++)
		{
			if (data.at(i + j) != seq.at(j))
			{
				valid_sequence = false;
				break;
			}
		}
		if (valid_sequence) return i - 1;
	}
	return 0;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid command-line arguments! Please specify a saved game file as a parameter." << std::endl;
		return 1;
	}
	std::string filename = std::string(argv[1]);
	std::ifstream stream(filename, std::ios::in | std::ios::binary);
	if (!stream.good())
	{
		std::cout << "Could not open file: " << filename << std::endl;
		return 1;
	}
	std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	stream.close();
	
	const uint32_t war_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x5D, 0x0E, 0xB5, 0x18, 0x01});
	//if (war_index) std::cout << "War victory byte located: " << std::to_string(war_index) << std::endl;
	const uint32_t diplo_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xAD, 0xDD, 0x34, 0xE4, 0x05});
	//if (diplo_index) std::cout << "Diplomatic victory byte located: " << std::to_string(diplo_index) << std::endl;
	const uint32_t cult_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xD1, 0x22, 0xBE, 0xEC, 0x01});
	//if (cult_index) std::cout << "Cultural victory byte located: " << std::to_string(cult_index) << std::endl;
	const uint32_t sci_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x8C, 0xFF, 0x43, 0x18, 0x01});
	//if (sci_index) std::cout << "Science victory byte located: " << std::to_string(sci_index) << std::endl;
	const uint32_t score_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x82, 0x4C, 0x6C, 0x62, 0x03});
	//if (score_index) std::cout << "Score victory byte located: " << std::to_string(score_index) << std::endl;
	const uint32_t relig_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xD9, 0xDC, 0xE5, 0x18, 0x01});
	//if (relig_index) std::cout << "Religious victory byte located: " << std::to_string(relig_index) << std::endl;
	
	uint32_t turn_limit_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAF, 0xF0, 0xF9, 0xDE, 0xE7, 0xA4, 0x0A, 0x2A, 0x02});
	if (turn_limit_index)
	{
		turn_limit_index++;
		//std::cout << "Turn limit enabled, bytes located: " << std::to_string(turn_limit_index) << std::endl;
	}
	else
	{
		turn_limit_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x25, 0x99, 0x80, 0xE7, 0xA4, 0x0A, 0x2A, 0x02});
		if (turn_limit_index)
		{
			turn_limit_index++;
			//std::cout << "Turn limit disabled, bytes located: " << std::to_string(turn_limit_index) << std::endl;
		}
		else
		{
			std::cout << "Could not locate turn limit data! Is this a valid Civ6 save file?" << std::endl;
			return 1;
		}
	}
	std::cout << "Successfully loaded Civ6 save file: " << filename << std::endl << std::endl;
	
	int choice = 0;
	while(true)
	{
		std::cout << "Raine's Civ6 Victory Editor v1.0" << std::endl << std::endl;
		std::cout << "1. War Victory: " << (contents.at(war_index) ? "ENABLED" : "disabled") << std::endl;
		std::cout << "2. Cultural Victory: " << (contents.at(cult_index) ? "ENABLED" : "disabled") << std::endl;
		std::cout << "3. Scientific Victory: " << (contents.at(sci_index) ? "ENABLED" : "disabled") << std::endl;
		std::cout << "4. Score Victory: " << (contents.at(score_index) ? "ENABLED" : "disabled") << std::endl;
		std::cout << "5. Religious Victory: " << (contents.at(relig_index) ? "ENABLED" : "disabled") << std::endl;
		if (diplo_index) std::cout << "6. Diplomatic Victory: " << (contents.at(diplo_index) ? "ENABLED" : "disabled") << std::endl;
		else std::cout << "   Diplomatic Victory: n/a" << std::endl;
		std::cout << "9. Save adjusted game file" << std::endl;
		std::cout << "0. Exit, do not save" << std::endl;
		std::cout << std::endl << "Make your choice:" << std::endl;
		std::cin >> choice;
		
		switch(choice)
		{
			case 0: std::cout << "Goodbye!" << std::endl; exit(0);
			case 1: contents.at(war_index) = !contents.at(war_index); break;
			case 2: contents.at(cult_index) = !contents.at(cult_index); break;
			case 3: contents.at(sci_index) = !contents.at(sci_index); break;
			case 4: contents.at(score_index) = !contents.at(score_index); break;
			case 5: contents.at(relig_index) = !contents.at(relig_index); break;
			case 6:
				if (diplo_index)
				{
					contents.at(diplo_index) = !contents.at(diplo_index);
					break;
				}	// do not break on false
			case 9:
			{
				std::ofstream stream_out(filename, std::ios::out | std::ios::binary);
				stream_out.write(reinterpret_cast<const char*>(contents.data()), contents.size());
				stream_out.close();
				std::cout << "Save file updated!" << std::endl;
				return 0;
			}
			default: std::cout << "Invalid choice!" << std::endl << std::endl;
		}
	}
	return 0;
}
