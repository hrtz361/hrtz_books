#include<iostream>
#include<string>
using namespace std;
class Person{
	private:
		string fullName,gender, birthYear,phoneNumber;
	public:
		Person();
		void Input();
		void Display();
};
     Person::Person() {
     	fullName="Nguyen Van A";
     	gender="Nam";
     	birthYear="01/01/2011";
     	phoneNumber="0123456789";
	 }
	void Person::Input() {
		cout<<"Nhap ho ten: ";cin>>ws;getline(cin,fullName);
		cout<<"Nhap gioi tinh: ";cin>>ws;getline(cin,gender);
		cout<<"Nhap ngay thang nam sinh: ";cin>>ws;getline(cin,birthYear);
		cout<<"Nhap so dien thoai: ";cin>>ws;getline(cin,phoneNumber);
	} 
	void Person::Display() {
		cout<<"Ho ten:"<<fullName<<endl<<"Gioi tinh:"<<gender<<endl<<"Nam sinh:"<<birthYear<<endl<<"SDT:"<<phoneNumber;
	}
	class Reader:public Person {
		private:
			string readerID,registrationDate;
			int maxBooks;
		public:
			Reader();
			void Input();
			void Display();
	};
	Reader::Reader() {
		readerID=registrationDate="";
		maxBooks=0;
	}
	void Reader::Input() {
		Person::Input();
		cout<<"Nhap ma nguoi doc: ";
		cin>>ws;getline(cin,readerID);
		cout<<"Nhap ngay dang ki: ";
		cin>>ws;getline(cin,registrationDate);
		cout<<"Nhap so sach toi da duoc muon: ";
		cin>>maxBooks;
	}
	void Reader::Display() {
		Person::Display();
		cout<<endl<<"Ma nguoi doc: "<<readerID<<endl<<"Ngay dang ki: "<<registrationDate<<endl<<"So sach duoc muon: "<<maxBooks;
	}
	class Book {
		protected :
			string bookID;
			string title ;
			string author;
			string publisher;
			double price;
		public :
			Book();
			virtual void Input();
			virtual void Output();
			virtual double calculateRentalFee() = 0;
		};
		Book::Book() {
			bookID=title=author=publisher="0";
			price=0;
		}
		void Book::Input() {
			cout<<"Nhap ma sach:";
			cin>>ws;getline(cin,bookID) ;
			cout<<"Nhap tieu de:";
			cin>>ws;getline(cin,title) ;
			cout<<"Nhap ten tac gia:" ;
			cin>>ws;getline(cin,author) ;
			cout<<"Nhap ten nha xuat ban:" ;
			cin>>ws;getline(cin,publisher) ;
			cout<<"Nhap gia sach:" ;
			cin>>price;
		}
		void Book::Output() {
			cout<<"\n Ma sach: "<<bookID
			    <<"\n Tieu de: "<<title
			    <<"\n Tac gia: "<<author
			    <<"\n Nha xuat ban: "<<publisher
			    <<"\n Gia sach: "<<price;
		}
		class PrintedBook : public Book {
			private :
				int pageCount ;
				string weight ,
				       status ,
					   shelfLocation ;
			public :
				PrintedBook();
				void Input();
				void Output();
				double calculateRentalFee() {
					return price*0.03;
				}
				
				};
		
		PrintedBook::PrintedBook() {
		    pageCount=0;		
		    shelfLocation=weight=status="";
		    }
		void PrintedBook::Input() {
			Book::Input() ;
			cout<<"Nhap so trang sach: ";
			cin>>pageCount;
			cout<<"Nhap trong luong sach (kg): ";
			cin>>ws;getline(cin,weight);
			cout<<"Nhap vi tri ke: " ;
			cin>>ws;getline(cin,shelfLocation);
			cout<<"Nhap trang thai (con/het): ";
			cin>>ws;getline(cin,status);
			}
		void PrintedBook::Output() {
			Book::Output();
			cout<<endl<<" So trang:"<<pageCount
			    <<"\n Trong luong:"<<weight<<" kg"
				<<"\n Vi tri ke:"<<shelfLocation
				<<"\n Trang thai:"<<status
				<<"\n Phi muon sach:"<<calculateRentalFee() ;
				}
	
				
		class Ebook : public Book {
		    private :
			    	  string fileSize,fileFormat,downloadLink;
			public :
				Ebook();
				void Input();
				void Output();
				double calculateRentalFee() {
					return price*0.02;
				} ;
				
					};
			Ebook::Ebook() {
				fileSize=fileFormat=downloadLink="" ;
				}
			void Ebook::Input() {
				Book::Input() ;
				cout<<"Nhap dung luong file (MB):";
				cin>>ws;getline(cin,fileSize) ;
				cout<<"Nhap dinh dang file (PDF/EPUB):";
				cin>>ws;getline(cin,fileFormat) ;
				cout<<"Nhap Link tai ve:" ;
				cin>>ws;getline(cin,downloadLink);
			}
			void Ebook::Output() {
				Book::Output() ;
				cout<<endl<<"Dung luong file:"<<fileSize<<"MB"
				          <<"\n Dinh dang file:"<<fileFormat
				          <<"\n Link tai ve:"<<downloadLink 
				          <<"\n Phi muon sach:"<<calculateRentalFee() ;
			} 
			
			
	int main() {
		Reader r ;
		r.Input() ;
		PrintedBook pr ;
		pr.Input() ;
		Ebook eb ;
        eb.Input() ;
		cout<<"========THONG TIN NGUOI DOC========"<<endl ;
		r.Display() ;
	
		cout<<endl<<"========THONG TIN SACH IN========" ;
		pr.Output() ;
		
		cout<<endl<<"========THONG TIN SACH DIEN TU========" ;
        eb.Output() ;
	}
	
     
