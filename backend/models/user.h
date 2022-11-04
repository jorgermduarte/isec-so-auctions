typedef struct User {
	char username[50];
	char password[50];
	int budget;
	Item items_for_sale[10];
	Item items_bought[10]; 
} User;