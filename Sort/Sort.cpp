#include <iostream>
using namespace std;

void Swap(int *a, int *b);
int BubbleSort(int *a, int n);
int QSort(int *a, int low, int high);
int InsertSort(int *a, int n);
int SelectSort(int *a, int n);
int ShellSort(int *a, int n);
int Merge(int *a, int low, int mid, int high);
int MergeSort(int *a, int low, int high);

int main()
{
    int a[10] = {12, 32, 54, 67, 78, 98, 5, 8, 980};

    cout<<"before sorting: ";
    for(int i = 0; i < 10; i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;

//  cout<<"***********BubbleSort*************"<<endl;  
//  BubbleSort(a, 10);

//    cout<<"***********QuickSort**************"<<endl;
//    QSort(a, 0, 9);

//    cout<<"*************InsertSort*************"<<endl;
//    InsertSort(a, 10);

//    cout<<"************SelectSort**************"<<endl;
//    SelectSort(a, 10);

//    cout<<"************ShellSort*************"<<endl;
//    ShellSort(a, 10);

    cout<<"************MergeSort***************"<<endl;
    MergeSort(a, 0, 9);

    cout<<"before sorting: ";
    for(int i = 0; i < 10; i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;
    return 0;   
}

void Swap(int *a, int *b)
{
    int nTemp = *a;
    *a = *b;
    *b = nTemp;
}

int BubbleSort(int *a, int n)
{
    if(NULL == a)
    {
        return -1;
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(a[j] > a[j + 1])
            {
                Swap(&a[j], &a[j + 1]);
            }
        }
    }

    return 0;
}

int QSort(int *a, int low, int high)
{
    if(NULL == a)
    {
        return -1;
    }

    int i = low; 
    int j = high;

    if(i < j)
    {
        while(i < j)
        {
            while(i < j && a[i] < a[j])
            {
                j--;
            }
            Swap(&a[i], &a[j]);

            while(i < j && a[i] < a[j])
            {
                i++;
            }
            Swap(&a[i], &a[j]);
        }
    }
    else
    {
        return -1;
    }
    
    QSort(a, low, i);
    QSort(a, i + 1, high);

    return 0;
}
int InsertSort(int *a, int n)
{
    if(NULL == a)
    {
        return -1;
    }

    int nTemp = 0;

    for(int i = 0; i < n; i++)
    {
        nTemp = a[i];
        for(int j = i; j > 0 && nTemp < a[j - 1]; j--)
        {
            if(a[j] < a[j - 1])
            {
                Swap(&a[j], &a[j - 1]);
            }
        }
    }

    return 0;
}
int SelectSort(int *a, int n)
{
    if(NULL == a)
    {
        return -1;
    }

    int min = 0;

    for(int i = 0; i < n; i++)
    {
        min = i;
        for(int j = 0; j < n; j++)
        {
            if(a[j] > a[min])
            {
                min = j;
            }

            if(min != i)
            {
                Swap(&a[i], &a[min]);
            }
        }
    }

    return 0;
}
int ShellSort(int *a, int n)
{
    if(NULL == a)
    {
        return -1;
    }

    int nTemp = 0;

    for(int step = n / 2; step > 0; step /= 2)
    {
        for(int i = step; i < n; i++)
        {
            nTemp = a[i];
            for(int j = i - step; j >= 0 && nTemp < a[j]; j -= step)
            {
                Swap(&a[j], &a[j + step]);
            }
        }
    }

    return 0;
}
int Merge(int *a, int low, int mid, int high)
{
    if(NULL == a)
    {
        return -1;
    }

    int i = 0;
    int k = 0;
    int left_low = low;
    int left_high = mid;
    int right_low = mid + 1;
    int right_high = high;
    int *aTemp = new int[high - low + 1];

    for(k = 0; left_low <= left_high && right_low <= right_high; k++)
    {
        if(a[left_low] <= a[right_low])
        {
            aTemp[k] = a[left_low++];
        }
        else
        {
            aTemp[k] = a[right_low++];
        }
        
    }

    if(left_low <= left_high)
    {
        for(i = left_low; i <= left_high; i++)
        {
            aTemp[i] = a[k++];
        }
    }

    if(right_low <= right_high)
    {
        for(i = right_low; i <= right_high; i++)
        {
            aTemp[i] = a[k++];
        }
    }

    for(i = 0; i < high - low + 1;  i++)
    {
        a[low + i] = aTemp[i];
    }
    delete []aTemp;

    return 0;
}
int MergeSort(int *a, int low, int high)
{
    if(NULL == a)
    {
        return -1;
    }

    int mid = 0;

    if(low < high)
    {
        mid = (low + high) / 2;
        MergeSort(a, low, mid);
        MergeSort(a, mid + 1, high);
        Merge(a, low, mid, high);
    }

    return 0;
}