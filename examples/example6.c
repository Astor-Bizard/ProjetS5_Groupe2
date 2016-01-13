#include <stdio.h>

int truc(int weee, char whee, int whhe)
{
	return weee*whee+whhe;
}

int main(){
	int a,b;
	char c='O';
	a=9;
	b=a*2;
	printf("%d %d cou%ccou",a,b,c);
	printf("%d",truc(a,c,b));
	return 0;
}
