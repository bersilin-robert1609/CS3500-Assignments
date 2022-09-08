#include <iostream>
#include <algorithm>
#include <pthread.h>
using namespace std;

int DEBUG = 0;

int count_ans = 0;

typedef struct arguments {
	int* array;
	int left;
	int right;	
} arguments ;

typedef struct arguments_count {
	int* array;
	int left;
	int right;	
	int element;
} arguments_count ;

void* sort_func(void* args)
{
	arguments* argsf = (arguments*)args;
	
	sort((argsf->array)+(argsf->left), (argsf->array)+(argsf->right));
	
	return NULL;
}

void* merge_func(void* args)
{	
	arguments* argsf = (arguments*)args;
	
	int* temp = (int*)malloc(sizeof(int)*(argsf->right));
	
	int mid = (argsf->right)/2;
	int i = 0, j = mid, k = 0;
	
	while(i<mid && j<argsf->right)
	{
		if(argsf->array[i] < argsf->array[j])
		{
			temp[k] = argsf->array[i];
			i++;
			k++;
		}
		else
		{
			temp[k] = argsf->array[j];
			j++;
			k++;
		}
	}
	while(i<mid)
	{
		temp[k] = argsf->array[i];
		i++;
		k++;
	}
	while(j<argsf->right)
	{
		temp[k] = argsf->array[j];
		j++;
		k++;
	}
	
	for(int i=0; i<argsf->right; i++)
		argsf->array[i] = temp[i];
	
	return NULL;
}

void* count_func(void* args)
{
	arguments_count* argsf = (arguments_count*)args;
	
	if(DEBUG) cout<<"The left is "<<argsf->left<<endl;
	if(DEBUG) cout<<"The right is "<<argsf->right<<endl;
	
	for(int i = argsf->left; i < (argsf->right) ; i++)
	{	
		if((argsf->array[i]) % (argsf->element) == 0){
			count_ans++;
		}
		if(DEBUG) cout<<"The value is "<<argsf->array[i]<<" and "<<argsf->element<<endl;
	}
	if(DEBUG) cout<<"Thread ended for left "<<argsf->left<<endl;
	
	return NULL;
}

int main()
{
	int n;
	cin>>n;
	int* arr;
	arr = (int*)malloc(sizeof(int)*n);
	for(int i=0; i<n; i++) cin>>arr[i];
	
	int N; cin>>N;
	int element; cin>>element;
	
	pthread_t ptid1, ptid2;
	
	arguments args1, args2;
	args1.array = arr; args1.left = 0; args1.right = n/2;
	args2.array = arr; args2.left = n/2; args2.right = n;
	
	pthread_create(&ptid1, NULL, sort_func, (void*)&args1);
	pthread_create(&ptid2, NULL, sort_func, (void*)&args2);
	
	pthread_join(ptid1, NULL);
	pthread_join(ptid2, NULL);
	
	pthread_t ptid_merge;
	
	arguments args_merge;
	args_merge.array = arr; args_merge.left = 0; args_merge.right = n;
	pthread_create(&ptid_merge, NULL, merge_func, (void*)&args_merge);
	
	pthread_join(ptid_merge, NULL);
	
	for(int i=0; i<n; i++) cout<<arr[i]<<" ";
	cout<<endl;
	
	pthread_t ptids[N];
	
	arguments_count argsN[N];
	int sizeOfSubArray = n/N;
	for(int i=0; i<N; i++)
	{
		argsN[i].array = arr;
		argsN[i].left = (i)*sizeOfSubArray;
		argsN[i].right = (i+1)*sizeOfSubArray;
		if(i == N-1) argsN[i].right = n;
		argsN[i].element = element;
		
		if(DEBUG) cout<<"the value of argsleft is "<<argsN[i].left<<" and right is "<<argsN[i].right<<endl;
	}
	
	for(int i=0; i<N; i++)
	{
		int rcl = pthread_create(&ptids[i], NULL, count_func, (void*)&argsN[i]);
		if(rcl) {perror("Thread Not Created!"); exit(1);}
	}
	
	for(int i=0; i<N; i++) {
		if(DEBUG) cout<<"Closing thread "<<i<<endl;
		pthread_join(ptids[i], NULL);
	}
		
	cout<<"The count is "<<count_ans<<endl;
	return 0;
}
