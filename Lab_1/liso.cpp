#include <iostream> 
#include <thread> 
void Thread1() { 
 std::cout << "1" << std::endl; 
} 
void Thread2() { 
 std::cout << "2" << std::endl; 
} 
int main() { 
 std::thread t1(Thread1);  
 std::thread t2(Thread2);  
  
 return 0; 
} 

#include <iostream> 
#include <thread> 
void Thread1() { 
 std::cout << "1" << std::endl; 
} 
void Thread2() { 
 std::cout << "2" << std::endl; 
} 
int main() { 
 std::thread t1(Thread1);  
 std::thread t2(Thread2);  
  
 t1.detach(); 
 t2.detach(); 
 return 0; 
} 

#include <iostream> 
#include <list> 
#include <thread> 
#include <algorithm> 
std::list<int> l; 
void AddToList(int start_value) {
 for (int i = 0; i < 10; ++i) { 
 l.push_back(start_value + i); 
 std::cout << "Adding element: " << start_value + i << std::endl; 
 } 
} 
void ListContains(int value) { 
 for (int i = 0; i < 10; ++i) { 
 auto it = std::find(l.begin(), l.end(), value); 
 if (it != l.end()) { 
 std::cout << "The list contains: " << value << std::endl; 
 } else { 
 std::cout << "The list does not contain: " << value << std::endl;  } 
 } 
} 
int main() { 
 int main_value = 3; 
 std::thread t1(AddToList, main_value); 
 std::thread t2(ListContains, main_value); 
 t1.join(); 
 t2.join(); 
 return 0; 
}

#include <iostream> 
#include <list> 
#include <thread> 
#include <mutex> 
std::list<int> l; 
std::mutex mtx; 
void AddToList(int start_value) { 
 for (int i = 0; i < 10; ++i) { 
 mtx.lock(); 
 l.push_back(start_value + i); 
 std::cout << "Adding element: " << start_value + i << std::endl;  mtx.unlock(); 
 } 
} 
void ListContains(int value) { 
 for (int i = 0; i < 10; ++i) { 
 mtx.lock(); 
 auto it = std::find(l.begin(), l.end(), value); 
 if (it != l.end()) { 
 std::cout << "The list contains: " << value << std::endl;  } else { 
 std::cout << "The list does not contain: " << value << std::endl;  } 
 mtx.unlock(); 
 } 
} 
int main() { 
 int main_value = 3; 
 std::thread t1(AddToList, main_value); 
 std::thread t2(ListContains, main_value); 
 t1.join(); 
 t2.join(); 
 return 0; 
} 

#include <iostream> 
#include <list> 
#include <thread> 
#include <mutex> 
std::list<int> l; 
std::mutex mtx; 
void AddToList(int value) { 
 std::lock_guard<std::mutex> lock(mtx);  
 l.push_back(value); 
 std::cout << "Adding element: " << value << std::endl; 
} 
void ListContains(int value) { 
 std::lock_guard<std::mutex> lock(mtx);  
 auto it = std::find(l.begin(), l.end(), value); 
 if (it != l.end())  
 std::cout << "The list contains: " << value << std::endl; 
 else  
 std::cout << "The list does not contain: " << value << std::endl; 
} 
int main() { 
 int main_value = 3; 
 int current_value = main_value; 
 for (int i = 0; i < 10; ++i) { 
 std::thread t1(AddToList, current_value); 
 std::thread t2(ListContains, main_value); 
 t1.join(); 
 t2.join(); 
 ++current_value; 
 } 
 return 0; 
}
#include <iostream> 
#include <list> 
#include <thread> 
#include <mutex> 
class someData { 
public: 
 std::string firstName; 
 std::string lastName; 
 std::string address; 
 int age; 
 someData() : firstName(""), lastName(""), address(""), age(0) {} 
 void print() const { 
 std::cout << "First Name: " << firstName << ", Last Name: " << lastName  << ", Address: " << address << ", Age: " << age << std::endl;  }
}; 
class exchangePerson { 
public: 
 someData data; 
 std::mutex mtx; 
 static void JohnDoe(exchangePerson& person) { 
 std::lock_guard<std::mutex> lock(person.mtx); 
 person.data.firstName = "John"; 
 person.data.lastName = "Doe"; 
 person.data.address = "Unknown"; 
 person.data.age = 120; 
 } 
 static void JacobSmith(exchangePerson& person) { 
 std::lock_guard<std::mutex> lock(person.mtx); 
 person.data.firstName = "Jacob"; 
 person.data.lastName = "Smith"; 
 person.data.address = "Known"; 
 person.data.age = 1; 
 } 
 static void Swap(exchangePerson& person1, exchangePerson& person2) {  if (&person1 == &person2) { 
 std::cout << "Warning: References point to the same object!" << std::endl;  return; 
 } 
 std::lock(person1.mtx, person2.mtx); 
 std::lock_guard<std::mutex> lock1(person1.mtx, std::adopt_lock);  std::lock_guard<std::mutex> lock2(person2.mtx, std::adopt_lock); 
 std::swap(person1.data, person2.data); 
 } 
}; 
int main() { 
 exchangePerson person1, person2; 
 std::thread t1(&exchangePerson::JohnDoe, std::ref(person1)); 
 std::thread t2(&exchangePerson::JacobSmith, std::ref(person2)); 
 t1.detach(); 
 t2.detach(); 
 std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
 std::cout << "Before swapping:" << std::endl; 
 std::cout << "Object 1: "; 
 person1.data.print(); 
 std::cout << "Object 2: "; 
 person2.data.print(); 
 std::thread t3(&exchangePerson::Swap, std::ref(person1), std::ref(person2));  t3.join(); 
 std::cout << "After swapping:" << std::endl; 
 std::cout << "Object 1: "; 
 person1.data.print(); 
 std::cout << "Object 2: "; 
 person2.data.print(); 
 return 0; 
}

#include <iostream> 
#include <list> 
#include <thread> 
#include <mutex> 
class someData { 
public: 
 std::string firstName; 
 std::string lastName; 
 std::string address; 
 int age; 
 someData() : firstName(""), lastName(""), address(""), age(0) {} 
 void print() const { 
 std::cout << "First Name: " << firstName << ", Last Name: " << lastName  << ", Address: " << address << ", Age: " << age << std::endl;  } 
}; 
class exchangePerson { 
public: 
 someData data; 
 mutable std::mutex mtx; 
 static void JohnDoe(exchangePerson& person) { 
 std::lock_guard<std::mutex> lock(person.mtx); 
 person.data.firstName = "John"; 
 person.data.lastName = "Doe"; 
 person.data.address = "Unknown"; 
 person.data.age = 120; 
 } 
 static void JacobSmith(exchangePerson& person) { 
 std::lock_guard<std::mutex> lock(person.mtx); 
 person.data.firstName = "Jacob"; 
 person.data.lastName = "Smith"; 
 person.data.address = "Known"; 
 person.data.age = 1; 
 } 
 static void Swap(exchangePerson& person1, exchangePerson& person2) {  if (&person1 == &person2) { 
 std::cout << "Warning: References point to the same object!" << std::endl;  return; 
 } 
 std::unique_lock<std::mutex> lock1(person1.mtx, std::defer_lock);  std::unique_lock<std::mutex> lock2(person2.mtx, std::defer_lock); 
 std::lock(lock1, lock2); 
 std::swap(person1.data, person2.data); 
 } 
};
int main() { 
 exchangePerson person1, person2; 
 std::thread t1(&exchangePerson::JohnDoe, std::ref(person1)); 
 std::thread t2(&exchangePerson::JacobSmith, std::ref(person2)); 
 t1.detach(); 
 t2.detach(); 
 std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
 std::cout << "Before swapping:" << std::endl; 
 std::cout << "Object 1: "; 
 person1.data.print(); 
 std::cout << "Object 2: "; 
 person2.data.print(); 
 std::thread t3(&exchangePerson::Swap, std::ref(person1), std::ref(person2));  t3.join(); 
 std::cout << "After swapping:" << std::endl; 
 std::cout << "Object 1: "; 
 person1.data.print(); 
 std::cout << "Object 2: "; 
 person2.data.print(); 
 return 0; 
} 
 
 
