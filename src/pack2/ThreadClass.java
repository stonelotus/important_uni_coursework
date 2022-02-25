package pack2;
public class ThreadClass extends Thread {
	
	private String name;		// Nume Producer or Consumer
	public ThreadClass(){}

	public ThreadClass(boolean type){
		this.name = type ? "Producer" : "Consumer";          	// In functie de type(true/false), selectam daca e consumer si Producer
		System.out.println("New thread name: " + this.name);
	}
	
	public void start(){ 
		
		System.out.println("Start thread: " + this.name);
		
	//  Apelarea metodei start din clasa parinte (clasa Thread)
		super.start();
	}
	
	public void run(){
		
	//  Afisarea inceperii rularii Threadului specific
		System.out.println("Run " + this.name);
	}
	
	// Getter al numelui Threadului
	public String getThreadName(){
		return this.name;
	}
	

}


