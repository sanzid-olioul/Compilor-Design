#include<stdio.h>
void main()
{
    int a = 25;         
    int b;       //comment one
    b = 5;
    int c = 52;
    if(a<b){
        printf("i am also true from if");
        // scanf("%d",&a);
    }
    else{
        printf("i am also true from else");
    }

    if(a>=25){
        // printf("hellow");
        printf("i am true %d",a);
    }
    while(a<b)
    {
        printf("hellow how are you");
        scanf("%d",&a);
    }
    for(int i=0;i<10;i++) 
    {
        printf("i am here %d",i);
        printf("hii there %d",i);

    }
    
    b =  57 + 4588;
    c = 25 *45;
    printf("this is the value of %d %d",a,b);
    
    /*
    some comments
    of multi line
    
    */
    //comment 2
    //another comments
    float x = 34.44;
    return 0;      
}

