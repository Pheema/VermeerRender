#include "vermeerRender.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include "xorShift128.hpp"

int main()
{
	VermeerRender::VermeerRender vr;
	std::thread t(&VermeerRender::VermeerRender::Render, &vr);
	t.detach();
	
	const double kLimitTime = 60 * 5 - 1;
	const auto startTime = std::chrono::system_clock::now();
	for (int i = 0;;++i)
	{
		auto nowTime = std::chrono::system_clock::now();
		auto dt = std::chrono::duration_cast<std::chrono::seconds>(nowTime - startTime);
		if (dt.count() > kLimitTime) break;

		std::this_thread::sleep_for(std::chrono::seconds(30));

		std::stringstream filepath;
		filepath << std::setw(3) << std::setfill('0');
		filepath << "./rendered_" << i << ".png";
		vr.SaveImage(filepath.str().c_str());
	}
	vr.SaveImage("./rendered_exceeded.png");
	std::cout << "Duration: LimitTime" << std::endl;

	exit(0);
	return 0;
}