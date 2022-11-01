#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <ctime>

using namespace std;

int extraSize = 16 + 4 + 4 + 12 + 128 + 28 + 28;
int DiskFileSize;
int DiskBlockSize;
int blockCount;
stack<int> freeBlocks;

void printCommands()
{
    cout << endl;
    cout << "The available commands are:" << endl;
    cout << "1. load <file_path> - loads the file from the disk" << endl;
    cout << "This command creates a new file in the given path and asks for the file size and content." << endl;
    cout << "2. delete <file_path> - deletes the file from the disk" << endl;
    cout << "3. print <file_path> - prints the file details" << endl;
    cout << "4. append <file_path> - appends the file with new content" << endl;
    cout << "5. display - displays the compact details of all the files" << endl;
    cout << "6. freespace - displays the free space available on the disk" << endl;
    cout << "7. printinode <file_path> - prints the inode of the file" << endl;
    cout << "8. exit - exits the program" << endl;
}

string get_date()
{
    time_t now = time(0);
    string dt = ctime(&now);
    dt = dt.substr(0, dt.length() - 1);
    return dt;
}

class File
{
public:
    string filePath;                    // 16 bytes
    int fileSize;                       // 4 bytes - hold file content size
    int totalFileSize;                  // 4 bytes
    vector<int> directBlockAddresses;   // 12 bytes
    vector<int> indexBlockContents;     // max of 32 * 4 bytes = 128 bytes             
    string fileContent;                 
    string dateCreated;                 // 28 bytes
    string lastModified;                // 28 bytes

    File()
    {
        this->filePath = "";
        this->fileSize = 0;
        this->totalFileSize = 0;
        this->fileContent = "";
        this->dateCreated = lastModified = get_date();
    }

    File(string filePath, string fileContent)
    {
        this->filePath = filePath;
        this->fileContent = fileContent;
        this->dateCreated = this->lastModified = get_date();
        this->fileSize = fileContent.size();
        this->totalFileSize = this->fileSize + extraSize;
    }
    int allocateBlocks()
    {
        if(this->totalFileSize > DiskBlockSize * 35)
        {
            cout << "File size is too large" << endl;
            return -1;
        }
        int blocksNeeded = this->totalFileSize / DiskBlockSize;
        if (this->totalFileSize % DiskBlockSize != 0) blocksNeeded++;

        if (blocksNeeded > freeBlocks.size())
        {
            cout << "Not enough space on disk" << endl;
            return -1;
        }
        for (int i = 0; i < blocksNeeded; i++)
        {
            if(i<3) directBlockAddresses.push_back(freeBlocks.top());
            else indexBlockContents.push_back(freeBlocks.top());
            freeBlocks.pop();
        }
        return 0;
    }
    void clearBlocks()
    {
        for (int i = 0; i < directBlockAddresses.size(); i++) 
            freeBlocks.push(directBlockAddresses[i]);
        for (int i = 0; i < indexBlockContents.size(); i++) 
            freeBlocks.push(indexBlockContents[i]);
        return;
    }
    void printFileDetails()
    {
        cout << "******************************" << endl;
        cout << "Filename: " << filePath << endl;
        cout << "File Size: " << fileSize << " bytes" << endl;
        cout << "Total File Size: " << totalFileSize << " bytes" << endl;
        cout << "Direct Block Addresses: ";
        for (int i = 0; i < directBlockAddresses.size(); i++) 
            cout << directBlockAddresses[i] << " ";
        cout << endl;
        cout << "Index Block Contents: ";
        for (int i = 0; i < indexBlockContents.size(); i++) 
            cout << indexBlockContents[i] << " ";
        cout << endl;
        cout << "Date Created: " << dateCreated << endl;
        cout << "Last Modified: " << lastModified << endl;
        cout << "******************************" << endl;
    }
    int appendContents(string newContent)
    {
        int newSize = newContent.size();

        if(newSize + this->totalFileSize > DiskBlockSize * 35)
        {
            cout << "File space limit exceeded" << endl;
            return -1;
        }

        this->fileContent += newContent;
        this->fileSize += newSize;
        this->lastModified = get_date();

        int leftOverSpaceInit = DiskBlockSize - (this->totalFileSize % DiskBlockSize);
        this->totalFileSize += newSize;

        int currentBlockCount = directBlockAddresses.size() + indexBlockContents.size();
        int newSpaceRequired = newSize - leftOverSpaceInit;
        int newBlocksNeeded = newSpaceRequired / DiskBlockSize;
        if (newSpaceRequired % DiskBlockSize != 0) newBlocksNeeded++;

        if (newBlocksNeeded > freeBlocks.size())
        {
            cout << "Not enough space on disk" << endl;
            return -1;
        }
        if (newBlocksNeeded > 0)
        {
            for (int i = 0; i < newBlocksNeeded; i++)
            {
                if (currentBlockCount < 3) directBlockAddresses.push_back(freeBlocks.top());
                else indexBlockContents.push_back(freeBlocks.top());
                freeBlocks.pop();
                currentBlockCount++;
            }
        }
        return 0;
    }
};

map<string, File> files;

void getInputs();
int getNewFile(string filePath);

int main()
{
    getInputs();

    blockCount = DiskFileSize * 1024 / DiskBlockSize;
    for (int i = blockCount - 1; i >= 0; i--) freeBlocks.push(i);

    cout<<"Total blocks: "<<blockCount<<endl;

    printCommands();

    while(true)
    {
        cout << endl;
        cout << "Enter command: ";
        string command;
        cin >> command;
        if (command == "load")
        {
            string filePath;
            cin >> filePath;
            if(getNewFile(filePath) == -1)
            {
                cout << "File creation failed" << endl;
                continue;
            }
        }
        else if (command == "delete")
        {
            string filePath;
            cin >> filePath;
            if (files.find(filePath) == files.end())
            {
                cout << "File not found" << endl;
                continue;
            }
            files[filePath].clearBlocks();
            files.erase(filePath);
        }
        else if (command == "print")
        {
            string filePath;
            cin >> filePath;
            if (files.find(filePath) == files.end())
            {
                cout << "File not found" << endl;
                continue;
            }
            cout << files[filePath].fileContent << endl;
        }
        else if (command == "append")
        {
            string filePath;
            cin >> filePath;
            if (files.find(filePath) == files.end())
            {
                cout << "File not found" << endl;
                continue;
            }
            cout << "Enter new content: ";
            string newContent;
            cin.ignore();
            getline(cin, newContent);
            files[filePath].appendContents(newContent);
        }
        else if (command == "display")
        {
            cout<<"File Path\tFile Size\t\tLast Modified"<<endl;
            for (auto it = files.begin(); it != files.end(); it++)
            {
                cout << it->first << "\t\t" << it->second.fileSize << "\t\t" << it->second.lastModified << endl;
            }
        }
        else if (command == "freespace")
        {
            cout << "Free space: " << freeBlocks.size() * DiskBlockSize << " bytes" << endl;
        }
        else if (command == "printinode")
        {
            string filePath;
            cin >> filePath;
            if (files.find(filePath) == files.end())
            {
                cout << "File not found" << endl;
                continue;
            }
            files[filePath].printFileDetails();
        }
        else if (command == "exit")
        {
            break;
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }
}

void getInputs()
{
    cout << "Enter the size of the disk file (in kilobytes): ";
    cin >> DiskFileSize;
    cout << "Enter the size of the disk block (in bytes): ";
    cin >> DiskBlockSize;
    if (DiskBlockSize <= 0)
    {
        cout << "Invalid input. Please enter a positive integer." << endl;
        cout << "Enter the size of the disk block (in bytes): ";
        cin >> DiskBlockSize;
    }
    if (DiskFileSize <= 0)
    {
        cout << "Invalid input. Please enter a positive integer." << endl;
        cout << "Enter the size of the disk file (in kilobytes): ";
        cin >> DiskFileSize;
    }
}

int getNewFile(string filePath)
{
    string fileContent;
    cout << "Enter the file content: ";
    cin.ignore();
    getline(cin, fileContent);
    File newFile(filePath, fileContent);
    if (newFile.allocateBlocks() == -1)
    {
        cout << "File creation failed due to to lack of free blocks" << endl;
        return -1;
    }
    files[filePath] = newFile;
    return 0;
}