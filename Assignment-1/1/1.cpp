#include <bits/stdc++.h>
#include <mpi.h>

// Check for input 2 2 3 and any k the output

using namespace std;


void log(string s, int n)
{
    // ofstream myfile;
    // myfile.open(to_string(n),ios::app);
    // myfile<<s<<endl;
    // myfile.close();
    // cout<<s<<endl;
}

int find_mandelbrot(double x1, double y1,double k)
{
    double x = 0, y = 0;
    int count = 0;
    while(x*x + y*y <= 4 && count < k)
    {
        double xtemp = x*x - y*y + x1;
        y = 2*x*y + y1;
        x = xtemp;
        count++;
    }
    if(count == k)
        return 1;
    else
        return 0;
}

// Divide n*m points evenly across x1 and x2

void find_points(vector<vector<double>>&x_grid, vector<vector<double>>&y_grid)
{
    double n = x_grid.size(), m = x_grid[0].size();
    double x1 = -1.5, x2 = 1, y1 = -1, y2 = 1;
    double distx = 2.5/(n-1), disty = 2.0/(m-1);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j <  m; j++)
        {
            x_grid[i][j] = x1 + i*distx;
            y_grid[i][j] = y1 + j*disty;
        }
    }
}

void writeTofile(vector<vector<int>>&final_result, int n, int m)
{
    ofstream myfile;
    myfile.open("result.txt");
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(final_result[j][i] == 1)
                myfile<<"1";
            else
                myfile<<"0";
        }
        myfile<<endl;
    }
}

void find_mandelbrot_set(vector<vector<double>>&x_grid, vector<vector<double>>&y_grid, double k)
{
    double n = x_grid.size(), m = x_grid[0].size();
    vector<vector<int>>result(n,vector<int>(m,0));
    
    // Find mandelbrot set for each point
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            result[i][j] = find_mandelbrot(x_grid[i][j],y_grid[i][j],k);
        }
    }

    // Write result to file

    writeTofile(result,n,m);
}


int main(int argc, char *argv[])
{
   
    MPI_Init(&argc, &argv);
    int size, rank;
    srand(time(NULL));
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nprocess = size;

    if (rank == 0)
    {
        double n,m,k;
        cin>>n>>m>>k;
        vector<vector<double>>x_grid(n,vector<double>(m,0));
        vector<vector<double>>y_grid(n,vector<double>(m,0));
        find_points(x_grid,y_grid);

        log("Points found",rank);

        // Handle Case when no of process are more than no of rows
        if(nprocess >= n)
        {
            find_mandelbrot_set(x_grid,y_grid,k);
            try{
                MPI_Abort(MPI_COMM_WORLD,1);
                throw 1;
            }
            catch(int e)
            {
                exit(1);
            }
            
            
            return 0;

        }
        // If process count = 1 handle all the work
        if(nprocess == 1)
        {
            // cout<<"dsd ";
            find_mandelbrot_set(x_grid,y_grid,k);
            MPI_Finalize();
            return 0;
        }
        
        int count = 0;
        double staticData[3] = {n,m,k};
        int last = -1;

        for(int i = 0; i < n; i++)
        {
            int process_number = ((i)/ceil((n*1.0)/(nprocess-1)))+1;
            if(last != process_number)
            {
                MPI_Send(&staticData, 3, MPI_DOUBLE, process_number, 0, MPI_COMM_WORLD);
                last = process_number;
            }
            
            // log(to_string(i)+" "+to_string(process_number),rank);

            MPI_Send(&x_grid[i][0], m, MPI_DOUBLE, process_number, 0, MPI_COMM_WORLD);
            log(to_string(i)+" row_x "+to_string(process_number)+" process_num "+to_string(x_grid[i][0]),rank);
            MPI_Send(&y_grid[i][0], m, MPI_DOUBLE, process_number, 0, MPI_COMM_WORLD);

            log(to_string(i)+" yyy "+to_string(process_number),rank);
            
            
        }

        log("Data sent",rank);

        // Receive result from all processes

        vector<vector<int>>final_result;
        for(int i = 1; i < nprocess; i++)
        {
            int n_row_count = ceil((n*1.0)/(nprocess-1));
        
            if(i == nprocess-1)
            {
                n_row_count = n - (nprocess-2)*n_row_count;
            }
            for(int j = 0; j < n_row_count; j++)
            {
                vector<int>temp(m,0);
                MPI_Recv(&temp[0], m, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                final_result.push_back(temp);
                log("Result received from "+to_string(i)+" "+to_string(j),rank);
            }
        }

        // Write result to file
        writeTofile(final_result,n,m);
        

    }
    else if(rank < nprocess && rank != 0)
    {

        double staticDataRecv[3];
        vector<vector<double>>x_grid_recv, y_grid_recv;


        MPI_Recv(&staticDataRecv, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // log(" rrrccdcdc "+to_string(staticDataRecv[0]), rank);
        // log(to_string(staticDataRecv[0])+" sdsdsds ", rank);

        double n_recv = staticDataRecv[0], m_recv = staticDataRecv[1], k_recv = staticDataRecv[2];

        // log(to_string(n_recv)+" ds "+to_string(m_recv)+" sdads "+to_string(k_recv)+" "+to_string(nprocess),rank);

        int n_row_count = ceil((n_recv*1.0)/(nprocess-1));
        
        if(rank == nprocess-1)
        {
            // last process
            n_row_count = n_recv - (nprocess-2)*n_row_count;
        }

        for(int i = 0; i < n_row_count; i++)
        {
            vector<double>xtemp(m_recv,0);
            vector<double>ytemp(m_recv,0);
            log(to_string(i)+" x_recv_before ",rank);
            MPI_Recv(&xtemp[0], m_recv, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            log(to_string(xtemp[0])+" x_recv ",rank);
            MPI_Recv(&ytemp[0], m_recv, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            log(to_string(i)+" y_recv "+ to_string(ytemp[0]),rank);
            x_grid_recv.push_back(xtemp);
            y_grid_recv.push_back(ytemp);
        }

        vector<vector<int>>result(n_row_count,vector<int>(m_recv,0));

        for(int i = 0; i < n_row_count; i++)
        {
            for(int j = 0; j < m_recv; j++)
            {
                result[i][j] = find_mandelbrot(x_grid_recv[i][j],y_grid_recv[i][j],k_recv);
            }
        }
        log("Result calculated"+to_string(result[0][0]),rank);
        // Send result to process 0
        for(int i = 0; i < n_row_count; i++)
        {
            MPI_Send(&result[i][0], m_recv, MPI_INT, 0, 1, MPI_COMM_WORLD);
            log("Result sent"+to_string(rank),rank);
        }

        // log("Result sent"+to_string(rank),rank);
       
    }
       
    MPI_Finalize();

    return 0;
}