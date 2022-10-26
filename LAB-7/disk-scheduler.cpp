#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include <fstream>

using namespace std;

#define CYLINDER_COUNT 5000
#define REQUEST_COUNT 1000
#define DEFAULT_HEAD 2000
#define lsint long unsigned int

/*
 * Algorithm Code
 * FCFS - 1
 * SSTF - 2
 * SCAN -3
 * C-SCAN - 4
 * LOOK - 5
 * C-LOOK - 6
 */

int head_pos = DEFAULT_HEAD;
int algorithm = 1;
vector<int> requests;
vector<int> service_order;
int head_movement;
bool debug = false;
bool justAvgValue = false;

void manage_inputs(int, char**);
void generate_requests(int);
void scheduler_run();
void display_outputs();
void set_sstf_service_order();
void set_scan_service_order();
void set_cscan_service_order();
void set_look_service_order();
void set_clook_service_order();

int main(int argc, char** argv)
{
    srand(time(NULL));
    manage_inputs(argc, argv);
    generate_requests(REQUEST_COUNT);
    head_movement = 0;
    scheduler_run();
    display_outputs();
}

void manage_inputs(int argc, char** argv)
{
    // -h argument for head 
    // -a for algorithm
    // -d for debug

    // error case
    if(argc != 1 && argc != 3 && argc != 5)
    {   
        cerr << "Invalid number of arguments" << endl;
        cout << endl;
        cout << "Usage: ./disk-scheduler -h <head position> -a <algorithm>" << endl;
        cout << endl;
        cout << "Optional Arguments:" << endl;
        cout << "Usage: ./disk-scheduler <arguments>" << endl;
        cout << endl;
        cout << "Arguments: " << endl;
        cout << "-h for <head position>" << endl;
        cout << "Limits for head position: 0 - 4999" << endl;
        cout << endl;
        cout << "-a for <algorithm>" << endl;
        cout << "Algorithm: " << endl;
        cout << "1 - FCFS" << endl;
        cout << "2 - SSTF" << endl;
        cout << "3 - SCAN" << endl;
        cout << "4 - C-SCAN" << endl;
        cout << "5 - LOOK" << endl;
        cout << "6 - C-LOOK" << endl;
        cout << endl;
        cout << "Default head position: " << DEFAULT_HEAD << endl;
        cout << "Default algorithm: FCFS" << endl;
        exit(1);
    }

    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-h") == 0)
        {
            head_pos = atoi(argv[i+1]);
            if(head_pos < 0 || head_pos > CYLINDER_COUNT)
            {
                cerr << "Invalid head position" << endl;
                exit(1);
            }
            i++;
        }
        else if(strcmp(argv[i], "-a") == 0)
        {
            algorithm = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            debug = true;
        }
    }
    return;
}

void generate_requests(int request_count)
{
    requests.resize(request_count);
    for(int i=0; i<request_count; i++)
    {
        requests[i] = rand() % CYLINDER_COUNT;
    }
    return;
}

void scheduler_run()
{
    if (!justAvgValue) cout << "Given Head Position: " << head_pos << endl;
    switch(algorithm)
    {
        case 1:
        {
            // FCFS Algorithm
            if(!justAvgValue) cout << "Algorithm: FCFS" << endl;
            service_order.resize(REQUEST_COUNT);
            for(int i=0; i<REQUEST_COUNT; i++)
            {
                head_movement += abs(requests[i] - head_pos);
                head_pos = requests[i];
                service_order[i] = requests[i];
            }
            break;
        }
        case 2:
        {
            // SSTF Algorithm
            if(!justAvgValue) cout << "Algorithm: SSTF" << endl;
            service_order.resize(REQUEST_COUNT);
            set_sstf_service_order();
            break;
        }
        case 3:
        {
            // SCAN Algorithm
            if(!justAvgValue) cout << "Algorithm: SCAN" << endl;
            set_scan_service_order();
            break;
        }
        case 4:
        {
            // C-SCAN Algorithm
            if(!justAvgValue) cout << "Algorithm: C-SCAN" << endl;
            set_cscan_service_order();
            break;
        }
        case 5:
        {
            // LOOK Algorithm
            if(!justAvgValue) cout << "Algorithm: LOOK" << endl;
            set_look_service_order();
            break;
        }
        case 6:
        {
            // C-LOOK Algorithm
            if(!justAvgValue) cout << "Algorithm: C-LOOK" << endl;
            set_clook_service_order();
            break;
        }
        default:
        {
            cerr<<"NOT A VALID ALGORITHM"<<endl;
            exit(0);
        }
    }
}

void set_sstf_service_order()
{
    vector<int> requests_backup;
    for(int i=0; i<REQUEST_COUNT; i++) requests_backup.push_back(requests[i]);

    for(int i=0; i<REQUEST_COUNT; i++)
    {
        int min_index = -1;
        int min_diff = INT_MAX;
        for(int j = 0; j<REQUEST_COUNT; j++)
        {
            if(min_diff > abs(requests[j] - head_pos))
            {
                min_diff = abs(requests[j] - head_pos);
                min_index = j;
            }
        }
        service_order[i] = requests[min_index];
        head_movement += abs(service_order[i] - head_pos);
        head_pos = service_order[i];
        requests[min_index] = 10 * CYLINDER_COUNT;
    }

    for(int i=0; i<REQUEST_COUNT; i++) requests[i] = requests_backup[i];
    requests_backup.clear();
    return;
}

void set_scan_service_order()
{
    vector<int> lt_head;
    vector<int> mt_head;

    for(lsint i=0; i<requests.size(); i++)
    {
        if(requests[i] < head_pos) lt_head.push_back(requests[i]);
        else mt_head.push_back(requests[i]);
    }

    sort(mt_head.begin(), mt_head.end());
    sort(lt_head.rbegin(), lt_head.rend());

    int newCount = 0;

    if(lt_head.size() == 0)
    {
        for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[0])
        {
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            if(service_order[service_order.size() - 1] != CYLINDER_COUNT-1)
            {
                service_order.push_back(CYLINDER_COUNT - 1);
                newCount++;
            }
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            if(service_order[service_order.size() - 1] != 0)
            {
                service_order.push_back(0);
                newCount++;
            }
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert((int)service_order.size() == REQUEST_COUNT + newCount);

    for(lsint i=0; i<service_order.size(); i++)
    {
        head_movement += abs(service_order[i] - head_pos);
        head_pos = service_order[i];
    }
    return;
}

void set_cscan_service_order()
{
    vector<int> lt_head;
    vector<int> mt_head;

    for(int i=0; i<REQUEST_COUNT; i++)
    {
        if(requests[i] < head_pos) lt_head.push_back(requests[i]);
        else mt_head.push_back(requests[i]);
    }

    sort(mt_head.begin(), mt_head.end());
    sort(lt_head.begin(), lt_head.end());

    int newCount = 0;
    if(lt_head.size() == 0)
    {
        for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[lt_head.size()-1])
        {
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            if(service_order[service_order.size() - 1] != CYLINDER_COUNT-1)
            {
                service_order.push_back(CYLINDER_COUNT - 1);
                newCount++;
            }
            if(lt_head[0] != 0)
            {
                service_order.push_back(0);
                newCount++;
            }
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            reverse(mt_head.begin(), mt_head.end());
            reverse(lt_head.begin(), lt_head.end());

            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            if(service_order[service_order.size() - 1] != 0)
            {
                service_order.push_back(0);
                newCount++;
            }
            if(mt_head[0] != CYLINDER_COUNT - 1)
            {
                service_order.push_back(CYLINDER_COUNT - 1);
                newCount++;
            }
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert((int)service_order.size() == REQUEST_COUNT + newCount);

    for(lsint i=0; i<service_order.size(); i++)
    {
        head_movement += abs(service_order[i] - head_pos);
        head_pos = service_order[i];
    }
    return;
}

void set_look_service_order()
{
    vector<int> lt_head;
    vector<int> mt_head;

    for(int i=0; i<REQUEST_COUNT; i++)
    {
        if(requests[i] < head_pos) lt_head.push_back(requests[i]);
        else mt_head.push_back(requests[i]);
    }

    sort(mt_head.begin(), mt_head.end());
    sort(lt_head.rbegin(), lt_head.rend());

    if(lt_head.size() == 0)
    {
        for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[0])
        {
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert((int)service_order.size() == REQUEST_COUNT);

    for(lsint i=0; i<service_order.size(); i++)
    {
        head_movement += abs(service_order[i] - head_pos);
        head_pos = service_order[i];
    }
    return;
}

void set_clook_service_order()
{
    vector<int> lt_head;
    vector<int> mt_head;

    for(int i=0; i<REQUEST_COUNT; i++)
    {
        if(requests[i] < head_pos) lt_head.push_back(requests[i]);
        else mt_head.push_back(requests[i]);
    }

    sort(mt_head.begin(), mt_head.end());
    sort(lt_head.begin(), lt_head.end());

    if(lt_head.size() == 0)
    {
        for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[lt_head.size()-1])
        {
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            reverse(mt_head.begin(), mt_head.end());
            reverse(lt_head.begin(), lt_head.end());

            for(lsint i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            for(lsint i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert(service_order.size() == REQUEST_COUNT);

    for(lsint i=0; i<service_order.size(); i++)
    {
        head_movement += abs(service_order[i] - head_pos);
        head_pos = service_order[i];
    }
    return;
}

void display_outputs()
{
    if(debug)
    {
        cerr<<"The request order is "<<endl;
        for(int i = 0; i<REQUEST_COUNT; i++)
        {
            cerr<<requests[i]<<" ";
        }
        cerr<<endl;
        cerr<<"The service order is "<<endl;
        for(lsint i = 0; i<service_order.size(); i++)
        {
            cerr<<service_order[i]<<" ";
        }
        cerr<<endl;
    }

    // Write service order to file
    ofstream fout;
    fout.open("output_service_order.txt");
    for(lsint i = 0; i<service_order.size(); i++)
    {
        fout<<service_order[i]<<endl;
    }

    if(justAvgValue) cout<<head_movement / (double)REQUEST_COUNT<<endl;
    else
    {
        cout<<"The average head movement is " << head_movement / (double)REQUEST_COUNT <<endl;
        cout<<"The total head movement is " << head_movement << endl;
        cout<<"Service order printed to output_service_order.txt"<<endl;
    }
    return;
}