
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>

int InputInt(std::string msg, std::function<bool(int)> validInt = nullptr)
{
	std::string inp;
	int number = 0;
	while (true)
	{
		std::cout << msg << std::endl;
		std::getline(std::cin, inp);		
		try
		{
			size_t pos;
			number = std::stoi(inp, &pos);

			if (pos != inp.length()) {
				std::cout << "Incorrect input! No digits numbers in a string!" << std::endl;
				continue;
			}

			if (validInt == nullptr)
			{
				break;
			}
			else if(!validInt(number))
			{
				continue;
			}

			break;
		}
		catch (...)
		{
			std::cout << "Incorrect input! Please enter number." << std::endl;
		}
	}
	return number;
}

auto posIntGreaterThenZero = [](int n) -> bool
	{
		if (n <= 0)
		{
			std::cout << "Value can't be less or Zero!" << std::endl;
			return false;
		}
		return true;
	};

double Calculate(long start, long end, double step)
{
	double tempSum = 0;
	for (long i = start; i < end; ++i)
	{
		auto q = step * (i + 0.5);
		tempSum += 4 / (1 + q * q);
	}
	return tempSum;
}

int main()
{
	using namespace std;
	cout << "Lab Work 1" << endl;

	time_t startTime, endTime;

	int n = InputInt("Please enter number of step N:", posIntGreaterThenZero);
	int threadsCount = InputInt("Please enter number of threads for calculation:", posIntGreaterThenZero);
	double h = (double)1 / (double)n;
	double totalSum = 0;
	int chunkSize = n / threadsCount;
	vector<unique_ptr<thread>> threads;
	mutex m;
	time(&startTime);
	for (int i = 0; i < threadsCount; ++i)
	{
		long start = i * chunkSize;
		long end = (i == threadsCount - 1) ? n : (i + 1) * chunkSize;

		threads.push_back(make_unique<thread>([&m,start, end, h, &totalSum, i]() 
			{ 
				double tempSum = Calculate(start, end, h);
				m.lock();
				cout << "Thread {" << i << "} finished..." << endl;
				totalSum += tempSum;
				m.unlock();
			}));
	}

	for (int i = 0; i < threadsCount; ++i)
	{
		threads[i].get()->join();
	}

	//Waiting for results
	time(&endTime);
	auto elapsed = endTime - startTime;
	cout << "Calculation of Pi finished!" << "\nValue: " << totalSum*h << "\nElapsed time: " << elapsed << " sec" << endl;
    return 0;
}
