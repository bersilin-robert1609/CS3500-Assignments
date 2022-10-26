#include <iostream>
#include <vector>
#include <queue>
using namespace std;

bool debug = false;
string checkNewProcess = "idle";
int inOneLine = 0;

class Process
{
public:
    int index;
    string process_id;
    int arrival_time;
    int burst_time;
    int bkp_burst_time;
    string type;
    int priority;
    int completion_time;
    int turn_around_time;
    int waiting_time;
    
    Process(int index, string process_id, int arrival_time, int burst_time, int priority, string type)
    {
        this->index = index;
        this->process_id = process_id;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        this->priority = priority;
        this->type = type;
        this->completion_time = 0;
        this->turn_around_time = 0;
        this->waiting_time = 0;
        this->bkp_burst_time = burst_time;
    }
    void complete_process(int t);
};

class CompareSystem
{
public: 
    bool operator() (const Process &p1, const Process &p2) 
    {
        if(p1.priority == p2.priority) return p1.arrival_time > p2.arrival_time;
        return p1.priority > p2.priority;
    } 
};
class CompareInteractive
{
public: 
    bool operator() (const Process &p1, const Process &p2) 
    {
        if(p1.arrival_time == p2.arrival_time) return p1.index > p2.index;
        return p1.arrival_time > p2.arrival_time;
    } 
};
class CompareBatch
{
public: 
    bool operator() (const Process &p1, const Process &p2) 
    {
        if(p1.bkp_burst_time == p2.bkp_burst_time) return p1.arrival_time > p2.arrival_time;
        return p1.bkp_burst_time > p2.bkp_burst_time;
    } 
};

class CompareCompletetion{public: bool operator() (const Process &p1, const Process &p2) {return p1.index > p2.index;} };

class MultiLevelQueue
{
private:
    int quantum;
    int N;
    priority_queue<Process, vector<Process>, CompareInteractive> processes;
    priority_queue<Process, vector<Process>, CompareSystem> system_process;
    priority_queue<Process, vector<Process>, CompareInteractive> interactive_process;
    priority_queue<Process, vector<Process>, CompareInteractive> interactive_editing_process;
    priority_queue<Process, vector<Process>, CompareBatch> batch_process;
    priority_queue<Process, vector<Process>, CompareBatch> student_process;
    priority_queue<Process, vector<Process>, CompareCompletetion> completed_process;

public:
    void process_info_input();
    void queue_process();
    void print_process_info();
    void queue_assign(int t);
    bool check_idle();
    bool check_done();
    void print_processed_data(Process p, int t);
};

int main()
{
    if(debug) freopen("debug.txt", "w", stderr);
    MultiLevelQueue multi_level_queue = MultiLevelQueue();
    multi_level_queue.process_info_input();
    multi_level_queue.queue_process();
    multi_level_queue.print_process_info();
    return 0;
}

void MultiLevelQueue::process_info_input()
{
    cin >> quantum;
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        string id;
        int arrival, burst, priority;
        string type;
        cin >> id >> arrival >> burst >> type >> priority;
        processes.push(Process(i, id, arrival, burst, priority, type));
    }
    if(debug) cerr<<"Processes Input taken successfully"<<endl;
}

void MultiLevelQueue::print_process_info()
{
    cout<<"Quantum: "<<quantum<<endl;
    cout<<"Number of processes: "<<completed_process.size()<<endl;
    while(!completed_process.empty())
    {
        Process p = completed_process.top();
        cout<<"PID: "<<p.process_id;
        cout<<" AT: "<<p.arrival_time;
        cout<<" CT: "<<p.completion_time;
        cout<<" TAT: "<<p.turn_around_time;
        cout<<" WT: "<<p.waiting_time;
        cout<<" Type: "<<p.type<<endl;
        completed_process.pop();
    }
}

void MultiLevelQueue::queue_process()
{
    cout<<"Output Sequence / Execution Sequence: "<<endl;
    Process pIdle = Process(-1, "Idle", 0, 0, 0, "Idle");
    int quantum_time = 1, i = 1, t = 0;
    bool done = false;
    while(!done)
    {
        if(debug) cerr<<"Time: "<<t<<" Quantum Time: "<<quantum_time<<" i: "<<i<<endl;
        queue_assign(t);
        if(quantum_time > quantum)
        {
            if(debug) cerr<<"Quantum time exceeded increasing i: "<<i;
            quantum_time = 1;
            i++;
            if(i>5) i = 1;
            if(debug) cerr<<" to i: "<<i<<endl;
        }
        if(check_idle())
        {
            if(check_done()) {if(debug){cerr<<"All processes done. Exiting..."<<endl;} done = true; break;}
            else {if(debug) {cerr<<"The CPU is idle for time t: "<<t<<endl;} t++; print_processed_data(pIdle, t); continue;}
        }
        while(1)
        {
            if(i == 1 && !system_process.empty()) break;
            else if(i == 1 && system_process.empty())
            {
                i++;
                quantum_time = 1;
            }
            if(i == 2 && !interactive_process.empty()) break;
            else if(i == 2 && interactive_process.empty())
            {
                i++;
                quantum_time = 1;
            }
            if(i == 3 && !interactive_editing_process.empty()) break;
            else if(i == 3 && interactive_editing_process.empty())
            {
                i++;
                quantum_time = 1;
            }
            if(i == 4 && !batch_process.empty()) break;
            else if(i == 4 && batch_process.empty())
            {
                i++;
                quantum_time = 1;
            }
            if(i == 5 && !student_process.empty()) break;
            else if(i == 5 && student_process.empty())
            {
                i = 1;
                quantum_time = 1;
            }
        }
        if(debug) cerr<<"At time t: "<<t<<" queue i: "<<i<<" is not empty and is selected, quantum_time is: "<<quantum_time<<endl;
        Process p = Process(0, "", 0, 0, 0, "");
        switch(i)
        {
            case 1:
                if(!system_process.empty())
                {
                    p = system_process.top();
                    system_process.pop();
                    p.burst_time -= 1;
                    if(p.burst_time == 0)
                    {
                        p.complete_process(t);
                        completed_process.push(p);
                    }
                    else system_process.push(p);
                }
                break;
            case 2:
                if(!interactive_process.empty())
                {
                    p = interactive_process.top();
                    interactive_process.pop();
                    p.burst_time -= 1;
                    if(p.burst_time == 0)
                    {
                        p.complete_process(t);
                        completed_process.push(p);
                    }
                    else interactive_process.push(p);
                }
                break;
            case 3:
                if(!interactive_editing_process.empty())
                {
                    p = interactive_editing_process.top();
                    interactive_editing_process.pop();
                    p.burst_time -= 1;
                    if(p.burst_time == 0)
                    {
                        p.complete_process(t);
                        completed_process.push(p);
                    }
                    else interactive_editing_process.push(p);
                }
                break;
            case 4:
                if(!batch_process.empty())
                {
                    p = batch_process.top();
                    batch_process.pop();
                    p.burst_time -= 1;
                    if(p.burst_time == 0)
                    {
                        p.complete_process(t);
                        completed_process.push(p);
                    }
                    else batch_process.push(p);
                }
                break;
            case 5:
                if(!student_process.empty())
                {
                    p = student_process.top();
                    student_process.pop();
                    p.burst_time -= 1;
                    if(p.burst_time == 0)
                    {
                        p.complete_process(t);
                        completed_process.push(p);
                    }
                    else student_process.push(p);
                }
                break;
        }
        print_processed_data(p, t);
        quantum_time++;
        t++;
    }
    cout<<endl;
}

void MultiLevelQueue::queue_assign(int t)
{
    while(!processes.empty())
    {
        Process p = processes.top();
        if(p.arrival_time <= t)
        {
            if(p.type == "sys") system_process.push(p);
            else if(p.type == "ip") interactive_process.push(p);
            else if(p.type == "iep") interactive_editing_process.push(p);
            else if(p.type == "bp") batch_process.push(p);
            else if(p.type == "sp") student_process.push(p);
            processes.pop();
        } else break;
    }
}

bool MultiLevelQueue::check_idle()
{
    int empty = 0;
    if(system_process.empty()) empty++;
    if(interactive_process.empty()) empty++;
    if(interactive_editing_process.empty()) empty++;
    if(batch_process.empty()) empty++;
    if(student_process.empty()) empty++;
    if(empty == 5) return true;
    else return false;
}

bool MultiLevelQueue::check_done()
{
    if(completed_process.size() == (long unsigned int)N) return true;
    else return false;
}

void MultiLevelQueue::print_processed_data(Process p, int t)
{
    if(debug) cerr<<"At Time: "<<t<<" Scheduled Process: "<<p.process_id<<" Type: "<<p.type<<" Burst Time Left: "<<p.burst_time<<endl;
    else if(p.process_id != checkNewProcess)
    {
        if(debug) cerr<<"PROCESS ID: "<<p.process_id<<" Temp: "<<checkNewProcess<<endl;
        cout<<p.process_id<<" ";
        checkNewProcess = p.process_id;
        inOneLine ++;
    }

    if(inOneLine == 10)
    {
        cout<<endl;
        inOneLine = 0;
    }
}

void Process::complete_process(int t)
{
    completion_time = t+1;
    turn_around_time = completion_time - arrival_time;
    waiting_time = turn_around_time - bkp_burst_time;
}