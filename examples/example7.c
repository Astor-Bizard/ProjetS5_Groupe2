int f1(int a){
	if(a>5)return f2(a+1);
	else return f3(a*2);
}

int f2(int x){
	if(x<6) return 42;
	else return f4(x);
}

int f3(int y){
	return (y*2+3)/2+y/10;
}

int f4(int yolo){
	if(yolo<10) return f1(yolo);
	else return yolo;
}

int main(int argc, char*argv[]){
	int weee=argv[1][0];
	f1(weee);
	return 0;
}
