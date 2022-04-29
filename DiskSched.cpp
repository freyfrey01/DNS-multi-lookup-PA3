#include <vector>
#include <iostream>

using namespace std;

void fcfs(vector<int> list,int start)
{
    int seek_count = 0;
    int distance, cur_track;
    int size=list.size();

    for (int i = 0; i < size; i++) {
        cur_track = list[i];
 
        // calculate absolute distance
        distance = abs(cur_track - start);
 
        // increase the total count
        seek_count += distance;
 
        // accessed track is now new head
        start = cur_track;
    }
 
        cout << "FCFS: " << seek_count << " [";
        for (int i=0;i<size-1;i++)
        {
            cout << list[i] << ", ";
        }
        cout << list[size-1] << "]" << endl;
 
}
//MUST MAKE HEAD THE FIRST VALUE IN POSITIONS 
void sstf(vector <int> positions,int head)
{
    int movement,shortest,index;
    int n=positions.size();
    bool done[n];
    vector <int> result;
    for (int i=0;i<n;i++)
    {
        done[i]=false;
    }
    movement = 0;
    for(int i=0; i<n; i++)
    {
        index = 0;
        shortest = INT_MAX;
 
        for(int k=0; k<n; k++)
        {
            if(abs(head - positions[k]) < shortest && !done[k])
            {
                index = k;
                shortest = abs(head - positions[k]);
            }
        }
        done[index] = true;
        movement += shortest;
        head = positions[index];
        result.push_back(head);
    }
    cout << "SSTF: " << movement << " [";
        for (int i=0;i<n-1;i++)
        {
            cout << result[i] << ", ";
        }
        cout << result.back() << "]" << endl;
    return;
}

void scan(vector <int> arr, int head, int disk_size,string direction)
{
    int seek_count = 0;
    int distance, cur_track;
    vector<int> left, right;
    vector<int> seek_sequence;
    int size=arr.size();
    // appending end values
    // which has to be visited
    // before reversing the direction
    if(direction=="right")
    {
        right.push_back(disk_size);
    }
    else
    {
        left.push_back(0);
    }
 
    for (int i = 0; i < size; i++) {
            if (arr[i] < head)
            left.push_back(arr[i]);
            if (arr[i] > head)
            right.push_back(arr[i]);
    }
 
    // sorting left and right vectors
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
 
    // run the while loop two times.
    // one by one scanning right
    // and left of the head
    seek_sequence.push_back(head);
    int run = 2;
    while (run--) {
        if (direction == "left") {
            for (int i = left.size() - 1; i >= 0; i--) {
                cur_track = left[i];
 
                // appending current track to seek sequence
                seek_sequence.push_back(cur_track);
 
                // calculate absolute distance
                distance = abs(cur_track - head);
 
                // increase the total count
                seek_count += distance;
 
                // accessed track is now the new head
                head = cur_track;
            }
            direction = "right";
        }
        else if (direction == "right") {
            for (int i = 0; i < right.size(); i++) {
                cur_track = right[i];
                // appending current track to seek sequence
                seek_sequence.push_back(cur_track);
 
                // calculate absolute distance
                distance = abs(cur_track - head);
 
                // increase the total count
                seek_count += distance;
 
                // accessed track is now new head
                head = cur_track;
            }
            direction = "left";
        }
    }
 
    cout << "SCAN: " << seek_count << " [";
        for (int i=0;i<seek_sequence.size()-1;i++)
        {
            cout << seek_sequence[i] << ", ";
        }
        cout << seek_sequence[seek_sequence.size()] << "]" << endl;
}
void cscan(vector <int> arr, int head,int disk_size)
{
    int seek_count = 0;
    int distance, cur_track;
    vector<int> left, right;
    vector<int> seek_sequence;
    int size=arr.size();
 
    // appending end values
    // which has to be visited
    // before reversing the direction
    left.push_back(0);
    right.push_back(disk_size);
 
    // tracks on the left of the
    // head will be serviced when
    // once the head comes back
    // to the beginning (left end).
    for (int i = 0; i < size; i++) {
        if (arr[i] < head)
            left.push_back(arr[i]);
        if (arr[i] > head)
            right.push_back(arr[i]);
    }
 
    // sorting left and right vectors
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    seek_sequence.push_back(head);
    // first service the requests
    // on the right side of the
    // head.
    for (int i = 0; i < right.size(); i++) {
        cur_track = right[i];
        // appending current track to seek sequence
        seek_sequence.push_back(cur_track);
 
        // calculate absolute distance
        distance = abs(cur_track - head);
 
        // increase the total count
        seek_count += distance;
 
        // accessed track is now new head
        head = cur_track;
    }
 
    // once reached the right end
    // jump to the beginning.
    head = 0;
 
    // adding seek count for head returning from 199 to 0
    seek_count += disk_size;
 
    // Now service the requests again
    // which are left.
    for (int i = 0; i < left.size(); i++) {
        cur_track = left[i];
        // appending current track to seek sequence
        seek_sequence.push_back(cur_track);
        // calculate absolute distance
        distance = abs(cur_track - head);
 
        // increase the total count
        seek_count += distance;
 
        // accessed track is now the new head
        head = cur_track;
    }
    cout << "C-SCAN: " << seek_count << " [";
        for (int i=0;i<seek_sequence.size()-1;i++)
        {
            cout << seek_sequence[i] << ", ";
        }
        cout << seek_sequence.back() << "]" << endl;
}

void look(vector <int> arr, int head, string direction)
{
    int seek_count = 0;
    int distance, cur_track;
    vector<int> left, right;
    vector<int> seek_sequence;
    int size=arr.size();
 
    // appending values which are
    // currently at left and right
    // direction from the head.
    for (int i = 0; i < size; i++) {
        if (arr[i] < head)
            left.push_back(arr[i]);
        if (arr[i] > head)
            right.push_back(arr[i]);
    }
 
    // sorting left and right vectors
    // for servicing tracks in the
    // correct sequence.
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    seek_sequence.push_back(head);
    // run the while loop two times.
    // one by one scanning right
    // and left side of the head
    int run = 2;
    while (run--) {
        if (direction == "left") {
            for (int i = left.size() - 1; i >= 0; i--) {
                cur_track = left[i];
 
                // appending current track to seek sequence
                seek_sequence.push_back(cur_track);
 
                // calculate absolute distance
                distance = abs(cur_track - head);
 
                // increase the total count
                seek_count += distance;
 
                // accessed track is now the new head
                head = cur_track;
            }
            // reversing the direction
            direction = "right";
        }
        else if (direction == "right") {
            for (int i = 0; i < right.size(); i++) {
                cur_track = right[i];
                // appending current track to seek sequence
                seek_sequence.push_back(cur_track);
 
                // calculate absolute distance
                distance = abs(cur_track - head);
 
                // increase the total count
                seek_count += distance;
 
                // accessed track is now new head
                head = cur_track;
            }
            // reversing the direction
            direction = "left";
        }
    }
 
    cout << "LOOK: " << seek_count << " [";
        for (int i=0;i<seek_sequence.size()-1;i++)
        {
            cout << seek_sequence[i] << ", ";
        }
        cout << seek_sequence.back() << "]" << endl;
}

void clook(vector<int> arr, int head)
{
    int seek_count = 0;
    int distance, cur_track;
    vector<int> left, right;
    vector<int> seek_sequence;
    int size=arr.size();
 
    // Tracks on the left of the
    // head will be serviced when
    // once the head comes back
    // to the beginning (left end)
    for (int i = 0; i < size; i++) {
        if (arr[i] < head)
            left.push_back(arr[i]);
        if (arr[i] > head)
            right.push_back(arr[i]);
    }
 
    // Sorting left and right vectors
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    seek_sequence.push_back(head);
    // First service the requests
    // on the right side of the
    // head
    for (int i = 0; i < right.size(); i++) {
        cur_track = right[i];
 
        // Appending current track to seek sequence
        seek_sequence.push_back(cur_track);
 
        // Calculate absolute distance
        distance = abs(cur_track - head);
 
        // Increase the total count
        seek_count += distance;
 
        // Accessed track is now new head
        head = cur_track;
    }
 
    // Once reached the right end
    // jump to the last track that
    // is needed to be serviced in
    // left direction
    seek_count += abs(head - left[0]);
    head = left[0];
 
    // Now service the requests again
    // which are left
    for (int i = 0; i < left.size(); i++) {
        cur_track = left[i];
 
        // Appending current track to seek sequence
        seek_sequence.push_back(cur_track);
 
        // Calculate absolute distance
        distance = abs(cur_track - head);
 
        // Increase the total count
        seek_count += distance;
 
        // Accessed track is now the new head
        head = cur_track;
    }
 
    cout << "C-LOOK: " << seek_count << " [";
        for (int i=0;i<seek_sequence.size()-1;i++)
        {
            cout << seek_sequence[i] << ", ";
        }
        cout << seek_sequence.back() << "]" << endl;
}

int main(int argc, char* argv[])
{
    vector <int> numbers;
    int size=argc;
    int start;
    int disksize;
    string direction;
    for (int i=1;i<size;i++)
    {
        numbers.push_back(stoi(argv[i]));
    }
    start=numbers[0];
    cout << "Enter max disk size" << endl;
    cin >> disksize;
    cout << "Enter direction" << endl;
    cin >> direction;
    fcfs(numbers,start);
    sstf(numbers,start);
    scan(numbers,start,disksize,direction);
    cscan(numbers,start,disksize);
    look(numbers,start,direction);
    clook(numbers,start);
}