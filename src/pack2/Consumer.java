package pack2;

public class Consumer extends ThreadClass{
	private Buffer buffer;
	private String writePath;
	
	public Consumer(){}
	public Consumer(Buffer buffer, String writePath) {
		super(false);			// Initializam clasa parinte ThreadClass ca si Consumer
		this.buffer = buffer;
		this.writePath = writePath;
	}
	
	@Override 
	public void start() {
		super.start();
	}
	@Override
	public void run(){
		super.run();
	}
}
