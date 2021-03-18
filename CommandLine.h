#pragma once

#include <string>
#include <exception>

/// Helper class to parse the command line arguments
class CommandLine {
public:
	inline CommandLine () :
		m_width(480),
		m_height(270),
		m_outputFilename("output.ppm"),
		m_dacrt(2) {}

	virtual ~CommandLine() {}

	inline size_t width() const { return m_width; }

	inline size_t height() const { return m_height; }

	inline const std::string& outputFilename() const { return m_outputFilename; }

    inline int dacrt() const { return m_dacrt; }

	void printUsage(const char* command) {
//		std::cerr << "USAGE: " << command << " [-w/-width <image width>][-h/-height <image height>][-o/-output <outputfilename>" << std::endl;
		std::cerr << "USAGE: " << command << " [-d/-dacrt d][-w/-width <image width>][-h/-height <image height>][-o/-output <outputfilename>]" << std::endl;
	}

	inline void parse(int argc, char** argv) {
		for (int i = 1; i < argc; i++) {
			std::string argi = argv[i];
			if (i == argc - 1) {
				if (argi == "-h" || argi == "-help") {
					printUsage(argv[0]);
					exit(0);
				}
				else {
					// throw std::exception("Missing argument");
				}
			}
			if (argi == "-w" || argi == "-width") {
				m_width = std::atoi(argv[++i]);
			}
			else if (argi == "-h" || argi == "-height") {
				m_height = std::atoi(argv[++i]);
			}
			else if (argi == "-o" || argi == "-output") {
				m_outputFilename = std::string(argv[++i]);
			}
			else if (argi == "-d" || argi == "-dacrt"){
			    std::cout << "Entered\n";
			    std::cout << argi << '\n';
			    if(std::atoi(argv[i+1]) != 0 && std::atoi(argv[i+1]) != 1){
			        printUsage(argv[0]);
                    exit(0);
                }
                m_dacrt = std::atoi(argv[++i]);
			}
			else {
				// throw std::exception(std::string("Unknown argument <" + std::string(argi) + ">").c_str());
			}
		}
	}

private:
	size_t m_width;
	size_t m_height;
	std::string m_outputFilename;
	int m_dacrt;
};
