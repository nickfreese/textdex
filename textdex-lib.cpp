#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>
namespace fs = std::filesystem;

std::string version = "0.1";

std::string default_tag_seperator = ":";

std::vector<std::string> search_results;
std::vector<std::string> tag_search_results;

class AdvSearchResults
{         
  public: 
    std::vector<std::string> search_results;
    std::vector<std::string> keyval_search_results;
};

std::vector<std::string>
getFiles(std::string path)
{

    auto path2 = std::filesystem::canonical(path);

    std::vector<std::string> files;

    for (const fs::directory_entry& dir_entry :
         fs::recursive_directory_iterator(path2)) {

        if (dir_entry.is_directory()) {

        } else if (dir_entry.is_regular_file()) {
            files.push_back(dir_entry.path().string());
        }
    }

    return files;
}

std::string
hasSearchString(std::string field, std::string line)
{

    std::string val;

    //std::regex pattern(".*a.*", std::regex_constants::icase);

    std::regex target("^.*" + field + ".*$", std::regex_constants::icase);
    std::smatch match;
    std::regex_search(line, match, target);

    return match[0];
}

std::string
hasKeyValueMatch(std::string field,
                 std::string val,
                 std::string seperator,
                 std::string line)
{

    std::regex target("^.*" + field + "(\\|| " + seperator + "| \\|?|" +
                      seperator + "?)( *)" + val + ".*$", std::regex_constants::icase);
    std::smatch match;
    std::regex_search(line, match, target);

    return match[0];
}

AdvSearchResults
adv_search_file(std::string filepath,
                std::string search_string,
                std::vector<std::string> key_val,
                std::string seperator)
{

    AdvSearchResults adv_results;
    std::vector<std::string> search_matches;
    std::vector<std::string> keyval_matches;

    std::string data;
    std::fstream data_file;
    data_file.open(filepath, std::ios::in);

    if (data_file.is_open()) {

        bool search_match = false;
        bool keyval_match = false;
        std::string data;
        while (std::getline(data_file, data)) {

            std::string search_result;
            std::string search_result2;

            if (search_string != "") {

                search_result = hasSearchString(search_string, data);
                // found a match
            }

            if (!key_val.empty()) {
                search_result2 =
                  hasKeyValueMatch(key_val[0], key_val[1], seperator, data);
            }

            if (!search_result.empty()) {
                search_match = true;
            }

            if (!search_result2.empty()) {
                keyval_match = true;
            }
        }

        if (search_match) {
            search_matches.push_back(filepath);
            adv_results.search_results = search_matches;
        }
        if (keyval_match) {
            keyval_matches.push_back(filepath);
            adv_results.keyval_search_results = keyval_matches;
        }
        data_file.close();
    }
    return adv_results;
}

/*
 *
 */
int
main(int argc, char** argv)
{

    // std::cout << "Hello World!\n";

    // argument defaults
    std::filesystem::path folder = "./";
    bool hasFolder = false;
    bool help = false;
    std::string search = "";
    bool hasSearch = false;

    std::vector<std::string> keyval;
    std::string keyvalString;
    bool hasKeyval = false;

    bool hasDelimiterOverride = false;

    std::string curArg = "";
    std::string delimiter = "=";

    // parse args
    for (int i = 1; i < argc; ++i) {
        curArg = argv[i];
        std::string token = curArg.substr(0, curArg.find(delimiter));
        if (token == "folder") {
            folder = curArg.substr(curArg.find(delimiter) + 1);
            hasFolder = true;
        }
        if (token == "help") {
            help = true;
        }
        if (token == "s") {
            search = curArg.substr(curArg.find(delimiter) + 1);
            hasSearch = true;
        }
        if (token == "keyval" || token == "k") {
            keyvalString = curArg.substr(curArg.find(delimiter) + 1);

            std::string key = keyvalString.substr(0, keyvalString.find(":"));
            std::string val = keyvalString.substr(keyvalString.find(":") + 1);

            keyval.push_back(key);
            keyval.push_back(val);

            hasKeyval = true;
        }
        // default delimiter for keyvalue pairings
        if (token == "d") {
            default_tag_seperator = curArg.substr(curArg.find(delimiter) + 1);
            hasDelimiterOverride = true;
        }
    }

    // std::cout << "Folder: " << folder << "\n";

    if (help || argc == 1) {
        std::cout << "textdex, version: " << version << "\n";
        std::cout
          << "_____________________________________________________________\n";
        std::cout << "Arguments\n";
        std::cout
          << "_____________________________________________________________\n";
        std::cout << "  folder = <filepath>\n";
        std::cout << "  s      = <search_string>\n";
        std::cout << "  keyval = <key>:<value>          - comma seperated "
                     "list of key value pairs\n";
        std::cout << "  d      = <delimeter>            - a character used for seperating key value pairs. It can still have spaces on either side\n";
        std::cout << "  help                            - if present outputs the help menu\n";
        std::cout
          << "_____________________________________________________________\n";

        return 0;
    }

    // do search and keyval checking

    AdvSearchResults total_adv_results;

    // std::cout << "getting files\n";
    std::vector<std::string> files = getFiles(folder);

    // std::cout << "its this long: " << files.size() << " \n";

    for (int i = 0; i < files.size(); ++i) {
        // std::cout << "vector works! " << files[i] << "\n";

        // open and do searches
        AdvSearchResults adv_results;
        if (hasSearch && hasKeyval) {
            adv_results =
              adv_search_file(files[i], search, keyval, default_tag_seperator);
        } else if (hasSearch && !hasKeyval) {
            adv_results =
              adv_search_file(files[i], search, keyval, default_tag_seperator);
        } else if (!hasSearch && hasKeyval) {
            adv_results =
              adv_search_file(files[i], "", keyval, default_tag_seperator);
        }

        for (int e = 0; e < adv_results.search_results.size(); ++e) {
            // std::cout << adv_results.search_results[e] << "\n";
            total_adv_results.search_results.push_back(
              adv_results.search_results[e]);
        }
        for (int e = 0; e < adv_results.keyval_search_results.size(); ++e) {
            // std::cout << "search key value pair matches: " <<
            // adv_results.keyval_search_results[e] << "\n";
            total_adv_results.keyval_search_results.push_back(
              adv_results.keyval_search_results[e]);
        }
    }
    
    std::cout << "___\n";
    std::cout << "Search matches found: "
              << total_adv_results.search_results.size() << "\n";

    for (int e = 0; e < total_adv_results.search_results.size(); ++e) {
        std::cout << total_adv_results.search_results[e] << "\n";
    }
    std::cout << "\n";
    std::cout << "Key Value pair matches found: "
              << total_adv_results.keyval_search_results.size() << "\n";

    for (int e = 0; e < total_adv_results.keyval_search_results.size(); ++e) {
        std::cout << total_adv_results.keyval_search_results[e] << "\n";
    }
    
    std::cout << "___\n";



    return 0;
}