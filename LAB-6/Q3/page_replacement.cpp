#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

vector<int> page_request_sequence;
int page_frame_size;

void process_fifo();
void process_lru();
void process_optimal();

int main()
{
    cout<<"Sequence Length: ";
    int n;
    cin>>n;
    cout<<"Enter Sequence: ";
    for(int i=0; i<n; i++) 
    {
        int x;
        cin>>x;
        page_request_sequence.push_back(x);
    }
    cout<<"Page Frame Size: ";
    cin>>page_frame_size;

    cout<< "FIFO - 1\n"
        << "OPTIMAL - 2\n"
        << "LRU - 3\n"
        << "Enter Page Replacement Strategy: ";
    
    int strategy;
    cin>>strategy;
    switch(strategy)
    {
        case 1:
            process_fifo();
            break;
        case 2:
            process_optimal();
            break;
        case 3:
            process_lru();
            break;
        default:
            cout<<"Invalid Strategy";
            return 0;
    }
    return 0;
}

void process_fifo()
{
    queue<int> page_frame;
    int page_faults = 0;
    for(int i=0; i<page_request_sequence.size(); i++)
    {
        int page = page_request_sequence[i];
        bool isFault = true;
        for(int i=0; i<page_frame.size(); i++)
        {
            if(page_frame.front() == page) isFault = false;
            page_frame.push(page_frame.front());
            page_frame.pop();
        }
        if(isFault)
        {
            if(page_frame.size() < page_frame_size)
            {
                page_frame.push(page_request_sequence[i]);
                page_faults++;
            }
            else
            {
                page_frame.pop();
                page_frame.push(page_request_sequence[i]);
                page_faults++;
            }
            for(int i=0; i<page_frame.size(); i++)
            {
                cout<<page_frame.front()<<" ";
                page_frame.push(page_frame.front());
                page_frame.pop();
            }
            cout<<endl;
        }
        else
        {
            cout<<"No page fault"<<endl;
        }
    }
    cout<<"Total number of page faults: "<<page_faults<<endl;
}

void process_optimal()
{
    queue<pair<int, int>> page_frame;
    int page_faults = 0;
    for(int i=0; i<page_request_sequence.size(); i++)
    {
        int page = page_request_sequence[i];
        bool isFault = true;
        for(int i=0; i<page_frame.size(); i++)
        {
            if(page_frame.front().first == page) isFault = false;
            page_frame.push(page_frame.front());
            page_frame.pop();
        }
        if(isFault)
        {
            if(page_frame.size() < page_frame_size)
            {
                int next_use = INT_MAX;
                for(int j=i+1; j<page_request_sequence.size(); j++)
                {
                    if(page_request_sequence[j] == page)
                    {
                        next_use = j;
                        break;
                    }
                }
                page_frame.push(make_pair(page_request_sequence[i], next_use));
                page_faults++;
            }
            else
            {
                int to_remove = 0;
                int max_use = -1;
                for(int j=0; j<page_frame.size(); j++)
                {
                    if(page_frame.front().second > max_use)
                    {
                        max_use = page_frame.front().second;
                        to_remove = j;
                    }
                    page_frame.push(page_frame.front());
                    page_frame.pop();
                }
                for(int j=0; j<to_remove; j++)
                {
                    page_frame.push(page_frame.front());
                    page_frame.pop();
                }
                page_frame.pop();
                int next_use = i+1;
                for(int j=i+1; j<page_request_sequence.size(); j++)
                {
                    if(page_request_sequence[j] == page)
                    {
                        next_use = j;
                        break;
                    }
                }
                page_frame.push(make_pair(page_request_sequence[i], next_use));
                page_faults++;
            }
            for(int i=0; i<page_frame.size(); i++)
            {
                cout<<page_frame.front().first<<" ";
                page_frame.push(page_frame.front());
                page_frame.pop();
            }
            cout<<endl;
        }
        else
        {
            //update_next_use
            int next_use = INT_MAX;
            for(int j=i+1; j<page_request_sequence.size(); j++)
            {
                if(page_request_sequence[j] == page)
                {
                    next_use = j;
                    break;
                }
            }
            for(int j=0; j<page_frame.size(); j++)
            {
                if(page_frame.front().first == page)
                {
                    page_frame.front().second = next_use;
                }
                page_frame.push(page_frame.front());
                page_frame.pop();
            }
            cout<<"No page fault"<<endl;
        }
    }
    cout<<"Total number of page faults: "<<page_faults<<endl;
}

void process_lru()
{
    queue<pair<int, int>> page_frame;
    int page_faults = 0;
    for(int i=0; i<page_request_sequence.size(); i++)
    {
        int page = page_request_sequence[i];
        bool isFault = true;
        for(int i=0; i<page_frame.size(); i++)
        {
            if(page_frame.front().first == page) isFault = false;
            page_frame.push(page_frame.front());
            page_frame.pop();
        }
        if(isFault)
        {
            if(page_frame.size() < page_frame_size)
            {
                page_frame.push(make_pair(page_request_sequence[i], i));
                page_faults++;
            }
            else
            {
                int to_remove = 0;
                int min_use = page_frame.front().second;
                for(int j=0; j<page_frame.size(); j++)
                {
                    if(page_frame.front().second < min_use)
                    {
                        min_use = page_frame.front().second;
                        to_remove = j;
                    }
                    page_frame.push(page_frame.front());
                    page_frame.pop();
                }
                for(int j=0; j<to_remove; j++)
                {
                    page_frame.push(page_frame.front());
                    page_frame.pop();
                }
                page_frame.pop();
                page_frame.push(make_pair(page_request_sequence[i], i));
                page_faults++;
            }
            for(int i=0; i<page_frame.size(); i++)
            {
                cout<<page_frame.front().first<<" ";
                page_frame.push(page_frame.front());
                page_frame.pop();
            }
            cout<<endl;
        }
        else
        {
            //update_last_use
            for(int j=0; j<page_frame.size(); j++)
            {
                if(page_frame.front().first == page)
                {
                    page_frame.front().second = i;
                }
                page_frame.push(page_frame.front());
                page_frame.pop();
            }
            cout<<"No page fault"<<endl;
        }
    }
    cout<<"Total number of page faults: "<<page_faults<<endl;
}