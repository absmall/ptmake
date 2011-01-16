#ifndef __GRAPHVIZ_H__
#define __GRAPHVIZ_H__

#include "rules.h"
#include <string>

class GraphViz
{
	public:
		void output(const std::string &filename);
};

#endif /* __GRAPHVIZ_H__ */
