#include "mpi.h"
#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;


void log(string s, int n)
{
    // ofstream myfile;
    // myfile.open(to_string(n),ios::app);
    // myfile<<s<<endl;
    // myfile.close();
    // cout<<s<<endl;
}



void merge_helper(int keys_arr[], int left_keys_arr[], int right_keys_arr[], int freq_arr[], int left_freq_arr[], int right_freq_arr[], int l, int m, int h){
    int n1 = m-l, n2 = h-m+1, lf_count_val = 0, rf_count_val = 0, lk_count_val = 0, rk_count_val = 0;

    log("Merge helper ",n1);
    n1 = n1 + 1;
    for (int i = 0; i < n1; i++){
        left_keys_arr[i] = keys_arr[l+i];
        lk_count_val++;
        left_freq_arr[i] = freq_arr[l+i];
        lf_count_val++;
    }
    log("Merge helper ",n2);
    n2 = n2 -1;
    for (int j = 0; j < n2; j++){
        right_keys_arr[j] = keys_arr[m+1+j];
        rk_count_val++;
        right_freq_arr[j] = freq_arr[m+1+j];
        rf_count_val++;
    }
    log("lk_count_val ",lk_count_val);
}




void merge(int keys_arr[],int freq_arr[],int l,int m,int h){
    int n1 = m-l;
    n1 = n1 + 1;
    int n2 = h-m+1;
    n2 = n2 - 1;
    

    int left_keys_arr[n1], right_keys_arr[n2],left_freq_arr[n1], right_freq_arr[n2];

    merge_helper(keys_arr,left_keys_arr,right_keys_arr,freq_arr, left_freq_arr, right_freq_arr,l,m,h);

    int i=0,j=0,k=l;
    int lesser_count = 0, greater_count = 0;

    while(i < n1 && j < n2) 
    {
        if(left_keys_arr[i] <= right_keys_arr[j]){
            keys_arr[k] = left_keys_arr[i];
            lesser_count++;
            freq_arr[k] = left_freq_arr[i];
            i++;

        }
        else{
            keys_arr[k] = right_keys_arr[j];
            greater_count++;
            freq_arr[k] = right_freq_arr[j];
            j++;
        }
        k = k + 1;
    }

    log("Lesser_count ",lesser_count);
    log("Greater_count ",greater_count);

    // log("Indexes ",i);
    // log("Indexes ",j);


    while (i < n1) {
        keys_arr[k] = left_keys_arr[i];
        freq_arr[k] = left_freq_arr[i];
        i = i + 1;
        k = k + 1;
    }

    while (j < n2) 
    {
        keys_arr[k] = right_keys_arr[j];
        freq_arr[k] = right_freq_arr[j];
        j = j + 1;
        k = k + 1;
    }

}



void merge_sort(int keys_arr[],int freq_arr[],int begin,int end){
    if (begin >= end)
        return; 
 
    auto mid = begin + (end - begin) / 2;
    merge_sort(keys_arr,freq_arr,begin,mid);
    log("Merge sort ",mid);
    merge_sort(keys_arr,freq_arr,mid+1,end);
    log("Merge sort ",end);
    merge(keys_arr,freq_arr, begin, mid, end);
    return ;
}


int handle(vector<vector<int>> &cost_array, int x, int y, int r)
{
    int c = 0;
    if(r > x)
        c = c + cost_array[x][r-1];
    if(r < y)
        c = c + cost_array[r+1][y];
    return c;
}


void diagonal_compute(int x_co_arr[],int y_co_arr[],int freq_arr[],int res[],int chunkSize,vector<vector<int>> &cost_array,int root_buffer[]){

    int count = 0, flg = 0;
    for(int a=0;a<chunkSize;a++){
        if(x_co_arr[a] == y_co_arr[a]){
            count++;
            res[a] = freq_arr[x_co_arr[a]];
            root_buffer[a]=x_co_arr[a];
            continue;
        }
        int sum=0;
        for(int j = x_co_arr[a];j <= y_co_arr[a];j++){
            sum+=freq_arr[j];
        }
        
        for (int r = x_co_arr[a]; r <= y_co_arr[a]; r++){
            
            int c = handle(cost_array,x_co_arr[a],y_co_arr[a],r) + sum;
            log("handle ",c);
            if(c < res[a]){
                res[a]=c;
                flg = 1;
                root_buffer[a]=r;
            }
            else
            {
                flg = 0;
            }
        }
        if(flg == 1)
            log("Success", flg);
        flg = 0;
    }
    
}

void recursive_fun(vector<vector<int>> &root_arr,int parent[],int l,int r,int curr_par){

    int row_c = root_arr.size(), flg = 0;
    row_c = row_c+1;
    if(l>r)
        return;

    if(l<=r){
        parent[root_arr[l][r]]=curr_par;
        int left = l,right = root_arr[l][r]-1;
        flg = 1;
        recursive_fun(root_arr,parent,left,right,root_arr[l][r]);
        log("rec1  ",flg);
        left = root_arr[l][r]+1,right = r;
        flg++;
        recursive_fun(root_arr,parent,left,right,root_arr[l][r]);
        log("rec2  ",flg);
    }
}


void send_size_compute(int n,int size,int size_arr[])
{
    for(int i=0;i<size;i++)
        size_arr[i]=n/size;
    log("size_arr[0] ",size_arr[0]);
    size_arr[0]+=n%size;
}

void displacement_compute(int size,int size_arr[],int displacements[])
{
    displacements[0]=0;
    log("size ",size);
    for(int i=1;i<size;i++)
    {
        displacements[i] = displacements[i-1]+size_arr[i-1];
    }
        
}

void handle_output(vector<vector<int>> &root_arr, vector<vector<int>> &cost_array, int n)
{
    int n1 = root_arr.size();
    int parent[n];
    log("Root array Size = ",n1);
    
    recursive_fun(root_arr,parent,0,n-1,-1);
    log("Rec Function Called",n1);
    cout<<cost_array[0][n-1]<<endl;
    for(int i = 0;i < n; i++){
        n1--;
        cout<<parent[i]+1<<" ";
        
    }
    cout<<endl;
}

int main(int argc, char *argv[]){
    int rank, size, comm_size, name_length, n;
	MPI_Init(&argc, &argv);
	MPI_Comm comm = MPI_COMM_WORLD;
	char name[MPI_MAX_OBJECT_NAME];

	MPI_Comm_size(comm, &comm_size);
	MPI_Comm_get_name(comm, name, &name_length);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    if(rank==0)
    {
        cin>>n;
        log(" Input Taken ",0);
    }
        
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

    int keys_arr[n],freq_arr[n];
    int temp_buff_arr[n];
    int temp_buff_size = 0;
    if(rank==0){
        for(int i=0;i<n;i++){
            cin>>keys_arr[i]>>freq_arr[i];
        }
    }
   
    int size_arr[size];
    send_size_compute(n,size,size_arr);

    int displacements[size];
    displacement_compute(size,size_arr,displacements);
    log("Displacement Computation Done",0);


    int recv_meta_arr[1];
    MPI_Scatter(size_arr,1,MPI_INT,recv_meta_arr,1,MPI_INT,0,MPI_COMM_WORLD);
    log("Scatter Done",0);
    int chunkSize = recv_meta_arr[0];
    int recv_buffer_keys[chunkSize],recv_buffer_freq[chunkSize];
    log(" Chunk Recv Check ",chunkSize);
    MPI_Scatterv(keys_arr,size_arr,displacements,MPI_INT,recv_buffer_keys,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatterv(freq_arr,size_arr,displacements,MPI_INT,recv_buffer_freq,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
    log(" Merging Before ",chunkSize);
    merge_sort(recv_buffer_keys,recv_buffer_freq,0,chunkSize-1);
    log(" Merging Done ",chunkSize);
    MPI_Gatherv(recv_buffer_keys,chunkSize,MPI_INT,keys_arr,size_arr,displacements,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Gatherv(recv_buffer_freq,chunkSize,MPI_INT,freq_arr,size_arr,displacements,MPI_INT,0,MPI_COMM_WORLD);
   
   log("Gather Done",chunkSize);

    if(rank==0){
        for(int i = 1;i < size;i++){
            merge(keys_arr,freq_arr,0,displacements[i]-1,displacements[i]+size_arr[i]-1);
        }
    }
    
    log("Before Bcast",chunkSize);
    MPI_Bcast(keys_arr,n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(freq_arr,n,MPI_INT,0,MPI_COMM_WORLD);
    log(" Bcast done ",chunkSize);
    
    vector<vector<int>> cost_array(n,vector<int> (n,0));

    int pcount_val = 0;
    vector<vector<int>>root_arr(n,vector<int> (n,0));
    int flg = 0;
    for(int i=0;i<n;i++){
        int diag[n-i],res_arr[n-i];
        int x_co_arr[n-i],y_co_arr[n-i];
        int a1 = 0;
        if(rank==0)
        {
            for(int j=0;j<n-i;j++){
                x_co_arr[j]=j;
                a1 = 0;
                y_co_arr[j]=j+i;
            }
            flg = 1;
            a1++;
        }
            
        int diag_size=n-i;
        a1++;
        for(int j=0;j<size;j++)
            size_arr[j]=diag_size/size;
        a1 = 0;
        size_arr[0]+=diag_size%size;
        
        log(" displacement_compute to do ",0);
        displacement_compute(size, size_arr, displacements);
        log(" displacement_compute Done ",0);


        MPI_Scatter(size_arr,1,MPI_INT,recv_meta_arr,1,MPI_INT,0,MPI_COMM_WORLD);

        chunkSize = recv_meta_arr[0];
        int sct_recv = 0;
        int x_rec_buf[chunkSize],y_rec_buf[chunkSize];
        int sct_buff_size = chunkSize;
        
        int res[chunkSize], root_buffer[chunkSize];
        for(int j=0;j<chunkSize;j++){
            res[j] = INT_MAX;
        }

        log(" Before Scatter ",0);
        int flg_sc = 0;
        MPI_Scatterv(x_co_arr,size_arr,displacements,MPI_INT,x_rec_buf,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
        flg_sc++;
        MPI_Scatterv(y_co_arr,size_arr,displacements,MPI_INT,y_rec_buf,chunkSize,MPI_INT,0,MPI_COMM_WORLD);
        flg_sc++;
        log(" After Scatter ",flg_sc);


        log(" Diagonal Computation Start ",chunkSize);
        diagonal_compute(x_rec_buf,y_rec_buf,freq_arr,res,chunkSize,cost_array,root_buffer);
        log(" Diagonal Computation Done ",chunkSize);
        int flg_alg = 0;
        MPI_Allgatherv(res,chunkSize,MPI_INT,diag,size_arr,displacements,MPI_INT,MPI_COMM_WORLD);
        flg_alg++;
        MPI_Allgatherv(root_buffer,chunkSize,MPI_INT,res_arr,size_arr,displacements,MPI_INT,MPI_COMM_WORLD);
        flg_alg++;
        log(" Allgather Done ",flg_alg);

        int c_arr_update = 0;
        for(int x=0,j=i;j<n;){
            cost_array[x][j]=diag[x];
            c_arr_update++;
            root_arr[x][j]=res_arr[x];
            x++;
            j++;
        }
    }
    
    if(rank==0){
       
        handle_output(root_arr,cost_array,n);
        log(" Output Printed ",0);
    }

    MPI_Finalize();
    return 0;
}