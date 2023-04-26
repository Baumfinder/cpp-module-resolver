#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set>

#include <filesystem>

std::vector<std::string> split_string(std::string input, char delim) {
    std::vector<std::string> result;
    std::string curr_substr;

    for (char c: input) {
        if (c == delim) {
            result.push_back(curr_substr);
            curr_substr = "";
            continue;
        }

        curr_substr = curr_substr + c;
    }
    result.push_back(curr_substr);
    return result;
}

template <typename T>
bool contains(std::vector<T> vec, T & elem) {
    if( find(vec.begin(), vec.end(), elem) != vec.end() ) {
        return true;
    }
    return false;
}

std::string file_to_string(std::string filename) {
    std::ifstream infile(filename);
    std::string result;
    std::string tmp;

    while(std::getline(infile, tmp)) {
        result += tmp;
        result += "\n";
    }

    infile.close();
    return result;
}

struct DepObject {
    std::string name;
    std::string filename;
    std::vector<DepObject> deps;

    DepObject(std::string name)
    : name(name) {}

    bool operator<(const DepObject& rhs) const { return name < rhs.name; }
};

DepObject parse_file(std::string input) {
    DepObject result("");

    for(std::string line: split_string(input, '\n')) {
        if (line == "")
            continue;

        line.pop_back(); // remove last character (";")
        std::vector<std::string> line_list = split_string(line, ' ');

        if (line_list[0] == "export" && line_list[1] == "module") {
            result.name = line_list[2];
            continue;
        }
        if (line_list[0] == "import") {
            result.deps.push_back(line_list[1]);
            continue;
        }
    }

    return result;
}

std::vector<DepObject> solve_list(std::set<DepObject> input) {
    std::vector<DepObject> result;
    std::set<DepObject> used;

    for (auto d: input) {
        if (d.deps.size() == 0) {
            result.push_back(d);
            used.insert(d);
            continue;
        }
    }

    while (used.size() != input.size()) {
        for (DepObject dobj: input) {
            if (used.contains(dobj))
                continue;

            int ctr = 0;

            for (DepObject dep: dobj.deps) {

                if (! input.contains(dep)) {
                    std::cout << "\nUnresolvable!\n";
                    exit(1);
                }

                if (used.contains(dep)) {
                    ctr++;
                }
            }

            if (ctr == dobj.deps.size()) {
                result.push_back(dobj);
                used.insert(dobj);
            }
        }
    }

    return result;
}

int main(int argc, char** argv) {
    const std::string inp_path = "./src/";

    std::set<DepObject> all_files;

    for (const auto & entry : std::filesystem::recursive_directory_iterator(inp_path)) {
        if (entry.is_directory() || entry.path().extension() != ".cpp")
            continue;

        DepObject n_object = parse_file(file_to_string(entry.path()));
        n_object.filename = entry.path();
        all_files.insert(n_object);
    }


    std::vector sorted_files = solve_list(all_files);

    for (auto entry: sorted_files) {
        std::cout << entry.filename << " ";
    }
    std::cout << "\n";
}