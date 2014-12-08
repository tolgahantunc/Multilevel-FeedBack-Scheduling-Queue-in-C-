//MULTILEVEL FEEDBACK QUEUE

#include<stdio.h>
#include<queue>

using namespace std;


struct process
{
	int pid;
	int atime;
	int btime;
	int rtime; //Remaining Time
	int priority;
	
	//Parameters for predicting output
	int stime;
	int etime;
};


struct mycomp
{
	bool operator()(const process& lhs, const process& rhs) const
	{
		return lhs.priority > rhs.priority;
	}
};

void printproc(process arr[],int n)
{
	printf("\nPROCESS TABLE");
	for(int i=0;i<n;i++)
	{
		printf("%d %d %d %d",arr[i].pid,arr[i].atime,arr[i].btime,arr[i].priority);
		printf("\n");
	}
}

int main()
{
	int i,j,n,tot=0;
	scanf("%d",&n);
	int pid,atime,btime,priority;
	process arr[n+1];
	for( i=0;i<n;i++)
	{
	
		scanf("%d %d %d %d",&pid,&atime,&btime,&priority);
		arr[i].pid = pid;
		arr[i].atime=atime;
		arr[i].btime=btime;
		arr[i].priority=priority;
		arr[i].rtime=btime;
		tot = tot + btime;
	}
	
	//print before sorting
	//printproc(arr,n);
	
	//Sort The Processes as per arrival time
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(arr[i].atime<arr[j].atime)
			{
				//Swap i and j
				int tpid = arr[i].pid;
				int tatime = arr[i].atime;
				int tbtime = arr[i].btime;
				int tpriority=arr[i].priority;
				int rtime = arr[i].rtime;
				
				arr[i].pid=arr[j].pid;
				arr[i].atime = arr[j].atime;
				arr[i].btime  = arr[j].btime;
				arr[i].priority = arr[j].priority;
				arr[i].rtime = arr[j].rtime;
				
				arr[j].pid=tpid;
				arr[j].atime = tatime;
				arr[j].btime  = tbtime;
				arr[j].priority = tpriority;
				arr[j].rtime = rtime;
				
			}
		}
	}
	
	//Print process details
	//printproc(arr,n);

	//The gannt chart
	int chart[1000];
	//Initialize chart
	for(i=0;i<tot+1;i++)
		chart[i]=0;
	priority_queue<process, vector<process>, mycomp> q1;
	queue<process> q2;
	
	int time=0,turn;
	turn = 0;
	int quanta=4,rquanta=4;
	int flag1=0;
	while((!q1.empty())||(!q2.empty())||(flag1==0)||(turn<n))
	{
		//printf("\ntime: %d",time);
		while((turn<n) && (arr[turn].atime==time))
		{
			
			
			if(quanta!=4&&(arr[turn].priority<q1.top().priority))
			{
				//printf("\t here pushed %d into q2",q1.top().pid);
				q2.push(q1.top());
				q1.pop();
				quanta=4;
				
				
				
			}
			//if(flag1==0)
			//	printf("\t***flag1 value Changed***");
			
			flag1=1;
			//printf("\npushed: %d in q1",arr[turn].pid);
			q1.push(arr[turn]);
			turn = turn +1;
		}
		
		//Process Queue
		if(!q1.empty())
		{
			if((rquanta!=4)&&(!q2.empty()))
			{
				//Pop from Queue 2 and push at back
				//printf("\tpushing %d with rtime %d in q2",q2.front().pid,q2.front().rtime);
				q2.push(q2.front());
				q2.pop();
				rquanta=4;
			}
			
			process cur = q1.top();
			//printf("\nPopped %d",cur.pid);
			chart[time]=cur.pid;
			cur.rtime = cur.rtime-1;
			q1.push(cur);
			q1.pop();
			quanta=quanta-1;
			if(q1.top().rtime==0)
			{
				//printf("\tRemaining Time is 0");
				q1.pop();
				quanta=4;
			}
			else if(quanta==0)
			{	
				//printf("\tTime quanta over for %d rtime is %d",cur.pid,cur.rtime);
				
				if(q1.top().rtime>0)
					q2.push(q1.top());
				q1.pop();
				quanta=4;
			}
		}
		else if(!q2.empty())
		{
			//Process Queue 2 for Round Robin
			
			q2.front().rtime = q2.front().rtime-1;
			//printf("\nIn RR rtime for %d is : %d",q2.front().pid,q2.front().rtime);
			chart[time]=q2.front().pid;
			rquanta--;
			if(q2.front().rtime==0)
			{
				q2.pop();
				rquanta=4;
			}
			
			else if(rquanta==0)
			{
				//printf("\trquanta is zero, removing and adding at end %d",q2.front().pid);
				q2.push(q2.front());
				q2.pop();
				rquanta=4;
			}
			
		}	
		
		/*	//Print Current chart
		printf("\nAt time= %d Current Chart: ",time);
		for(i=0;i<=time;i++)
			printf("%d",chart[i]);
		printf("\n");
		*/	
		
		time= time+1;
		
	}
	
	printf("\nCurrent Chart: ");
		for(i=0;i<=time;i++)
			printf("%d",chart[i]);
		printf("\n");
		
	//Predict output
	for(i=0;i<n;i++)
	{
		int flag=0;
		int curpid = arr[i].pid;
		for(j=0;j<time;j++)
		{
			if(chart[j]==curpid)
			{
				if(flag==0)
				{
					arr[i].stime=j;
					flag=1;
				}
				else
					arr[i].etime=j+1;
			}
		}
	}
	
	//Sort The Processes as per pid
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(arr[i].pid<arr[j].pid)
			{
				//Swap i and j
				int tpid = arr[i].pid;
				int tatime = arr[i].atime;
				int tbtime = arr[i].btime;
				int tpriority=arr[i].priority;
				int rtime = arr[i].rtime;
				int stime = arr[i].stime;
				int etime = arr[i].etime;
				
				arr[i].pid=arr[j].pid;
				arr[i].atime = arr[j].atime;
				arr[i].btime  = arr[j].btime;
				arr[i].priority = arr[j].priority;
				arr[i].rtime = arr[j].rtime;
				arr[i].stime = arr[j].stime;
				arr[i].etime = arr[j].etime;
				
				arr[j].pid=tpid;
				arr[j].atime = tatime;
				arr[j].btime  = tbtime;
				arr[j].priority = tpriority;
				arr[j].rtime = rtime;
				arr[j].stime=stime;
				arr[j].etime=etime;
				
			}
		}
	}
	
	
	//Print Output
	for(i=0;i<n;i++)
	{
		int opid,rtime,wtime,ftime;
		opid = arr[i].pid;
		rtime = arr[i].stime - arr[i].atime;
		wtime = arr[i].etime - arr[i].atime - arr[i].btime;
		ftime=arr[i].etime;
		
		printf("%d %d %d %d\n",opid,rtime,ftime,wtime);
	}
	
}
