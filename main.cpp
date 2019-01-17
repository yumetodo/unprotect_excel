#include <xlnt/xlnt.hpp>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
using std::size_t;
constexpr auto create_charset() {
	constexpr size_t size = (u8'z' - u8'a' + 1) + 1;
	std::array<char, size> re{};
	for (char c = u8'a', i = 0; c <= u8'z'; ++c, ++i) {
		re[i] = c;
	}
	re.back() = '-';
	return re;
}
constexpr auto charset = create_charset();
constexpr size_t min_length = 2;
constexpr size_t max_length = 30;
std::string try_unprotect(const std::string& input, std::string base_str = "")
{
	if (max_length < base_str.size() + 1)
	{
		return {};
	}
	if (min_length <= base_str.size() + 1) for(auto c : charset)
	{
		auto pass = base_str + c;
		std::cout << pass << std::endl;
		xlnt::workbook wb;
		try {
			std::fstream in{input};
			wb.load(in, pass);
		}
		catch (const std::exception&) {
			continue;
		}
		return pass;
	}
	for(auto c : charset)
	{
		if(auto re = try_unprotect(input, base_str + c); !re.empty()) return re;
	}
	return {};
}
int main(int argc, char* argv[])
{
	if (2 != argc) {
		std::cerr << "invalid input" << std::endl;
		return 1;
	}
	if (!std::ifstream(argv[1])) {
		std::cerr << "file not found" << std::endl;
		return 1;
	}
	auto is_protected = [](const std::string& input) {
		xlnt::workbook wb;
		try {
			std::fstream in{input};
			wb.load(in);
		}
		catch (...) {
			return true;
		}
		return false;
	};
	if (!is_protected(argv[1])) {
		std::cerr << "not protected" << std::endl;
		return 0;
	}
	std::cout << "start..." << std::endl;
	if (auto re = try_unprotect(argv[1]); re.empty()) {
		std::cerr << "fail to find password" << std::endl;
		return 1;
	}
	else {
		std::cout << "password: " << re << std::endl;
	}
}