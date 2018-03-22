#include "pugixml.hpp"
#include <iostream>
#include <string.h>

int node_parse(pugi::xml_node root_node)
{
	pugi::xml_node node;
	pugi::xml_attribute attr;

	for (node = root_node.first_child(); node; node = node.next_sibling()) {
		if (node) {
			if (strcmp(node.child_value(), ""))
				std::cout << "[" << node.name() << "] -> {" << node.child_value() << "}" << std::endl;
			else if (strcmp(node.name(), ""))
				std::cout << "[" << node.name() << "]" << std::endl;

			for (attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
				std::cout << "\t attr [" << attr.name() << "] -> {" << attr.value() << "}" << std::endl;
			}
			node_parse(node);
		}
	}

	return 0;
}
int main(int argc, char **argv)
{
	pugi::xml_document doc;
	pugi::xml_node root_node, node;
	pugi::xml_attribute attr;

	if (argc < 2) {
		std::cout << "usage: " << argv[0] << " xml_file_name" << std::endl;
		goto final;
	}
	if (!doc.load_file(argv[1])) {
		std::cout << "load " << argv[1] << " failed" << std::endl;
		goto final;
	} else
		std::cout << "load " << argv[1] << " success" << std::endl;

	root_node = doc.child("MPD");
	std::cout << "root node : " << root_node.name() << std::endl;

	node_parse(root_node);
final:
	return 0;
}
