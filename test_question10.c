
int main(void)
{
	volatile int i;
	for(i = 1 ;i<867287599 ; i++)
	{
		if(i%867287600==0)
			break;
		
		while(i%8672876001 == 0)
			break;
	}
}
