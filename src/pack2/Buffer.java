package pack2;
import java.awt.image.BufferedImage;

public class Buffer {
	
	private BufferedImage image;	
	private boolean available = true ;

	public synchronized void put ( BufferedImage image) {
		
		while ( !available ) {
				try {
					wait ();
				}catch (InterruptedException e){	
					e.printStackTrace(); 
				}
			}
		this.image = image;
		
		available = false;			
		notifyAll();
		}
	
	public synchronized BufferedImage get () {
		while (available) {
				try {
					wait ();
				}catch (InterruptedException e){ 
					e.printStackTrace(); 	
				}
			}
			available = true ;
			
			notifyAll();
			
		return image ;
	}


}
