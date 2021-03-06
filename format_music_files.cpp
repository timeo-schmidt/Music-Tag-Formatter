/*
  Goal of program is to convert untagged MP3s such as below
    "Max Manie - Dance (Original Mix) my-free-mp3s.com"
  and format filename, as well as adding the Artist and Title Tags to the file

  The program takes a root path for the operation to be executed in as a command line argument
*/


#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <cstdlib>

using namespace std;
namespace fs = experimental::filesystem;

// Program takes in root path for operation as command line argument
int main(int argc, char** argv)
{

    struct file {
        string file_name;
        string full_path;
    };

    vector<file> all_files;


    // Iterating through directory recursively and adding all the file paths
    for(auto& p: fs::recursive_directory_iterator(argv[1]))
    {
        string file_name_substr, file_path_substr;
        string current_element = p.path().string();
        file_name_substr = current_element.substr(current_element.find_last_of("/")+1);

        int ending_sequence_pos = file_name_substr.find("my-free-mp3s.com");
        if(ending_sequence_pos  != string::npos)
        {
          // cout << "file_name_substr: " << file_name_substr.substr(0, ending_sequence_pos) << endl;
          all_files.push_back({file_name_substr, p.path().string()});
        }
    }


    for(auto const& f: all_files)
    {

        // Re-Format File, if file contains "my-free-mp3s.com".
        string artist_name, track_title;
        int separator_sequence_pos = f.file_name.find(" - ");
        int ending_sequence_pos1 = f.file_name.find("my-free-mp3s.com");

        artist_name = f.file_name.substr(0, separator_sequence_pos);
        track_title = f.file_name.substr(separator_sequence_pos+3, ending_sequence_pos1-(separator_sequence_pos+3));

        // // Execute id3tool to modify MP3s meta data
        string cmd_tag_edit = "id3tool --set-title=\"" + track_title + "\" --set-artist=\"" + artist_name + "\" " + "\"" + f.full_path + "\"";
        system(cmd_tag_edit.c_str());

        // // Change File Name. Find Occurence of File Name in Path. Change
        int file_name_in_path_pos = f.full_path.find(f.file_name);
        string new_file_name = f.file_name.substr(0, ending_sequence_pos1) + ".mp3";
        // Renaming file
        string cmd_rename_file = "mv \"" + f.full_path + "\" \"" + f.full_path.substr(0,file_name_in_path_pos) + new_file_name + "\"";
        system(cmd_rename_file.c_str());

        cout << "Modified Track: " + new_file_name << endl;
    }

    cout << endl << endl << "Successfully Modified " + to_string(all_files.size()) + " tracks." << endl;
    return 0;

}

/*

#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
    fs::create_directories("sandbox/a/b");
    std::ofstream("sandbox/file1.txt");
    fs::create_symlink("a", "sandbox/syma");
    for(auto& p: fs::recursive_directory_iterator("sandbox"))
        std::cout << p.path() << '\n';
    fs::remove_all("sandbox");
}

*/
