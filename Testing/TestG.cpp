#include <iostream>

int main(){
    std::cout << "Welcome to this awesome game!" << std::endl;

    std::cout << "You have three options:" << std::endl;
    std::cout << "1. Create a new Hero \n2. Load existing Hero \n3. Exit" << std::endl;
    

    int choice;
    std::cin >> choice;

    if (choice == 1){
        std::string name;
        std::cout << "Enter hero name: ";
        std::cin >> name;
    } else if (choice == 2) {
        std::string name;
        std::cout << "Enter hero name to load: ";
        std::cin >> name;
    } else if (choice == 3){
        return 0;
    } else {
         std::cout << "Invalid choice. Please try again." << std::endl;
    }




    return 0;
}