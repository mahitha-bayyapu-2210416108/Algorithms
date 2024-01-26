#ifndef _MY_EXTERNAL_SORT_
#define _MY_EXTERNAL_SORT_
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <assert.h>
#include <string>
#include <fstream>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;
const int TOTAL_TAPES = 16;
const int BATCH_SIZE = 32;
static std::vector<size_t> fibo_series;
std::string OUTPUT_I_FILE= "intermediate_output.txt";
const std::string PATH_BASE = "temp/";
void newFileName(const string oldFileName,
const string newFileName)
{
    if (std::rename(oldFileName.c_str(), newFileName.c_str()) == 0)
    {
    }
    else
    {
        std::cout << oldFileName << " renamed to " << newFileName << " not successful" << std::endl;
    }
}
bool removeDir(const std::string& pathDir) {
    DIR* dir = opendir(pathDir.c_str());
    if (!dir) {
        return false;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
    std::string ename = entry->d_name;
    if (ename == "." || ename == ".." ||ename.compare(0, 4, ".nfs") == 0 ) {
        continue;
    }
    std::string ep = pathDir + "/" + ename;
    struct stat ei;
    if (stat(ep.c_str(), &ei) == -1) {
        closedir(dir);
        return false;
    }
    if (S_ISDIR(ei.st_mode)) {
        if (!removeDir(ep)) {
            closedir(dir);
            return false;
        }
    }
    }
    closedir(dir);
    if (rmdir(pathDir.c_str()) == -1) {
        return false;
    }
        return true;
}
/*------------------------------------------------------------------------------
Function: IsSorted
Checks if the content in input_file is sorted
Vairables:
input_file - the file name that you want to check if it's sorted
reverse - if set true, sort in descending order; otherwise in ascending
order
Note: you can use this to check if your result is correctly sorted.
------------------------------------------------------------------------------*/
bool IsSorted(const std::string &input_file, bool reverse = false)
{
    bool sorted = true;
    std::ifstream in(input_file.c_str());
    if (!in.is_open())
    {
        std::cout << input_file << " doesn't exist!\n";
        return false;
    }
    else
    {
        std::string buffer;
        int prev = INT_MIN, curr;
        if (reverse)
            prev = INT_MAX;
        while (!in.eof())
        {
            in >> curr;
            if ((curr < prev && !reverse) || (curr > prev && reverse))
            {
                sorted = false;
                std::cout << "Out of order: " << prev << ", " <<
                curr << std::endl;
                break;
            }
            prev = curr;
        }
    }
    in.close();
    return sorted;
}
/*------------------------------------------------------------------------------
Function: ComputeKthFibonacci
Compute K-th order Fibonacci number for computing number of blocks should store
in each tape.
Note: you may call this function as many times as needed.
------------------------------------------------------------------------------*/
void ComputeKthFibonacci(void)
{
/* formula:
F(k)(0, 1, ..., k-1) = 0, 0, ..., 1
F(k)(n) = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k), n >= k
when n == k:
F(k)(k) = F(k)(k − 1) + F(k)(k − 2) + ··· + F(k)(k − k)
= 1 + 0 + ... + 0 = 1
when n > k:
F(k)(n) = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k)
F(k)(n) = F(k)(n − 1) + [F(k)(n − 2) + ··· + F(k)(n − k) + F(k)(n −
k-1)] - F(k)(n − k-1)
F(k)(n − 1) = F(k)(n − 2) + F(k)(n − 3) + ··· + F(k)(n − k) + F(k)(n −
k-1)
F(k)(n) = 2*F(k)(n − 1) - F(k)(n − k-1)
*/
    int k = TOTAL_TAPES - 1;
    if (fibo_series.empty()) // initial fibo_series
    {
        for (int i = 0; i < k - 1; i++)
            fibo_series.push_back(0);
        fibo_series.push_back(1);
        fibo_series.push_back(1); // this is fibo_series[k]
    }
    for (int i = 0; i < 100; i++) // compute next 100 items for usage
    {
        int n = fibo_series.size();
        size_t new_item = 2 * fibo_series[n - 1] - fibo_series[n - k - 1];
        fibo_series.push_back(new_item);
    }
}
/*------------------------------------------------------------------------------
Function: GetBlocksAt
get the number of blocks you should write to at current pass.
Vairables:
pass - current pass of PolyPhaseMergePass.
blocks_at - where you want to store the block counts of eath tape to.
Note:
For the t-th input tape at pass n, it should hold blocks_at[t] blocks:
blocks_at[t] = F(k)(n+k-2) + F(k)(n+k-3) + ... + F(k)(n+t-2), t = 1, 2, ..., k
------------------------------------------------------------------------------*/
void GetBlocksAt(int pass, std::vector<int> &blocks_at)
{
    if (pass < 1)
        return;
    int k = TOTAL_TAPES - 1;
    blocks_at = std::vector<int>(k, 0);
    for (int t = 0; t < k; t++)
    {
        for (int tmp_i = pass + t - 1; tmp_i <= pass + k - 2; tmp_i++)
        {
            if (tmp_i >= fibo_series.size())
                ComputeKthFibonacci(); // compute more fibo_series
            blocks_at[t] += fibo_series[tmp_i];
        }
    }
}
/*------------------------------------------------------------------------------
The insertion sort algorithm.
Vairables:
a - the input array
left and right - the left and end indexes of the range of the elements to be
sorted, inclusive
reverse - if set true, sort in descending order. Default: false
(Hint: You can reuse your implementation from Lab 01)
------------------------------------------------------------------------------*/
template <typename Comparable>
void InsertionSort(std::vector<Comparable> &a, int left, int right, bool reverse =false)
{
    bool descending = reverse;
    for (int i = left + 1; i <= right; ++i)
    {
        Comparable c = a[i];
        int j = i;
        if (descending)
        {
            for(;j > left && a[j - 1] < c;--j){
                a[j] = a[j - 1];
            }
        }
        else
        {
            for(;j > left && a[j - 1] > c; --j){
                a[j] = a[j - 1];
            }
        }
        a[j] = c;
    }
}
/*------------------------------------------------------------------------------
Function: LoadSortInputBlocks
You should:
(1) Loads input data as blocks;
(2) sort each block internally;
(3) distribute blocks to the tapes for PolyPhaseMerge;
(4) return how many passes you need to perform the PolyPhaseMerge.
Vairables:
input_file - the file name that contains the inputs
ext_arrays - names of the files that serve as the external tapes
reverse - if set true, sort in descending order; otherwise in ascending
order
------------------------------------------------------------------------------*/
int LoadSortInputBlocks(
const std::string &input_file,
vector<string> &ext_arrays,
bool reverse = false)
{
    vector<ofstream>
    tape_os;
    for (int i = 0; i < ext_arrays.size(); i++)
    {
        ofstream tapei(ext_arrays[i]);
        tape_os.emplace_back(std::move(tapei));
    }
    std::ifstream input(input_file);
    if (!input.is_open())
    {
        std::cerr << "Error opening input file: " << input_file <<
        std::endl;
        return 0;
    }
    int pass = 0;
    std::vector<int> current_blocks_at(TOTAL_TAPES - 1, 0);
    int element_count = 0;
    vector<int> elements(TOTAL_TAPES, 0);
    while (true)
    {
        pass++;
        std::vector<std::ofstream> tapes;
        bool blocks_read = false;
        std::vector<int> blocks_at(TOTAL_TAPES - 1, 0);
        GetBlocksAt(pass, blocks_at);
        bool pass_complete = false;
        blocks_read = true;
        while (blocks_read)
        {
            blocks_read = false;
            for (int i = 0; i < TOTAL_TAPES - 1; i++)
            {
                if (current_blocks_at[i] < blocks_at[i])
                {
                    current_blocks_at[i]++;
                    std::vector<int> block_data;
                    for (int b = 0; b < BATCH_SIZE; b++)
                    {
                        int num;
                        blocks_read = true;
                        if (input >> num)
                        {
                            block_data.push_back(num);
                            element_count++;
                        }
                        else if (input.eof())
                        {
                        }
                    }
                    string input = "";
                    if (!block_data.empty())
                    {
                        InsertionSort(block_data, 0,
                        block_data.size() - 1, reverse);
                        for (int f = 0; f <block_data.size(); f++)
                        {
                            if (current_blocks_at[i] ==1 && elements[i] == 0)
                            {
                                elements[i]++;
                                input +=
                                to_string(block_data[f]);
                            }
                            else
                            {
                                input += "\n";
                                input +=
                                to_string(block_data[f]);
                            }
                        }
                    }
                    input += "\nb";
                    tape_os[i] << input;
                }
            }
        }
        if (input.eof())
        {
        break;
        }
    }
    for (int i = 0; i < ext_arrays.size(); i++)
    {
    tape_os[i].close();
    }
    return pass;
}
/*------------------------------------------------------------------------------
Function: PolyPhaseMerge
You should
(1) load data from k external tapes
(2) repeat (3) until you have merged all blocks in any one of the tapes:
(3) merge one block from each tape into one block, and write the merged block
into an external tape.
(4) clean up intermediate files.
Vairables:
cnt_pass - total number of passes you need to perform PolyPhaseMerges,
this is computed from LoadSortInputBlocks
ext_arrays - the names of the list of files that serve as the external arrays
reverse - if set true, sort in descending order; otherwise in ascending
order
------------------------------------------------------------------------------*/
void finalPass(bool &reverse, vector<ifstream> &tape_file_i_streams, string
&output_file)
{
ofstream tape_file_o_stream(output_file);
bool processed = false;
string value_str;
while (!processed)
{
std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,
int>>, std::function<bool(const std::pair<int, int> &, const std::pair<int, int>
&)>> priorityQueue(
[reverse](const std::pair<int, int> &a,
const std::pair<int, int> &b)
{
return reverse ? a.first < b.first : a.first > b.first;
});
processed = true;
for (int i = 0; i < TOTAL_TAPES; i++)
{
if (tape_file_i_streams[i].eof())
{
continue;
}
std::getline(tape_file_i_streams[i], value_str);
if(!(value_str == "b") && !(value_str == "\n" ||
value_str.empty()))
{
processed = false;
priorityQueue.push(make_pair(stoi(value_str), i));
}
}
if (processed)
break;
string output = "";
int counte = 0;
while (!priorityQueue.empty())
{
pair<int, int> node = priorityQueue.top();
priorityQueue.pop();
counte++;
int current = node.first;
int fileInd = node.second;
std::getline(tape_file_i_streams[fileInd], value_str);
if(!(value_str == "b") && !(value_str == "\n" ||
value_str.empty()))
{
priorityQueue.push(make_pair(stoi(value_str),
fileInd));
}
output += to_string(current);
output += "\n";
if (counte % BATCH_SIZE == 0)
{
tape_file_o_stream << output;
output = "";
}
}
if (processed)
break;
}
tape_file_o_stream.close();
}
void executePass(bool &reverse, int &output_tape, int &next_output_tape,
vector<ifstream> &tape_file_i_streams, int pass, int cnt_pass)
{
    string tape_file = PATH_BASE + "tape" + to_string(output_tape);
    string value_str;
    ofstream tape_file_o_stream(tape_file);
    bool processed = false;
    vector<int> counts(TOTAL_TAPES, 0);
    int counte = 0;
    int it = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
    std::function<bool(const std::pair<int, int> &, const std::pair<int, int> &)>>
    priorityQueue(
    [reverse](const std::pair<int, int> &a,
    const std::pair<int, int> &b)
    {
        return reverse ? a.first < b.first : a.first > b.first;
    });
    while (!processed)
    {
        it++;
        processed = true;
        int counter = (BATCH_SIZE / TOTAL_TAPES);
        int heapSize = 1 * (TOTAL_TAPES - 1);
        bool pass_completed = false;
        for (int i = 0; i < TOTAL_TAPES; i++)
        {
            if (i == output_tape)
            {
                continue;
            }
            std::getline(tape_file_i_streams[i], value_str);
            if (value_str == "b")
            {
                counts[i]++;
            }
            else if (value_str == "\n" || value_str.empty())
            {
            }
            else
            {
                processed = false;
                heapSize--;
                priorityQueue.push(make_pair(stoi(value_str), i));
            }
        }
        if (processed)
            break;
        string output = "";
        while (!priorityQueue.empty())
        {
            pair<int, int> node = priorityQueue.top();
            priorityQueue.pop();
            int current = node.first;
            int fileInd = node.second;
            if (counte == 0)
            {
                output += to_string(current);
            }
            else
            {
                output += "\n";
                output += to_string(current);
            }
            counte++;
            std::getline(tape_file_i_streams[fileInd], value_str);
            if (value_str == "b")
            {
                counts[fileInd]++;
            }
            else if (value_str == "\n" || value_str.empty())
            {
            }
            else
            {
            priorityQueue.push(make_pair(stoi(value_str),
            fileInd));
            }
            if (counte % BATCH_SIZE == 0)
            {
                tape_file_o_stream << output;
                output = "";
            }
        }
        output += "\nb";
        tape_file_o_stream << output;
        if (tape_file_i_streams[next_output_tape].eof())
        {
        processed = true;
        }
    }
    tape_file_o_stream.close();
}
void PolyPhaseMerge(const int cnt_pass, std::vector<std::string> &ext_arrays, bool reverse)
{
    int o2 = TOTAL_TAPES - 1;
    vector<ifstream>
    v1;
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
        ifstream tape_file_i_stream(ext_arrays[i]);
        v1.push_back(std::move(tape_file_i_stream));
    }
    for (int pass = 1; pass <= cnt_pass; pass++)
    {
        int o1 = (TOTAL_TAPES + o2 - 1) % TOTAL_TAPES;
        v1[o2].close();
        executePass(reverse, o2, o1, v1, pass, cnt_pass);
        ifstream x1(ext_arrays[o2]);
        v1[o2] = std::move(x1);
        string empty_file_name = PATH_BASE + "empty";
        ofstream emptyStream(empty_file_name);
        emptyStream.close();
        v1[o1].close();
        newFileName(empty_file_name.c_str(), ext_arrays[o1].c_str());
        ifstream tape_file_i_stream0(ext_arrays[o1]);
        v1[o1] = std::move(tape_file_i_stream0);
        o2 = o1;
    }
    finalPass(reverse, v1, OUTPUT_I_FILE);
}
/*------------------------------------------------------------------------------
The driver external sort function function
input_file - the file name that contains the inputs
output_file - the file name that contains the final sorted elements
reverse - if set true, sort in descending order; otherwise in ascending
order
------------------------------------------------------------------------------*/
void ExternalSort(const std::string &input_file,
const std::string &output_file, bool reverse = false)
{
    removeDir(PATH_BASE);
    mkdir(PATH_BASE.c_str(), 0777);
    vector<string> ext_arrays;
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
        string tape_file = PATH_BASE + "tape" + to_string(i);
        ext_arrays.push_back(tape_file);
    }
    int cnt_pass = LoadSortInputBlocks(input_file, ext_arrays, reverse);
    PolyPhaseMerge(cnt_pass, ext_arrays, reverse);
    newFileName(OUTPUT_I_FILE, output_file);
    removeDir(PATH_BASE);
    cout << endl;
}
#endif