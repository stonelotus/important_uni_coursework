package pack2;
import pack1.Execute;
public class Producer extends ThreadClass{
	private Buffer buffer;
	private String inputString;
	
	public Producer() {};
	public Producer( Buffer buffer, String inputString) {
		super(true); 			// Initializam clasa parinte ThreadClass ca Producer
		this.buffer = buffer;
		this.inputString = inputString;
	}
	
	@Override 
	public void start() {
		super.start();
	}
	@Override
	public void run(){
		super.run();
        Image img = new Execute();
        img.image = img.readImage(this.inputString);
        buffer.put(img.image);
	}
	
}
