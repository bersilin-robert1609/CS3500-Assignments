#include <iostream>
#include <vector>

using namespace std;

// Global Variables
int memorySize;
int noOfProcesses;
vector<int> blockSizes;
vector<int> processSizes;
vector<pair<int, int>> processAllocation;
vector<int> isFree;

// Function Prototypes
void take_memory_input();
void take_process_input();
void process_first_fit();
void process_best_fit();
void process_worst_fit();
void print_memory_allocation();
void reinitialize();
void memory_clear();

int main()
{
    take_memory_input();
    take_process_input();
    processAllocation.resize(noOfProcesses, make_pair(-1, -1));
    isFree.resize(memorySize, 1);

    process_first_fit();
    cout<<"First-Fit"<<endl;
    print_memory_allocation();
    reinitialize();
    process_best_fit();
    cout<<"Best-Fit"<<endl;
    print_memory_allocation();
    reinitialize();
    process_worst_fit();
    cout<<"Worst-Fit"<<endl;
    print_memory_allocation();
    memory_clear();
    return 0;
}

void take_memory_input()
{
    cout << "Enter Number of Blocks: ";
    cin >> memorySize;
    cout << "Enter "<< memorySize <<" block sizes: ";
    for (int i = 0; i < memorySize; i++)
    {
        int blockSize;
        cin >> blockSize;
        blockSizes.push_back(blockSize);
    }
    return;
}

void take_process_input()
{
    cout << "Enter Number of processes: ";
    cin >> noOfProcesses;
    cout << "Enter "<< noOfProcesses <<" Process Sizes: ";
    for (int i = 0; i < noOfProcesses; i++)
    {
        int processSize;
        cin >> processSize;
        processSizes.push_back(processSize);
    }
    return;
}

void reinitialize()
{
    for(int i = 0; i < memorySize; i++)
        isFree[i] = 1;
    for(int i = 0; i < noOfProcesses; i++)
        processAllocation[i] = make_pair(-1, -1);
    return;
}

void memory_clear()
{
    blockSizes.clear();
    processSizes.clear();
    processAllocation.clear();
    isFree.clear();
    return;
}

void print_memory_allocation()
{
    cout << "Process Number | Process Size | Block Size | Block Number" << endl;
    for (int i = 0; i < noOfProcesses; i++)
    {
        cout << i + 1 << "\t\t" << processSizes[i] << "\t\t";
        if (processAllocation[i].first != -1)
        {
            cout << processAllocation[i].second << "\t" << processAllocation[i].first + 1 << endl;
        }
        else
        {
            cout << "Not Allocated" << endl;
        }
    }
    return;
}

void process_first_fit()
{
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<memorySize; j++)
        {
            if(isFree[j] && blockSizes[j] >= processSizes[i])
            {
                processAllocation[i] = make_pair(j, blockSizes[j]);
                isFree[j] = 0;
                break;
            }
        }
    }
}

void process_best_fit()
{
    for(int i=0; i<noOfProcesses; i++)
    {
        int bestBlock = -1;
        for(int j=0; j<memorySize; j++)
        {
            if(isFree[j] && blockSizes[j] >= processSizes[i])
            {
                if(bestBlock == -1)
                    bestBlock = j;
                else if(blockSizes[bestBlock] > blockSizes[j])
                    bestBlock = j;
            }
        }
        if(bestBlock != -1)
        {
            processAllocation[i] = make_pair(bestBlock, blockSizes[bestBlock]);
            isFree[bestBlock] = 0;
        }
    }
}

void process_worst_fit()
{
    for(int i=0; i<noOfProcesses; i++)
    {
        int worstBlock = -1;
        for(int j=0; j<memorySize; j++)
        {
            if(isFree[j] && blockSizes[j] >= processSizes[i])
            {
                if(worstBlock == -1)
                    worstBlock = j;
                else if(blockSizes[worstBlock] < blockSizes[j])
                    worstBlock = j;
            }
        }
        if(worstBlock != -1)
        {
            processAllocation[i] = make_pair(worstBlock, blockSizes[worstBlock]);
            isFree[worstBlock] = 0;
        }
    }
}