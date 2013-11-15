#include <iostream>
#include <algorithm>

#include <libwpl/program.h>
#include <libwpl/module_loader.h>

using namespace std;

// Code for command line parsing
// from http://stackoverflow.com/questions/865668/parse-command-line-arguments
char *getCmdOption (char **begin, char **end, const std::string &option) {
	char ** itr = find(begin, end, option);
	if (itr != end && ++itr != end) {
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
	return find(begin, end, option) != end;
}

void usage (char *argv0) {
	cout << argv0 << " [-h] -f filename\n";
}

int main (int argc, char *argv[]) {
	if (cmdOptionExists(argv, argv+argc, "-h")) {
		usage (argv[0]);
		return 0;
	}

	char *filename = getCmdOption (argv, argv+argc, "-f");
	if (filename == NULL) {
		usage(argv[0]);
		return -1;
	}

	int ret = 1;

	try {
		wpl_module_loader mysql_loader(argc, argv, "mysql");
		wpl_program program(argc, argv);
		program.set_parent_namespace(mysql_loader.get_namespace());

		program.parse_file(filename);

		ret = program.run();
	}
	catch (exception &e) {
		throw;
	}

	return ret;
}
