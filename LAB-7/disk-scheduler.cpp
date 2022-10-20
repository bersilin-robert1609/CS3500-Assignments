#include <bits/stdc++.h>

using namespace std;

#define CYLINDER_COUNT 5000
#define REQUEST_COUNT 1000
#define DEFAULT_HEAD 2000

/*
 * Algorithm Code
 * FCFS - 1
 * SSTF - 2
 * SCAN -3
 * C-SCAN - 4
 * LOOK - 5
 * C-LOOK - 6
 */

int head_pos;
int algorithm;
vector<int> requests;
vector<int> service_order;
int head_movement;
bool debug = false;

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
    switch(argc)
    {
        case 3:
        {
            head_pos = atoi(argv[1]);
            if(head_pos >= CYLINDER_COUNT)
            {
                cerr<<"INVALID HEAD POSITION"<<endl;
                exit(0);
            }
            algorithm = atoi(argv[2]);
            break;
        }
        case 2:
        {
            head_pos = DEFAULT_HEAD;
            algorithm = stoi(argv[1]);
            break;
        }
        default:
        {
            cerr<<"The usage is ./<binary> <head position> <algorithm> or ./<binary> <algorithm>"<<endl;
            exit(0);
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
    switch(algorithm)
    {
        case 1:
        {
            // FCFS Algorithm
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
            service_order.resize(REQUEST_COUNT);
            set_sstf_service_order();
            break;
        }
        case 3:
        {
            // SCAN Algorithm
            set_scan_service_order();
            break;
        }
        case 4:
        {
            // C-SCAN Algorithm
            set_cscan_service_order();
            break;
        }
        case 5:
        {
            // LOOK Algorithm
            set_look_service_order();
            break;
        }
        case 6:
        {
            // C-LOOK Algorithm
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

    for(int i=0; i<requests.size(); i++)
    {
        if(requests[i] < head_pos) lt_head.push_back(requests[i]);
        else mt_head.push_back(requests[i]);
    }

    sort(mt_head.begin(), mt_head.end());
    sort(lt_head.rbegin(), lt_head.rend());

    int newCount = 0;

    if(lt_head.size() == 0)
    {
        for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[0])
        {
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            if(service_order[service_order.size() - 1] != CYLINDER_COUNT-1)
            {
                service_order.push_back(CYLINDER_COUNT - 1);
                newCount++;
            }
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            if(service_order[service_order.size() - 1] != 0)
            {
                service_order.push_back(0);
                newCount++;
            }
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert(service_order.size() == REQUEST_COUNT + newCount);

    for(int i=0; i<service_order.size(); i++)
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
        for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[lt_head.size()-1])
        {
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
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
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            reverse(mt_head.begin(), mt_head.end());
            reverse(lt_head.begin(), lt_head.end());

            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
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
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert(service_order.size() == REQUEST_COUNT + newCount);

    for(int i=0; i<service_order.size(); i++)
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
        for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[0])
        {
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert(service_order.size() == REQUEST_COUNT);

    for(int i=0; i<service_order.size(); i++)
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
        for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
    }
    else if(mt_head.size() == 0)
    {
        for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
    }
    else
    {
        if(mt_head[0] - head_pos < head_pos - lt_head[lt_head.size()-1])
        {
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
        }
        else
        {
            reverse(mt_head.begin(), mt_head.end());
            reverse(lt_head.begin(), lt_head.end());

            for(int i=0; i<lt_head.size(); i++) service_order.push_back(lt_head[i]);
            for(int i=0; i<mt_head.size(); i++) service_order.push_back(mt_head[i]);
        }
    }

    assert(service_order.size() == REQUEST_COUNT);

    for(int i=0; i<service_order.size(); i++)
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
        for(int i = 0; i<service_order.size(); i++)
        {
            cerr<<service_order[i]<<" ";
        }
        cerr<<endl;
    }
    cerr<<service_order.size()<<endl;
    cerr<<"The average head movement is " << head_movement / (double)REQUEST_COUNT <<endl;
    cerr<<"The total head movement is " << head_movement << endl;
    return;
}