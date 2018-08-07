class ErrorRec{
	public:
	int col;
	int row;
	int message;
	char info[50];
	ErrorRec *next;
};
	
class Errors{
    ErrorRec* head;
	public:
		Errors()
		{
			head->col = head->row = head->message = 0;
			head->info[0]='\0';
			head->next= NULL;
		}
		
		void insert(int t_col, int t_row, int t_message, char* info)
		{			
		
			 ErrorRec* temp = new ErrorRec;
			 temp->col = t_col;
			 temp->row = t_row;
			 temp->message = t_message;
			 strcpy(temp->info,info);
			 if(head == NULL)
			 {
				head = temp; 
			 }
			 else{
				temp->next = head;
				head = temp;
			 }	
		}
		void display()
		{
			if(head == NULL)
			 std::cout<<"\nNo Errors";
			else{
				ErrorRec* temp = head;
				while(temp!=NULL)
				{
					std::cout<<ERRORS[temp->message]<<" Info: "<<temp->info<<"  Line Number: "<<temp->row;
					temp = temp->next;
				}
			}
		}	
};
