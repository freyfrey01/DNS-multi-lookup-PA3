#include <iostream>
#include <algorithm>
#include <deque>
#include <array>
#include <vector>
using namespace std;

typedef struct
{
    int arrival;
    int execution;
    int deadline;
    int pid;
    int timeremaining;
    bool added;
}ProcessSched;
bool operator<(const ProcessSched& a, const ProcessSched& b) {
    return a.timeremaining < b.timeremaining;
}
bool compare(const ProcessSched& a, const ProcessSched& b) {
    return a.deadline < b.deadline;
}
bool arrivalsort(ProcessSched a, ProcessSched b)
{
    if(a.arrival < b.arrival)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void fcfs(ProcessSched numbers[],int size)
{
    int tick=numbers[0].arrival;
    int done=0;
    cout << "FCFS:" << endl;
    for (int i=0;i<size;i++)
    {
        cout << "Process " << numbers[i].pid << " - Start time: " << tick << endl;
        tick=tick+numbers[i].execution;
        done++;
        if(done==size)
        {
            cout << "No Process - Start time: " << tick << endl;
        }
    }
}
void sjf(ProcessSched numbers[],int size)
{
    cout << "SJF:" << endl;
    for(int i=0;i<size;i++)
    {
        numbers[i].timeremaining=numbers[i].execution;
    }
    deque <ProcessSched> readyqueue;
    ProcessSched running=numbers[0];
    int tick=numbers[0].arrival;
    cout << "Process " << running.pid << " - Start time: " << tick << endl;
    int done=0;
    while(1)
    {
        if(done==size-1)
        {
            tick=tick+running.timeremaining;
            cout << "No Process - Start time: " << tick << endl;
            break;
        }
        for(int i=1;i<size;i++)
        {
            if(numbers[i].arrival==tick)
            {
                readyqueue.push_back(numbers[i]);
            }
        }
        if(readyqueue.empty()==false)
        {
            sort(readyqueue.begin(),readyqueue.end());
            if(running.timeremaining>readyqueue.front().timeremaining)
            {
                readyqueue.push_back(running);
                running=readyqueue.front();
                cout << "Process " << running.pid << " - Start time: " << tick << endl;
                readyqueue.pop_front();
            }
        }
        if(running.timeremaining==0 || running.deadline==tick)
        {
            done++;
            running=readyqueue.front();
            cout << "Process " << running.pid << " - Start time: " << tick << endl;
            readyqueue.pop_front();
        }
        running.timeremaining=running.timeremaining-1;
        tick++;
    }
}
void edf(ProcessSched numbers[],int size)
{
    cout << "EDF:" << endl;
    deque <ProcessSched> readyqueue;
    ProcessSched running=numbers[0];
    int tick=numbers[0].arrival;
    cout << "Process " << running.pid << " - Start time: " << tick << endl;
    int done=0;
    while(1)
    {
        vector <ProcessSched> readyvec;
        if(done==size-1)
        {
            tick=tick+running.timeremaining;
            cout << "No Process - Start time: " << tick << endl;
            break;
        }
        for(int i=1;i<size;i++)
        {
            if(numbers[i].arrival==tick)
            {
                readyqueue.push_back(numbers[i]);
            }
        }
        if(readyqueue.empty()==false)
        {
            while(readyqueue.empty()==false)
            {
                readyvec.push_back(readyqueue.front());
                readyqueue.pop_front();
            }
            sort(readyvec.begin(),readyvec.end(),compare);
            for(int k=0;k<readyvec.size();k++)
            {
                readyqueue.push_back(readyvec[k]);
            }
            if(running.deadline>readyqueue.front().deadline)
            {
                readyqueue.push_back(running);
                running=readyqueue.front();
                cout << "Process " << running.pid << " - Start time: " << tick << endl;
                readyqueue.pop_front();
            }
        }
        if(running.timeremaining==0 || running.deadline==tick)
        {
            done++;
            running=readyqueue.front();
            cout << "Process " << running.pid << " - Start time: " << tick << endl;
            readyqueue.pop_front();
        }
        running.timeremaining=running.timeremaining-1;
        tick++;
    }
}
void roundrobin(ProcessSched numbers[],int size)
{
    for(int i=0;i<size;i++)
    {
        numbers[i].added=false;
    }
    cout << "Round Robin:" << endl;
    for(int i=0;i<size;i++)
    {
        numbers[i].timeremaining=numbers[i].execution;
    }
    deque <ProcessSched> readyqueue;
    ProcessSched running=numbers[0];
    int tick=numbers[0].arrival;
    int one=0;
    cout << "Process " << running.pid << " - Start time: " << tick << endl;
    int done=0;
    while(1)
    {
        vector <ProcessSched> readyvec;
        if(done==size)
        {
            tick=tick+running.timeremaining;
            cout << "No Process - Start time: " << tick << endl;
            break;
        }
        for(int i=size-1;i>0;i--)
        {
            if(numbers[i].arrival<=tick && numbers[i].added==false)
            {
                readyvec.push_back(numbers[i]);
                numbers[i].added=true;
                // while(readyqueue.empty()==false)
                //     {
                //         readyvec.push_back(readyqueue.front());
                //         readyqueue.pop_front();
                //     }
                //     sort(readyvec.begin(),readyvec.end(),arrivalsort);
                //     for(int k=0;k<readyvec.size();k++)
                //     {
                //         readyqueue.push_back(readyvec[k]);
                //     }
            }
        }
        if(readyvec.size()>=1)
        {
            if(readyvec.size()>1)
            {
                sort(readyvec.begin(),readyvec.end(),arrivalsort);
            }
            for(int l=0;l<readyvec.size();l++)
            {
                readyqueue.push_back(readyvec.at(l));
            }
        }
        if(readyqueue.empty()==false || running.timeremaining==0 || running.deadline==tick)
        {
            if(running.timeremaining==0 || running.deadline==tick)
            {
                done++;
                if(readyqueue.empty()==false)
                {
                    running=readyqueue.front();
                    cout << "Process " << running.pid << " - Start time: " << tick << endl;
                    readyqueue.pop_front();
                }
            }
            else
            {
                readyqueue.push_back(running);
                running=readyqueue.front();
                cout << "Process " << running.pid << " - Start time: " << tick << endl;
                readyqueue.pop_front();
            }
        }
        else
        {
            if(one==0)
            {
                one=1;
            }
            else
            {
                cout << "Process " << running.pid << " - Start time: " << tick << endl;
            }
        }
        if(running.timeremaining>=20)
        {
            tick+=20;
            running.timeremaining=running.timeremaining-20;
        }
        else
        {
            tick+=running.timeremaining;
            running.timeremaining=0;
        }
    }
}
int main(int argc, char*argv[])
{
    int size=stoi(argv[1]);
    // int size=4;
    ProcessSched processes[size];
    int arrival,execution,deadline;
    for (int i=0;i<size;i++)
    {
        cout << "Enter Process " << i << "'s arrival time" << endl; 
        cin >> arrival;
        cout << "Enter Process " << i << "'s execution time" << endl; 
        cin >> execution;
        cout << "Enter Process " << i << "'s deadline time" << endl; 
        cin >> deadline;
        processes[i].arrival=arrival;
        processes[i].execution=execution;
        processes[i].deadline=deadline;
        processes[i].pid=i;
    }
    // processes[0].arrival=0;
    // processes[0].execution=40;
    // processes[0].deadline=200;
    // processes[0].pid=0;

    // processes[1].arrival=20;
    // processes[1].execution=90;
    // processes[1].deadline=230;
    // processes[1].pid=1;

    // processes[2].arrival=55;
    // processes[2].execution=40;
    // processes[2].deadline=200;
    // processes[2].pid=2;

    // processes[3].arrival=85;
    // processes[3].execution=20;
    // processes[3].deadline=200;
    // processes[3].pid=3;
    sort(processes,processes+size,arrivalsort);
    fcfs(processes,size);
    sjf(processes,size);
    edf(processes,size);
    roundrobin(processes,size);
}