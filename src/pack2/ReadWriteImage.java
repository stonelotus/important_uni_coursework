package pack2;

import java.awt.image.BufferedImage;

//clasa abstracta cu metode de citire/scriere a fisierului si metoda de rotatie	 - nivel 1
public abstract class ReadWriteImage {
   public abstract BufferedImage readImage(String string_input);
   public abstract void writeImage(String string_output, String string_name, BufferedImage rotated);
   public abstract BufferedImage rotate(int ...degrees);

}
