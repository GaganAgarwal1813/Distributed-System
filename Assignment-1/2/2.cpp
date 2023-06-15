#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <mpi.h>


using namespace std;

void log(string s, int n)
{
    // ofstream myfile;
    // myfile.open(to_string(n),ios::app);
    // myfile<<s<<endl;
    // myfile.close();
    // cout<<s<<endl;
}


void handle_output(unordered_map<int,vector<int>>& particle_mp, int k)
{
    int lc = 0, rc = 0;
    set<pair<pair<pair<int,int>,int>,char>> m;
    int dc = 0, uc = 0;
    for(auto i:particle_mp)
    {
        vector<int> arr = i.second;
        int l = 0;
        char ch;
        pair<int,int> p = {arr[l],arr[l+1]};
        log("Pair Updated ",1);
        if(arr[l+2] == 0 && arr[l+3] == -1)
        {
            ch = 'D';
            dc++;
        }
        else if(arr[l+2] == 0 && arr[l+3] == 1)
        {
            ch = 'U';
            uc++;
        }
        else if(arr[l+2] == -1 && arr[l+3] == 0)
        {
            ch = 'L';
            lc++;
        }
        else
        {
            ch = 'R';
            rc++;
        }
        log("Up = ",uc);
        m.insert({{p,i.first},ch});
        log("Down = ",dc); 
    } 

    for(auto i:m)
    {
        cout<<i.first.first.first<<" "<<i.first.first.second<<" "<<i.second<<endl;
    }  
}

bool check1(int temp[], int n, int idx)
{
    int l = 0, t1 = 0;
    int tidx = idx;
    if(l == tidx)
    {
        l = 1;   
        return true;
    }
    return true;
    
}



void ip_handle(int arr[], int k)
{
    int down_count = 0, up_count = 0;
    int right_count = 0;
    int left_count = 0;
    for(int l=0; l < 4*k; l+=4)
    {
        int i,j,dx,dy;
        int limitval = 4*k;
        if(l >= limitval+1)
            continue;
        char ch;
        cin>>i;
        arr[l] = i;
        cin>>j;
        arr[l+1] = j;
        cin>>ch;
        if(ch == 'D')
        {
            dx = 0;
            dy = -1;
            down_count++;
        }
        else if(ch == 'U')
        {
            dx = 0;
            dy = 1;
            up_count++;
        }
        else if(ch == 'L')
        {
            dx  = -1;
            dy = 0;
            left_count++;
        }
        else
        {
            dx = 1;
            dy = 0;
            right_count++;
        }
        
        arr[l+2] = dx;
        arr[l+3] = dy;
    }
}



int collision_coordinate_add(int temp[], int n,int& idx, map<pair<int,int>,vector<int>>& collisions,unordered_map<int,vector<int>>& particle_mp, int add_val1)
{
    long long iter = 4*n;
    for(int l=0;l < 4*n;l = l + 4)
    {
        if(check1(temp, n, idx))
        {
            particle_mp[idx] = {temp[l],temp[l+1],temp[l+2],temp[l+3]};
            collisions[{temp[l],temp[l+1]}].push_back(idx);
            idx++;
        }
        
    }
    log("Value Added ",1);
    return 1;
}



void change_dir(unordered_map<int,vector<int>>& arr, int l1, int l2, int n, int m, int k1)
{
    int lc = 0, rc = 0, uc = 0, dc = 0,c1 = 0;
    if(arr[l1][2] == -1 && arr[l1][3] == 0) 
    {
        c1 = 2;
        if(arr[l1][1] == 0 || arr[l1][1] == m-1)
            return;
        else
        {   
            arr[l1][2] = 0;
            log("Moving Left", 1);
            arr[l1][3] = 1;
            lc++;

            arr[l2][2] = 0;
            arr[l2][3] = -1;
            lc -= 1;
        }
        
    }
    else if(arr[l1][2] == 1 && arr[l1][3] == 0) 
    {
        c1 = 4;
        if(arr[l1][1] == 0 || arr[l1][1] == m-1)
            return;
        else
        {
            arr[l1][2] = 0;
            log("Moving Right", 1);
            arr[l1][3] = -1;
            rc++;
            arr[l2][2] = 0;
            arr[l2][3] = 1;
            rc-=1;
        }
        
    }
    else if(arr[l1][2] == 0 && arr[l1][3] == 1)
    {
        c1 = 8;
        if(arr[l1][0] == 0 || arr[l1][0] == n-1)
            return;
        else
        {
            log("Moving UP", 1);
            arr[l1][2] = 1;
            arr[l1][3] = 0;
            uc++;
            arr[l2][2] = -1;
            arr[l2][3] = 0;
            uc-=1;
        }
        
    }
    else //Going down
    {
        c1 = 16;
        if(arr[l1][0] == 0 || arr[l1][0] == n-1)
            return;
        else
        {
            log("Moving Down", 1);
            arr[l1][2] = -1;
            arr[l1][3] = 0;
            dc++;
            arr[l2][2] = 1;
            arr[l2][3] = 0;
        }
        
    }
    log("Dir = ",c1);
}

bool condition_check(vector<int> &val)
{
    if(val.size()<=1 || val.size()>2)
        return true;
    return false;
}




void solve_part1(int pnum,map<pair<int,int>,vector<int>>& collisions,int n,int m, unordered_map<int,vector<int>>& arr, int tval)
{
    int isFalsecon = 0, iscol = 0;;
    for(auto i:collisions)
    {
        pair<int,int> key = i.first;
        vector<int> val = i.second;
        if(condition_check(val))
        {
            isFalsecon = 1;
            continue;
        }
            
        if((arr[val[0]][2] + arr[val[1]][2] == 0) && (arr[val[0]][3] + arr[val[1]][3] == 0))
        {
            int l  = val[0], l1 = val[1];
            iscol = 1;
            change_dir(arr,val[0],val[1],n,m, m);

        }
    }
    log("Skipped? ",isFalsecon);
    log("Collision? ",iscol);
}


void helper(vector<int> &arr, int l, int m, int n)
{
    int flg = 0;
    if( arr[l] < 0 )
    {
       
        arr[l] = 0;
        arr[l+2] = -arr[l+2];
        flg++;
        arr[l] += arr[l+2];
        flg++;
        log("Less than 0 updated", flg);
    }
    else if(arr[l]>=n)
    {
        flg = 0;
        arr[l] = n-1;
        flg++;
        arr[l+2] = -arr[l+2];
        flg++;
        arr[l] += arr[l+2];
        flg = 1;
        log("Greater than n updated", flg);
    }
    if(arr[l+1]<0)
    {
        flg = 0;
        arr[l+1] = 0;
        flg++;
        arr[l+3] = -arr[l+3];
        flg++;
        arr[l+1] += arr[l+3];
        flg = 1;
        log("Less than 0 updated", flg);
    }
    else if(arr[l+1]>=m)
    {
        flg = 0;
        arr[l+1] = m-1;
        flg++;
        arr[l+3] = -arr[l+3];
        flg++;
        arr[l+1] += arr[l+3];
        flg = 1;
        log("Greater than m updated", flg);
    }
}


map<pair<int,int>,vector<int>> iteration_compute(int n,int m, unordered_map<int,vector<int>>& prev,int st, int ed, unordered_map<int,vector<int>>& next, unordered_map<int,vector<int>>& particle_mp)
{
    unordered_map<int,int>part_mp;
    int partmp_size = 0;
    map<pair<int,int>,vector<int>> collisions;
    int mp_c_size = 0;
    unordered_map<int,vector<int>> temp;
    int stc = 0, edc = 0, rem = 0;
    for(auto i:particle_mp)
    {

        int idx = i.first;
        mp_c_size++;
        vector<int> arr = i.second;
        int l  = 0;
        int flg = 0;
        arr[l] += arr[l+2];
        mp_c_size = 0;
        arr[l+1] += arr[l+3];
        
        helper(arr, l, m, n);
        flg = 1;
        log("Helper Worked Successfully", flg);
        
        if(arr[l+1] < st)
        {
            prev[idx] = arr;
            stc++;
        }
        else if(arr[l+1] > ed)
        {
            next[idx] = arr;
            edc++;
        } 
        else
        {
            collisions[{arr[l],arr[l+1]}].push_back(idx);
            rem++;
        }
            
        
        temp[idx] = arr; 
    }
    particle_mp = temp;
    for(auto i:prev)
    {
        stc--;
        particle_mp.erase(i.first);
    }   
        
    
    for(auto i:next)
    {   
        edc--;
        particle_mp.erase(i.first);
    }
    log("Loss Start Particle",stc);
    log("Loss End Particle", edc);
    return collisions;
}



int main(int argc, char** argv){
    int rank, n ,m ,k ,t, size;
    int meta_data_arr[4];
    int *arr = NULL;



    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0)
    {
        cin>>n>>m>>k>>t;
        meta_data_arr[0] = n;
        meta_data_arr[1] = m;
        meta_data_arr[2] = k;
        meta_data_arr[3] = t;
        log("Meta Data Taken", 1);
        arr = new int[4*k];
        ip_handle(arr,k);
        log("Input Taken", 1);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(meta_data_arr,4,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    n = meta_data_arr[0];
    int flg11 = 0;
    m = meta_data_arr[1];
    int tmval = m;
    k = meta_data_arr[2];
    t = meta_data_arr[3];
    
    int row_count_process = ceil(((double)(m))/size);
    log("Row Division Count ",row_count_process);
    int max_rows_per_pro = row_count_process, isNorows = 0;

    if((m - (rank-1)*row_count_process) <= 0)
    {   
        isNorows = 1;
        row_count_process = 0;
    }
    else if((m - (rank-1)*row_count_process) < row_count_process)
        row_count_process =  m - (rank-1)*row_count_process;
    else
        flg11 = 1;

    if(arr == NULL)
        arr = new int[4*k];
    else
        flg11 = 0;
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(arr,4*k,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    int st = rank * max_rows_per_pro;
    log("Start = ",st);
    int ed = st + row_count_process - 1;
    log("End = ",ed);
    int ptemp_size = 0;
    int idx = 0;

    unordered_map<int,vector<int>> particle_mp;
    
    for(int l = 0; l < 4 * k; l = l + 4)
        if(arr[l+1] >= st && arr[l+1]<= ed)
        {
            particle_mp[idx] = {arr[l],arr[l+1],arr[l+2],arr[l+3]};
            ptemp_size = ptemp_size + 1;
            idx++;
        }
    

    delete[] arr;

    log("Arr deleted ",1);
    map<pair<int,int>,vector<int>> collisions;

    for(int i=0;i<t && st<m;i++)
    {
        
            solve_part1(i,collisions,n,m,particle_mp, t);
            int p_count_val = 0, count_next = 0;
            int cprev1 = 0;
            unordered_map<int,vector<int>> prev;
            int tcount = 0;
            unordered_map<int,vector<int>> next;
            collisions = iteration_compute( n, m,  prev,st, ed, next, particle_mp);
            tcount++;
            if(rank!=0)// If Previous array exists
            {
                MPI_Recv(&p_count_val, 1, MPI_INT, rank-1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int flgSend = 1; 
                if(p_count_val)
                {
                    int* temp = new int[4*p_count_val];
                    MPI_Recv(temp, 4*p_count_val, MPI_INT, rank-1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    log("Received Successful", p_count_val);
                    collision_coordinate_add(temp,p_count_val,idx,collisions,particle_mp, 1);
                    log("Value Added ",1);
                    delete[] temp;
                    tcount = 0;
                }
                int count = prev.size();
                MPI_Send(&count, 1, MPI_INT, rank-1, i, MPI_COMM_WORLD);
                if(count)
                {
                    int temp_size_val = 4*count;
                    int *temp = new int[4*count];
                    int j=0, flgt = 0;
                    for(auto t1: prev)
                    {
                        vector<int> val = t1.second;
                        temp[j] = val[0];
                        flgt++;
                        temp[j+1] = val[1];
                        flgt++;
                        temp[j+2] = val[2];
                        flgt++;
                        temp[j+3] = val[3];
                        flgt++;
                        log("temp filled", flgt++);
                        j+=4;
                    }
                    MPI_Send(temp, 4*count, MPI_INT, rank-1, i, MPI_COMM_WORLD);
                    temp_size_val = 0;
                    delete[] temp;
                    log("Temp Deleted",0);
                }
            }

            if(ed+1<m )
            {
                int count = next.size();
                MPI_Send(&count, 1, MPI_INT, rank+1, i, MPI_COMM_WORLD);
                if(count)
                {
                    int temp_size_val = 4*count;
                    int *temp = new int[4*count];
                    int j=0, flgt1 = 0;
                    for(auto t1: next)
                    {
                        vector<int> val = t1.second;
                        temp[j] = val[0];
                        flgt1++;
                        temp[j+1] = val[1];
                        flgt1++;
                        temp[j+2] = val[2];
                        flgt1++;
                        temp[j+3] = val[3];
                        flgt1++;
                        log("temp filled", flgt1);
                        j+=4;
                    }
                    MPI_Send(temp, 4*count, MPI_INT, rank+1, i, MPI_COMM_WORLD);
                    temp_size_val = 0;
                    delete[] temp;
                    log("Temp Deleted",0);
                }

                MPI_Recv(&count_next, 1, MPI_INT, rank+1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if(count_next)
                {
                    int* temp = new int[4*count_next];
                    log("Trying to Receive",1);
                    MPI_Recv(temp, 4*count_next, MPI_INT, rank+1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    log("Received Successfully",0);
                    collision_coordinate_add(temp,count_next,idx,collisions,particle_mp, 1);
                    log("Value Added ",1);
                    delete[] temp;
                }

            }
        

    }
   
    MPI_Barrier(MPI_COMM_WORLD);
    
        int count1 = particle_mp.size();
        MPI_Send(&count1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if(count1)
        {
            int flgc1 = 0;
            int *temp = new int[4*count1];
            int j=0, temp_sz1 = 4*count1;
            for(auto t1: particle_mp)
            {
                vector<int> val = t1.second;
                temp[j] = val[0];
                flgc1++;
                temp[j+1] = val[1];
                flgc1++;
                temp[j+2] = val[2];
                flgc1++;
                temp[j+3] = val[3];
                log("Count", flgc1++);
                j= j + 4;
            }
            flgc1 = 0;
            MPI_Send(temp, 4*count1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            log("Message Sent",1);
            delete[] temp;
        }
    // }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
    {
        int flg = 0;
        unordered_map<int,vector<int>> particle;
        int r = 0, rem = k;
        while(rem)
        {
            int count;
            MPI_Recv(&count, 1, MPI_INT, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            log("MetaData Received", 1);
            if(count)
            {
                int* temp = new int[4*count];
                flg = 1;
                MPI_Recv(temp, 4*count, MPI_INT, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                log("Received", 2);
                collision_coordinate_add(temp,count,idx,collisions,particle, 1);
                log("Value Added ",1);
                log("Vector used Successfully", 1);
                delete[] temp;
            }
            r = r + 1;
            rem -= count;
            flg = 0;
            
        }
        log("Output Built", 1);
        cout<<endl;
        handle_output(particle,k);
    }
    
    MPI_Finalize();
}



