package pack2;

import java.awt.image.BufferedImage;

//interfata cu cele 3 metode de rotatie	- nivel 1
public interface Rotation {
    public BufferedImage rotate90();
    public BufferedImage rotate180();
    public BufferedImage rotate270();
}
