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
	if (war_index) std::cout << "War victory byte located: " << std::to_string(war_index) << std::endl;
	const uint32_t diplo_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xAD, 0xDD, 0x34, 0xE4, 0x05});
	if (diplo_index) std::cout << "Diplomatic victory byte located: " << std::to_string(diplo_index) << std::endl;
	const uint32_t cult_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xD1, 0x22, 0xBE, 0xEC, 0x01});
	if (cult_index) std::cout << "Cultural victory byte located: " << std::to_string(cult_index) << std::endl;
	const uint32_t sci_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x8C, 0xFF, 0x43, 0x18, 0x01});
	if (sci_index) std::cout << "Science victory byte located: " << std::to_string(sci_index) << std::endl;
	const uint32_t score_index = find_sequence(contents, {0x00, 0x00, 0x00, 0x82, 0x4C, 0x6C, 0x62, 0x03});
	if (score_index) std::cout << "Score victory byte located: " << std::to_string(score_index) << std::endl;
	const uint32_t relig_index = find_sequence(contents, {0x00, 0x00, 0x00, 0xD9, 0xDC, 0xE5, 0x18, 0x01});
	if (relig_index) std::cout << "Religious victory byte located: " << std::to_string(relig_index) << std::endl;
	return 0;
}
