#ifndef __PLOTTER_H__
#define __PLOTTER_H__

#include <fstream>
#include <string>

class Plotter
{
	public:
		Plotter();
		~Plotter();
		void open(const std::string &filename);
		void output(std::string target, std::string dep);
	private:
		bool active;
		std::ofstream file;
};

#endif /* __PLOTTER_H__ */
