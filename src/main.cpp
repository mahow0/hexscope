#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <getopt.h>
#include <fstream>


std::string dump_hex(char* input, int COLS, int GROUP_SIZE, bool include_lines) {

    char* seek = input;

    std::stringstream output;
    int group = 0;
    int col = 0;
    int line = 0;

    if (include_lines)
        output << std::hex << std::setfill('0') << std::setw(8) << line << ": ";
    
    while (*seek != '\0') {

        if (group == GROUP_SIZE && include_lines) {
            output << " ";
            group = 0;
            col += 1;
        }

        if (col == COLS && include_lines) {
            line += COLS * GROUP_SIZE;
            output << std::endl;
            output << std::hex << std::setfill('0') << std::setw(8) << line << ": ";
        }

        const char character = *seek;
        output << std::hex << std::setfill('0') << std::setw(2) << (int) character;
        group += 1;
        
        

        seek++;
    }

    return output.str();

}


int main(int argc, char *argv[])
{   


    // TODO: interaction with pipe
    if (argc <= 1) {
        std::cout << "Usage:" << std::endl 
        << "\thexsc [options] infile [outfile]" << std::endl 
        << "Options:" << std::endl
        << "\t-c\tnumber of octet columns per line. default is 16. must be >0." << std::endl
        << "\t-g\tnumber of octets per group. default is 2. must be >0" << std::endl
        << "\t-p\toutput in postscript plain style.";
        
        return 0;
    }
    
    int COLS = 16;
    int GROUP_SIZE = 2;
    bool include_lines = true;

    //read arguments from command line
    while (true) {

        int arg = getopt(argc, argv, "c:g:p");

        if (arg == -1) {
            break;
        }

        switch (arg) {

            case 'c':
                COLS = atoi(optarg);
                if (COLS <= 0) {
                    std::cout << "-c must be followed with a positive integer" << std::endl;
                    return -1;
                }
                break;
            case 'g':
                GROUP_SIZE = atoi(optarg);
                if (GROUP_SIZE <= 0) {
                    std::cout << "-g must be followed with a positive integer" << std::endl;
                    return -1;
                }
                break;
            case 'p':
                include_lines = false;
                break;
            case '?':
                std::cout << "Unknown option or no argument provided for option that required it" << std::endl;
                return -1;
            
        }

    }

    //read input file and output file (if they are provided)
    char* infile_path = nullptr;
    char* outfile_path = nullptr;
    if (optind >= argc) {
        std::cout << "No input file provided" << std::endl;
        return -1;
    }

    while (optind < argc) {

        if (!infile_path) {
            infile_path = argv[optind];
        }
        else if (!outfile_path) {
            outfile_path = argv[optind];
        }
        else {
            std::cout << "Too many file arguments provided" << std::endl;
            return -1;
        }

        optind++;
    }

    std::ifstream infile(infile_path, std::ios::binary | std::ios::ate);

    if (!infile) {
        std::cout << "File " << infile_path << "does not exist" << std::endl;
        return -1;
    } 

    std::streampos size = infile.tellg();
    char* input = new char[size];
    infile.seekg(0, std::ios::beg);
    infile.read(input, size);
    infile.close();

    
    std::string hex_dump = dump_hex(input, COLS,  GROUP_SIZE, include_lines);
    delete[] input;

    if (!outfile_path) {
        std::cout << hex_dump << std::endl;
    }
    else {
        std::ofstream outfile(outfile_path, std::ios::binary);
        if (!outfile) {
            std::cout << "Error opening outfile " << outfile_path << std::endl;
            return -1;
        }

        outfile << hex_dump;
        outfile.close();
    }

    return 0;
}
