#pragma once

#include <iostream> 
#include <filesystem>
#include <vector>
#include<thread>
#include <fstream>



class Searcher
{
public:
	void FileToFind(const std::string& fileName);
	void FileToFind(const std::string& fileName, const std::string startPath );
private:
	void write_to_file(std::string what_to_write);
	std::string find(const std::string& fileName, std::vector <std::filesystem::path>& list_of_file);
	void fill_vector(const std::filesystem::path startPath, std::vector <std::filesystem::path>& list_of_file);
	std::string wheresafe = "directory.txt";
};


void Searcher :: fill_vector(const std::filesystem::path startPath, std::vector <std::filesystem::path>& list_of_file)
{
	for (const auto& file : std::filesystem::directory_iterator(startPath, std::filesystem::directory_options::skip_permission_denied))
	{
		list_of_file.push_back(file.path());
	}
}

std::string Searcher::find(const std::string& fileName, std::vector <std::filesystem::path>& list_of_file)
{
	std::string ifFile = "not found";
	for (size_t i = 0; i < list_of_file.size(); i++)
	{
		try
		{
			if (is_regular_file(list_of_file[i]))
			{
				ifFile = list_of_file[i].string();
				if (ifFile.find(fileName) != std::string::npos)
				{
					return ifFile;
				}
				ifFile = "not found";
				continue;
			}
			else
			{
				for (const auto& file : std::filesystem::directory_iterator(list_of_file[i], std::filesystem::directory_options::skip_permission_denied))
				{
					list_of_file.push_back(file.path());
				}
			}

		}
		catch (const std::exception& e)
		{
			if (list_of_file[i] != fileName)
			{
				continue;
			}
		}
	}
	return ifFile;
}

void Searcher::write_to_file(std::string what_to_write)
{
	std::ofstream writter;
	writter.open(wheresafe, std::ios::app);
	if (!writter)
	{
		std::cout << "Error open file" << std::endl;
	}
	what_to_write += "\n";
	writter << what_to_write;
	writter.close();
}

void Searcher ::FileToFind(const std::string& fileName)
{
	std::string 
#ifdef _WIN32
	startPath  = "C:\\";
#elif defined(__linux__)
		startPath = "/";
#endif
	std::vector <std::filesystem::path> list_of_file;
	fill_vector(startPath, list_of_file);

	size_t counter_folder = list_of_file.size();
	size_t size_of_pool = std::thread::hardware_concurrency();
	if (counter_folder < size_of_pool)
	{
		size_of_pool = counter_folder;
	}

	std::string res = "not found";
	for (size_t i = 0; i < list_of_file.size(); i++)
	{
		std::thread first([&]()
			{
				std::vector <std::filesystem::path> list_of_file_1;
				try
				{
					fill_vector(list_of_file[i], list_of_file_1);
				}
				catch (...)
				{
				}
				res = find(fileName, list_of_file_1);

			});
		if (i + 1 < list_of_file.size())
		{
			i++;
		}
		std::thread second([&]()
			{
				std::vector <std::filesystem::path> list_of_file_2;
				try
				{
					fill_vector(list_of_file[i], list_of_file_2);
				}
				catch (...)
				{

				}
				res = find(fileName, list_of_file_2);
			});
		first.join();
		second.join();
		if (res != "not found")
		{
			write_to_file(res);
			std::cout << "Success! Information was written in the "<< wheresafe << std::endl;
			break;
		}
	}
}
void Searcher::FileToFind(const std::string& fileName, const std::string startPath)
{
	std::vector <std::filesystem::path> list_of_file;
	fill_vector(startPath, list_of_file);
	size_t counter_folder = list_of_file.size();
	size_t size_of_pool = std::thread::hardware_concurrency();
	if (counter_folder < size_of_pool)
	{
		size_of_pool = counter_folder;
	}

	std::string res = "not found";
	for (size_t i = 0; i < list_of_file.size(); i++)
	{
		std::thread first([&]()
			{
				std::vector <std::filesystem::path> list_of_file_1;
				try
				{
					fill_vector(list_of_file[i], list_of_file_1);
				}
				catch (...)
				{
				}
				res = find(fileName, list_of_file_1);

			});
		if (i + 1 < list_of_file.size())
		{
			i++;
		}
		std::thread second([&]()
			{
				std::vector <std::filesystem::path> list_of_file_2;
				try
				{
					fill_vector(list_of_file[i], list_of_file_2);
				}
				catch (...)
				{

				}
				res = find(fileName, list_of_file_2);
			});
		first.join();
		second.join();
		if (res != "not found")
		{
			write_to_file(res);
			std::cout << "Success! Information was written in the "<< wheresafe << std::endl;
			break;
		}
	}
}
